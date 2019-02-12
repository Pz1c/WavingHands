#include "qmonster.h"

QMonster::QMonster(const QString &Name, const QString &Status, const QString &Owner, const QString &Target)
{
    qDebug() << "QMonster::QMonster" << Name << Status << Owner << Target;
    _name = Name;
    _status = Status;
    _owner = Owner;
    _target = Target;
    if (_name.indexOf("Goblin") != -1) {
        _strength = 1;
    } else if (_name.indexOf("Orc") != -1) {
        _strength = 2;
    } else if (_name.indexOf("Trol") != -1) {
        _strength = 3;
    } else if (_name.indexOf("Giant") != -1) {
        _strength = 4;
    } else {
        _strength = 0;
    }
    //_hp = _strength;
    _hp = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Health", ":", " #;#)"));
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

bool QMonster::is_under_attack(const QString &user_login) {
    return _target.toLower().compare(user_login.toLower()) == 0;
}

bool QMonster::is_owner(const QString &user_login) {
    return _owner.toLower().compare(user_login.toLower()) == 0;
}

QString QMonster::getColor(const QString &user_login) {
    QString color;
    if (is_owner(user_login)) {
        color = "#7FD49F";
    } else if (is_under_attack(user_login)) {
        color = "#D190A4";
    } else {
        color = "#D2D2D2";
    }
    return color;
}

int QMonster::getHp() const
{
    return _hp;
}

int QMonster::getStrength() const
{
    return _strength;
}

QString QMonster::html(const QString &user_login) {
    return QString("<font color=\"%1\">%2 %3 (%4) &gt; %5</font>").arg(getColor(user_login), _name, _status, _owner, _target);
}

QString QMonster::json(const QString &user_login) {
    QString under_control = _owner.toLower().compare(user_login.toLower()) == 0 ? "true" : "false";
    QString targeted = _target.toLower().compare(user_login.toLower()) == 0 ? "true" : "false";
    return QString("{\"under_control\":%1,\"name\":\"%2\",\"status\":\"%3\",\"owner\":\"%4\",\"target\":\"%5\",\"targeted\":%6,\"strength\":%7,\"color\":\"%8\",\"hp\":%9}")
             .arg(under_control, _name, _status, _owner, _target, targeted, intToStr(_strength), getColor(user_login), intToStr(_hp));
}
