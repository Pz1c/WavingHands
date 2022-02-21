#ifndef QBATTLEINFO_H
#define QBATTLEINFO_H

#include "qgameconstant.h"

#include <QDateTime>

class QBattleInfo
{
public:
    QBattleInfo();
    QBattleInfo(const QString &battle_info);

    const QString toJSON();
    const QString toString();
    const QString getInListDescription(const QString &Login);
    const bool isWinner(const QString &Login);
    const bool withBot();
    const bool canForceSurrendering();

    int hint() const;
    void setHint(int newHint);

    int status() const;
    void setStatus(int newStatus);

    int wait_from() const;
    void setWaitFrom(int newWait_from);

    int battleID() const;
    void setBattleID(int newBattleID);

    void addParticipant(const QString &login);
    void addChat(int battle_turn, const QString &chat_msg);
    const QString getChat();
    void addHistory(int battle_turn, const QString &hist_msg);
    const QString getHistory();

    int size() const;
    void setSize(int newSize);

    const QString &winner() const;
    void setWinner(const QString &newWinner);

    int turn() const;
    void setTurn(int newTurn);

protected:
    const QString prepareToPrint(QString str);
    void parseString(const QString &battle_info);
    void init();
    const QString getEnemy(const QString &Login);

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
    QString _description;
    QStringList _participant;
    QStringList _chat;
    QStringList _history;
    QString _winner;
};

#endif // QBATTLEINFO_H
