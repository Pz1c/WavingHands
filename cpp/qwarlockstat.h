#ifndef QWARLOCKSTAT_H
#define QWARLOCKSTAT_H

#include <QDebug>
#include <QDateTime>

class QWarlockStat
{
public:
    QWarlockStat();
    QWarlockStat(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity);
    QWarlockStat(QString Raw, bool FromIni = false);

    QString toString() const;
    QString toJSON() const;

    const QString &name() const;

    bool registered() const;

    int ladder() const;

    int melee() const;

    int played() const;

    int won() const;

    int died() const;

    int elo() const;

    bool online() const;

    void parseAndInit(QString Raw);
    void parseIniAndInit(QString Raw);

    qint64 lastActivity() const;

    bool ai() const;

protected:
    void init(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity);
    qint64 getLastActivityByColor(const QString Color);
private:
    QString _name;
    bool _registered;
    bool _ai;
    int _ladder;
    int _melee;
    int _played;
    int _won;
    int _died;
    int _elo;
    qint64 _lastActivity;
    QString _color;
};

#endif // QWARLOCKSTAT_H
