#include "qmonster.h"

QMonster::QMonster(QString Name, QString Status, QString Owner, QString Target)
{
    _name = Name;
    _status = Status;
    _owner = Owner;
    _target = Target;
}

QString QMonster::name() {
    return _name;
}

QString QMonster::status() {
    return _status;
}

QString QMonster::owner() {
    return _owner;
}

QString QMonster::target() {
    return _target;
}

QString QMonster::html(QString &user_login) {
    QString color;
    if (_owner.toLower().compare(user_login.toLower()) == 0) {
        color = "#7FD49F";
    } else if (_target.toLower().compare(user_login.toLower()) == 0) {
        color = "#D190A4";
    } else {
        color = "#D2D2D2";
    }
    return QString("<font color=\"%1\">%2 %3 (%4) &gt; %5</font>").arg(color, _name, _status, _owner, _target);
}
