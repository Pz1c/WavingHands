#ifndef QBATTLEINFO_H
#define QBATTLEINFO_H

//#include "qgameconstant.h"

#include <QDateTime>

#include "qwarlock.h"
#include <qgameutils.h>


class QBattleInfo
{
public:
    QBattleInfo();
    QBattleInfo(const QString &battle_info);
    ~QBattleInfo();

    QString toJSON(const QString &Login = "") const;
    QString toString(bool Short = false) const;
    QString getInListDescription(const QString &Login) const;
    QString getInListParticipant(const QString &Login, bool Short = false) const;
    QString getInListStatus(const QString &Login) const;
    bool isWinner(const QString &Login) const;
    bool canForceSurrendering(int checkTime = 75 * 60 * 60) const;

    int hint() const;
    void setHint(int newHint);

    int status() const;
    void setStatus(int newStatus);

    int wait_from() const;
    void setWaitFrom(int newWaitFrom);

    int battleID() const;
    void setBattleID(int newBattleID);

    void addParticipant(const QString &login, bool Challenged = false, bool Rejected = false);
    void cleanParticipant();
    void addChat(int battle_turn, const QString &chat_msg);
    QString getChat() const;
    void addHistory(int battle_turn, const QString &hist_msg);
    QString getHistory() const;
    QString getFullHist(const QString &Login) const;
    QString getFinishedBattleInfo(const QString &Login) const;
    QString getTurnInfo(int Turn, const QString &Login) const;
    void parseAllTurns(QString& Data, QString &Login, bool Last = false);
    // Fill the turn log from a "&full=1" page without touching the units, winner or
    // the stored battle JSON. Returns false when the page carries no turn log.
    bool parseFullHistory(const QString &Data);
    // True when every resolved turn is present in the history. Gestures are stored,
    // so an empty entry means the turn was never parsed, not a turn without news.
    bool historyComplete() const;

    bool active(const QString &login) const;

    int size() const;
    void setSize(int newSize);

    const QString &winner() const;
    void setWinner(const QString &newWinner);

    int turn() const;
    void setTurn(int newTurn);

    int level() const;
    void setLevel(int newLevel);

    bool maladroit() const;
    void setMaladroit(bool newMaladroit);

    bool parafc() const;
    void setParafc(bool newParafc);

    bool parafdf() const;
    void setParafdf(bool newParafdf);

    bool fast() const;
    void setFast(bool newFast);

    bool for_bot() const;
    void setForBot(bool newFor_bot);

    bool with_bot() const;

    const QString &description() const;
    void setDescription(const QString &newDescription);

    QString containParticipant(const QString& line);

    void setSubTitle(const QString &newSub_title);

    bool fullParsed() const;

    QString getEnemy(const QString &Login, bool All = false) const;
    QStringList getEnemies(const QString &Login) const;

    QString fullJSON() const;

    bool isInviteRejected();
    void checkRejection();
    // The first warlock invited to this unstarted battle, as far as its page was read.
    QString challengedWarlock() const;

    // Live match (https://github.com/Pz1c/WavingHands/issues/287): a private duel played
    // against the clock, see QWarlockUtils::liveDescription. The clock is this player's
    // own and kept by the app. A turn gets liveBudget(): what is left of the total, and
    // never less than the extra. It starts when the player opens the turn
    // (liveTurnEntered) or LIVE_MATCH_NOTICE_SEC after the app learned of it
    // (liveTurnNoticed), whichever comes first, and stops when the orders are in
    // (liveTurnDone). Every Now is in seconds since epoch.
    bool live() const;
    void setLive(int Total, int Extra, qint64 ValidBy);
    qint64 liveValidBy() const;
    int liveTotal() const;
    int liveExtra() const;
    int liveUsed() const;
    void liveTurnNoticed(qint64 Now);
    void liveTurnEntered(qint64 Now);
    void liveTurnDone(qint64 Now);
    // true between liveTurnNoticed / liveTurnEntered and liveTurnDone
    bool liveTurnOpen() const;
    int liveBudget() const;
    // When this turn's clock started, 0 while it does not run yet.
    qint64 liveClockStart(qint64 Now) const;
    // Seconds left for this turn; negative once the time is out.
    int liveLeft(qint64 Now) const;
    // When this turn's time is (or would be, by the notice rule) out; 0 with no turn open.
    qint64 liveDeadline(qint64 Now) const;

protected:
    QString prepareToPrint(QString str) const;
    void parseString(const QString &battle_info);
    void init();
    void cleanLists();
    void parseUnits(QString &Data, QString &Login);
    void generateJSON(QString &Login);

private:
    int _battleID;
    int _status; // -2 deleted, -1 not started, 0 wait, 1 ready, 2 finished
    int _size; // 2-6
    int _level; // 0 Ladder, 1 Friendly, 2 V.Friendly
    int _turn;
    int _wait_from;
    int _hint;
    bool _maladroit;
    bool _parafc;
    bool _parafdf;
    bool _fast;
    bool _for_bot;
    bool _with_bot;
    bool _fullParsed;
    bool _isOnline;
    bool _inviteRejected;

    QString _description;
    QStringList _participant;
    QStringList _challenged;
    QStringList _rejected;
    QStringList _chat;
    QStringList _history;
    QString _winner;
    QString _sub_title;
    QString _fullJSON;
    int _app_version;

    QList<QMonster *> _Monsters;
    QList<QWarlock *> _Warlock;

    // live match: the rules (_liveTotal 0 = not a live match), the seconds spent in the
    // turns played so far, when the app learned of the current turn and when the player
    // opened it (0 = not yet)
    int _liveTotal;
    int _liveExtra;
    qint64 _liveValidBy;
    int _liveUsed;
    qint64 _liveNotified;
    qint64 _liveStart;
};

#endif // QBATTLEINFO_H
