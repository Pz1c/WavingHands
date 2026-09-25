#include "qcore.h"

#include <QTimer>

namespace {

// Carried by every reply, so it can be retried and accounted for without any
// state outside the reply itself.
const char *const kPropBackground = "wh_background";
const char *const kPropReleased = "wh_released";
const char *const kPropVerb = "wh_verb";
const char *const kPropBody = "wh_body";
const char *const kPropAttempt = "wh_attempt";

// Inactivity bound. Background replies no longer clear the overlay, so without
// one a stalled request would keep the UI blocked until the OS gave up on it.
const int kRequestTimeoutMs = 30000;
const int kRetryDelayMs = 5000;
const int kMaxRetries = 3;

}

QGameCore::QGameCore(QObject *parent) :
    QObject(parent)
{
    _requestIdx = 0;
    _request_code = "dda";
    _httpResponceCode = 0;
    _isLoading = false;
    _reply = nullptr;
    _foregroundRequests = 0;
    _sessionEpoch = 0;

    ignoredSslErrors.clear();
    ignoredSslErrors.append(QSslError(QSslError::CertificateSignatureFailed));
    ignoredSslErrors.append(QSslError(QSslError::CertificateNotYetValid));
    ignoredSslErrors.append(QSslError(QSslError::CertificateExpired));
    ignoredSslErrors.append(QSslError(QSslError::SelfSignedCertificate));
    ignoredSslErrors.append(QSslError(QSslError::SelfSignedCertificateInChain));
    ignoredSslErrors.append(QSslError(QSslError::UnableToGetLocalIssuerCertificate));
    ignoredSslErrors.append(QSslError(QSslError::HostNameMismatch));
}

QGameCore::~QGameCore() {
    settings->sync();
    settings->deleteLater();
}

void QGameCore::init() {
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, _organizationName, _applicationName);
    loadParameters();
    applyProxySettings();
    GameDictionary = QGameDictionary::getInstance();
}


void QGameCore::saveRequest(QString &data) {
    qDebug() << "QGameCore::saveRequest" << data.length() << _httpResponceCode << _lastRequestType << _lastRequestUrl;
#ifdef QT_DEBUG
    QString file_name = QString("%1_request_%2.html").arg(_request_code, QString::number(++_requestIdx));
    QFile file(file_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "<!-- " << QString::number(_httpResponceCode) << " " << _lastRequestType << " " << _lastRequestUrl << " " << _lastRequestData << " -->\n";
    out << data;
    file.close();
    qDebug() << "data stored into " << file_name;
#endif
}

bool QGameCore::isLoading() {
    return _isLoading;
}

QString QGameCore::cookieHeaderFor(const QUrl &url) const {
    QNetworkCookieJar *jar = _nam.cookieJar();
    if (!jar) {
        return QString();
    }

    QStringList parts;
    foreach(QNetworkCookie c, jar->cookiesForUrl(url)) {
        parts.append(QString::fromUtf8(c.toRawForm(QNetworkCookie::NameAndValueOnly)));
    }
    QString header = parts.join("; ");

    // The caster server rejects an empty or malformed cookie, and a rejection
    // costs both a strike on the connection and a slot in its per-IP budget.
    // Never send one: an unusable cookie means "stay on the HTTP roster".
    QByteArray raw = header.toUtf8();
    if (raw.isEmpty() || raw.size() > 4096) {
        return QString();
    }
    foreach(char c, raw) {
        if ((c < 0x20) || (c > 0x7E)) {
            return QString();
        }
    }
    return header;
}

QNetworkProxy QGameCore::currentProxy() const {
    return _proxyHost.isEmpty() ? QNetworkProxy(QNetworkProxy::NoProxy) : _proxy;
}


void QGameCore::slotReadyRead() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        return;
    }
    reply->deleteLater();
    releaseRequest(reply);
    releaseLoading();

    if (reply->error() != QNetworkReply::NoError) {
        return;
    }
	
    QString url = reply->url().toString();
    QString data = reply->readAll();
    qDebug() << "from url: " << url << " slotReadyRead";
    if (!data.isEmpty()) {
        saveRequest(data);
    }

}

void QGameCore::slotError(QNetworkReply::NetworkError error) {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qDebug() << "slotError" << error << (reply ? reply->errorString() : QString());
}

void QGameCore::slotSslErrors(QList<QSslError> error_list) {
    qDebug() << "slotSslErrors " << error_list;
}

void QGameCore::resendLastRequest() {
    startRequest(_lastRequestType, _lastRequestUrl, _lastRequestData, false, 0, false);
}

bool QGameCore::isBackground(QNetworkReply *reply) {
    return reply && reply->property(kPropBackground).toBool();
}

bool QGameCore::releaseRequest(QNetworkReply *reply) {
    if (!reply || reply->property(kPropReleased).toBool()) {
        return false;
    }
    reply->setProperty(kPropReleased, true);
    if (isBackground(reply)) {
        return false;
    }
    if (_foregroundRequests > 0) {
        --_foregroundRequests;
    }
    return true;
}

void QGameCore::releaseLoading() {
    if ((_foregroundRequests == 0) && !loadingHeld()) {
        setIsLoading(false);
    }
}

bool QGameCore::loadingHeld() const {
    return false;
}

bool QGameCore::retryOutlivesSession(const QString &url) const {
    Q_UNUSED(url)
    return false;
}

bool QGameCore::retryRequest(QNetworkReply *reply) {
    int attempt = reply->property(kPropAttempt).toInt();
    if (attempt >= kMaxRetries) {
        return false;
    }
    QString verb = reply->property(kPropVerb).toString();
    QString url = reply->url().toString();
    QByteArray body = reply->property(kPropBody).toByteArray();
    bool background = isBackground(reply);
    // The retry inherits this reply's share of the overlay, so a user action
    // stays covered while it waits.
    reply->setProperty(kPropReleased, true);
    int epoch = _sessionEpoch;
    QTimer::singleShot(kRetryDelayMs, this, [this, verb, url, body, background, attempt, epoch]() {
        if ((epoch != _sessionEpoch) && !retryOutlivesSession(url)) {
            // The account changed meanwhile: resending would act for the previous
            // one. Give back the overlay share this retry inherited.
            if (!background && (_foregroundRequests > 0)) {
                --_foregroundRequests;
            }
            releaseLoading();
            return;
        }
        startRequest(verb, url, body, background, attempt + 1, true);
    });
    return true;
}

QNetworkReply *QGameCore::sendPostRequest(const QString &url, const QByteArray &data, bool Background) {
    return startRequest("POST", url, data, Background, 0, false);
}

QNetworkReply *QGameCore::sendGetRequest(const QString &url, bool Background) {
    return startRequest("GET", url, QByteArray(), Background, 0, false);
}

QNetworkReply *QGameCore::startRequest(const QString &Verb, const QString &Url, const QByteArray &Data, bool Background, int Attempt, bool Counted) {
    bool is_post = Verb.compare("POST") == 0;
    _lastRequestType = is_post ? "POST" : "GET";
    _lastRequestUrl = Url;
    _lastRequestData = Data;
    _httpResponceCode = 0;
    saveRequest(_lastRequestUrl);

    QNetworkRequest request;
    request.setUrl(QUrl(Url));
    if (is_post) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    }
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    request.setTransferTimeout(kRequestTimeoutMs);
    _reply = is_post ? _nam.post(request, Data) : _nam.get(request);
    //_reply->ignoreSslErrors(ignoredSslErrors);
    _reply->ignoreSslErrors();
    _reply->setProperty(kPropBackground, Background);
    _reply->setProperty(kPropVerb, _lastRequestType);
    _reply->setProperty(kPropBody, Data);
    _reply->setProperty(kPropAttempt, Attempt);
    if (!Background && !Counted) {
        ++_foregroundRequests;
    }
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    return _reply;
}

/*
void QGameCore::setOrganization() {
    qDebug() << "QGameCore::setOrganization()";
    _organizationName = "Steel Programming";
    _applicationName = "Game";
}
*/
void QGameCore::setProxySettings(QString IP, int Port, QString Username, QString Password) {
    _proxyHost = IP;
    _proxyPort = Port;
    _proxyUser = Username;
    _proxyPass = Password;
    saveParameters(false, true);
}

void QGameCore::saveParameters(bool user, bool proxy, bool game, bool stats, bool options) {
    if (user) {
        settings->beginGroup("User");
        saveUserParameters();
        settings->endGroup();
    }

    if (proxy) {
        settings->beginGroup("Proxy");
        saveProxyParameters();
        settings->endGroup();
    }

    if (game) {
        settings->beginGroup("Game");
        saveGameParameters();
        settings->endGroup();
    }

    if (stats) {
        settings->beginGroup("Statistic");
        saveStatsParameters();
        settings->endGroup();
    }

    if (options) {
        settings->beginGroup("Options");
        saveOptionsParameters();
        settings->endGroup();
    }    
}

void QGameCore::loadParameters() {
    settings->beginGroup("User");
    loadUserParameters();
    settings->endGroup();

    settings->beginGroup("Proxy");
    loadProxyParameters();
    settings->endGroup();

    settings->beginGroup("Game");
    loadGameParameters();
    settings->endGroup();

    settings->beginGroup("Statistic");
    loadStatsParameters();
    settings->endGroup();

    settings->beginGroup("Options");
    loadOptionsParameters();
    settings->endGroup();
}

void QGameCore::saveUserParameters() {
    settings->setValue("uuid", _UUID);
}

void QGameCore::saveProxyParameters() {
    settings->setValue("host", _proxyHost);
    settings->setValue("port", _proxyPort);
    settings->setValue("user", _proxyUser);
    settings->setValue("password", _proxyPass);
}

void QGameCore::saveGameParameters() {

}

void QGameCore::saveStatsParameters() {

}

void QGameCore::saveOptionsParameters() {

}

void QGameCore::loadUserParameters() {
    _UUID = settings->value("uuid", QUuid::createUuid().toString().replace("{", "").replace("}", "").replace("-", "")).toString();
}

void QGameCore::loadProxyParameters() {
    _proxyHost = settings->value("host", "").toString();
    _proxyPort = settings->value("port", "").toInt();
    _proxyUser = settings->value("user", "").toString();
    _proxyPass = settings->value("password", "").toString();
}

void QGameCore::loadGameParameters() {

}

void QGameCore::loadStatsParameters() {

}

void QGameCore::loadOptionsParameters() {

}

void QGameCore::setIsLoading(bool isLoading)
{
    bool changed = _isLoading != isLoading;
    _isLoading = isLoading;
    if (changed) {
        qDebug() << "QGameCore::setIsLoading" << isLoading <<"emit isLoadingChanged()";
        emit isLoadingChanged();
    }
}

void QGameCore::applyProxySettings() {
    if (_proxyHost.isEmpty()) {
        _nam.setProxy(QNetworkProxy::NoProxy);
    } else {
        _proxy.setType(QNetworkProxy::HttpProxy);
        _proxy.setHostName(_proxyHost);
        _proxy.setPort(static_cast<quint16>(_proxyPort));
        if (!_proxyUser.isEmpty()) {
            _proxy.setUser(_proxyUser);
        }
        if (!_proxyPass.isEmpty()) {
            _proxy.setPassword(_proxyPass);
        }
        _nam.setProxy(_proxy);
    }
}

QString QGameCore::proxyHost() {
    return _proxyHost;
}

int QGameCore::proxyPort() {
    return _proxyPort;
}

QString QGameCore::proxyUser() {
    return _proxyUser;
}

QString QGameCore::proxyPass() {
    return _proxyPass;
}

QString QGameCore::uuid() {
    return _UUID;
}

void QGameCore::setOrganization(const QString &OrgName, const QString &AppName) {
    _organizationName = OrgName;
    _applicationName = AppName;
}
