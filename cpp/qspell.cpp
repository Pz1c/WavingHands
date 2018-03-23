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
    _hand = WARLOCK_HAND_NONE;
}

int QSpell::calcPriority(int Priority, int Danger, int TurnToCast, bool Enemy) {
    if (Enemy) {
      return Danger - TurnToCast;
    } else {
      return Priority - TurnToCast;
    }
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

QSpell::QSpell(QSpell *Spell, int Hand, int TurnToCast, bool Enemy) {
    _spellID = Spell->_spellID;
    _gesture = Spell->_gesture;
    _name = Spell->_name;
    _spellType = Spell->_spellType;
    _turnToCast = TurnToCast;
    _priority = calcPriority(Spell->_priority, Spell->_danger, TurnToCast, Enemy);
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

QString QSpell::json() const {
    return QString("{\"id\":%1,\"n\":\"%2\",\"g\":\"%3\",\"t\":%4,\"st\":%5,\"p\":%6,\"h\":%7,\"l\":%8}").
            arg(intToStr(_spellID), _name, _gesture, intToStr(_turnToCast), intToStr(_spellType), intToStr(_priority), intToStr(_hand), intToStr(_level));
}

bool QSpell::sortAsc(QSpell *s1, QSpell *s2) {
    return s1->priority() < s2->priority();
}

bool QSpell::sortDesc(QSpell *s1, QSpell *s2) {
    return s1->priority() > s2->priority();
}

