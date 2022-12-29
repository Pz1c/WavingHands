#include "qspell.h"

int QSpell::_orderType = 0;

QSpell::QSpell(int SpellID, QString Gesture, QString Name, int SpellType, int Priority, int Level, int Danger, int DefTarget, int Damage,
               bool Active, bool Basic, int SpellGroup, int SpellBookLevel)
{
    _spellID = SpellID;
    _gesture = Gesture;
    _twoHand = Gesture.toUpper().compare(Gesture) != 0;
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
    _spellGroup = SpellGroup;
    _spellBookLevel = SpellBookLevel;
}

int QSpell::calcPriority(bool Enemy) {
    double res = Enemy ? _danger : _priority;
    qreal ln = _gesture.length();
    qreal already_cast = ln - _turnToCast;

    qDebug() << "QSpell::calcPriority" << _gesture << (static_cast<qreal>(static_cast<int>(100 * res))/100) << _turnToCast << ln;
    qreal prc = (already_cast + 1)/(ln + 1);
    res *= prc;
    qDebug() << "QSpell::calcPriority result" << res;
    return static_cast<int>(res);
}

bool QSpell::twoHand() const
{
    return _twoHand;
}

int QSpell::spellBookLevel() const
{
    return _spellBookLevel;
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
    _priority = Spell->_priority;
    _danger = Spell->_danger;
    _level = Spell->_level;
    _hand = Hand;
    _defTarget = Spell->_defTarget;
    _damage = Spell->_damage;
    _basic = Spell->_basic;
    _priority = calcPriority(Enemy);
    _realPriority = Spell->_realPriority;
    _spellGroup = Spell->_spellGroup;
    _spellBookLevel = Spell->_spellBookLevel;
    _twoHand = Spell->_twoHand;
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
    //qDebug() << "QSpell::checkValidSequence" << _gesture << s._gesture << gesture1 << gesture2 << Ln;
    for (int i = 0; i < Ln; ++i) {
        char1 = gesture1.at(i);
        char2 = gesture2.at(i);
        char1U = char1.toUpper();
        char2U = char2.toUpper();
        both_hand = (char1U.compare(char1) != 0) || (char2U.compare(char2) != 0);
        //qDebug() << "QSpell::checkValidSequence" << char1 << char1U << char2 << char2U << both_hand;
        if (both_hand && (char1U.compare(char2U) != 0)) {
            //qDebug() << "QSpell::checkValidSequence return false 1";
            return false;
        }

        if ((char1.compare(char2) == 0) && (char1.compare("P") == 0)) {
            //qDebug() << "QSpell::checkValidSequence return false 2";
            return false;
        }
    }
    //qDebug() << "QSpell::checkValidSequence return true";
    return true;
}

QString QSpell::json(bool Short) const {
    if (Short) {
        return QString("{\"g\":\"%1\",\"id\":%2,\"ac\":%3,\"h\":%4,\"sg\":%5,\"sbl\":%6}").
                arg(_gesture, intToStr(_spellID), intToStr(_alreadyCasted), intToStr(_hand), intToStr(_spellGroup), intToStr(_spellBookLevel));
    } else {
    QString ng = nextGesture();
    return QString("{\"id\":%1,\"n\":\"%2\",\"g\":\"%3\",\"t\":%4,\"st\":%5,\"p\":%6,\"h\":%7,\"l\":%8,\"a\":%9,\"ng\":\"%10\","
                   "\"th\":%11,\"dt\":%12,\"active\":%13,\"dmg\":%14,\"basic\":%15,\"rp\":%16,\"sg\":%17,\"sbl\":%18}").
            arg(intToStr(_spellID), _name, _gesture, intToStr(_turnToCast), intToStr(_spellType), intToStr(_priority),
                intToStr(_hand), intToStr(_level), intToStr(_alreadyCasted)).//1-9
            arg(ng.toUpper(), boolToIntS(ng.compare(ng.toUpper()) != 0), intToStr(_defTarget), boolToIntS(_active), intToStr(_damage), boolToStr(_basic), QString::number(_realPriority)).
            arg(intToStr(_spellGroup), intToStr(_spellBookLevel));
    }
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
        if (s1->_hand != s2->_hand) {
            return s1->_hand < s2->_hand;
        }

        /*if (s1->_spellGroup != s2->_spellGroup) {
            return s1->_spellGroup < s2->_spellGroup;
        }*/

        if (s1->_alreadyCasted != s2->_alreadyCasted) {
            return s1->_alreadyCasted < s2->_alreadyCasted;
        }

        if (s1->length() != s2->length()) {
            return s1->length() > s2->length();
        }

        if (s1->_gesture.compare(s2->_gesture) != 0) {
            QString s1g = s1->spellID() == SPELL_STAB ? "ZZZZZZZZZ" : s1->_gesture;
            QString s2g = s2->spellID() == SPELL_STAB ? "ZZZZZZZZZ" : s2->_gesture;
            return s1g.compare(s2g) > 0;
        }

        return s1->_spellID < s2->_spellID;
    } else if (_orderType == 2) {
        if (s1->_alreadyCasted != s2->_alreadyCasted) {
            return s1->_alreadyCasted < s2->_alreadyCasted;
        }
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

void QSpell::sort(QList<QSpell *> &list, int order_type) {
    setOrderType(order_type);
    bool print_debug = false && (order_type == 1);
    QList<QSpell *> res;
    //logSpellList(list, "QSpell::sort before");
    int check_idx = 0, high_idx = 0, low_idx = 0, max_iteration_count = 10, i, idx_diff, spell_idx = 0, insert_idx;
    foreach(QSpell *s, list) {
        ++spell_idx;
        if (res.isEmpty()) {
            res.append(s);
            continue;
        }
        high_idx = res.size();
        low_idx = 0;
        i = 0;
        if (print_debug) {
            qDebug() << "QSpell::sort start" << s->json(true);
        }
        while(++i < max_iteration_count) {
            idx_diff = high_idx - low_idx;
            if (print_debug) qDebug() << "while check" << i << low_idx << high_idx << idx_diff;
            if (idx_diff <= 1) {
                bool compare = sortDesc(res.at(low_idx), s);
                if (compare) {
                    insert_idx = low_idx + 1;
                } else {
                    insert_idx = low_idx;
                }
                res.insert(insert_idx, s);
                if (print_debug) {
                    qDebug() << "while last check" << res.at(low_idx)->json(true) << compare << insert_idx;
                }
                break;
            } else {
                check_idx = low_idx + idx_diff / 2;
                bool compare = sortDesc(res.at(check_idx), s);
                if (compare) {
                    low_idx = check_idx;
                } else {
                    high_idx = check_idx;
                }
                if (print_debug) {
                    qDebug() << "while internal check" << check_idx << low_idx << high_idx << res.at(check_idx)->json(true) << compare;
                }
            }
        }
        if (print_debug) {
            logSpellList(res, QString("QSpell::sort iteration %1").arg(intToStr(spell_idx)));
        }
    }
    list.clear();
    foreach(QSpell *s, res) {
        list.append(s);
    }
    setOrderType(0);
    if (print_debug) {
        logSpellList(list, "QSpell::sort after");
    }
}
