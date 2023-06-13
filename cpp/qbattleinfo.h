#ifndef QBATTLEINFO_H
#define QBATTLEINFO_H

//#include "qgameconstant.h"

#include <QDateTime>

#include "qwarlock.h"


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
    bool canForceSurrendering() const;

    int hint() const;
    void setHint(int newHint);

    int status() const;
    void setStatus(int newStatus);

    int wait_from() const;
    void setWaitFrom(int newWait_from);

    int battleID() const;
    void setBattleID(int newBattleID);

    void addParticipant(const QString &login, bool Challenged = false);
    void cleanParticipant();
    void addChat(int battle_turn, const QString &chat_msg);
    QString getChat() const;
    void addHistory(int battle_turn, const QString &hist_msg);
    QString getHistory() const;
    QString getFullHist(const QString &Login) const;
    QString getFinishedBattleInfo(const QString &Login) const;
    QString getTurnInfo(int Turn, const QString &Login) const;
    void parseAllTurns(QString& Data, QString &Login, bool Last = false);

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

    QString fullJSON() const;

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
    QString _description;
    QStringList _participant;
    QStringList _challenged;
    QStringList _chat;
    QStringList _history;
    QString _winner;
    QString _sub_title;
    QString _fullJSON;
    int _app_version;

    QList<QMonster *> _Monsters;
    QList<QWarlock *> _Warlock;

};

#endif // QBATTLEINFO_H
