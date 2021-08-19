#include "qspell.h"

int QSpell::_orderType = 0;

QSpell::QSpell(int SpellID, QString Gesture, QString Name, int SpellType, int Priority, int Level, int Danger, int DefTarget, int Damage, bool Active, bool Basic)
{
    _spellID = SpellID;
    _gesture = Gesture;
    _name = Name;
    _spellType = SpellType;
    _priority = Priority;
    _danger = Danger;
    _level = Level;
    _turnToCast = -1;
    _alreadyCasted = -1;
    _hand = WARLOCK_HAND_NONE;
    _active = true;
    _defTarget = DefTarget;
    _damage = Damage;
    _active = Active;
    _basic = Basic;
    _realPriority = 0;
}

int QSpell::calcPriority(int Priority, int Danger, int TurnToCast, bool Enemy, int FullTurnToCast) {
    double res = Enemy ? Danger : Priority;
    qDebug() << "QSpell::calcPriority" << _gesture << res << TurnToCast << FullTurnToCast;
    /*if (Enemy) {
      res = Danger * (FullTurnToCast - TurnToCast)/FullTurnToCast;
    } else {
      res = Priority - TurnToCast + (FullTurnToCast - TurnToCast);
    }*/
    if (TurnToCast == FullTurnToCast) {
        res -= 5;
    } else if (TurnToCast < 5) {
        res += 5 - TurnToCast;
    }
    //res *= (FullTurnToCast - TurnToCast) / FullTurnToCast + 1;
    /*if (FullTurnToCast > TurnToCast) {
        res *= (FullTurnToCast - TurnToCast)/FullTurnToCast;
    } else if (FullTurnToCast == TurnToCast) {
        res -= 2;
    } else if (FullTurnToCast > TurnToCast) {
        res -= 3;
    }*/
    qDebug() << "QSpell::calcPriority result" << res;
    return static_cast<int>(res);
}

int QSpell::danger() const
{
    return _danger;
}

qreal QSpell::realPriority() const
{
    return _realPriority;
}

void QSpell::setRealPriority(qreal newRealPriority)
{
    _realPriority = newRealPriority;
}

int QSpell::damage() const
{
    return _damage;
}

int QSpell::alreadyCasted() const
{
    return _alreadyCasted;
}

int QSpell::length() const {
    return _gesture.length();
}

int QSpell::orderType()
{
    return _orderType;
}

bool QSpell::active() const
{
    return _active;
}

void QSpell::setActive(bool active)
{
    _active = active;
}

int QSpell::level() const
{
    return _level;
}

int QSpell::spellType() const
{
    return _spellType;
}

int QSpell::hand() const
{
    return _hand;
}

int QSpell::turnToCast() const
{
    return _turnToCast;
}

void QSpell::setTurnToCast(int turnToCast)
{
    _turnToCast = turnToCast;
}

int QSpell::priority() const
{
    return _priority;
}

void QSpell::setPriority(int priority)
{
    _priority = priority;
}

void QSpell::changePriority(int priority)
{
    _priority += priority;
}

QSpell::QSpell(QSpell *Spell, int Hand, int TurnToCast, bool Enemy) {
    _spellID = Spell->_spellID;
    _gesture = Spell->_gesture;
    _name = Spell->_name;
    _spellType = Spell->_spellType;
    _turnToCast = TurnToCast;
    _alreadyCasted = _gesture.length() - _turnToCast;
    _priority = calcPriority(Spell->_priority, Spell->_danger, TurnToCast, Enemy, _gesture.length());
    _danger = SPELL_PRIORITY_ZERO;
    _level = Spell->_level;
    _hand = Hand;
    _defTarget = Spell->_defTarget;
    _damage = Spell->_damage;
    _basic = Spell->_basic;
}

int QSpell::spellID() const
{
    return _spellID;
}

QString QSpell::gesture() const
{
    return _gesture;
}

QString QSpell::name() const
{
    return _name;
}

bool QSpell::possibleCast() const {
    return _turnToCast < _gesture.length();
}

QString QSpell::nextGesture() const {
    if (possibleCast()) {
        return _gesture.mid(_gesture.length() - _turnToCast, 1);
    } else {
        return _gesture.left(1);
    }
}

bool QSpell::checkValidSequence(const QSpell &s) {
    QString gesture1 = _gesture.right(turnToCast()), gesture2 = s._gesture.right(s._turnToCast);
    QString char1, char2, char1U, char2U;
    bool both_hand;
    int Ln = qMin(3, qMin(gesture1.length(), gesture2.length()));
    qDebug() << "QSpell::checkValidSequence" << _gesture << s._gesture << gesture1 << gesture2 << Ln;
    for (int i = 0; i > Ln; ++i) {
        char1 = gesture1.at(i);
        char2 = gesture2.at(i);
        char1U = char1.toUpper();
        char2U = char2.toUpper();

        both_hand = (char1U.compare(char1) != 0) || (char2U.compare(char2) != 0);
        if (both_hand && (char1U.compare(char2U) != 0)) {
            return false;
        }

        if ((char1.compare(char2) == 0) && (char1.compare("P") == 0)) {
            return false;
        }
    }
    return true;
}

QString QSpell::json() const {
    QString ng = nextGesture();
    return QString("{\"id\":%1,\"n\":\"%2\",\"g\":\"%3\",\"t\":%4,\"st\":%5,\"p\":%6,\"h\":%7,\"l\":%8,\"a\":%9,\"ng\":\"%10\",\"th\":%11,\"dt\":%12,\"active\":%13,\"dmg\":%14,\"basic\":%15}").
            arg(intToStr(_spellID), _name, _gesture, intToStr(_turnToCast), intToStr(_spellType), intToStr(_priority), intToStr(_hand), intToStr(_level), intToStr(_alreadyCasted)).
            arg(ng.toUpper(), boolToIntS(ng.compare(ng.toUpper()) != 0), intToStr(_defTarget), boolToIntS(_active), intToStr(_damage), boolToStr(_basic));
}

QString QSpell::toString() const {
    return json();
}

bool QSpell::operator < (const QSpell &s) const {
    //qDebug() << "QSpell::operator <" << this->_spellID << this->_name << s._spellID << s._name << this->_basic << s._basic;
    return QSpell::sortAsc(this, &s);
}

bool QSpell::sortAsc(const QSpell *s1, const QSpell *s2) {
    //qDebug() << "QSpell::sortAsc" << _orderType << s1->json() << s2->json();

    if (_orderType == 1) {
        if (s1->_alreadyCasted != s2->_alreadyCasted) {
            return s1->_alreadyCasted < s2->_alreadyCasted;
        }
        //return s1->_spellID < s2->_spellID;
        if (s1->_name.compare(s2->_name) != 0) {
            return s1->_name.compare(s2->_name) > 0;
        }
    } else if (_orderType == 2) {
        if (s1->_realPriority != s2->_realPriority) {
            return s1->_realPriority < s2->_realPriority;
        }
    }


    if ((s1->_spellType == s2->_spellType) && (s1->_turnToCast == s2->_turnToCast) && (s1->_turnToCast != -1) && (s1->_level != s2->_level)) {
        return s1->_level < s2->_level;
    }
    if (s1->_priority != s2->_priority) {
        return s1->_priority < s2->_priority;
    }
    if ((s1->_turnToCast != s2->_turnToCast) && (s1->_turnToCast != -1) && (s2->_turnToCast != -1)) {
        return s1->_turnToCast > s2->_turnToCast;
    }
    if (s1->_level != s2->_level) {
        return s1->_level < s2->_level;
    }
    if (s1->_hand != s2->_hand) {
        return s1->_hand < s2->_hand;
    }
    //if (s1->_spellID != s2->_spellID) {
        return s1->_spellID < s2->_spellID;
    //}

    //return s1->_hand < s2->_hand;
}

bool QSpell::sortDesc(const QSpell *s1, const QSpell *s2) {
    return !sortAsc(s1, s2);
}

void QSpell::setOrderType(int OrderType) {
    _orderType = OrderType;
}
