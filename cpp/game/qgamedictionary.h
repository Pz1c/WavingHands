#ifndef QGAMEDICTIONARY_H
#define QGAMEDICTIONARY_H

#include <QObject>
#include <QMap>
#include <QMutex>
#include <QLocale>
#include <QSettings>
#include <QQmlEngine>
#include <QJSEngine>
#include <QDebug>
#include "qgameutils.h"

class QGameDictionary : public QObject
{
    Q_OBJECT
    // QML reads it to re-translate on a switch: getStringByCode() alone is a call, not a binding dependency
    Q_PROPERTY(QString lang READ getCurrentLang WRITE setCurrentLang NOTIFY langChanged)

public:
    static QGameDictionary *getInstance();

    Q_INVOKABLE QString getStringByCode(const QString &Code);
    Q_INVOKABLE QString getCurrentLang();
    Q_INVOKABLE void setCurrentLang(QString Lang);

protected:
    QMap<QString, QString> dictionary_en;
    QMap<QString, QString> dictionary_de;
    QMap<QString, QString> dictionary_es;

    void fillDictionary(const QString& Code, const QString& en, const QString& de, const QString& es);

    virtual void fillGameDictionary();
    virtual void storeLang();
    virtual QString getLang();

    QString lang; //en (default), de, es
    // setCurrentLang() runs on the UI thread while the bot service thread looks strings up
    QMutex langMutex;

    static QGameDictionary *self;
    explicit QGameDictionary(QObject *parent = 0);

signals:
    void langChanged();

public slots:

};

#endif // QGAMEDICTIONARY_H
