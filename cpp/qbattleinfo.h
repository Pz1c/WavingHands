#ifndef QBATTLEINFO_H
#define QBATTLEINFO_H

#include <QDebug>

class QBattleInfo
{
public:
    QBattleInfo();



private:
    int _battleID;
    int _status; //-1, 0, 1 ready, 2 finished
    int _size; // 2-6
    int _level; // 0 Ladder, 1 Friendly, 2 V.Friendly
    bool _maladroit;
    bool _parafc;
    bool _parafdf;
    QString _description;
    QStringList _participant;
};

#endif // QBATTLEINFO_H
