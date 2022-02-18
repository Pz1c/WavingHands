#ifndef QBATTLEINFO_H
#define QBATTLEINFO_H

#include "qgameconstant.h"

class QBattleInfo
{
public:
    QBattleInfo();
    QBattleInfo(const QString &battle_info);

    const QString toJSON();
    const QString toString();
    int hint() const;
    void setHint(int newHint, bool only_not_set = false);

    int status() const;
    void setStatus(int newStatus, bool only_not_set = false);

    int wait_from() const;
    void setWaitFrom(int newWait_from, bool only_not_set = false);

protected:
    const QString prepareToPrint(QString str);
    void parseString(const QString &battle_info);
    void init();


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
    QString _participant;
    QString _chat;
    QString _history;
    QString _winner;
};

#endif // QBATTLEINFO_H
