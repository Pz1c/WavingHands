#include "qrosterlink.h"

#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>
#include <QtNetwork/QSslConfiguration>

#include "../qgameconstant.h"

namespace {

// Client-owned deadlines (D.4). The watchdogs on the server protect the SERVER
// and are useless as a latency bound for us: kIdleTimeout 600 s,
// kVerifyTimeout 90 s (which do_write can stretch to 600 s), kWriteTimeout 60 s.
const int kConnectDeadlineMs    = 5000;
const int kAuthDeadlineMs       = 20000;
const int kLoginDeadlineMs      = 10000;
const int kWhoSilentDeadlineMs  = 2500;
const int kWhoUserDeadlineMs    = 1200;
const int kPingDeadlineMs       = 2500;
const int kLogoutDeadlineMs     = 2000;
const int kTurnDeadlineMs       = 2500;

const int kKeepaliveMs          = 240000;
const int kKeepaliveSeconds     = 240;
const int kProbeDebounceSeconds = 30;

const int kBusyRetryMs          = 30000;    // ERR busy, socket stays open, no strike
const int kSiteRetryMs          = 180000;   // ERR site-unreachable, ditto
const int kRateLimitMs          = 660000;   // >= 11 min, or the 10 minute window re-arms

const int kMaxAuthRetries       = 3;
const int kMaxConnectTimeouts   = 3;
const int kMaxMalformed         = 3;
const int kMaxWrongPlayer       = 2;
const int kMaxQueuedTurns       = 16;
const int kTurnMaxAgeSeconds    = 600;     // past this a "just moved" hint is noise

const int    kMaxRxBytes        = 65536;              // one line may not exceed 64 KiB
const qint64 kMaxBlockBytes     = 8LL * 1024 * 1024;  // one block may not exceed 8 MiB

// 2, 4, 8, 15, 30, 60, 120, 300 s, cap 300.
const int kBackoffMs[] = {2000, 4000, 8000, 15000, 30000, 60000, 120000, 300000};
const int kBackoffCount = static_cast<int>(sizeof(kBackoffMs) / sizeof(kBackoffMs[0]));
const int kDnsBackoffIdx = 4;   // the 30 s step; a DNS failure usually means "no network"

// +/-20 % jitter. Mandatory, not cosmetic: the server scans on a fixed 60 s
// timer and unjittered clients resynchronise into a herd.
int jitteredMs(int ms) {
    const int span = ms / 5;
    if (span <= 0) {
        return ms;
    }
    return ms - span + static_cast<int>(QRandomGenerator::global()->bounded(2 * span + 1));
}

}

QRosterLink::QRosterLink(QObject *parent) :
    QObject(parent)
{
    _host = QStringLiteral(GAME_CASTER_HOST);
    _port = GAME_CASTER_PORT;

    _deadline.setSingleShot(true);
    _reconnect.setSingleShot(true);
    _keepalive.setInterval(kKeepaliveMs);

    connect(&_deadline, &QTimer::timeout, this, &QRosterLink::onDeadline);
    connect(&_reconnect, &QTimer::timeout, this, &QRosterLink::onReconnect);
    connect(&_keepalive, &QTimer::timeout, this, &QRosterLink::onKeepalive);
}

QRosterLink::~QRosterLink() {
    _reconnectAllowed = false;
    _reconnect.stop();
    // Empty failCode: a dying object must not emit.
    resetSocket(Down, QString());
}

void QRosterLink::setEndpoint(const QString &host, quint16 port) {
    _host = host;
    _port = port;
}

void QRosterLink::setProxy(const QNetworkProxy &p) {
    // [FIX-9] A raw QSslSocket uses QNetworkProxy::applicationProxy(), which this
    // app never sets: only _nam gets applyProxySettings() (qcore.cpp:246-261).
    _proxy = p;
}

// Rows 1, 2, 3, 4, 38.
void QRosterLink::start(const QString &login, const QString &cookieHeader, const QString &token) {
    if (_stickyDisabled) {                                  // row 37
        qDebug() << "QRosterLink::start ignored, sticky disabled";
        return;
    }
    // Row 4: never send AUTH<TAB>name<TAB> with an empty cookie. validCookie
    // rejects it and fail_auth burns one of kMaxStrikes=5 AND a per-IP slot.
    if (login.isEmpty() || (cookieHeader.isEmpty() && token.isEmpty())) {
        qDebug() << "QRosterLink::start no usable credentials, staying on HTTP";
        return;
    }
    const QString key = login.toLower();
    if (_state == Disabled) {                               // row 38, non-sticky
        qDebug() << "QRosterLink::start lifting non-sticky Disabled";
        _state = Down;
    }
    if (key != _loginKey) {                                 // row 2
        resetSocket(Down, QStringLiteral("transport"));
        _turnQueue.clear();                                 // moves of the previous account
        _deltaCapable = false;
        _wrongPlayer = 0;
        _canonical.clear();
    } else if (_state == Ready || _state == Connecting || _state == Handshaking) {   // row 3
        _cookie = cookieHeader;                             // refresh the cookie only
        if (!token.isEmpty()) {
            _token = token;
        }
        _reconnectAllowed = true;
        return;                                             // NO reconnect
    }
    _login = login;
    _loginKey = key;
    _cookie = cookieHeader;
    _token = token;
    _reconnectAllowed = true;
    _authRetries = 0;
    qDebug() << "QRosterLink::start" << _host << _port << "token" << redact(_token);
    if (_state == Down) {                                   // row 1
        _reconnect.stop();
        openSocket();
    }
}

// Row 32.
void QRosterLink::stop() {
    _reconnectAllowed = false;
    _reconnect.stop();
    resetSocket(Down, QStringLiteral("transport"));
}

// Rows 33, 34.
void QRosterLink::clearIdentity() {
    _reconnectAllowed = false;
    _reconnect.stop();
    if (_state == Ready && _deltaCapable && _pending == PNone && _sock) {
        // Best effort LOGOUT so the device token is revoked server side [FIX-10].
        // The socket dies on the OK reply, on disconnected(), or on the deadline.
        _pending = PLogout;
        _deadline.start(kLogoutDeadlineMs);
        sendLine("LOGOUT");
    } else {                                                // row 34
        resetSocket(Down, QStringLiteral("transport"));
    }
    _deltaCapable = false;
    _turnQueue.clear();
    _cookie.clear();
    _token.clear();
    _login.clear();
    _loginKey.clear();
    _canonical.clear();
}

// Rows 16, 20, 24, 26 and E.2 #12/#17/#18/#23/#24.
void QRosterLink::disableForSession(const QString &reason) {
    qDebug() << "QRosterLink::disableForSession" << reason;
    _stickyDisabled = true;
    _reconnectAllowed = false;
    _reconnect.stop();
    resetSocket(Disabled, QStringLiteral("transport"));
    _turnQueue.clear();                     // after resetSocket, which re-queues a TURN in flight
    emit protocolMismatch(reason);
}

// Rows 35, 36.
void QRosterLink::probeNow() {
    if (_state == Disabled || _stickyDisabled) {
        return;
    }
    const qint64 now = QDateTime::currentSecsSinceEpoch();
    if (_lastProbe != 0 && now - _lastProbe < kProbeDebounceSeconds) {
        return;                                             // debounced 30 s
    }
    if (_state == Ready && _pending == PNone) {             // row 35
        _lastProbe = now;
        _pending = PPing;
        _deadline.start(kPingDeadlineMs);
        sendLine("PING");
        return;
    }
    if (_state == Down && _reconnectAllowed) {              // row 36
        // resetBackoff() only when the port is not obviously filtered, otherwise
        // alt-tabbing on such a network buys a fresh 5 s connect attempt on every
        // focus change. Keep the escalated backoff and let _reconnect do its job.
        if (_connectTimeouts != 0) {
            return;
        }
        _lastProbe = now;
        resetBackoff();
        _reconnect.stop();
        openSocket();
    }
}

// Rows 18, 19.
bool QRosterLink::requestWho(const QString &epoch, quint64 sinceRevision) {
    if (!canRequestWho()) {
        return false;                                       // row 19: the caller goes to HTTP
    }
    // An empty or invalid epoch, or since==0, is how the caller expresses ForceFull.
    const bool full = (sinceRevision == 0) || !QRosterWire::isEpochHex(epoch);
    const QString wireEpoch = full ? QStringLiteral("-") : epoch;
    const quint64 since = full ? 0 : sinceRevision;

    _inBlock = false;
    _block = QRosterBlock();
    _blockBytes = 0;
    _pending = PWho;
    // ForceFull is user initiated only (the Hall of Fame button) and it is the
    // only caller that omits the watermark, so `full` is exactly the
    // user-initiated tier. Re-armed on every body line, so a large FULL that
    // keeps making progress never trips it.
    _deadline.start(full ? kWhoUserDeadlineMs : kWhoSilentDeadlineMs);

    QByteArray line = "WHO\t";
    line += wireEpoch.toUtf8();
    line += '\t';
    line += QByteArray::number(static_cast<qulonglong>(since));
    sendLine(line);
    // A dead socket surfaces as errorOccurred/disconnected (row 28) or as the
    // deadline (row 27). Either way EXACTLY ONE async rosterFailed follows. [FIX-1]
    return true;
}

void QRosterLink::sendTurn(const QString &opponent) {
    if (_stickyDisabled || _loginKey.isEmpty()) {
        return;
    }
    // Mirrors validName() on the server: a TURN it refuses goes through fail_auth,
    // costing one of kMaxStrikes=5 AND a slot of the per-IP auth budget. On top of
    // validWireName() that means no comma and no leading or trailing space.
    const QString name = opponent.trimmed();
    if (!QRosterWire::validWireName(name) || name.contains(QLatin1Char(',')) ||
        (name.toLower() == _loginKey)) {
        qDebug() << "QRosterLink::sendTurn skipped, unusable name" << name;
        return;
    }
    const qint64 now = QDateTime::currentSecsSinceEpoch();
    const int queued = queuedTurnIndex(name);
    if (queued != -1) {
        // One hint per opponent is enough, but it takes the time of the latest
        // move, or the age cap could drop it while that move is still fresh.
        _turnQueue[queued].second = now;
        return;
    }
    if (_turnQueue.size() >= kMaxQueuedTurns) {
        _turnQueue.removeFirst();
    }
    _turnQueue.append(qMakePair(name, now));
    flushTurns();
}

void QRosterLink::flushTurns() {
    // _deltaCapable as for WHO: a server too old for the 3-field WHO may not know
    // TURN either, and its "unknown command" would be a strike.
    if (_state != Ready || !_deltaCapable || _pending != PNone || !_sock) {
        return;
    }
    const qint64 now = QDateTime::currentSecsSinceEpoch();
    while (!_turnQueue.isEmpty()) {
        const QPair<QString, qint64> turn = _turnQueue.takeFirst();
        if (now - turn.second > kTurnMaxAgeSeconds) {
            continue;
        }
        // A pending TURN of its own, so its OK/ERR can never be taken for the reply
        // to a WHO, a PING or a LOGOUT written behind it.
        _pending = PTurn;
        _turnInFlight = turn;               // resetSocket() re-queues it if no reply comes
        _deadline.start(kTurnDeadlineMs);
        QByteArray line = "TURN\t";
        line += turn.first.toUtf8();
        sendLine(line);
        return;
    }
}

int QRosterLink::queuedTurnIndex(const QString &name) const {
    for (int i = 0; i < _turnQueue.size(); ++i) {
        if (_turnQueue.at(i).first.compare(name, Qt::CaseInsensitive) == 0) {
            return i;
        }
    }
    return -1;
}

void QRosterLink::openSocket() {
    _state = Connecting;
    _sock = new QSslSocket(this);
    _sock->setProxy(_proxy);                                   // [FIX-9]
    QSslConfiguration cfg = _sock->sslConfiguration();
    cfg.setPeerVerifyMode(QSslSocket::VerifyNone);
    _sock->setSslConfiguration(cfg);
    // The ONLY sslErrors handler. It does exactly this and nothing else.
    connect(_sock, &QSslSocket::sslErrors, this,
            [this](const QList<QSslError> &) { if (_sock) _sock->ignoreSslErrors(); });
    connect(_sock, &QSslSocket::encrypted,   this, &QRosterLink::onEncrypted);
    connect(_sock, &QSslSocket::readyRead,   this, &QRosterLink::onReadyRead);
    connect(_sock, &QAbstractSocket::errorOccurred, this, &QRosterLink::onSocketError);
    connect(_sock, &QAbstractSocket::disconnected, this, &QRosterLink::onDisconnected);
    _sock->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    _sock->connectToHostEncrypted(_host, _port);
    _deadline.start(kConnectDeadlineMs);
}

// [FIX-1] THE ONLY TEARDOWN PRIMITIVE.
void QRosterLink::resetSocket(State next, const QString &failCode) {
    const bool hadWho = (_pending == PWho);
    if ((_pending == PTurn) && !_turnInFlight.first.isEmpty() &&
        (queuedTurnIndex(_turnInFlight.first) == -1)) {
        // The TURN got no reply (deadline, socket error, a server that idle-closed
        // us while the app was suspended): send it again after the reconnect,
        // unless a newer move against that player is queued already. The age cap
        // in flushTurns() bounds the retries.
        _turnQueue.prepend(_turnInFlight);
    }
    _turnInFlight = QPair<QString, qint64>();
    _pending = PNone;                       // cleared BEFORE anything can re-enter
    _inBlock = false; _block = QRosterBlock(); _blockBytes = 0;
    _rx.clear();
    _deadline.stop(); _keepalive.stop();
    _tokenPromoted = false; _reauthedThisConn = false;
    if (_sock) {
        _sock->disconnect(this);            // no slot of ours can fire again
        _sock->abort();
        _sock->deleteLater();
        _sock = nullptr;
    }
    _deltaCapable = false;
    _state = next;
    if (hadWho && !failCode.isEmpty()) {
        emit rosterFailed(failCode);        // LAST, with state fully consistent
    }
}

void QRosterLink::sendLine(const QByteArray &lineNoNewline) {
    if (!_sock) {
        return;
    }
    // LOGGING RULE (D.9): the verb and _pending, never the payload.
    const int tab = lineNoNewline.indexOf('\t');
    qDebug() << "QRosterLink::sendLine" << (tab < 0 ? lineNoNewline : lineNoNewline.left(tab))
             << "pending" << _pending;
    QByteArray out = lineNoNewline;
    out.append('\n');
    _sock->write(out);
    _lastTx = QDateTime::currentSecsSinceEpoch();
    // A write failure is NOT handled here: it must never fire synchronously out
    // of requestWho(). It surfaces asynchronously through errorOccurred /
    // disconnected (row 28) or through the step deadline. [FIX-1]
}

// Row 6.
void QRosterLink::sendHandshake() {
    if (!_sock) {
        return;
    }
    _state = Handshaking;
    if (!_token.isEmpty()) {
        _pending = PLogin;
        _deadline.start(kLoginDeadlineMs);
        QByteArray line = "LOGIN\t";
        line += _token.toUtf8();
        sendLine(line);
        return;
    }
    if (_cookie.isEmpty()) {                // row 4, defensively: never AUTH bare
        resetSocket(Down, QString());
        return;
    }
    _pending = PAuth;
    _deadline.start(kAuthDeadlineMs);
    QByteArray line = "AUTH\t";
    line += _login.toUtf8();
    line += '\t';
    line += _cookie.toUtf8();
    sendLine(line);
}

void QRosterLink::onEncrypted() {
    if (!_sock) {
        return;
    }
    // resetBackoff() is DELIBERATELY NOT called here, nor on a successful TCP
    // connect: the server closes post-accept at kMaxSessions=64, and resetting
    // here turns that into a 2 s hot loop against an overloaded server (E.2 #5).
    sendHandshake();
}

void QRosterLink::onReadyRead() {
    if (!_sock) {
        return;
    }
    _lastRx = QDateTime::currentSecsSinceEpoch();
    _rx.append(_sock->readAll());
    int nl;
    while ((nl = _rx.indexOf('\n')) >= 0) {
        QByteArray line = _rx.left(nl);
        _rx.remove(0, nl + 1);
        if (line.endsWith('\r')) line.chop(1);
        handleLine(line);
        if (!_sock) return;                 // a handler tore the socket down
    }
    if (_rx.size() > kMaxRxBytes) {
        resetSocket(Down, QStringLiteral("transport"));
        bumpBackoff();
        scheduleReconnect();
        return;
    }
    // Every reply that frees the link (OK of a handshake, END, PONG, a TURN's own
    // OK/ERR) arrives here, so this is where a queued TURN goes out.
    flushTurns();
}

// Rows 7, 28 and E.2 #2/#3/#5/#6/#21/#22.
void QRosterLink::onSocketError(QAbstractSocket::SocketError e) {
    if (!_sock) {
        return;
    }
    qDebug() << "QRosterLink::onSocketError" << e << "state" << _state << "pending" << _pending;
    if (e == QAbstractSocket::HostNotFoundError && _backoffIdx < kDnsBackoffIdx) {
        _backoffIdx = kDnsBackoffIdx;       // E.2 #2: no DNS usually means no network
    }
    resetSocket(Down, QStringLiteral("transport"));
    bumpBackoff();
    scheduleReconnect();
}

// Row 28.
void QRosterLink::onDisconnected() {
    if (!_sock) {
        return;
    }
    qDebug() << "QRosterLink::onDisconnected state" << _state << "pending" << _pending;
    resetSocket(Down, QStringLiteral("transport"));
    bumpBackoff();
    scheduleReconnect();
}

// Rows 7, 17, 27, 31, 33, 35.
void QRosterLink::onDeadline() {
    qDebug() << "QRosterLink::onDeadline state" << _state << "pending" << _pending;
    if (_pending == PLogout) {                              // row 33, best effort
        resetSocket(Down, QString());
        return;
    }
    if (_pending == PPing) {
        // A probe PING (row 35) stamps _lastProbe immediately before sending, a
        // keepalive PING (row 31) never touches it. That is how the two are told
        // apart without a further member.
        const qint64 now = QDateTime::currentSecsSinceEpoch();
        const bool wasProbe = (_lastProbe != 0 && now - _lastProbe <= 5);
        resetSocket(Down, QStringLiteral("timeout"));
        if (wasProbe) {
            if (_connectTimeouts == 0) {
                resetBackoff();
            }
            scheduleReconnect(0);                           // immediate reconnect
        } else {
            bumpBackoff();
            scheduleReconnect();
        }
        return;
    }
    if (_state == Connecting) {                             // row 7
        resetSocket(Down, QStringLiteral("transport"));
        bumpBackoff();
        if (++_connectTimeouts >= kMaxConnectTimeouts) {
            _backoffIdx = kBackoffCount - 1;                // port 7777 is filtered here
            qDebug() << "QRosterLink::onDeadline" << _connectTimeouts
                     << "connect timeouts, pinned to the 300 s cap";
        }
        scheduleReconnect();
        return;
    }
    if (_state == Handshaking) {                            // row 17
        resetSocket(Down, QString());
        bumpBackoff();
        scheduleReconnect();
        return;
    }
    // Row 27 [FIX-5]: a WHO deadline TEARS THE SOCKET DOWN. No socket means no
    // late reply, so no double rosterFailed, no second HTTP fetch, no late apply
    // racing an HTTP reply, and no malformed strike from ordinary latency.
    // A TURN deadline takes the same path: its late OK/ERR would otherwise be
    // taken for the reply to whatever request went out next.
    resetSocket(Down, QStringLiteral("timeout"));
    bumpBackoff();
    scheduleReconnect();
}

// Rows 5, 11, 12.
void QRosterLink::onReconnect() {
    if (_state == Handshaking && _sock) {
        // ERR busy / site-unreachable: the server kept the socket open and
        // counted no strike, so re-send the same command in place.
        sendHandshake();
        return;
    }
    if (_state != Down || !_reconnectAllowed || _stickyDisabled) {
        return;
    }
    openSocket();
}

// Row 31.
void QRosterLink::onKeepalive() {
    if (_state != Ready || _pending != PNone) {
        return;
    }
    if (QDateTime::currentSecsSinceEpoch() - _lastTx < kKeepaliveSeconds) {
        return;
    }
    _pending = PPing;
    _deadline.start(kPingDeadlineMs);
    sendLine("PING");
}

void QRosterLink::handleLine(const QByteArray &line) {
    if (line.isEmpty()) {
        return;
    }
    const QList<QByteArray> f = QRosterWire::fields(line);
    if (f.isEmpty()) {
        return;
    }

    // Row 24: discard the whole block, fail this request, count a strike.
    const auto structuralFailure = [this](const char *why) {
        qDebug() << "QRosterLink::handleLine structural failure:" << why;
        const bool hadWho = (_pending == PWho);
        _inBlock = false;
        _block = QRosterBlock();
        _blockBytes = 0;
        _pending = PNone;
        _deadline.stop();
        if (hadWho) {
            emit rosterFailed(QStringLiteral("malformed"));
        }
        if (++_malformed >= kMaxMalformed) {
            disableForSession(QStringLiteral("protocol"));
        }
    };

    const QByteArray &verb = f.at(0);

    // Row 29. Legal ANYWHERE, including inside a block and as the first line read
    // after a WHO is written. Dispatch is on field[0], never on request order.
    if (verb == "TURNREADY") {
        if (f.size() == 2) {
            emit turnReady(QString::fromUtf8(f.at(1)));
        }
        return;
    }
    // Row 30.
    if (verb == "PONG") {
        if (_pending == PPing) {
            _pending = PNone;
            _deadline.stop();
        }
        return;
    }

    if (_inBlock) {
        if (verb == "W") {                                  // row 21
            _blockBytes += line.size();
            if (_blockBytes > kMaxBlockBytes) {
                structuralFailure("block exceeds 8 MiB");
                return;
            }
            QRosterRow r;
            if (QRosterWire::parseRow(f, &r)) {
                _block.rows.append(r);
            } else {
                ++_block.badRows;           // [FIX-6] a bad row is data, not a protocol break
            }
            ++_block.bodyLines;             // EITHER WAY
            _deadline.start(_deadline.interval());
            return;
        }
        if (verb == "D") {                                  // row 22
            _blockBytes += line.size();
            if (_blockBytes > kMaxBlockBytes) {
                structuralFailure("block exceeds 8 MiB");
                return;
            }
            QString name;
            if (QRosterWire::parseRemoved(f, &name)) {
                _block.removed.append(name);
            } else {
                ++_block.badRows;
            }
            ++_block.bodyLines;
            _deadline.start(_deadline.interval());
            return;
        }
        if (verb == "END") {                                // row 23
            // parseEnd enforces bodyLines == declaredCount AND that the END
            // epoch/revision equal the ones from the header.
            if (!QRosterWire::parseEnd(f, &_block)) {
                structuralFailure("bad END");
                return;
            }
            const QRosterBlock done = _block;
            _inBlock = false;
            _block = QRosterBlock();
            _blockBytes = 0;
            _pending = PNone;
            _deadline.stop();
            _malformed = 0;
            qDebug() << "QRosterLink::handleLine block" << (done.full ? "FULL" : "DELTA")
                     << done.rows.size() << "rows," << done.badRows << "bad, revision"
                     << done.revision;
            emit rosterBlock(done);
            return;
        }
        structuralFailure("unexpected verb inside a block");
        return;
    }

    if (verb == "WHO") {                                    // row 20
        if (_pending != PWho) {
            structuralFailure("unsolicited WHO header");
            return;
        }
        if (f.size() == 2) {
            // Legacy 1-field-reply server. It cannot serve a delta at all.
            disableForSession(QStringLiteral("legacy WHO reply shape"));
            return;
        }
        if (!QRosterWire::parseHeader(f, &_block)) {
            structuralFailure("bad WHO header");
            return;
        }
        _inBlock = true;
        _blockBytes = line.size();
        _deadline.start(_deadline.interval());
        return;
    }
    if (verb == "OK") {
        handleOk(f);
        return;
    }
    if (verb == "ERR") {
        handleErr(f);
        return;
    }
    structuralFailure("unknown verb");
}

// Rows 8, 9 and the LOGOUT ack of row 33.
void QRosterLink::handleOk(const QList<QByteArray> &f) {
    if (_pending == PTurn) {                                // OK<TAB>delivered | OK<TAB>offline
        qDebug() << "QRosterLink::handleOk turn" << (f.size() > 1 ? f.at(1) : QByteArray());
        _pending = PNone;
        _turnInFlight = QPair<QString, qint64>();
        _deadline.stop();
        return;
    }
    if (_pending == PLogout) {                              // OK<TAB>bye
        qDebug() << "QRosterLink::handleOk logout acknowledged";
        resetSocket(Down, QString());
        return;
    }
    if (_pending != PAuth && _pending != PLogin) {
        return;                                             // unsolicited, ignore
    }
    const bool fromAuth = (_pending == PAuth);
    const int wide = fromAuth ? 5 : 4;                      // capability probe (A.3)
    const int narrow = fromAuth ? 3 : 2;
    if (f.size() != wide && f.size() != narrow) {
        disableForSession(QStringLiteral("unexpected OK shape"));
        return;
    }
    _canonical = QString::fromUtf8(f.at(1));
    if (fromAuth) {
        _token = QString::fromUtf8(f.at(2));
    }
    // [FIX-2] ONLY the field count is used. f[3]/f[4] carry the epoch and
    // revision of the server and are DELIBERATELY DISCARDED: adopting them would
    // tell a fresh install with an empty _playerStats that it is already caught
    // up, it would get n=0 DELTA forever and show an empty Hall of Fame with no
    // error anywhere.
    _deltaCapable = (f.size() == wide);

    _pending = PNone;
    _deadline.stop();
    _state = Ready;
    resetBackoff();                     // HERE AND ONLY HERE (plus the probe of row 36)
    _connectTimeouts = 0;
    _authRetries = 0;
    _keepalive.start(kKeepaliveMs);
    qDebug() << "QRosterLink::handleOk ready as" << _canonical << "delta" << _deltaCapable
             << "token" << redact(_token);
    emit authenticated(_canonical, _token);
    if (!_deltaCapable) {                                   // row 9
        emit protocolMismatch(QStringLiteral("server has no delta WHO"));
    }
}

// Rows 10-16, 25, 26.
void QRosterLink::handleErr(const QList<QByteArray> &f) {
    const QString code = f.size() > 1 ? QString::fromUtf8(f.at(1)) : QStringLiteral("unknown");
    qDebug() << "QRosterLink::handleErr" << code << "state" << _state << "pending" << _pending;

    if (_pending == PWho) {
        _pending = PNone;
        _deadline.stop();
        _inBlock = false;
        _block = QRosterBlock();
        _blockBytes = 0;
        if (code == QLatin1String("need-auth")) {           // row 25
            emit rosterFailed(QStringLiteral("err:need-auth"));
            if (!_reauthedThisConn) {
                _reauthedThisConn = true;
                sendHandshake();                            // once per connection
            } else {
                resetSocket(Down, QString());
                bumpBackoff();
                scheduleReconnect();
            }
            return;
        }
        emit rosterFailed(QStringLiteral("err:") + code);
        if (code == QLatin1String("bad-request") || code == QLatin1String("line-too-long")) {
            // Row 26: each retry costs one of kMaxStrikes=5 and a per-IP slot, so
            // a new client against an old server would throttle itself off entirely.
            disableForSession(QStringLiteral("WHO ") + code);
        }
        return;
    }
    if (_pending == PTurn) {
        _pending = PNone;
        _turnInFlight = QPair<QString, qint64>();
        _deadline.stop();
        if (code == QLatin1String("need-auth")) {           // as for WHO, row 25
            if (!_reauthedThisConn) {
                _reauthedThisConn = true;
                sendHandshake();
            } else {
                resetSocket(Down, QString());
                bumpBackoff();
                scheduleReconnect();
            }
        }
        // Anything else drops only this hint. bad-request can only mean sendTurn's
        // name check drifted from validName(), and every retry would be a strike.
        return;
    }
    if (_pending == PLogout) {
        resetSocket(Down, QString());
        return;
    }
    if (_pending != PAuth && _pending != PLogin) {
        return;                                             // nothing outstanding
    }

    if (code == QLatin1String("bad-token")) {               // row 10
        _token.clear();
        emit tokenRejected();
        if (!_tokenPromoted && !_cookie.isEmpty()) {
            _tokenPromoted = true;
            sendHandshake();            // AUTH on the SAME open socket, once per connection
        } else {
            resetSocket(Down, QString());
            bumpBackoff();
            scheduleReconnect();
        }
        return;
    }
    if (code == QLatin1String("busy") || code == QLatin1String("site-unreachable")) {
        // Rows 11, 12. The server did NOT close and did NOT count a strike, so
        // throwing away a good TLS session would cost a handshake for nothing.
        if (++_authRetries >= kMaxAuthRetries) {
            resetSocket(Down, QString());
            bumpBackoff();
            scheduleReconnect();
            return;
        }
        _deadline.stop();
        _reconnect.start(jitteredMs(code == QLatin1String("busy") ? kBusyRetryMs : kSiteRetryMs));
        return;
    }
    if (code == QLatin1String("not-logged-in") || code == QLatin1String("wrong-player")) {
        // Rows 13, 14. Both go through fail_auth, so both burn a strike AND the
        // per-IP budget. NEVER retry: only the next successful finishLogin revives us.
        _cookie.clear();
        _token.clear();
        _reconnectAllowed = false;
        _reconnect.stop();
        if (code == QLatin1String("wrong-player") && ++_wrongPlayer >= kMaxWrongPlayer) {
            disableForSession(QStringLiteral("wrong-player"));
            _stickyDisabled = false;    // row 14: NON sticky, the next start() lifts it
        } else {
            resetSocket(Down, QString());
        }
        emit needCookieRefresh();
        return;
    }
    if (code == QLatin1String("rate-limit")) {              // row 15
        resetSocket(Down, QString());
        scheduleReconnect(kRateLimitMs);   // >= 11 min, anything faster re-arms the window
        return;
    }
    if (code == QLatin1String("bad-request") || code == QLatin1String("line-too-long")) {
        disableForSession(QStringLiteral("handshake ") + code);   // row 16, pure client bug
        return;
    }
    // Anything else during the handshake: treat as transport and back off.
    resetSocket(Down, QString());
    bumpBackoff();
    scheduleReconnect();
}

void QRosterLink::scheduleReconnect(int minMs) {
    if (!_reconnectAllowed || _stickyDisabled || _state == Disabled) {
        return;
    }
    int ms = jitteredMs(kBackoffMs[_backoffIdx]);
    if (minMs > 0 && ms < minMs) {
        // Never dip BELOW a floor: rate-limit needs >= 11 min or the 10 minute
        // window of the server simply re-arms. Jitter upward only.
        ms = minMs + static_cast<int>(QRandomGenerator::global()->bounded(minMs / 5 + 1));
    }
    _reconnect.start(ms);
}

void QRosterLink::bumpBackoff() {
    if (_backoffIdx < kBackoffCount - 1) {
        ++_backoffIdx;
    }
}

void QRosterLink::resetBackoff() {
    _backoffIdx = 0;
}

QString QRosterLink::redact(const QString &secret) {
    if (secret.isEmpty()) {
        return QStringLiteral("<empty>");
    }
    return QStringLiteral("<%1B:%2…>").arg(secret.size()).arg(secret.left(4));
}
