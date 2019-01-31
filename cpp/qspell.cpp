#include "qspell.h"

QSpell::QSpell(int SpellID, QString Gesture, QString Name, int SpellType, int Priority, int Level, int Danger)
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
}

int QSpell::calcPriority(int Priority, int Danger, int TurnToCast, bool Enemy, int FullTurnToCast) {
    int res;
    if (Enemy) {
      res = Danger - TurnToCast;
    } else {
      res = Priority - TurnToCast + (FullTurnToCast - TurnToCast);
    }
    if (FullTurnToCast == TurnToCast) {
        res -= 4;
    }

    return res;
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

QString QSpell::json() const {
    QString ng = nextGesture();
    return QString("{\"id\":%1,\"n\":\"%2\",\"g\":\"%3\",\"t\":%4,\"st\":%5,\"p\":%6,\"h\":%7,\"l\":%8,\"a\":%9,\"ng\":\"%10\",\"th\":%11}").
            arg(intToStr(_spellID), _name, _gesture, intToStr(_turnToCast), intToStr(_spellType), intToStr(_priority), intToStr(_hand), intToStr(_level), intToStr(_alreadyCasted)).
            arg(ng.toUpper(), ng.compare(ng.toUpper()) == 0 ? "0" : "1");
}

bool QSpell::sortAsc(QSpell *s1, QSpell *s2) {
    if (s1->_priority != s2->_priority) {
        return s1->_priority < s2->_priority;
    }
    if (s1->_turnToCast != s2->_turnToCast) {
        return s1->_turnToCast > s2->_turnToCast;
    }
    return s1->_level < s2->_level;
}

bool QSpell::sortDesc(QSpell *s1, QSpell *s2) {
    return !sortAsc(s1, s2);
}

