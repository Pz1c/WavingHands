#include "qwarlock.h"
#include "qwarlockspellchecker.h"

QWarlock::QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player, bool isAI) :
    _scared(0), _confused(0), _charmed(0), _paralized(0), _shield(0), _coldproof(0), _fireproof(0), _hp(0), _poison(0), _disease(0), _amnesia(0),
    _maladroit(0), _bestSpellL(nullptr), _bestSpellR(nullptr)
{
    _name = Name;
    _status = Status;
    _leftGestures = LeftGestures;
    _rightGestures = RightGestures;
    _player = Player;
    _AI = isAI;
    parseStatus();
    checkPossibleGesture();
}

QWarlock::QWarlock(QWarlock *CopyFrom) {
    _name = CopyFrom->_name;
    _status = CopyFrom->_status;
    _leftGestures = CopyFrom->_leftGestures;
    _rightGestures = CopyFrom->_rightGestures;
    _player = CopyFrom->_player;
    _AI = CopyFrom->_AI;
    _possibleLeftGestures = CopyFrom->_possibleLeftGestures;
    _possibleRightGestures = CopyFrom->_possibleRightGestures;
    _surrender = CopyFrom->_surrender;
    _hp = CopyFrom->_hp;
    _active = CopyFrom->_active;
    _scared = CopyFrom->_scared;
    _confused = CopyFrom->_confused;
    _charmed = CopyFrom->_charmed;
    _paralized = CopyFrom->_paralized;
    _shield = CopyFrom->_shield;
    _coldproof = CopyFrom->_coldproof;
    _fireproof = CopyFrom->_fireproof;
    _poison = CopyFrom->_poison;
    _disease = CopyFrom->_disease;
    _amnesia = CopyFrom->_amnesia;
    _maladroit = CopyFrom->_maladroit;
    _mshield = CopyFrom->_mshield;
    _delay = CopyFrom->_delay;
    _time_stop = CopyFrom->_time_stop;
    _haste = CopyFrom->_haste;
    _permanency = CopyFrom->_permanency;
    _blindness = CopyFrom->_blindness;
    _invisibility = CopyFrom->_invisibility;
}

QWarlock::~QWarlock() {
    if (_possibleSpells.isEmpty()) {
        return;
    }
    foreach(QSpell *s, _possibleSpells) {
        delete s;
    }
    _possibleSpells.clear();
}

QString QWarlock::name() {
    return _name;
}

QString QWarlock::status() {
    return _status;
}


QString QWarlock::leftGestures() {
    return _leftGestures;
}

QString QWarlock::rightGestures() {
    return _rightGestures;
}

bool QWarlock::isParaFDF() const
{
    return _isParaFDF;
}

void QWarlock::setIsParaFDF(bool isParaFDF)
{
    _isParaFDF = isParaFDF;
}

void QWarlock::setParalyzedHand(int Hand) {
    QString lastGesture;
    switch(Hand) {
    case WARLOCK_HAND_LEFT:
        lastGesture = _leftGestures.right(1);
        if (_isParaFC && gestureParaFCMap.contains(lastGesture)) {
            _possibleLeftGestures = gestureParaFCMap[lastGesture];
        } else if (!_isParaFC && gestureParaCFMap.contains(lastGesture)) {
            _possibleLeftGestures = gestureParaCFMap[lastGesture];
        } else {
            _possibleLeftGestures = lastGesture;
        }
        break;
    case WARLOCK_HAND_RIGHT:
        lastGesture = _rightGestures.right(1);
        if (_isParaFC && gestureParaFCMap.contains(lastGesture)) {
            _possibleRightGestures = gestureParaFCMap[lastGesture];
        } else if (!_isParaFC && gestureParaCFMap.contains(lastGesture)) {
            _possibleRightGestures = gestureParaCFMap[lastGesture];
        } else {
            _possibleRightGestures = lastGesture;
        }
        break;
    }
}

QString QWarlock::possibleRightGestures() const
{
    return _possibleRightGestures;
}

QString QWarlock::possibleLeftGestures() const
{
    return _possibleLeftGestures;
}

void QWarlock::setPossibleGestures(QString left, QString right) {
    /*_possibleLeftGestures = left;
    _possibleRightGestures = right;
    if (_possibleLeftGestures.indexOf("As Right") != -1) {
        _possibleLeftGestures = _possibleRightGestures;
    }*/
    qDebug() << "QWarlock::setPossibleGestures" << left << right << _possibleLeftGestures << _possibleRightGestures;
    //*/
}

void QWarlock::parseStatus() {
    _surrender = _status.indexOf("Surrender") != -1;
    int pos = 0;
    _hp = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Health", ":", " #;#)", pos, true));
    _active = _hp > 0 && !_surrender;
    _scared = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Afraid", "(", ")", pos, true));
    _confused = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Confused", "(", ")", pos, true));
    _charmed = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Charmed", "(", ")", pos, true));
    _paralized = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Paralysed", "(", ")", pos, true));
    _shield = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Shield", "(", ")", pos, true));
    _coldproof = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Coldproof", "(", ")", pos, true));
    _fireproof = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Fireproof", "(", ")", pos, true));
    _poison = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Poison", "(", ")", pos, true));
    _disease = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Disease", "(", ")", pos, true));
    _amnesia = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Forgetful", "(", ")", pos, true));
    _maladroit = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Maladroit", "(", ")", pos, true));
    _mshield = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "MShield", "(", ")", pos, true));
    _delay = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Delay", "(", ")", pos, true));
    _time_stop = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Time stop", "(", ")", pos, true));
    _haste = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Haste", "(", ")", pos, true));
    _permanency = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Permanency", "(", ")", pos, true));
    _blindness = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Blindness", "(", ")", pos, true));
    _invisibility = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Invisibility", "(", ")", pos, true));
}

void QWarlock::checkPossibleGesture() {
    if (_amnesia > 0) {
        _possibleLeftGestures = _leftGestures.right(1);
        _possibleRightGestures = _rightGestures.right(1);
    } else if (_scared > 0) {
        _possibleLeftGestures = "W,P";
        _possibleRightGestures = "W,P";
    } else if ((_maladroit > 0) && _AI) {
        _possibleLeftGestures = "W,S,D,C,F";
        _possibleRightGestures = "W,S,D,C,F";
    } else {
        _possibleLeftGestures = "W,S,D,P,C,F";
        _possibleRightGestures = "W,S,D,P,C,F";
    }
}

void QWarlock::setIsMaladroit(bool newIsMaladroit)
{
    _isMaladroit = newIsMaladroit;
}

const QList<QSpell *> &QWarlock::possibleSpells() const
{
    return _possibleSpells;
}

int QWarlock::maladroit() const
{
    return _maladroit;
}

bool QWarlock::isParaFC() const
{
    return _isParaFC;
}

void QWarlock::setIsParaFC(bool newIsParaFC)
{
    _isParaFC = newIsParaFC;
}

QString QWarlock::separatedString() {
    //int Ln = _leftGestures.length();
    qDebug() << "QWarlock::separatedString" << _name << _bestSpellL << _bestSpellR << _possibleSpells;
    bool charmL = false;
    bool charmR = false;
    int summon_left = SPELL_ID_MAX, summon_right = SPELL_ID_MAX, spell_max_pass_left = 0, spell_max_pass_right = 0;
    QString res;
    foreach(QSpell *s, _possibleSpells) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        if (s->hand() == WARLOCK_HAND_LEFT) {
            spell_max_pass_left = qMax(spell_max_pass_left, s->alreadyCasted());
        } else {
            spell_max_pass_right = qMax(spell_max_pass_right, s->alreadyCasted());
        }
        if (s->turnToCast() == 1) {
            if (s->spellType() == SPELL_TYPE_SUMMON_MONSTER) {
                if  (s->hand() == WARLOCK_HAND_LEFT) {
                    summon_left = qMin(summon_left, s->spellID());
                } else {
                    summon_right = qMin(summon_right, s->spellID());
                }
            } else if (s->spellID() == SPELL_CHARM_PERSON) {
                if  (s->hand() == WARLOCK_HAND_LEFT) {
                    charmL = true;
                } else {
                    charmR = true;
                }
            }
        }

        res.append(s->json());
    }
    res.prepend("[").append("]");
    if (summon_right == SPELL_ID_MAX) {
        summon_right = 0;
    }
    if (summon_left == SPELL_ID_MAX) {
        summon_left = 0;
    }
    QString sbsL = "{}";
    QString sbsR = "{}";
    if (_bestSpellL) {
        sbsL = _bestSpellL->json();
        qDebug() << "sbsL" << sbsL;
    }
    if (_bestSpellR) {
        sbsR = _bestSpellR->json();
        qDebug() << "sbsR" << sbsR;
    }
    
    QString changed_mind = (_charmed > 0) || (_confused > 0) || (_paralized > 0) ? "1" : "0";

    return QString("{\"name\":\"%1\",\"status\":\"%2\",\"L\":\"%3\",\"R\":\"%4\",\"spells\":%5,\"player\":%6,\"bsL\":%7,\"bsR\":%8,\"hp\":%9,\"scared\":%10,"
                   "\"confused\":%11,\"charmed\":%12,\"paralized\":%13,\"shield\":%14,\"coldproof\":%15,\"fireproof\":%16,\"poison\":%17,\"disease\":%18,"
                   "\"amnesia\":%19,\"maladroit\":%20,\"summon_left\":%21,\"summon_right\":%22,\"active\":%23,\"mshield\":%24,\"delay\":%25,\"time_stop\":%26,"
                   "\"haste\":%27,\"permanency\":%28,\"blindness\":%29,\"invisibility\":%30,\"plg\":\"%31\",\"prg\":\"%32\",\"charm_left\":%33,\"charm_right\":%34,"
                   "\"lgL\":\"%35\",\"lgR\":\"%36\",\"smcL\":%37,\"smcR\":%38,\"changed_mind\":%39}").
            arg(_name, _status, _leftGestures, _rightGestures, res, boolToStr(_player), sbsL, sbsR, intToStr(_hp)).
            arg(intToStr(_scared), intToStr(_confused), intToStr(_charmed), intToStr(_paralized), intToStr(_shield),
                intToStr(_coldproof), intToStr(_fireproof), intToStr(_poison), intToStr(_disease)).
            arg(intToStr(_amnesia), intToStr(_maladroit), intToStr(summon_left), intToStr(summon_right), _active ? "1" : "0").
            arg(intToStr(_mshield), intToStr(_delay), intToStr(_time_stop), intToStr(_haste), intToStr(_permanency), intToStr(_blindness), intToStr(_invisibility),
                _possibleLeftGestures, _possibleRightGestures, boolToStr(charmL), boolToStr(charmR)).
            arg(_leftGestures.trimmed().right(1), _rightGestures.trimmed().right(1), intToStr(spell_max_pass_left), intToStr(spell_max_pass_right), changed_mind);
}

bool QWarlock::player() const
{
    return _player;
}

/*
void QWarlock::setSpellPriority(const QWarlock *enemy, const QList<QMonster *> &monsters) {
    int _coldproof_in = 999, _fireproof_in = 999;
    int under_attack = 0, paralysis_left = 3, paralysis_right = 3, monster_cnt_friend = 0, monster_cnt_enemy = 0;
    foreach(QMonster *m,  monsters) {
        if (m->is_owner(_name)) {
            ++monster_cnt_friend;
        } else {
            ++monster_cnt_enemy;
        }
        if (m->is_under_attack(_name)) {
            under_attack += m->getStrength();
        }
    }

    foreach(QSpell *spell, _possibleSpells) {
        //spell->setPriority(0);
        switch(spell->spellID()) {
            case SPELL_RESIST_COLD:
                _coldproof_in = qMin(_coldproof_in, spell->turnToCast());
                break;
            case SPELL_RESIST_HEAT:
                _fireproof_in = qMin(_fireproof_in, spell->turnToCast());
                break;
            case SPELL_PARALYSIS:
            case SPELL_PARALYSIS_FDF:
            case SPELL_PARALYSIS_FDFD:
                if (spell->hand() == WARLOCK_HAND_LEFT) {
                    paralysis_left = qMin(paralysis_left, spell->turnToCast());
                } else {
                    paralysis_right = qMin(paralysis_right, spell->turnToCast());
                }
                break;
        }
    }

    foreach(QSpell *spell, _possibleSpells) {
        switch(spell->spellType()) {
        case SPELL_TYPE_MASSIVE:
        case SPELL_TYPE_ELEMENTAL:
            if (((spell->level() == 0) && (_coldproof == 0) && (_coldproof_in - (spell->spellType() == SPELL_TYPE_ELEMENTAL ? 0 : 1) > spell->turnToCast())) ||
                ((spell->level() == 1) && (_fireproof == 0) && (_fireproof_in - (spell->spellType() == SPELL_TYPE_ELEMENTAL ? 0 : 1) > spell->turnToCast()))) {
                spell->changePriority(-4);
            }
            if (enemy && (((spell->level() == 0) && (enemy->_coldproof > 0)) || ((spell->level() == 1) && (enemy->_fireproof > 0)))) {
                spell->changePriority(-4);
            }
            break;
        case SPELL_CURE_HEAVY_WOUNDS:
            if (_disease >= spell->turnToCast()) {
                spell->changePriority(5);
            }
            break;
        case SPELL_TYPE_SHIELD:
            spell->changePriority(under_attack);
            break;
        case SPELL_TYPE_CONFUSION:
            if (((paralysis_left == 1) || (paralysis_right == 1)) && (spell->spellID() != SPELL_PARALYSIS) && (spell->spellID() != SPELL_PARALYSIS_FDF) && (spell->spellID() != SPELL_PARALYSIS_FDFD)) {
                spell->changePriority(-2);
            }
            break;
        }

        switch(spell->spellID()) {
        case SPELL_CURE_HEAVY_WOUNDS:
            if ((_disease > 0) && (_disease >= spell->turnToCast())) {
                spell->changePriority(5);
            }
            break;
        case SPELL_REMOVE_ENCHANTMENT:
        case SPELL_DISPEL_MAGIC:
            if ((qMin(_disease, _poison) > 0) && (qMin(_disease, _poison) >= spell->turnToCast())) {
                spell->changePriority(5);
            }
            break;
        case SPELL_PARALYSIS:
        case SPELL_PARALYSIS_FDF:
        case SPELL_PARALYSIS_FDFD:
            if (((spell->hand() == WARLOCK_HAND_LEFT) && (paralysis_right < spell->turnToCast())) || ((spell->hand() == WARLOCK_HAND_RIGHT) && (paralysis_left < spell->turnToCast()))) {
                spell->changePriority(-5);
            }
            break;
        }
    }

    //std::sort(_possibleSpells.begin(), _possibleSpells.end());
}*/

QSpell *QWarlock::getSpellByFilter(const QList<QSpell *> &sl, int CastFrom, int CastTo, int SpellType, const QList<int> &notID, const QList<int> &byID) const {
    if (CastTo > CastFrom) return nullptr;

    foreach(QSpell *s, sl) {
        if ((s->turnToCast() < CastFrom) || (s->turnToCast() > CastTo)) {
            continue;
        }
        if ((s->spellType() != SpellType) && (SpellType != -1)) {
            continue;
        }
        if ((notID.length() > 0) && (notID.indexOf(s->spellID()) != -1)) {
            continue;
        }
        if ((byID.length() > 0) && (byID.indexOf(s->spellID()) == -1)) {
            continue;
        }

        return s;
    }
    return nullptr;
}

QSpell *QWarlock::getAntiSpell(const QList<QSpell *> &sl, const QSpell *s) const {
    QSpell *res = getSpellByFilter(sl, 1, s->turnToCast() - 1, SPELL_TYPE_CONFUSION, QList<int>(), QList<int>());
    if (res) return res;

    if ((s->spellType() == SPELL_TYPE_CONFUSION) && (s->spellID() != SPELL_ANTI_SPELL)) {
        res = getSpellByFilter(sl, s->turnToCast(), s->turnToCast(), SPELL_TYPE_CONFUSION, QList<int>({SPELL_ANTI_SPELL}), QList<int>());
        if (res) return res;
    }

    if (s->spellType() == SPELL_TYPE_SUMMON_MONSTER) {
        res = getSpellByFilter(sl, s->turnToCast(), s->turnToCast(), -1, QList<int>(), QList<int>({SPELL_COUNTER_SPELL1, SPELL_COUNTER_SPELL2, SPELL_CHARM_MONSTER}));
        if (res) return res;
    }


    return getSpellByFilter(sl, s->turnToCast(), s->turnToCast(), SPELL_TYPE_MAGIC_SHIELD, QList<int>(), QList<int>());
}

void QWarlock::setPriceForMap(QMap<QString, qreal> &L, QMap<QString, qreal> &R, const QString &NG, qreal price) {
    QString U = NG.toUpper();
    if (U.compare(NG) != 0) {
        L[U] += price/2;
        R[U] += price/2;
    } else {
        L[U] += price;
    }
}

void QWarlock::processDecision(const QWarlockSpellChecker &SpellChecker, QWarlock *enemy, const QList<QMonster *> monsters) {
    QMap<QString, qreal> LeftHand {{"C", 0}, {"W", 0}, {"S", 0}, {"D", 0}, {"F", 0}, {"P", 0}, {">", 0}};
    QMap<QString, qreal> RightHand {{"C", 0}, {"W", 0}, {"S", 0}, {"D", 0}, {"F", 0}, {"P", 0}, {">", 0}};

    qDebug() << "QWarlock::processDecision" << QDateTime::currentMSecsSinceEpoch() << LeftHand << RightHand;
    qreal price;
    foreach(QSpell *css, _possibleSpells) {
        if (!css || !css->active() || (css->length() == 1)) {
            continue;
        }
        price = static_cast<qreal>(qMax(1, css->damage()))/css->turnToCast();
        if (css->hand() == WARLOCK_HAND_LEFT) {
            setPriceForMap(LeftHand, RightHand, css->nextGesture(), price);
        } else {
            setPriceForMap(RightHand, LeftHand, css->nextGesture(), price);
        }
    }
    qDebug() << "QWarlock::processDecision" << QDateTime::currentMSecsSinceEpoch() << LeftHand << RightHand;

    foreach(QSpell *ces, enemy->_possibleSpells) {
        if ((ces->turnToCast() >= ces->length()) || (ces->length() == 1) || !ces->active()) {
            continue;
        }

        //if (ces->turnToCast() == 1) {
            QSpell *as = getAntiSpell(_possibleSpells, ces);
            if (as) {
                if (as->hand() == WARLOCK_HAND_LEFT) {
                    setPriceForMap(LeftHand, RightHand, as->nextGesture(), 3/ces->turnToCast());
                } else {
                    setPriceForMap(RightHand, LeftHand, as->nextGesture(), 3/ces->turnToCast());
                }
            }
        //}
    }
    qDebug() << "QWarlock::processDecision" << QDateTime::currentMSecsSinceEpoch() << LeftHand << RightHand;




}

void QWarlock::setPossibleSpells(const QList<QSpell *> &possibleSpells)
{
    qDebug() << "QWarlock::setPossibleSpells";
    _possibleSpells = possibleSpells;
    _bestSpellL = nullptr;
    _bestSpellR = nullptr;

    return;

    /*QMap<QString, qreal> LeftHand {{"C", 0}, {"W", 0}, {"S", 0}, {"D", 0}, {"F", 0}, {"P", 0}, {">", 0}};
    QMap<QString, qreal> RightHand {{"C", 0}, {"W", 0}, {"S", 0}, {"D", 0}, {"F", 0}, {"P", 0}, {">", 0}};
    qreal price;
    foreach(QSpell *css, _possibleSpells) {
        if (!css || !css->active()) {
            continue;
        }
        price = static_cast<qreal>(qMax(1, css->damage()))/css->turnToCast();
        if (css->hand() == WARLOCK_HAND_LEFT) {
            setPriceForMap(LeftHand, RightHand, css->nextGesture(), price);
        } else {
            setPriceForMap(RightHand, LeftHand, css->nextGesture(), price);
        }
    }
    qDebug() << "QWarlock::setPossibleSpells" << LeftHand << RightHand;

    foreach(QSpell *ces, enemy->_possibleSpells) {
        if ((ces->turnToCast() >= ces->length()) || (ces->length() == 1) || !ces->active()) {
            continue;
        }

        //if (ces->turnToCast() == 1) {
            QSpell *as = getAntiSpell(_possibleSpells, ces);
            if (as) {
                if (as->hand() == WARLOCK_HAND_LEFT) {
                    setPriceForMap(LeftHand, RightHand, as->nextGesture(), 3/ces->turnToCast());
                } else {
                    setPriceForMap(RightHand, LeftHand, as->nextGesture(), 3/ces->turnToCast());
                }
            }
        //}
    }
    qDebug() << "QWarlock::setPossibleSpells" << LeftHand << RightHand;



    //logSpellList(possibleSpells, "QWarlock::setPossibleSpells before");
    //setSpellPriority(enemy, monsters);
    //logSpellList(possibleSpells, "QWarlock::setPossibleSpells after");
    //if (enemy) {
    //    setAntispell(enemy);
    //}
    //checkSpells();
    */
}

void QWarlock::checkSpells() {
    qDebug() << "QWarlock::checkSpells" << _name;
    logSpellList(_possibleSpells, "QWarlock::checkSpells");

    if (_maladroit > 0) {
        QStringList pg {"W", "S",  "D",  "C", "F"};
        int max_priority = SPELL_PRIORITY_ZERO, curr_priority = 0;
        QString best_gesture;
        foreach(QString G, pg) {
            _bestSpellL = nullptr;
            _bestSpellR = nullptr;
            curr_priority = 0;
            foreach(QSpell *s, _possibleSpells) {
                if (!s->possibleCast() || (s->nextGesture().toUpper().compare(G) != 0)) {
                    continue;
                }
                if ((s->hand() == WARLOCK_HAND_LEFT) && !_bestSpellL) {
                    _bestSpellL = s;
                    curr_priority += s->priority();
                }
                if ((s->hand() == WARLOCK_HAND_RIGHT) && !_bestSpellR) {
                    _bestSpellR = s;
                    curr_priority += s->priority();
                }
                if (_bestSpellL && _bestSpellR) {
                    break;
                }
            }
            if (max_priority < curr_priority) {
                max_priority = curr_priority;
                best_gesture = G;
            }
        }
        _bestSpellL = nullptr;
        _bestSpellR = nullptr;
        _gestureL = best_gesture;
        _gestureR = best_gesture;
        foreach(QSpell *s, _possibleSpells) {
            if (!s->possibleCast() || (s->nextGesture().toUpper().compare(best_gesture) != 0)) {
                continue;
            }
            if ((s->hand() == WARLOCK_HAND_LEFT) && !_bestSpellL) {
                _bestSpellL = s;
            }
            if ((s->hand() == WARLOCK_HAND_RIGHT) && !_bestSpellR) {
                _bestSpellR = s;
            }
            if (_bestSpellL && _bestSpellR) {
                break;
            }
        }
    } else {
        foreach(QSpell *s, _possibleSpells) {
            if ((s->hand() == WARLOCK_HAND_LEFT) && !_bestSpellL) {
                _bestSpellL = s;
                _gestureL = s->nextGesture();
            }
            if ((s->hand() == WARLOCK_HAND_RIGHT) && !_bestSpellR) {
                _bestSpellR = s;
                _gestureR = s->nextGesture();
            }
            if (_bestSpellL && _bestSpellR) {
                break;
            }
        }
    }
    qDebug() << "QWarlock::checkSpells" << _bestSpellL << _bestSpellR;
}

void QWarlock::emulateTurn(const QString &left, const QString &right) {
    _leftGestures.append(left);
    _rightGestures.append(right);
    if (_scared > 0) --_scared;
    if (_confused > 0) --_confused;
    if (_charmed > 0) --_charmed;
    if (_paralized > 0) --_paralized;
    if (_shield > 0) --_shield;
    if (_poison > 0) --_poison;
    if (_disease > 0) --_disease;
    if (_amnesia > 0) --_amnesia;
    if (_maladroit > 0) --_maladroit;
    if (_mshield > 0) --_mshield;
    if (_delay > 0) --_delay;
    if (_time_stop > 0) --_time_stop;
    if (_haste > 0) --_haste;
    if (_permanency > 0) --_permanency;
    if (_blindness > 0) --_blindness;
    if (_invisibility > 0) --_invisibility;
    checkPossibleGesture();
}
