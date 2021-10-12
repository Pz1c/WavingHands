#ifndef QWARLOCKSTAT_H
#define QWARLOCKSTAT_H

#include <QDebug>

class QWarlockStat
{
public:
    QWarlockStat();
    QWarlockStat(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, int Active);
    QWarlockStat(QString Raw, bool FromIni = false);

    QString toString() const;

    const QString &name() const;

    bool registered() const;

    int ladder() const;

    int melee() const;

    int played() const;

    int won() const;

    int died() const;

    int elo() const;

    int active() const;

    void parseAndInit(QString Raw);
    void parseIniAndInit(QString Raw);

protected:
    void init(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, int Active);

private:
    QString _name;
    bool _registered;
    int _ladder;
    int _melee;
    int _played;
    int _won;
    int _died;
    int _elo;
    int _active;
};

#endif // QWARLOCKSTAT_H
