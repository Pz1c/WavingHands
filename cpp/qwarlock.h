#ifndef QWARLOCK_H
#define QWARLOCK_H

#include <QDebug>
#include <QString>
#include <QStringList>

#define WARLOCK_GESTURE_LENGTH 8

class QWarlock
{
public:
    QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player);

    QString name();
    QString status();
    QString leftGestures();
    QString rightGestures();
    QString html(QString &posible_gestures);
    QString separatedString(QString &posible_spells);
    bool player() const;

private:
    QString _name;
    QString _status;
    QString _leftGestures;
    QString _rightGestures;
    bool _player;
};

#endif // QWARLOCK_H
