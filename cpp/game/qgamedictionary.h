#ifndef QGAMEDICTIONARY_H
#define QGAMEDICTIONARY_H

#include <QObject>
#include <QMap>
#include <QLocale>
#include <QSettings>
#include <QQmlEngine>
#include <QJSEngine>
#include <QDebug>
#include "qgameutils.h"

class QGameDictionary : public QObject
{
    Q_OBJECT

public:
    static QGameDictionary *getInstance();

    Q_INVOKABLE QString getStringByCode(const QString &Code);
    Q_INVOKABLE QString getCurrentLang();
    Q_INVOKABLE void setCurrentLang(QString Lang);

protected:
    QMap<QString, QString> dictionary_ua;
    QMap<QString, QString> dictionary_ru;
    QMap<QString, QString> dictionary_en;

    void fillDictionary(const QString& Code, const QString& ua, const QString& ru, const QString& en);

    virtual void fillGameDictionary();
    virtual void storeLang();
    virtual QString getLang();

    QString lang; //en, ua, ru

    static QGameDictionary *self;
    explicit QGameDictionary(QObject *parent = 0);

signals:

public slots:

};

[[maybe_unused]] static QObject *gamedictionary_qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    qDebug() << "gamesdictionary_qobject_singletontype_provider";
    return QGameDictionary::getInstance();
}

#endif // QGAMEDICTIONARY_H
