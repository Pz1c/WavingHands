#ifndef QWARLOCKSTAT_H
#define QWARLOCKSTAT_H

#include <QDebug>
#include <QDateTime>

class QWarlockStat
{
public:
    QWarlockStat();
    QWarlockStat(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo,
                 QString Color, qint64 LastActivity, bool Mobile);
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
    void parseSiteAndInit(QString Raw);

    qint64 lastActivity() const;

    bool ai() const;

    bool mobile() const;

    const QString &color() const;

    int warlockId() const;
    void setWarlockId(int newWarlockId);

    void setRegistered(bool newRegistered);

    void setLadder(int newLadder);

    void setMelee(int newMelee);

    void setPlayed(int newPlayed);

    void setWon(int newWon);

    void setDied(int newDied);

    void setElo(int newElo);

    void setLastActivity(qint64 newLastActivity);

    void setMobile(bool newMobile);

    void setColor(const QString &newColor);

protected:
    void init(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity, bool Mobile, int warlockId = 0);
    qint64 getLastActivityByColor(const QString Color);
private:
    QString _name;
    bool _registered = false;
    bool _ai = false;
    int _ladder = 0;
    int _melee = 0;
    int _played = 0;
    int _won = 0;
    int _died = 0;
    int _elo = 0;
    int _warlockId = 0;
    qint64 _lastActivity = 0;
    QString _color;
    bool _mobile = false;
};

#endif // QWARLOCKSTAT_H
