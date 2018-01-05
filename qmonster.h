#ifndef QMONSTER_H
#define QMONSTER_H

#include <QString>

class QMonster
{
public:
    QMonster(QString Name, QString Status, QString Owner, QString Target);

    QString name();
    QString status();
    QString owner();
    QString target();
    QString html(QString &user_login);

private:
    QString _name;
    QString _status;
    QString _owner;
    QString _target;
};

#endif // QMONSTER_H
