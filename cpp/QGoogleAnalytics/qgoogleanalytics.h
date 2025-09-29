#ifndef QGOOGLEANALYTICS_H
#define QGOOGLEANALYTICS_H

#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QList>
#include <QPair>
#include <QUuid>
#include <QDateTime>
#include <QLocale>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include "qgaconstant.h"

// https://developers.google.com/analytics/devguides/collection/protocol/v1/

class QGoogleAnalytics : public QObject
{
    Q_OBJECT
public:
    explicit QGoogleAnalytics(QObject *parent = 0);
    ~QGoogleAnalytics();

    void setNam(QNetworkAccessManager *newNam);

    void setApiSecret(const QString &newApiSecret);

    void setMeasurementId(const QString &newMeasurementId);

    void setClientId(const QString &newClientId);

    void sendEvent(QString EventName, QString &EventParams);

    void setUserId(const QString &newUserId);

    void setOs(const QString &newOs);

    void setLocale(const QString &newLocale);

    void setScreen(const QString &newScreen);

    void setUserProperties(const QString &newOs, const QString &newLocale, const QString &newScreen);

protected slots:

    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors(QList<QSslError> error_list);

protected:
    void saveRequest(int httpResponceCode, QString url, QString &data);
    void sendPostRequest(QString url, const QString &data);
    void prepareUserProperties();
    const QString parseEventParams(QString &params) const;

private:
    // netowrk
    QNetworkAccessManager *_nam;
    QNetworkReply *_reply;
    int _requestIdx;
    QString _lastRequestData;

    // GA4 settings
    QString _apiSecret;
    QString _measurementId;
    QString _clientId; // app
    QString _userId; // player
    QString _userProperties;
    QString _os;
    QString _locale;
    QString _screen;

};
#endif // QGOOGLEANALYTICS_H
