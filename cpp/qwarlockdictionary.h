#ifndef QWARLOCKDICTIONARY_H
#define QWARLOCKDICTIONARY_H

#include <QObject>
#include "qwarlockutils.h"
#include <QMap>
#include <QLocale>
#include <QSettings>
#include <QQmlEngine>
#include <QJSEngine>

class QWarlockDictionary : public QObject
{
    Q_OBJECT

public:
    static QWarlockDictionary *getInstance();

    Q_INVOKABLE QString getStringByCode(const QString &Code);
    Q_INVOKABLE QString getCurrentLang();
    Q_INVOKABLE void setCurrentLang(QString Lang);

protected:
    QMap<QString, QString> dictionary_ua;
    QMap<QString, QString> dictionary_ru;
    QMap<QString, QString> dictionary_en;

    void fillDictionary(const QString& Code, const QString& ua, const QString& ru, const QString& en);

private:
    QString lang; //en, ua, ru

    static QWarlockDictionary *self;
    explicit QWarlockDictionary(QObject *parent = nullptr);

signals:

public slots:

};

static QObject *warlocksdictionary_qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    qDebug() << "warlocksdictionary_qobject_singletontype_provider";
    return QWarlockDictionary::getInstance();
}

#endif // QWARLOCKDICTIONARY_H
