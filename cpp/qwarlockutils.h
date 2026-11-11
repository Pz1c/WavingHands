#ifndef QWARLOCKUTILS_H
#define QWARLOCKUTILS_H

#include <QString>
#include <QList>
#include <QStringList>
#include <QPair>
#include <QDebug>
#include <QTextDocumentFragment>

#define QValueName QPair<QString, QString>
#define QSpellDayCnt QPair<QString, int>

#include "qgameconstant.h"

class QWarlock;
class QMonster;
class QBattleInfo;

// A private challenge waiting for the player's answer ("Challenged to battles:" of the
// player page) and its row of the game list. The page does not show the challenge's
// description, so whether it is a live match, and by which rules, is for the caller to
// find out (QWarloksDuelCore::liveCheckInvites).
struct QPersonalChallenge {
    int id = 0;
    QString from;
    QString row;
    bool live = false;
    qint64 validBy = 0;
    int total = 0;
    int extra = 0;
};

class QWarlockUtils
{
public:
    QWarlockUtils();

    static QList<int> getBattleList(QString &Data, QString Search);
    static QString getStringFromData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd, int &Pos, bool NoChangePos = false);
    static int getIntFromPlayerData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd, int &Pos, bool NoChangePos = false);
    static bool parseTargetList(QString &Data, QList<QValueName> &_Targets, QString &error);
    static bool parseMonster(QString &Data, QList<QMonster *> &result, QString &error);
    static bool parseWarlock(QString &Data, QList<QWarlock *> &result, QString &error, const QString &player);
    static QList<QValueName> getSelectOptionFromData(QString &Data, QString SearchBegin, QString SearchEnd, QString ValueBegin, QString ValueEnd, QString &error);
    static bool parseGestures(QString &Data, QString &left, QString &right, QString &error);
    static bool parseMonsterCommad(QString &Data, QList<QMonster *> &result);
    static QString getCharmedPersonList(QString &Data);
    static QString getParalyseList(QString &Data);
    static QList<QBattleInfo *> parseChallengesList(QString &Data);
    static QString parseTopList(QString &Data);
    static QBattleInfo *parseChallenge(QString &Data);
    static QString parsePlayerTop(QString &Data);
    static QString parseChallengePart0(QString &Data);
    static QString parseChallengePart1(QString &Data);
    static QString parseChallengeDescription(QString &Data);
    static QString parseChallengePart3(QString &Data);
    // Parsed: the same challenges in the same order, for a caller that needs more than
    // the rows.
    static void parsePersonalChallenge(const QString &Data, QStringList &result, QList<QPersonalChallenge> *Parsed = nullptr);
    // Live matches (https://github.com/Pz1c/WavingHands/issues/287). The rules of one
    // travel in the description of its challenge on the site, which is all both apps
    // share: who reads "Live match ... Valid by UTC <epoch> - Total:<mm>:<ss>m -
    // Extra:<n>s" plays by them. ValidBy is when the invite expires (secs since epoch),
    // Total each player's time for the whole game and Extra the least a turn gets, in
    // seconds. parseLiveDescription() is false for any other description; a rule cut off
    // by the site falls back to the app's own.
    static QString liveDescription(qint64 ValidBy, int Total, int Extra);
    static bool parseLiveDescription(const QString &Description, qint64 *ValidBy, int *Total, int *Extra);
    static QString getFinishedBattleDescription(const QString &Data);
    static QString getBattleShortTitle(const QString &Title, int State, int BattleID);
    static int strValueToInt(QString val);
    static void appendSeparatedList(QString &list, const QString &data, const QString &separator = ",");
    static void parseBattleHistory(QString &history, QBattleInfo* bi);
    static void parseBattleTurn(QString &turn_text, QBattleInfo* bi, int current_turn, bool last_turn);
    static int getStrengthByMonsterName(QString val);
    static void parseUnstartedBattle(QString &Data, QBattleInfo *bi);
    static int getRand(int Min, int Max);
    static qint64 parseLastActivity(const QString &str);
    // Escapes a string for use between the quotes of a hand-built JSON value.
    static QString jsonEscape(const QString &str);
};

#endif // QWARLOCKUTILS_H
