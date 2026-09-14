#ifndef QROSTERLINK_H
#define QROSTERLINK_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QTimer>
#include <QNetworkProxy>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslError>

#include "qrosterwire.h"

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
    enum Pending { PNone, PAuth, PLogin, PWho, PPing, PLogout };

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

    void sendTurn(const QString &opponent);          // reserved, unused today

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
    void handleLine(const QByteArray &line);
    void handleOk(const QList<QByteArray> &f);
    void handleErr(const QList<QByteArray> &f);
    void scheduleReconnect(int minMs = 0);
    void bumpBackoff();
    void resetBackoff();
    static QString redact(const QString &secret);   // "<32B:9f3c...>"

    QSslSocket *_sock = nullptr;
    QString  _host; quint16 _port = 0;
    QNetworkProxy _proxy;

    State   _state = Down;
    Pending _pending = PNone;
    bool    _deltaCapable = false;
    bool    _reconnectAllowed = false;
    bool    _stickyDisabled = false;

    QString _login, _loginKey, _cookie, _token, _canonical;

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
