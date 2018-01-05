#ifndef QWARLOCKUTILS_H
#define QWARLOCKUTILS_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QPair>
#include <QDebug>
#include "qwarlock.h"
#include "qmonster.h"

#define QValueName QPair<QString, QString>
#define QSpellDayCnt QPair<QString, int>

#define SALT "(cfad$CKSvf*0#@*$Y&_(a)-FAASS23"
#define ORGANIZATION_NAME "Steel Programming"
#define APPLICATION_NAME "WarloksDuel"

class QWarlockUtils
{
public:
    QWarlockUtils();

    static QList<int> getBattleList(QString &Data, QString Search);
    static QString getStringFromData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd);
    static int getIntFromPlayerData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd);
    static bool parseTargetList(QString &Data, QList<QValueName> &_Targets, QString &error);
    static bool parseMonster(QString &Data, QList<QMonster *> &result, QString &error);
    static bool parseWarlock(QString &Data, QList<QWarlock *> &result, QString &error);
    static QList<QValueName> getSelectOptionFromData(QString &Data, QString SearchBegin, QString SearchEnd, QString ValueBegin, QString ValueEnd, QString &error);
    static bool parseGestures(QString &Data, QString &left, QString &right, QString &error);
    static bool parseMonsterCommad(QString &Data, QString &result, QString owner, QStringList &monsters, bool IsCharm);
    static QString getCharmedPersonList(QString &Data);
    static QString getParalyseList(QString &Data);
    static QString parseChallengesList(QString &Data);
    static QString parseChallenge(QString &Data);
    static QString parseChallengePart0(QString &Data);
    static QString parseChallengePart1(QString &Data);
    static QString parseChallengePart2(QString &Data);
    static QString parseChallengePart3(QString &Data);

};

#endif // QWARLOCKUTILS_H
