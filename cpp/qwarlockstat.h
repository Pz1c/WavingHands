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

protected:
    void init(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity, bool Mobile, int warlockId = 0);
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
    int _warlockId;
    qint64 _lastActivity;
    QString _color;
    bool _mobile;
};

#endif // QWARLOCKSTAT_H
