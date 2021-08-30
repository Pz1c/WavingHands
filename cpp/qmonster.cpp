#include "qmonster.h"

QMonster::QMonster(const QString &Name, const QString &Status, const QString &Owner, const QString &Target)
{
    qDebug() << "QMonster::QMonster" << Name << Status << Owner << Target;
    _name = Name;
    _status = Status;
    _owner = Owner;
    _target = Target;
    _newTarget = Target;
    _attackStrength = 0;
    _fireElemental = false;
    _iceElemental = false;
    _justCreated = Name.indexOf(":") != -1;
    if (_justCreated) {
        _hand = Name.indexOf("LH:") == 0 ? WARLOCK_HAND_LEFT : WARLOCK_HAND_RIGHT;
    }
    _underControl = -1;
    _strength = QWarlockUtils::getStrengthByMonsterName(_name);
    _hp = _strength;
    if (_strength == 3) {
        _fireElemental = _name.indexOf("Fire") != -1;
        _iceElemental = _name.indexOf("Ice") != -1;
    }

    if (_strength > 0) {
        int pos = 0;
        _hp = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Health", ":", " #;#)", pos));
    }
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
    return _target.compare(user_login, Qt::CaseInsensitive) == 0;
}

bool QMonster::is_owner(const QString &user_login) {
    if (_underControl == -1) {
        _underControl = (_owner.compare(user_login, Qt::CaseInsensitive) == 0) ? 1 : 0;
    }
    return _underControl == 1;
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

void QMonster::setHp(int newHp)
{
    _hp = newHp;
}

void QMonster::setStrength(int newStrength)
{
    _strength = newStrength;
}

void QMonster::setName(const QString &newName)
{
    _name = newName;
}

int QMonster::hand() const
{
    return _hand;
}

bool QMonster::justCreated() const
{
    return _justCreated;
}

int QMonster::underControl() const
{
    return _underControl;
}

void QMonster::setUnderControl(int newUnderControl)
{
    _underControl = newUnderControl;
}

bool QMonster::iceElemental() const
{
    return _iceElemental;
}

bool QMonster::fireElemental() const
{
    return _fireElemental;
}

int QMonster::attackStrength() const
{
    return _attackStrength;
}

void QMonster::setAttackStrength(int newAttackStrength)
{
    _attackStrength = newAttackStrength;
}

const QString &QMonster::newTarget() const
{
    return _newTarget;
}

void QMonster::setNewTarget(const QString &newNewTarget)
{
    _newTarget = newNewTarget;
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
    bool targeted = _target.compare(user_login, Qt::CaseInsensitive) == 0;

    return QString("{\"under_control\":%1,\"name\":\"%2\",\"status\":\"%3\",\"owner\":\"%4\",\"target\":\"%5\",\"targeted\":%6,\"strength\":%7,\"new_target\":\"%8\",\"hp\":%9}")
             .arg(boolToStr(is_owner(user_login)), _name, _status, _owner, _target, boolToStr(targeted), intToStr(_strength), _newTarget, intToStr(_hp));
}
