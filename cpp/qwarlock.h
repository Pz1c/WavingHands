#ifndef QWARLOCK_H
#define QWARLOCK_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include "qspell.h"

#define WARLOCK_GESTURE_LENGTH 8

class QWarlock
{
public:
    QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player);
    ~QWarlock();

    QString name();
    QString status();
    QString leftGestures();
    QString rightGestures();
    QString html(QString &posible_gestures);
    QString separatedString();
    bool player() const;

    void setPossibleSpells(const QList<QSpell *> &possibleSpells);
    void checkSpells();
private:
    QString _name;
    QString _status;
    QString _leftGestures;
    QString _rightGestures;
    bool _player;
    QList<QSpell *> _possibleSpells;
    QSpell *_bestSpellL;
    QSpell *_bestSpellR;
};

#endif // QWARLOCK_H
