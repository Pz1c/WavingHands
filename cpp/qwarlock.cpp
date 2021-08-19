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
    _charmMonsterLeft = false;
    _charmMonsterRight = false;
    _forcedHand = 0;
    parseStatus();
    checkPossibleGesture();
    _SpellChecker = QWarlockSpellChecker::getInstance();
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
    _forcedHand = CopyFrom->_forcedHand;
    _SpellChecker = CopyFrom->_SpellChecker;
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

void QWarlock::setParalyzedHand(int Hand, const QString &Gesture) {
    qDebug() << "QWarlock::setParalyzedHand" << _name << Hand << Gesture;
    _forcedHand = Hand;
    QString lastGesture;
    switch(Hand) {
    case WARLOCK_HAND_LEFT:
        if (!Gesture.isEmpty()) {
            _possibleLeftGestures = Gesture;
        } else {
            lastGesture = _leftGestures.right(1);
            if (_isParaFC && gestureParaFCMap.contains(lastGesture)) {
                _possibleLeftGestures = gestureParaFCMap[lastGesture];
            } else if (!_isParaFC && gestureParaCFMap.contains(lastGesture)) {
                _possibleLeftGestures = gestureParaCFMap[lastGesture];
            } else {
                _possibleLeftGestures = lastGesture;
            }
        }
        break;
    case WARLOCK_HAND_RIGHT:
        if (!Gesture.isEmpty()) {
            _possibleRightGestures = Gesture;
        } else {
            lastGesture = _rightGestures.right(1);
            if (_isParaFC && gestureParaFCMap.contains(lastGesture)) {
                _possibleRightGestures = gestureParaFCMap[lastGesture];
            } else if (!_isParaFC && gestureParaCFMap.contains(lastGesture)) {
                _possibleRightGestures = gestureParaCFMap[lastGesture];
            } else {
                _possibleRightGestures = lastGesture;
            }
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
    qDebug() << "QWarlock::separatedString" << _name << logSpellItem(_bestSpellL) << logSpellItem(_bestSpellR) << _possibleSpells;
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
                   "\"lgL\":\"%35\",\"lgR\":\"%36\",\"smcL\":%37,\"smcR\":%38,\"changed_mind\":%39,"
                   "\"pgL\":\"%40\",\"pgR\":\"%41\",\"ptL\":\"%42\",\"ptR\":\"%43\",\"psL\":\"%44\",\"psR\":\"%45\",\"id\":\"%46\",\"fh\":%47}").
            arg(_name, _status, _leftGestures, _rightGestures, res, boolToStr(_player), sbsL, sbsR, intToStr(_hp)).
            arg(intToStr(_scared), intToStr(_confused), intToStr(_charmed), intToStr(_paralized), intToStr(_shield),
                intToStr(_coldproof), intToStr(_fireproof), intToStr(_poison), intToStr(_disease)).
            arg(intToStr(_amnesia), intToStr(_maladroit), intToStr(summon_left), intToStr(summon_right), _active ? "1" : "0").
            arg(intToStr(_mshield), intToStr(_delay), intToStr(_time_stop), intToStr(_haste), intToStr(_permanency), intToStr(_blindness), intToStr(_invisibility),
                _possibleLeftGestures, _possibleRightGestures, boolToStr(charmL), boolToStr(charmR)).
            arg(_leftGestures.trimmed().right(1), _rightGestures.trimmed().right(1), intToStr(spell_max_pass_left), intToStr(spell_max_pass_right), changed_mind).
            arg(_gestureL, _gestureR, _targetL, _targetR, _spellL, _spellR, _id, intToStr(_forcedHand));
}

bool QWarlock::player() const
{
    return _player;
}


void QWarlock::setSpellPriority(const QWarlock *enemy, const QList<QMonster *> &monsters) {
    qDebug() << "QWarlock::setSpellPriority start";
    int _coldproof_in = 999, _fireproof_in = 999;
    int paralysis_left = 3, paralysis_right = 3;
    bool clap_off_lighting_used = (_leftGestures.replace(" ", "").indexOf("WDDC") != -1) || (_rightGestures.replace(" ", "").indexOf("WDDC") != -1);
    qDebug() << "QWarlock::setSpellPriority" << "clap_off_lighting_used" << clap_off_lighting_used;

    _turnToCast.clear();
    for(int i = 0; i < SPELL_ID_MAX; ++i) {
        _turnToCast[i] = nullptr;
    }
    qDebug() << "QWarlock::setSpellPriority" << "init _turnToCast";

    foreach(QSpell *spell, _possibleSpells) {
        if (!_turnToCast.contains(spell->spellID()) || !_turnToCast[spell->spellID()] || (_turnToCast[spell->spellID()]->turnToCast() > spell->turnToCast())) {
            _turnToCast[spell->spellID()] = spell;
        }
    }
    qDebug() << "QWarlock::setSpellPriority" << "fill _turnToCast";
    _coldproof_in = _turnToCast[SPELL_RESIST_COLD] ? _turnToCast[SPELL_RESIST_COLD]->turnToCast() : 5;
    _fireproof_in = _turnToCast[SPELL_RESIST_HEAT] ? _turnToCast[SPELL_RESIST_HEAT]->turnToCast() : 5;
    qDebug() << "QWarlock::setSpellPriority" << "_coldproof_in" << _coldproof_in << "_fireproof_in" << _fireproof_in;

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
        case SPELL_TYPE_SHIELD:
            if ((_totalEnemyAttack >= 2) && (_totalFriendlyAttack < _totalEnemyHP)) {
                spell->changePriority(2);
            }
        /*case SPELL_TYPE_CONFUSION:
            if (((paralysis_left == 1) || (paralysis_right == 1)) && (spell->spellID() != SPELL_PARALYSIS) && (spell->spellID() != SPELL_PARALYSIS_FDF) && (spell->spellID() != SPELL_PARALYSIS_FDFD)) {
                spell->changePriority(-2);
            }
            break;*/
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
        case SPELL_CHARM_MONSTER:
            if (spell->turnToCast() == 1) {
                if (spell->hand() == WARLOCK_HAND_LEFT) {
                    _charmMonsterLeft = true;
                } else {
                    _charmMonsterRight = true;
                }
            }
            break;
        case SPELL_CLAP_OF_LIGHTNING:
            if (clap_off_lighting_used) {
                spell->setPriority(-100);
            }
            break;
        case SPELL_RESIST_COLD:
            if (_coldproof > 0) {
                spell->setPriority(-100);
            }
            break;
        case SPELL_RESIST_HEAT:
            if (_fireproof > 0) {
                spell->setPriority(-100);
            }
            break;
        case SPELL_SUMMON_FIRE_ELEMENTAL:
            if ((_fireproof == 0) || (_fireproof_in > spell->turnToCast() + 1)) {
                spell->setPriority(-5);
            } else if (spell->alreadyCasted() > 0){
                spell->setPriority(3);
            }
            break;
        case SPELL_SUMMON_ICE_ELEMENTAL:
            if ((_coldproof_in == 0) || (_coldproof_in > spell->turnToCast() + 1)) {
                spell->setPriority(-5);
            } else if (spell->alreadyCasted() > 0){
                spell->setPriority(3);
            }
            break;
        }
    }

    //std::sort(_possibleSpells.begin(), _possibleSpells.end());
    QSpell::setOrderType(0);
    qDebug() << "QWarlock::setSpellPriority" << "before sort";
    struct {
            bool operator()(const QSpell *s1, const QSpell *s2) const { return QSpell::sortDesc(s1, s2); }
    } customOrder;
    std::sort(_possibleSpells.begin(), _possibleSpells.end(), customOrder);
    QSpell::setOrderType(0);
    qDebug() << "QWarlock::setSpellPriority" << "after sort";
    logSpellList(_possibleSpells, "QWarlock::setSpellPriority")
}

QSpell *QWarlock::getSpellByFilter(const QList<QSpell *> &sl, int CastFrom, int CastTo, int SpellType, const QList<int> &notID, const QList<int> &byID) const {
    if (CastTo < CastFrom) return nullptr;

    foreach(QSpell *s, sl) {
        if ((s->turnToCast() < CastFrom) || (s->turnToCast() > CastTo)) {
            continue;
        }
        if ((s->spellType() != SpellType) && (SpellType != -1)) {
            continue;
        }
        if (!notID.empty() && (notID.indexOf(s->spellID()) != -1)) {
            continue;
        }
        if (!byID.empty() && (byID.indexOf(s->spellID()) == -1)) {
            continue;
        }

        return s;
    }
    return nullptr;
}

bool QWarlock::checkAntiSpell(const QSpell *as, const QSpell *s) const {
    if (!as) {
        return false;
    }
    if (as->spellType() == SPELL_TYPE_CONFUSION) {
        if (as->turnToCast() < s->turnToCast()) {
            return true;
        }

        if ((s->spellType() == SPELL_TYPE_CONFUSION) && (s->spellID() != SPELL_ANTI_SPELL)) {
            if (as->turnToCast() == s->turnToCast()) {
                return true;
            }
        }
    }
    if (s->spellType() == SPELL_TYPE_SUMMON_MONSTER) {
        if ((as->turnToCast() == s->turnToCast()) && (QList<int>({SPELL_COUNTER_SPELL1, SPELL_COUNTER_SPELL2, SPELL_CHARM_MONSTER}).indexOf(as->spellID()) != -1)) {
            return true;
        }
    }
    if ((as->spellType() == SPELL_TYPE_MAGIC_SHIELD) && (s->turnToCast() == as->turnToCast())) {
        return true;
    }

    return false;
}

const QString &QWarlock::id() const
{
    return _id;
}

void QWarlock::setId(const QString &newId)
{
    _id = newId;
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

void QWarlock::analyzeMonster(QList<QMonster *> &monsters) {
    qDebug() << "QWarlock::analyzeMonster";
    _elemental = 0;
    _totalFriendlyAttack = 0;
    _totalEnemyAttack = 0;
    _totalEnemyHP = 0;
    foreach(QMonster *m,  monsters) {
        m->setAttackStrength(0);
        if (m->iceElemental()) {
            _elemental = -m->getHp();
        } else if (m->fireElemental()) {
            _elemental = m->getHp();
        } else if (m->is_owner(_name)) {
            _totalFriendlyAttack += m->getStrength();
        } else {
            _totalEnemyAttack += m->getStrength();
            _totalEnemyHP += m->getHp();
        }
    }
}

void QWarlock::processMonster(QList<QMonster *> &monsters, QWarlock *enemy) {
    qDebug() << "QWarlock::processMonster";
    QList<QMonster *> _evilMonster;
    QList<QMonster *> _friendMonster;
    int tfa = 0, tea = 0, insertIdx = 0;//, tehp = 0; // total friendly attack, total enemy hp
    int elemental_attack = 0;
    // separate monster
    QMonster *elemental = nullptr;
    foreach(QMonster *m,  monsters) {
        m->setAttackStrength(0);
        if (m->iceElemental() || m->fireElemental()) {
            elemental = m;
            elemental_attack = m->getStrength();
        } else if (m->underControl()) {
            insertIdx = 0;
            foreach(QMonster *mm, _friendMonster) {
                if (m->getStrength() <= mm->getStrength()) {
                    break;
                }
                ++insertIdx;
            }
            _friendMonster.insert(insertIdx, m);
            tfa += m->getStrength();
        } else {
            insertIdx = 0;
            foreach(QMonster *mm, _evilMonster) {
                if (m->getHp() <= mm->getHp()) {
                    break;
                }
                ++insertIdx;
            }
            _evilMonster.insert(insertIdx, m);
            //tehp += m->getHp();
            tea += m->getStrength();
        }
    }

    bool all_on_enemy = _evilMonster.empty();
    QString new_target = enemy->name();
    // process elemental
    if (elemental) {
        if (elemental->iceElemental()) {
            if (_coldproof > 0) {
                elemental = nullptr;
            } else if ((_bestSpellL && (_bestSpellL->spellID() == SPELL_RESIST_COLD)) ||
                       (_bestSpellR && (_bestSpellR->spellID() == SPELL_RESIST_COLD))) {
                elemental = nullptr;
            }
        } else { // fire
            if (_fireproof > 0) {
                elemental = nullptr;
            } else if ((_bestSpellL && (_bestSpellL->spellID() == SPELL_RESIST_HEAT)) ||
                       (_bestSpellR && (_bestSpellR->spellID() == SPELL_RESIST_HEAT))) {
                elemental = nullptr;
            }
        }

        if (!elemental) {
            all_on_enemy = true;
        } else {
            //tehp += elemental->getHp();
            insertIdx = 0;
            foreach(QMonster *mm, _evilMonster) {
                if (elemental->getHp() <= mm->getHp() - 3) {
                    break;
                }
                ++insertIdx;
            }
            _evilMonster.insert(insertIdx, elemental);
            elemental = nullptr;
        }
    }

    // process monster
    int actual_hp = 0;
    foreach(QMonster *fm, _friendMonster) {
        tfa -= fm->getStrength();
        fm->setNewTarget(new_target);
        if (!all_on_enemy) {
            foreach(QMonster *em, _evilMonster) {
              actual_hp = em->getHp() - elemental_attack - em->attackStrength();
              if (actual_hp <= 0) {
                  continue;
              }
              if ((actual_hp < fm->getStrength()) && (tfa >= actual_hp)) {
                  // hope current monster will be targeted with next monster
                  continue;
              } else {
                  fm->setNewTarget(em->name());
                  em->setAttackStrength(em->attackStrength() + fm->getStrength());
              }
            }
        }
    }
}

void QWarlock::analyzeEnemy(QWarlock *enemy, const QString &paralyzed, const QString &charmed) {
    qDebug() << "QWarlock::analyzeEnemy";
    enemy->_bestSpellL = nullptr;
    enemy->_bestSpellR = nullptr;

    qreal priority = 0;
    foreach(QSpell *s, enemy->_possibleSpells) {
        if (s->turnToCast() >= s->length()) {
            continue;
        }
        if ((_fireproof > 0) && (ARR_FIRE_SPELLS.indexOf(s->spellID()) != -1)) {
            continue;
        }
        if ((_coldproof > 0) && (ARR_ICE_SPELLS.indexOf(s->spellID()) != -1)) {
            continue;
        }
        priority = intToReal(s->danger()) * intToReal(s->length() - s->turnToCast())/ intToReal(s->length());
        s->setRealPriority(priority);
        if (s->hand() == WARLOCK_HAND_LEFT) {
            if (!enemy->_bestSpellL || (enemy->_bestSpellL->realPriority() < priority)) {
                enemy->_bestSpellL = s;
            }
        } else {
            if (!enemy->_bestSpellR || (enemy->_bestSpellR->realPriority() < priority)) {
                enemy->_bestSpellR = s;
            }
        }
    }
    bool gesture_changed = false;
    int danger_hand = WARLOCK_HAND_LEFT;
    if (_bestSpellR && (!_bestSpellL || (_bestSpellR->realPriority() > _bestSpellL->realPriority()))) {
        danger_hand = WARLOCK_HAND_RIGHT;
    }
    if (paralyzed.indexOf(enemy->id()) != -1) {
        enemy->setParalyzedHand(danger_hand);
        gesture_changed = true;
    }
    if (charmed.indexOf(enemy->id()) != -1) {
        enemy->setParalyzedHand(danger_hand, "-");
        gesture_changed = true;
    }
    if (gesture_changed) {
        enemy->setPossibleSpells(_SpellChecker->getSpellsList(enemy));
    }
}

void QWarlock::breakEnemy(QWarlock *enemy) {
    qDebug() << "QWarlock::breakEnemy";
    _bestSpellL = nullptr;
    _bestSpellR = nullptr;
    qreal priority = 0;
    QSpell *as;

    QSpell::setOrderType(2);
    struct {
            bool operator()(const QSpell *s1, const QSpell *s2) const { return QSpell::sortDesc(s1, s2); }
    } customOrder;
    std::sort(enemy->_possibleSpells.begin(), enemy->_possibleSpells.end(), customOrder);
    QSpell::setOrderType(0);

    foreach(QSpell *s, enemy->_possibleSpells) {
        if ((s->alreadyCasted() == 0) || !s->possibleCast()) {
            continue;
        }
        qDebug() << "QWarlock::breakEnemy try to break" << s->json();
        priority = 1.0 / s->turnToCast();
        if (checkAntiSpell(_bestSpellL, s)) {
            qDebug() << "QWarlock::breakEnemy Left spell looks fine" << _bestSpellL->json();
            _bestSpellL->setRealPriority(_bestSpellL->realPriority() + priority);
            continue;
        }

        if (checkAntiSpell(_bestSpellR, s)) {
            qDebug() << "QWarlock::breakEnemy Right spell looks fine" << _bestSpellR->json();
            _bestSpellR->setRealPriority(_bestSpellR->realPriority() + priority);
            continue;
        }

        as = getAntiSpell(_possibleSpells, s);
        if (!as) {
            qDebug() << "QWarlock::breakEnemy not found anti spell";
            continue;
        }
        if ((as->hand() == WARLOCK_HAND_LEFT) && (!_bestSpellL || (_bestSpellL->turnToCast() > as->turnToCast()))) {
            as->setRealPriority(priority);
            _bestSpellL = as;
            qDebug() << "QWarlock::breakEnemy new best LEFT spell" << _bestSpellL->json();
        } else if ((as->hand() == WARLOCK_HAND_RIGHT) && (!_bestSpellR || (_bestSpellR->turnToCast() > as->turnToCast()))) {
            as->setRealPriority(priority);
            _bestSpellR = as;
            qDebug() << "QWarlock::breakEnemy new best RIGHT spell" << _bestSpellR->json();
        }
    }

    _gestureL.clear();
    _gestureR.clear();
    if (_maladroit > 0) {
        if (_bestSpellL && _bestSpellR) {
            if (_bestSpellL->realPriority() > _bestSpellR->realPriority()) {
                _bestSpellR = nullptr;
                _gestureL = _bestSpellL->nextGesture();
                _gestureR = _bestSpellL->nextGesture();
            } else {
                _bestSpellL = nullptr;
                _gestureL = _bestSpellR->nextGesture();
                _gestureR = _bestSpellR->nextGesture();
            }
        }
    } else if (_bestSpellL && _bestSpellR && (_bestSpellL->nextGesture().compare(_bestSpellR->nextGesture()) == 0) && (_bestSpellL->nextGesture().compare("P") == 0)) {
        if (_bestSpellL->realPriority() > _bestSpellR->realPriority()) {
            _bestSpellR = nullptr;
        } else {
            _bestSpellL = nullptr;
        }
    }

    qDebug() << "QWarlock::breakEnemy" << (_bestSpellL ? _bestSpellL->json() : "LEFT SPELL EMPTY");
    qDebug() << "QWarlock::breakEnemy" << (_bestSpellR ? _bestSpellR->json() : "RIGHT SPELL EMPTY");
}

void QWarlock::processMaladroit() {
    qDebug() << "QWarlock::processMaladroit" << _gestureL << _gestureR << logSpellItem(_bestSpellL) << logSpellItem(_bestSpellR);
    if (_gestureL.isEmpty() && _gestureR.isEmpty()) {
        QMap<QString, qreal> GW{{"C", 0}, {"W", 0}, {"S", 0}, {"D", 0}, {"F", 0}, {"P", -1000}, {">", 0}};
        qreal max_priority = 0;
        foreach(QSpell *s, _possibleSpells) {
            if (!s->possibleCast()) {
                continue;
            }
            GW[s->nextGesture()] += intToReal(s->priority())/intToReal(s->turnToCast());
        }
        foreach(QString g, POSSIBLE_GESTURES) {
            if (max_priority < GW[g]) {
                max_priority = GW[g];
                _gestureL = g;
                _gestureR = g;
            }
        }
    }
    qDebug() << "QWarlock::processMaladroit" << _gestureL << _gestureR << (_bestSpellL ? _bestSpellL->nextGesture() : "NONE") << (_bestSpellR ? _bestSpellR->nextGesture() : "NONE");
    if (_bestSpellL && (_gestureL.compare(_bestSpellL->nextGesture()) != 0)) {
        _bestSpellL = nullptr;
    }
    if (_bestSpellR && (_gestureR.compare(_bestSpellR->nextGesture()) != 0)) {
        _bestSpellR = nullptr;
    }
    foreach(QSpell *s, _possibleSpells) {
        if (!_bestSpellL && (s->hand() == WARLOCK_HAND_LEFT) && (s->nextGesture().compare(_gestureL) == 0)) {
            _bestSpellL = s;
        }

        if (!_bestSpellR && (s->hand() == WARLOCK_HAND_RIGHT) && (s->nextGesture().compare(_gestureR) == 0)) {
            _bestSpellR = s;
        }

        if (_bestSpellR && _bestSpellL) {
            break;
        }
    }
    qDebug() << "QWarlock::processMaladroit" << logSpellItem(_bestSpellL) << logSpellItem(_bestSpellR);
}

void QWarlock::attackEnemy(QWarlock *enemy) {
    qDebug() << "QWarlock::attackEnemy";
    if (_maladroit > 0) {
        processMaladroit();
        return;
    }

    if (_bestSpellR && _bestSpellL) {
        return;
    }

    QSpell *bL = _bestSpellL, *bR = _bestSpellR;
    bool erase_one;
    foreach(QSpell *s, _possibleSpells) {
        if (!bL && (s->hand() == WARLOCK_HAND_LEFT)) {
            bL = s;
        }

        if (!bR && (s->hand() == WARLOCK_HAND_RIGHT)) {
            bR = s;
        }

        if (bR && bL) {
            erase_one = false;
            if ((bR->spellType() == SPELL_TYPE_CONFUSION) && (bR->spellType() == bL->spellType())) {
                if (bR->turnToCast() == bL->turnToCast()) {
                    erase_one = true;
                }
            } else if ((bR->spellType() == SPELL_TYPE_MAGIC_SHIELD) && (bR->spellType() == bL->spellType()) && (bR->spellID() != SPELL_MAGIC_MIRROR) && (bR->turnToCast() == bL->turnToCast())) {
                erase_one = true;
            } else if (!bR->checkValidSequence(*bL)) {
                erase_one = true;
            }
            if (erase_one) {
                if (bL->priority() > bR->priority()) {
                    bR = nullptr;
                } else {
                    bL = nullptr;
                }
            }
        }
        if (bR && bL) {
            break;
        }
    }

    if (!_bestSpellL && bL) {
        _bestSpellL = bL;
    }

    if (!_bestSpellR && bR) {
        _bestSpellR = bR;
    }

    qDebug() << "QWarlock::attackEnemy" << logSpellItem(_bestSpellL) << logSpellItem(_bestSpellR);
}

void QWarlock::validateSpellForTurn() {
    if (_bestSpellL) {
        _gestureL = _bestSpellL->nextGesture().toUpper();
    } else {
        _gestureL = "";
    }
    if (_bestSpellR) {
        _gestureR = _bestSpellR->nextGesture().toUpper();
    } else {
        _gestureR = "";
    }

    if (_gestureL.isEmpty()) {
        if (_gestureR.isEmpty()) {
            _gestureL = "P";
            _gestureR = "D";
        } else if ((_gestureR.compare(_bestSpellR->nextGesture()) != 0) || (_gestureR.compare("C") != 0)) {
            _gestureL = _gestureR;
        }
    }
    if (_gestureR.isEmpty()) {
        if ((_gestureL.compare(_bestSpellL->nextGesture()) != 0) || (_gestureL.compare("C") != 0)) {
            _gestureR = _gestureL;
        }
    }

    qDebug() << "QWarlock::validateSpellForTurn LEFT" << _gestureL << logSpellItem(_bestSpellL);
    qDebug() << "QWarlock::validateSpellForTurn RIGHT" << _gestureR << logSpellItem(_bestSpellR);
}

bool QWarlock::isSummoning() const {
    if (_bestSpellL && (ARR_SUMMON.indexOf(_bestSpellL->spellID()) != -1)) {
        return WARLOCK_HAND_LEFT;
    }
    if (_bestSpellR && (ARR_SUMMON.indexOf(_bestSpellR->spellID()) != -1)) {
        return WARLOCK_HAND_RIGHT;
    }
    return 0;
}

QString QWarlock::getTargetForSpell(const QSpell *spell, const QWarlock *enemy, const QList<QMonster *> &monsters) {
    if (spell->spellType() == SPELL_TYPE_CURE) {
        //return _id;
    }
    if ((ARR_COUNTER_SPELL.indexOf(spell->spellID()) != -1) && (enemy->isSummoning() > 0)) {
        return enemy->_id;
    }
    if (spell->spellID() == SPELL_CHARM_MONSTER) {
        int max = 0;
        if (enemy->_bestSpellL && (ARR_SUMMON.indexOf(enemy->_bestSpellL->spellID()) != -1)) {
            max = qMax(max, enemy->_bestSpellL->danger());
        }
        if (enemy->_bestSpellR && (ARR_SUMMON.indexOf(enemy->_bestSpellR->spellID()) != -1)) {
            max = qMax(max, enemy->_bestSpellR->danger());
        }
        foreach(QMonster *m, monsters) {
            if (!m->is_owner(_name) && (max < m->getStrength())) {
                max = m->getStrength();
            }
        }
        if (max == 0) {
            return _id;
        }
        if (enemy->_bestSpellL && (ARR_SUMMON.indexOf(enemy->_bestSpellL->spellID()) != -1) && (max == enemy->_bestSpellL->danger())) {
            return "LH:" + enemy->_id;
        }
        if (enemy->_bestSpellR && (ARR_SUMMON.indexOf(enemy->_bestSpellR->spellID()) != -1) && (max == enemy->_bestSpellR->danger())) {
            return "RH:" + enemy->_id;
        }
        foreach(QMonster *m, monsters) {
            if (!m->is_owner(_name) && (max == m->getStrength())) {
                m->setUnderControl(1);
                return m->name();
            }
        }
    }


    return ""; // default target
}

void QWarlock::targetSpell(const QWarlock *enemy, const QList<QMonster *> &monsters) {
    _targetL.clear();
    _targetR.clear();
    _spellL.clear();
    _spellR.clear();
    QString tgl = _leftGestures.replace(" ", " "), tgr = _rightGestures.replace(" ", " ");
    tgl.append(_gestureL);
    tgr.append(_gestureR);
    QList<QSpell *> sl = _SpellChecker->getStriktSpellsList(tgl, tgr, false);
    if (sl.size() > 0) {
        foreach(QSpell *s, sl) {
            if (_spellL.isEmpty()) {
                _spellL = s->name();
                _targetL = getTargetForSpell(s, enemy, monsters);
            }
            delete s;
        }
        sl.clear();
    }

    sl = _SpellChecker->getStriktSpellsList(tgr, tgl, false);
    if (sl.size() > 0) {
        foreach(QSpell *s, sl) {
            if (_spellR.isEmpty()) {
                _spellR = s->name();
                _targetR = getTargetForSpell(s, enemy, monsters);
            }
            delete s;
        }
        sl.clear();
    }
    if (_targetL.isEmpty()) {
        _targetL = "Default";
    }
    if (_targetR.isEmpty()) {
        _targetR = "Default";
    }
}

void QWarlock::processDecision(QWarlock *enemy, QList<QMonster *> &monsters, const QString &paralyzed, const QString &charmed) {
    qDebug() << "QWarlock::processDecision" << enemy << monsters.size() << paralyzed << charmed;
    //
    analyzeMonster(monsters);
    //
    analyzeEnemy(enemy, paralyzed, charmed);
    //
    setSpellPriority(enemy, monsters);
    //
    breakEnemy(enemy);
    //
    attackEnemy(enemy);
    //
    validateSpellForTurn();
    //
    targetSpell(enemy, monsters);
    //
    processMonster(monsters, enemy);    
}

void QWarlock::setPossibleSpells(const QList<QSpell *> &possibleSpells)
{
    qDebug() << "QWarlock::setPossibleSpells" << _possibleSpells.size() << possibleSpells.size();
    if (_possibleSpells.size() > 0) {
        foreach(QSpell *s, _possibleSpells) {
            delete s;
        }
        _possibleSpells.clear();
    }
    _possibleSpells = possibleSpells;
    _bestSpellL = nullptr;
    _bestSpellR = nullptr;
    _targetL.clear();
    _targetR.clear();
    _gestureL.clear();
    _gestureR.clear();

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
