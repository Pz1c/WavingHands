#ifndef QROSTERLINK_H
#define QROSTERLINK_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QNetworkProxy>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslError>
#include <QLoggingCategory>

#include "qrosterwire.h"

// "Push": the logcat tag of everything about the FCM token registration of issue #336, in
// the link (PUSH / UNPUSH written, the reply) and in the core (when and why). Tokens only
// ever go through redact().
Q_DECLARE_LOGGING_CATEGORY(lcPush)

// One open challenge of the site's /challenges page as a CHAL reply carries it (a "C" line):
// the cells the app's own parser reads from the page, see QWarlockUtils::makeChallenge.
struct QLinkChallenge {
    int battleId = 0;
    int level = 0;              // 0 ladder, 1 friendly, 2 very friendly
    bool fast = false;
    int need = 0;               // the N of "Need N more"
    QStringList names;          // participants in page order
    QString description;        // the third cell, HTML entities untouched
};

// The ONLY place a socket lives. Deliberately does NOT derive from QGameCore:
// QSslSocket::sslErrors(QList<QSslError>) has the SAME signature as the
// inherited QWarloksDuelCore::slotSslErrors(QList<QSslError>)
// (qwarloksduelcore.cpp:2103-2112), so a careless connect() would compile
// cleanly and pop a modal "Sll error details" dialog on the self-signed
// CN=games.8uy.at cert via main.qml:89 -> showErrorMessage (main.qml:804-824).
// Not inheriting makes that mistake impossible. It also structurally cannot
// touch _errorMsg, setIsLoading(), _reply, _lastRequestType/Url/Data, or
// QGameCore::saveRequest() (which writes request bodies to disk under QT_DEBUG,
// qcore.cpp:32-43).
//
// LOGGING RULE (non-negotiable, see D.9): no method of this class may ever pass
// _cookie or a device token to qDebug(). Use redact().
class QRosterLink : public QObject {
    Q_OBJECT
public:
    enum State { Disabled, Down, Connecting, Handshaking, Ready };
    enum Pending { PNone, PAuth, PLogin, PWho, PPing, PLogout, PTurn, PPush, PUnpush, PCaps, PLive, PChal, PWatch };

    explicit QRosterLink(QObject *parent = nullptr);
    ~QRosterLink() override;

    State state() const                 { return _state; }
    bool  deltaCapable() const          { return _state == Ready && _deltaCapable; }
    bool  canRequestWho() const         { return _state == Ready && _deltaCapable && _pending == PNone; }
    const QString &canonicalName() const{ return _canonical; }

    void setEndpoint(const QString &host, quint16 port);   // default GAME_CASTER_HOST/PORT
    void setProxy(const QNetworkProxy &p);                 // [FIX-9] mirror QGameCore's _nam proxy

    // Identity + bootstrap. `cookieHeader` is a full Cookie HEADER VALUE
    // ("a=b; c=d") for https://games.ravenblack.net/player. `token` may be
    // empty. A call with a different login is an implicit clearIdentity()
    // first. Safe to call repeatedly.
    void start(const QString &login, const QString &cookieHeader, const QString &token);

    // Teardown. clearIdentity() also sends LOGOUT if the socket is Ready and a
    // token was in use, then forgets cookie + token and stops auto-reconnect.
    void stop();                    // silent teardown, keeps identity, allows reconnect
    void clearIdentity();           // logout / account switch: no identity left to AUTH as
    void disableForSession(const QString &reason);   // sticky until app restart
    void probeNow();                // app resume / network change: immediate PING or reconnect

    // Sends WHO. Pass an empty/invalid epoch or since==0 for a FULL.
    // Returns false if the link cannot serve it -- the caller MUST then use HTTP.
    // Returning true guarantees that EXACTLY ONE of rosterBlock()/rosterFailed()
    // is emitted, always ASYNCHRONOUSLY (never from inside this call).  [FIX-1]
    bool requestWho(const QString &epoch, quint64 sinceRevision);

    // "I just made my move against <opponent>": the server pushes TURNREADY to
    // every session of that player. Best effort: queued until the link is Ready
    // and idle, one TURN per round trip, dropped when stale or on an identity
    // change. A name the server's validName() would refuse is never sent.
    void sendTurn(const QString &opponent);

    // Push notifications (https://github.com/Pz1c/WavingHands/issues/336): bind or
    // unbind this install's FCM token to the session's player. One wanted slot each,
    // the latest token wins; both survive reconnects, are cleared by clearIdentity(),
    // and UNPUSH goes out before PUSH (a rotation unbinds the old token first). The
    // flush shares the TURN discipline: Ready, delta capable, idle, one request per
    // round trip, a deadline tears the socket down and the slot is sent again after
    // the reconnect. A token the server's rule would refuse is never sent, and after
    // one bad-request (an old server, a strike each) nothing push-related goes out
    // on that connection again.
    void sendPush(const QString &token);
    void sendUnpush(const QString &token);
    bool isPushUnsupported() const      { return _pushUnsupported; }   // this connection refused PUSH / UNPUSH
    static bool validPushToken(const QString &token);   // 20..4096 bytes of [A-Za-z0-9_:.-]

    // Live matches (https://github.com/Pz1c/WavingHands/issues/287). Every connection
    // declares the app's "live" capability first (CAPS), which is what marks the player
    // as invitable in the other apps' rosters and lets the server push LIVEINVITE here.
    // sendLive(): "I have just invited <opponent> to the live match <battleId>", so the
    // server tells their app (or their phone) now. Best effort with the TURN discipline:
    // queued until the link is Ready and idle, dropped once the invite is older than a
    // minute, on an identity change, or by a server that knows no live matches.
    void sendLive(const QString &opponent, int battleId);
    bool isLiveSupported() const        { return _state == Ready && _liveSupported; }
    static QString redact(const QString &secret);       // "<163B:dXkaQ2Pq...>", the only way a token is logged

    // Fewer requests to the site (server 1.4.0). CAPS declares "chal", "watch" and "fin" next
    // to "live"; a server of today accepts only "live", and a command it does not know costs
    // a strike, so each of these is used only once its name is in the CAPS reply.
    // "chal": the site's open challenges, one copy the server fetches for every app. Returns
    // false when this connection cannot serve them (the caller reads the page itself);
    // otherwise EXACTLY ONE of challenges() / challengesFailed() follows, asynchronously.
    // Requests made while one is waiting are answered by the same reply.
    bool requestChallenges();
    // "watch": whom the player waits for and the battles still running, so the server can say
    // when one of those warlocks was active on the site (ACTIVE). One wanted slot like PUSH:
    // the latest lists win, they go out again after a reconnect, and lists equal to the ones
    // this connection already sent are not sent again. Names the server would refuse are
    // left out; past 32 names and 64 battles the rest is cut.
    void sendWatch(const QStringList &opponents, const QList<int> &battles);
    bool isChalSupported() const        { return _state == Ready && _chalSupported; }
    bool isWatchSupported() const       { return _state == Ready && _watchSupported; }
    // "fin": the server sends this install a "finished" FCM message (with the PUSH of its token).
    bool isFinSupported() const         { return _state == Ready && _finSupported; }

signals:
    // NOTE: carries only (name, token). The epoch/revision on the OK line are
    // deliberately NOT propagated -- see FIX-2.
    void authenticated(const QString &canonicalName, const QString &deviceToken);
    void rosterBlock(const QRosterBlock &block);
    void rosterFailed(const QString &code);   // timeout|malformed|transport|err:<code>
    void tokenRejected();                     // drop the cached token for this account
    void needCookieRefresh();                 // not-logged-in / wrong-player
    void protocolMismatch(const QString &reason);   // sticky-disable happened
    void turnReady(const QString &byName);
    // PUSH / UNPUSH acknowledged by the server, or refused as an unknown command.
    void pushRegistered(const QString &token);
    void pushUnregistered(const QString &token);
    void pushUnsupported();
    // Any other ERR to a PUSH / UNPUSH: the request is dropped, reason = "<code>: <text>".
    void pushFailed(const QString &token, const QString &reason);
    // LIVEINVITE: <byName> invited this player to the live match <battleId>.
    void liveInvite(const QString &byName, int battleId);
    // The server's answer to a LIVE: delivered | pushed | offline | busy | <ERR code>.
    void liveSent(const QString &opponent, int battleId, const QString &result);
    // What the CAPS reply allowed changed: it came in, or the connection that had it is gone.
    // Queued, so it never runs inside a teardown.
    void capsChanged();
    // ACTIVE: <byName>, whom the last WATCH named, was active on the site since the server's
    // previous roster scan.
    void active(const QString &byName);
    // The answer to requestChallenges(): the challenges in page order and the age in seconds
    // of the server's copy, or why there are none (timeout|malformed|transport|err:<code>).
    void challenges(const QList<QLinkChallenge> &list, int ageSeconds);
    void challengesFailed(const QString &code);

private slots:
    void onEncrypted();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError e);
    void onDisconnected();
    void onDeadline();
    void onReconnect();
    void onKeepalive();

private:
    void openSocket();
    // THE ONLY TEARDOWN PRIMITIVE. Every path that ends a socket goes through
    // it. Order is fixed: capture hadWho -> clear ALL per-request state ->
    // kill timers -> abort+deleteLater the socket -> set the new state ->
    // emit rosterFailed LAST. This is what makes "in-flight never leaks" a
    // structural property rather than a review item.  [FIX-1]
    void resetSocket(State next, const QString &failCode);
    void sendLine(const QByteArray &lineNoNewline);
    void sendHandshake();
    void flushTurns();              // sends the oldest queued TURN if Ready and idle
    int  queuedTurnIndex(const QString &name) const;   // case-insensitive, -1 if absent
    void flushPush();               // sends the wanted UNPUSH, else the wanted PUSH, if Ready and idle
    void flushLive();               // sends the wanted CAPS, else the oldest queued LIVE, if Ready and idle
    void flushChal();               // sends the wanted CHAL if Ready, idle and accepted
    void flushWatch();              // sends the wanted WATCH likewise, unless this connection sent it
    bool handleChalLine(const QList<QByteArray> &f, const QByteArray &line);   // false: out of shape
    void handleLine(const QByteArray &line);
    void handleOk(const QList<QByteArray> &f);
    void handleErr(const QList<QByteArray> &f);
    void scheduleReconnect(int minMs = 0);
    void bumpBackoff();
    void resetBackoff();

    QSslSocket *_sock = nullptr;
    QString  _host; quint16 _port = 0;
    QNetworkProxy _proxy;

    State   _state = Down;
    Pending _pending = PNone;
    bool    _deltaCapable = false;
    bool    _reconnectAllowed = false;
    bool    _stickyDisabled = false;

    QString _login, _loginKey, _cookie, _token, _canonical;

    // sendTurn() backlog: opponent name + enqueue time (secs since epoch),
    // oldest first. Survives reconnects, not identity changes.
    QList<QPair<QString, qint64>> _turnQueue;
    // The TURN written and still waiting for its OK/ERR (valid while PTurn).
    QPair<QString, qint64> _turnInFlight;
    // sendPush() / sendUnpush() slots. A slot stays set while its request is in
    // flight (_pushInFlight, valid while PPush / PUnpush) and is cleared by the
    // reply, so a deadline or a dropped socket sends it again after the reconnect.
    QString _pushWanted;
    QString _unpushWanted;
    QString _pushInFlight;
    bool    _pushUnsupported = false;   // bad-request to PUSH/UNPUSH on this connection
    // Live matches. _capsWanted: this connection has not declared "live" yet;
    // _liveSupported: the server took it (cleared with the connection, like
    // _pushUnsupported). sendLive() backlog and the LIVE in flight (valid while PLive).
    struct LiveInvite {
        QString opponent;
        int     battleId = 0;
        qint64  queued = 0;         // secs since epoch
    };
    bool    _capsWanted = false;
    bool    _liveSupported = false;
    QList<LiveInvite> _liveQueue;
    LiveInvite _liveInFlight;
    // The other names of the CAPS reply, cleared with the connection like _liveSupported.
    bool    _chalSupported = false;
    bool    _watchSupported = false;
    bool    _finSupported = false;
    // CHAL: asked for and not written yet; the block being read (valid while PChal).
    bool    _chalWanted = false;
    bool    _chalInBlock = false;
    int     _chalDeclared = 0;
    int     _chalAge = 0;
    int     _chalLines = 0;
    QList<QLinkChallenge> _chalRows;
    // WATCH: the line wanted, the one this connection sent last (cleared with it), the one
    // waiting for its OK (valid while PWatch).
    QByteArray _watchWanted;
    QByteArray _watchSent;
    QByteArray _watchInFlight;
    bool    _dying = false;         // the destructor runs: nothing may be emitted any more

    QByteArray  _rx;
    bool        _inBlock = false;
    QRosterBlock _block;
    qint64      _blockBytes = 0;

    QTimer _deadline;    // single-shot; whatever step is pending
    QTimer _reconnect;   // single-shot
    QTimer _keepalive;   // 240 s repeating
    int    _backoffIdx = 0;
    int    _connectTimeouts = 0;   // 3 => jump to the 300 s cap
    int    _malformed = 0;         // 3 => sticky Disabled; reset on any good block
    int    _wrongPlayer = 0;       // 2 for one account => Disabled
    bool   _tokenPromoted = false; // one bad-token -> AUTH promotion per connection
    bool   _reauthedThisConn = false;
    int    _authRetries = 0;       // busy / site-unreachable, max 3
    qint64 _lastTx = 0, _lastRx = 0, _lastProbe = 0;
};
#endif
