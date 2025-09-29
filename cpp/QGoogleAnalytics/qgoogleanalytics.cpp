#include "qgoogleanalytics.h"

QGoogleAnalytics::QGoogleAnalytics(QObject *parent): QObject(parent) {
    _requestIdx = 0;
}

QGoogleAnalytics::~QGoogleAnalytics() {

}

void QGoogleAnalytics::setNam(QNetworkAccessManager *newNam)
{
    _nam = newNam;
}

void QGoogleAnalytics::setApiSecret(const QString &newApiSecret)
{
    _apiSecret = newApiSecret;
}

void QGoogleAnalytics::setMeasurementId(const QString &newMeasurementId)
{
    _measurementId = newMeasurementId;
}

void QGoogleAnalytics::setClientId(const QString &newClientId)
{
    _clientId = newClientId;
}

const QString QGoogleAnalytics::parseEventParams(QString &params) const {
    QString res = "";
    QStringList sl = params.split(";");
    QString prop_time_key = "engagement_time_msec";
    bool find_time = false;
    for(int i = 0, Ln = sl.size() / 2; i < Ln; ++i) {
       if (!res.isEmpty()) {
           res.append(",");
       }
       res.append(QString("\"%1\":\"%2\"").arg(sl.at(i * 2), sl.at(i * 2 + 1)));
       if (!find_time && (sl.at(i * 2).compare(prop_time_key) == 0)) {
           find_time = true;
       }
    }
    if (!find_time) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        res.append(QString("\"%1\":\"1000\"").arg(prop_time_key));
    }
    return res;
}

void QGoogleAnalytics::sendEvent(QString EventName, QString &EventParams) {
    qDebug() << "QGoogleAnalytics::sendEvent" << _apiSecret << _measurementId << _clientId << EventName << EventParams;
    if (_apiSecret.isEmpty() || _measurementId.isEmpty() || _clientId.isEmpty() || EventName.isEmpty()) {
        return;
    }
    QString url = QString(GA_ENDPOINT_URL).arg(_apiSecret, _measurementId);
    QString data = QString("{\"%1\":\"%2\",\"user_id\":\"%3\",\"timestamp_micros\":\"%4\",%5\"non_personalized_ads\":false,"
                           "\"events\":[{\"name\":\"%6\",\"params\":{%7}}]}").
                       arg(GA_APP_ID_TAG_NAME, _clientId, _userId, QString::number(QDateTime::currentMSecsSinceEpoch() * 1000), _userProperties,
                           EventName, parseEventParams(EventParams));


    sendPostRequest(url, data);
}

void QGoogleAnalytics::sendPostRequest(QString url, const QString &data) {
    _lastRequestData.clear();
    _lastRequestData.append(data);
    saveRequest(0, url, _lastRequestData);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    _reply = _nam->post(request, data.toUtf8());
    _reply->ignoreSslErrors();
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QGoogleAnalytics::prepareUserProperties() {
    _userProperties = "";
    if (_screen.isEmpty() && _locale.isEmpty() && _os.isEmpty()) {
        return;
    }
    _userProperties = "\"user_properties\":{";
    bool first = true;
    if (!_screen.isEmpty()) {
        if (first) {
            first = false;
        }
        _userProperties.append(QString("\"screen\":{\"value\":\"%1\"}").arg(_screen));
    }

    if (!_locale.isEmpty()) {
        if (first) {
            first = false;
        } else {
            _userProperties.append(",");
        }
        _userProperties.append(QString("\"locale\":{\"value\":\"%1\"}").arg(_locale));
    }

    // add new there

    if (!_os.isEmpty()) {
        if (first) {
            //first = false;
        } else {
            _userProperties.append(",");
        }
        _userProperties.append(QString("\"os\":{\"value\":\"%1\"}").arg(_os));
    }

    _userProperties.append("},");
}

void QGoogleAnalytics::setScreen(const QString &newScreen)
{
    _screen = newScreen;
    prepareUserProperties();
}

void QGoogleAnalytics::setUserProperties(const QString &newOs, const QString &newLocale, const QString &newScreen) {
    _screen = newScreen;
    _locale = newLocale;
    _os = newOs;
    prepareUserProperties();
}

void QGoogleAnalytics::setLocale(const QString &newLocale)
{
    _locale = newLocale;
    prepareUserProperties();
}

void QGoogleAnalytics::setOs(const QString &newOs)
{
    _os = newOs;
    prepareUserProperties();
}

void QGoogleAnalytics::setUserId(const QString &newUserId)
{
    _userId = newUserId;
}


void QGoogleAnalytics::slotReadyRead() {
    QNetworkReply *reply = (QNetworkReply *)sender();
    QString url = reply->url().toString();
    QString data = reply->readAll();
    int httpResponceCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    saveRequest(httpResponceCode, url, data);
}

void QGoogleAnalytics::slotError(QNetworkReply::NetworkError error) {
    qDebug() << "QGoogleAnalytics::slotError" << error << _reply->errorString();
}

void QGoogleAnalytics::slotSslErrors(QList<QSslError> error_list) {
    qDebug() << "QGoogleAnalytics::slotSslErrors" << error_list;
}



void QGoogleAnalytics::saveRequest(int httpResponceCode, QString url, QString &data) {
    qDebug() << "QGoogleAnalytics::saveRequest" << data.length() << httpResponceCode;
#ifdef QT_DEBUG
    QString file_name = QString("ga4_request_%2.html").arg(QString::number(++_requestIdx));
    QFile file(file_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "<!-- " << QString::number(httpResponceCode) << " " << url << " " << _lastRequestData << " -->\n";
    out << data;
    file.close();
    qDebug() << "data stored into " << file_name;
#endif
}
