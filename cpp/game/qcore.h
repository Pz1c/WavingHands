#ifndef QGAMECORE_H
#define QGAMECORE_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>
#include <QtGlobal>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QNetworkProxy>
#include <QList>
#include <QPair>
#include <QUuid>
//#include <math.h>
#include "qgamedictionary.h"

class QGameCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString proxyHost READ proxyHost NOTIFY proxyHostChanged)
    Q_PROPERTY(int proxyPort READ proxyPort NOTIFY proxyPortChanged)
    Q_PROPERTY(QString proxyUser READ proxyUser NOTIFY proxyUserChanged)
    Q_PROPERTY(QString proxyPass READ proxyPass NOTIFY proxyPassChanged)
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(int isLoading READ isLoading NOTIFY isLoadingChanged)

public:
    explicit QGameCore(QObject *parent = 0);
    ~QGameCore();

    QString proxyHost();
    int proxyPort();
    QString proxyUser();
    QString proxyPass();
    QString uuid();
    bool isLoading();

    // Full "Cookie:" header value ("a=b; c=d") for url, or "" when the jar holds
    // nothing usable. QNetworkAccessManager's default jar already stores and
    // replays the site session cookie; this is the only way app code sees it.
    // Pass the exact URL the cookie must be valid for: the jar filters by host,
    // path and the secure flag, and _nam is shared with analytics.
    QString cookieHeaderFor(const QUrl &url) const;
    // The proxy currently applied to _nam, in a form a raw socket can use.
    QNetworkProxy currentProxy() const;


signals:
    void proxyHostChanged();
    void proxyPortChanged();
    void proxyUserChanged();
    void proxyPassChanged();
    void uuidChanged();
    void isLoadingChanged();

protected slots:

    // NAT
    virtual void slotReadyRead();
    virtual void slotError(QNetworkReply::NetworkError error);
    virtual void slotSslErrors(QList<QSslError> error_list);

    // A Background request neither holds the loading overlay nor reports its
    // failures: it is work the user did not ask for.
    QNetworkReply *sendPostRequest(const QString &url, const QByteArray &data, bool Background = false);
    QNetworkReply *sendGetRequest(const QString &url, bool Background = false);
    void resendLastRequest();
protected:
    void init();
    void setProxySettings(QString IP, int Port, QString Username, QString Password);
    void setIsLoading(bool isLoading);

    // Loading overlay bookkeeping. isLoading is one flag shared by every request,
    // so it may only drop once no foreground request is outstanding and nothing
    // else holds it (loadingHeld).
    static bool isBackground(QNetworkReply *reply);
    // Once per reply; true when it was a foreground request.
    bool releaseRequest(QNetworkReply *reply);
    void releaseLoading();
    virtual bool loadingHeld() const;
    // Schedules this same request again (never "the last one sent"), keeping its
    // share of the overlay. False once the attempts are used up.
    bool retryRequest(QNetworkReply *reply);
    // A retry of a request that does not depend on the logged-in account
    // survives an account change (see _sessionEpoch).
    virtual bool retryOutlivesSession(const QString &url) const;

    void saveParameters(bool user = false, bool proxy = false, bool game = false, bool stats = false, bool options = false);
    virtual void loadParameters();
    virtual void saveUserParameters();
    virtual void saveProxyParameters();
    virtual void saveGameParameters();
    virtual void saveStatsParameters();
    virtual void saveOptionsParameters();
    virtual void loadUserParameters();
    virtual void loadProxyParameters();
    virtual void loadGameParameters();
    virtual void loadStatsParameters();
    virtual void loadOptionsParameters();
    void setOrganization(const QString &OrgName, const QString &AppName);

    QString _organizationName;
    QString _applicationName;
    QSettings *settings;
    // dictionary
    QGameDictionary *GameDictionary;

    // network
    int _httpResponceCode;
protected:

    // system
    QString _UUID;
    QString _request_code;

    // proxy
    QString _proxyHost;
    int _proxyPort;
    QString _proxyUser;
    QString _proxyPass;

    // network
    bool _isLoading;
    int _requestIdx;
    QNetworkAccessManager _nam;
    QNetworkReply *_reply;
    QNetworkProxy _proxy;
    QList<QSslError> ignoredSslErrors;
    QString _lastRequestType;
    QString _lastRequestUrl;
    QByteArray _lastRequestData;
    // Foreground requests still outstanding (see releaseLoading).
    int _foregroundRequests;
    // Bumped whenever the account changes: a retry scheduled for the previous
    // one must not be sent on behalf of the new one.
    int _sessionEpoch;

    void applyProxySettings();
    void saveRequest(QString &data);

private:
    QNetworkReply *startRequest(const QString &Verb, const QString &Url, const QByteArray &Data, bool Background, int Attempt, bool Counted);
};

#endif // QGAMECORE_H
