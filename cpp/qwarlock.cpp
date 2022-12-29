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
    _totalEnemyAttack = 0;
    _totalEnemyHP = 0;
    _totalFriendlyAttack = 0;
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
    _forcedGesture = CopyFrom->_forcedGesture;
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

void QWarlock::setParalyzedHand(int Hand, const QString &Gesture, bool SetPossibleGesture) {
    qDebug() << "QWarlock::setParalyzedHand" << _name << Hand << Gesture << SetPossibleGesture;
    _forcedHand = Hand;
    _forcedGesture = Gesture;
    if (!SetPossibleGesture) {
        return ;
    }
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
    qDebug() << "QWarlock::setParalyzedHand" << _possibleLeftGestures << _possibleRightGestures;
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
    _banked_spell = QWarlockUtils::getStringFromData(_status, "(Banked", ":", ")", pos, true);
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
    qDebug() << "QWarlock::separatedString" << _name << logSpellItem(_bestSpellL) << logSpellItem(_bestSpellR);
    /*if (_name.compare("Galbarad") == 0) {
    logSpellList(_possibleSpells, "QWarlock::separatedString");
    }*/
    bool charmL = false;
    bool charmR = false;
    int summon_left = SPELL_ID_MAX, summon_right = SPELL_ID_MAX, spell_max_pass_left = 0, spell_max_pass_right = 0;
    /*QSpell::setOrderType(1);
    qDebug() << "QWarlock::separatedString" << "before sort";
    struct {
            bool operator()(const QSpell *s1, const QSpell *s2) const { return QSpell::sortDesc7(s1, s2); }
    } customOrder;
    std::sort(_possibleSpells.begin(), _possibleSpells.end(), customOrder);
    QSpell::setOrderType(0);*/
    QSpell::sort(_possibleSpells,  1);//_player ? 1 : 0);
    /*if (_name.compare("Galbarad") == 0) {
    logSpellList(_possibleSpells, "after sort");
    }*/

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
                   "\"pgL\":\"%40\",\"pgR\":\"%41\",\"ptL\":\"%42\",\"ptR\":\"%43\",\"psL\":\"%44\",\"psR\":\"%45\",\"id\":\"%46\",\"fh\":%47,\"bank\":\"%48\",\"fg\":\"%49\"}").
            arg(_name, _status, _leftGestures, _rightGestures, res, boolToStr(_player), sbsL, sbsR, intToStr(_hp)).
            arg(intToStr(_scared), intToStr(_confused), intToStr(_charmed), intToStr(_paralized), intToStr(_shield),
                intToStr(_coldproof), intToStr(_fireproof), intToStr(_poison), intToStr(_disease)).
            arg(intToStr(_amnesia), intToStr(_maladroit), intToStr(summon_left), intToStr(summon_right), _active ? "1" : "0").
            arg(intToStr(_mshield), intToStr(_delay), intToStr(_time_stop), intToStr(_haste), intToStr(_permanency), intToStr(_blindness), intToStr(_invisibility),
                _possibleLeftGestures, _possibleRightGestures, boolToStr(charmL), boolToStr(charmR)).
            arg(_leftGestures.trimmed().right(1), _rightGestures.trimmed().right(1), intToStr(spell_max_pass_left), intToStr(spell_max_pass_right), changed_mind).
            arg(_gestureL, _gestureR, _targetL, _targetR, _spellL, _spellR, _id, intToStr(_forcedHand), _banked_spell, _forcedGesture);
}

bool QWarlock::player() const
{
    return _player;
}

int QWarlock::getTurnToCastBySpellID(int spell_id, int def_value, int hand) const {
    if (!_turnToCast.contains(spell_id) || !_turnToCast[spell_id]) {
        return def_value;
    }
    if ((hand != -1) && (_turnToCast[spell_id]->hand() != hand)) {
        return def_value;
    }
    return _turnToCast[spell_id]->turnToCast();
}

int QWarlock::forcedHand() const
{
    return _forcedHand;
}

int QWarlock::paralized() const
{
    return _paralized;
}

void QWarlock::fillTurnToCast() {
    _turnToCast.clear();
    for(int i = 0; i < SPELL_ID_MAX; ++i) {
        _turnToCast[i] = nullptr;
    }
    qDebug() << "QWarlock::fillTurnToCast";

    foreach(QSpell *spell, _possibleSpells) {
        if (!_turnToCast.contains(spell->spellID()) || !_turnToCast[spell->spellID()] || (_turnToCast[spell->spellID()]->turnToCast() > spell->turnToCast())) {
            _turnToCast[spell->spellID()] = spell;
        }
    }
}

void QWarlock::setSpellPriority(QWarlock *enemy, const QList<QMonster *> &monsters) {
    Q_UNUSED(monsters);
    qDebug() << "QWarlock::setSpellPriority start";
    int _coldproof_in = 999, _fireproof_in = 999;
    int paralysis_left = 3, paralysis_right = 3;
    bool fff_now_left = (_leftGestures.replace(" ", "").right(3).compare("FFF") == 0);
    bool fff_now_right = (_rightGestures.replace(" ", "").right(3).compare("FFF") == 0);
    bool clap_off_lighting_used = (_leftGestures.replace(" ", "").indexOf("WDDC") != -1) || (_rightGestures.replace(" ", "").indexOf("WDDC") != -1);
    qDebug() << "QWarlock::setSpellPriority" << "clap_off_lighting_used" << clap_off_lighting_used;

    enemy->fillTurnToCast();
    this->fillTurnToCast();

    foreach(int sid, ARR_PARALYZES) {
        paralysis_left = qMin(paralysis_left, getTurnToCastBySpellID(sid, 5, WARLOCK_HAND_LEFT));
        paralysis_right = qMin(paralysis_right, getTurnToCastBySpellID(sid, 5, WARLOCK_HAND_RIGHT));
    }
    qDebug() << "QWarlock::setSpellPriority" << "fill _turnToCast";
    _coldproof_in = getTurnToCastBySpellID(SPELL_RESIST_COLD);
    _fireproof_in = getTurnToCastBySpellID(SPELL_RESIST_HEAT);
    qDebug() << "QWarlock::setSpellPriority" << "_coldproof_in" << _coldproof_in << "_fireproof_in" << _fireproof_in;

    int enemy_counter_spell = qMin(enemy->getTurnToCastBySpellID(SPELL_COUNTER_SPELL1, 3), enemy->getTurnToCastBySpellID(SPELL_COUNTER_SPELL2, 3));
    int enemy_magic_mirror  = enemy->getTurnToCastBySpellID(SPELL_MAGIC_MIRROR, 2);

    foreach(QSpell *spell, _possibleSpells) {
        if ((spell->spellType() == SPELL_TYPE_MASSIVE) || (spell->spellType() == SPELL_TYPE_ELEMENTAL)) {
            if (((spell->level() == 0) && (_coldproof == 0) && (_coldproof_in - (spell->spellType() == SPELL_TYPE_ELEMENTAL ? 0 : 1) > spell->turnToCast())) ||
                ((spell->level() == 1) && (_fireproof == 0) && (_fireproof_in - (spell->spellType() == SPELL_TYPE_ELEMENTAL ? 0 : 1) > spell->turnToCast()))) {
                spell->changePriority(-5);
            }
            if (enemy && (((spell->level() == 0) && (enemy->_coldproof > 0)) || ((spell->level() == 1) && (enemy->_fireproof > 0)))) {
                spell->changePriority(-5);
            }
        } else if (spell->spellType() == SPELL_TYPE_SHIELD) {
            if (((_totalEnemyAttack >= 2) && (_totalFriendlyAttack < _totalEnemyHP)) || (_totalEnemyAttack >= qMin(_hp, 3))) {
                spell->changePriority(3);
            }
        }
        if (spell->turnToCast() == 1) {
        if (spell->spellType() == SPELL_TYPE_SUMMON_MONSTER) {
            if (enemy_counter_spell == 1) {
                spell->changePriority(-3);
            }
            if (enemy->getTurnToCastBySpellID(SPELL_CHARM_MONSTER, 4) == 1) {
                spell->changePriority(-3);
            }
        }
        if ((spell->spellType() == SPELL_TYPE_MASSIVE) || (spell->spellType() == SPELL_TYPE_DAMAGE) || (spell->spellType() == SPELL_TYPE_POISON) || (spell->spellType() == SPELL_TYPE_CONFUSION)) {
            if ((spell->turnToCast() == 1) && (qMin(enemy_counter_spell, enemy_magic_mirror) == 1)) {
                spell->changePriority(-5);
            }
        }
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
            if (((spell->hand() == WARLOCK_HAND_LEFT) && fff_now_left) || ((spell->hand() == WARLOCK_HAND_RIGHT) && fff_now_right)) {
                spell->setPriority(-100);
            } else if (((spell->hand() == WARLOCK_HAND_LEFT) && (paralysis_right < spell->turnToCast())) || ((spell->hand() == WARLOCK_HAND_RIGHT) && (paralysis_left < spell->turnToCast()))) {
                spell->setPriority(-100);
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
        case SPELL_STAB:
        case SPELL_SHIELD:
        case SPELL_SURRENDER:
            spell->setPriority(-5);
            break;
        }
    }

    //std::sort(_possibleSpells.begin(), _possibleSpells.end());
    qDebug() << "QWarlock::setSpellPriority" << "before sort";
    /*struct {
        bool operator()(const QSpell *s1, const QSpell *s2) const { return QSpell::sortDesc4(s1, s2); }
    } customOrder2;
    // try sort by priority
    std::sort(_possibleSpells.begin(), _possibleSpells.end(), customOrder2);*/
    QSpell::sort(_possibleSpells);
    qDebug() << "QWarlock::setSpellPriority" << "after sort";
    //logSpellList(_possibleSpells, "QWarlock::setSpellPriority")
}

QSpell *QWarlock::getSpellByFilter(const QList<QSpell *> &sl, int CastFrom, int CastTo, int SpellType, const QList<int> &notID, const QList<int> &byID, int Hand) const {
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
        if ((s->priority() < 0) || !s->active()) {
            continue;
        }
        if ((Hand != -1) && (s->hand() != Hand)) {
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

QSpell *QWarlock::getAntiSpell(const QList<QSpell *> &sl, const QSpell *s, const QWarlock *enemy) const {
    int enemy_counter_spell = qMin(enemy->getTurnToCastBySpellID(SPELL_COUNTER_SPELL1, 3), enemy->getTurnToCastBySpellID(SPELL_COUNTER_SPELL2, 3));
    int min_turn_to_cast = 1;
    if (enemy_counter_spell == 1) {
        min_turn_to_cast = 2;
    }
    QList<int> spell_ids_empty;
    QList<int> spell_ids({SPELL_ANTI_SPELL});
    QSpell *res = getSpellByFilter(sl, min_turn_to_cast, s->turnToCast() - 1, -1, spell_ids_empty, spell_ids);
    if (res) return res;

    res = getSpellByFilter(sl, min_turn_to_cast, s->turnToCast(), SPELL_TYPE_CONFUSION, spell_ids, spell_ids_empty);
    if (res) return res;


    if ((s->spellType() == SPELL_TYPE_CONFUSION) && (s->spellID() != SPELL_ANTI_SPELL)) {
        res = getSpellByFilter(sl, s->turnToCast(), s->turnToCast(), SPELL_TYPE_CONFUSION, spell_ids, spell_ids_empty);
        if (res) return res;
    }
    spell_ids.clear();
    spell_ids.append(ARR_COUNTER_SPELL);
    spell_ids.append(SPELL_CHARM_MONSTER);
    if (s->spellType() == SPELL_TYPE_SUMMON_MONSTER) {
        res = getSpellByFilter(sl, s->turnToCast(), s->turnToCast(), -1, spell_ids_empty, spell_ids);
        if (res) return res;
    }
    spell_ids.clear();
    if ((s->danger() <= 3) || (s->spellType() == SPELL_TYPE_ELEMENTAL) || (s->spellType() == SPELL_TYPE_MASSIVE) || _enemyParalyze) {
        spell_ids.append(SPELL_MAGIC_MIRROR);
    }
    return getSpellByFilter(sl, s->turnToCast(), s->turnToCast(), SPELL_TYPE_MAGIC_SHIELD, spell_ids, spell_ids_empty);
}

void QWarlock::analyzeMonster(QList<QMonster *> &monsters, QWarlock *enemy) {
    qDebug() << "QWarlock::analyzeMonster";
    _elemental = 0;
    _totalFriendlyAttack = 0;
    _totalEnemyAttack = 0;
    _totalEnemyHP = 0;
    foreach(QMonster *m,  monsters) {
        m->setAttackStrength(0);
        if (m->justCreated()) {
            QSpell *summon = getSpellByFilter(m->is_owner(_name) ? _possibleSpells : enemy->_possibleSpells, 1, 1, SPELL_TYPE_SUMMON_MONSTER, QList<int>({}), QList<int>({}), m->hand());
            if (summon) {
                QString moster_name = summon->name().mid(summon->name().indexOf(" ") + 1);
                m->setStrength(QWarlockUtils::getStrengthByMonsterName(moster_name));
                m->setHp(m->getStrength());
                //m->setName(QString("%1H:%2").arg(m->hand() == WARLOCK_HAND_LEFT ? "L" : "R", moster_name));
                qDebug() << "QWarlock::analyzeMonster" << "JUST CREATED" << m->json(_name);
            }
        }
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
    bool summon_left = (_bestSpellL && (_bestSpellL->turnToCast() == 1) && (_bestSpellL->spellType() == SPELL_TYPE_SUMMON_MONSTER));
    bool summon_right = (_bestSpellR && (_bestSpellR->turnToCast() == 1) && (_bestSpellR->spellType() == SPELL_TYPE_SUMMON_MONSTER));
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
            if (((m->hand() == WARLOCK_HAND_LEFT) && !summon_left) ||
                ((m->hand() == WARLOCK_HAND_RIGHT) && !summon_right)) {
                // ignore
            } else {
                tfa += m->getStrength();
            }
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
    qDebug() << "QWarlock::processMonster" << _evilMonster.size() << _friendMonster.size() << (elemental ? elemental->json(_name) : "NO ELEMENTAL");

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
        qDebug() << "QWarlock::processMonster FRIEND" << fm->name() << fm->getStrength() << tfa << all_on_enemy;
        tfa -= fm->getStrength();
        fm->setNewTarget(new_target);
        if (fm->hand() > 0) {
            if (((fm->hand() == WARLOCK_HAND_LEFT) && !summon_left) ||
                ((fm->hand() == WARLOCK_HAND_RIGHT) && !summon_right)) {
                // set def target for just summoned monsters without spell
                continue;
            }
        }
        if (!all_on_enemy) {
            foreach(QMonster *em, _evilMonster) {
              actual_hp = em->getHp() - elemental_attack;
              qDebug() << "QWarlock::processMonster ENEMY" << em->name() << em->getHp() << tfa << elemental_attack << actual_hp << em->attackStrength();
              if ((actual_hp <= 0) || (em->attackStrength() >= actual_hp)) {
                  continue;
              }
              if ((actual_hp < fm->getStrength()) && (tfa >= actual_hp)) {
                  // hope current monster will be targeted with next monster
                  continue;
              } else {
                  fm->setNewTarget(em->name());
                  em->setAttackStrength(em->attackStrength() + fm->getStrength());
                  qDebug() << "QWarlock::processMonster set new target" << em->name();
                  break;
              }
            }
        }
    }
}

void QWarlock::analyzeEnemy(QWarlock *enemy, const QString &paralyzed, const QString &charmed) {
    qDebug() << "QWarlock::analyzeEnemy start";
    enemy->_bestSpellL = nullptr;
    enemy->_bestSpellR = nullptr;

    qreal priority = 0;
    foreach(QSpell *s, enemy->_possibleSpells) {
        if (s->turnToCast() >= s->length()) {
            continue;
        }
        if ((_fireproof > 0) && (ARR_FIRE_SPELLS.indexOf(s->spellID()) != -1)) {
            s->setRealPriority(-100);
            continue;
        }
        if ((_coldproof > 0) && (ARR_ICE_SPELLS.indexOf(s->spellID()) != -1)) {
            s->setRealPriority(-100);
            continue;
        }
        if ((s->spellID() == SPELL_CHARM_MONSTER) && (_totalFriendlyAttack == 0)) {
            s->setRealPriority(-100);
            continue;
        }
        if (s->length() == 1) {
            continue;
        }
        priority = intToReal(s->danger()) * intToReal(s->alreadyCasted())/ intToReal(s->length());
        s->setRealPriority(priority);
        qDebug() << "QWarlock::analyzeEnemy" << priority << s->json();

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

    // process char or paralyze
    bool gesture_changed = false;
    int danger_hand = WARLOCK_HAND_LEFT;
    if (enemy->_bestSpellR && (!enemy->_bestSpellL || (enemy->_bestSpellR->realPriority() > enemy->_bestSpellL->realPriority()))) {
        danger_hand = WARLOCK_HAND_RIGHT;
    }
    if (paralyzed.indexOf(enemy->id()) != -1) {
        enemy->setParalyzedHand(danger_hand, "", _AI);
        gesture_changed = true;
    }
    if (charmed.indexOf(enemy->id()) != -1) {
        enemy->setParalyzedHand(danger_hand, "-", _AI);
        gesture_changed = true;
    }
    if (_AI && gesture_changed) {
        enemy->setPossibleSpells(_SpellChecker->getSpellsList(enemy));
        analyzeEnemy(enemy, "", "");
    }
    qDebug() << "QWarlock::analyzeEnemy finish" << danger_hand << logSpellItem(enemy->_bestSpellL) << logSpellItem(enemy->_bestSpellR);
}

void QWarlock::breakEnemy(QWarlock *enemy) {
    qDebug() << "QWarlock::breakEnemy";
    _bestSpellL = nullptr;
    _bestSpellR = nullptr;
    qreal priority = 0;
    QSpell *as;
    _enemyParalyze = false;

    /*QSpell::setOrderType(2);
    struct {
            bool operator()(const QSpell *s1, const QSpell *s2) const { return QSpell::sortDesc5(s1, s2); }
    } customOrder;
    std::sort(enemy->_possibleSpells.begin(), enemy->_possibleSpells.end(), customOrder);
    QSpell::setOrderType(0);*/
    QSpell::sort(enemy->_possibleSpells, 2);

    QStringList one_turn_gesture_L, one_turn_gesture_R;
    foreach(QSpell *s, enemy->_possibleSpells) {
        //qDebug() << "QWarlock::breakEnemy try to break" << s->json();
        if ((s->alreadyCasted() == 0) || !s->possibleCast() || (ARR_IGNORE_SPELL.indexOf(s->spellID()) != -1) || (s->turnToCast() > 6)) {
            //qDebug() << "QWarlock::breakEnemy skip 1";
            continue;
        }
        if (s->spellType() == SPELL_TYPE_CURE) {
            if ((s->spellID() == SPELL_CURE_HEAVY_WOUNDS) && (enemy->_disease >= s->turnToCast())) {
                // try to break
            } else {
                //qDebug() << "QWarlock::breakEnemy skip 2";
                continue;
            }
        }

        if (s->length() > 1) {
            if (s->turnToCast() == 1) {
                if (s->hand() == WARLOCK_HAND_LEFT) {
                    if (one_turn_gesture_L.indexOf(s->nextGesture()) == -1) {
                        one_turn_gesture_L.append(s->nextGesture());
                    }
                } else {
                    if (one_turn_gesture_R.indexOf(s->nextGesture()) == -1) {
                        one_turn_gesture_R.append(s->nextGesture());
                    }
                }
            } else if (((s->hand() == WARLOCK_HAND_LEFT) && (one_turn_gesture_L.size() > 0) && (one_turn_gesture_L.indexOf(s->nextGesture()) == -1)) ||
                       ((s->hand() == WARLOCK_HAND_RIGHT) && (one_turn_gesture_R.size() > 0) && (one_turn_gesture_R.indexOf(s->nextGesture()) == -1))) {
                //qDebug() << "QWarlock::breakEnemy skip 3" << one_turn_gesture_L << one_turn_gesture_R;
                continue;
            }
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

        as = getAntiSpell(_possibleSpells, s, enemy);
        if (!as) {
            _enemyParalyze = _enemyParalyze || ((s->turnToCast() == 1) && (ARR_PARALYZES.indexOf(s->spellID()) != -1));
            qDebug() << "QWarlock::breakEnemy not found anti spell";
            continue;
        }
        if ((as->spellID() == SPELL_MAGIC_MIRROR) && (_bestSpellL || _bestSpellR)) {
            qDebug() << "QWarlock::breakEnemy MAGIC_MIRROR with not free hands";
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
        if (as->spellID() == SPELL_MAGIC_MIRROR) {
            break;
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
    if ((_bestSpellL && (_bestSpellL->spellID() == SPELL_MAGIC_MIRROR) && !_bestSpellR) ||
        (_bestSpellR && (_bestSpellR->spellID() == SPELL_MAGIC_MIRROR) && !_bestSpellL)) {
        _gestureL = _bestSpellL ? _bestSpellL->nextGesture() : _bestSpellR->nextGesture();
        _gestureR = _gestureL;
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
    Q_UNUSED(enemy);
    //logSpellList(_possibleSpells, "QWarlock::attackEnemy");
    if (_maladroit > 0) {
        processMaladroit();
        return;
    }

    if (_bestSpellR && _bestSpellL) {
        return;
    }

    QSpell *bL = _bestSpellL, *bR = _bestSpellR;
    int pbL = bL ? 3 : 0;// priority bonus left
    int pbR = bR ? 3 : 0;// priority bonus right
    bool erase_one;
    foreach(QSpell *s, _possibleSpells) {
        qDebug() << "QWarlock::attackEnemy process spell" << logSpellItem(s);
        if (!bL && (s->hand() == WARLOCK_HAND_LEFT) && (_gestureL.isEmpty() || (_gestureL.compare(s->nextGesture()) == 0))) {
            bL = s;
        }

        if (!bR && (s->hand() == WARLOCK_HAND_RIGHT) && (_gestureR.isEmpty() || (_gestureR.compare(s->nextGesture()) == 0))) {
            bR = s;
        }

        if (bR && bL) {
            qDebug() << "QWarlock::attackEnemy check erase" << logSpellItem(bL) << logSpellItem(bR);
            erase_one = false;
            if ((bR->spellType() == SPELL_TYPE_CONFUSION) && (bR->spellType() == bL->spellType())) {
                if (bR->turnToCast() == bL->turnToCast()) {
                    qDebug() << "QWarlock::attackEnemy erase 1";
                    erase_one = true;
                }
            } else if ((bR->spellType() == SPELL_TYPE_MAGIC_SHIELD) && (bR->spellType() == bL->spellType()) && (bR->spellID() != SPELL_MAGIC_MIRROR) && (bR->turnToCast() == bL->turnToCast())) {
                qDebug() << "QWarlock::attackEnemy erase 2";
                erase_one = true;
            } else if (!bR->checkValidSequence(*bL)) {
                qDebug() << "QWarlock::attackEnemy erase 3";
                erase_one = true;
            }
            if (erase_one) {
                if (bL->priority() + pbL > bR->priority() + pbR) {
                    pbR = 0;
                    bR = nullptr;
                    _gestureR.clear();
                } else {
                    pbL = 0;
                    bL = nullptr;
                    _gestureL.clear();
                }
                qDebug() << "QWarlock::attackEnemy after erase" << logSpellItem(bL) << logSpellItem(bR);
            }
        }
        if (bR && bL) {
            break;
        }
    }

    if (bL) {
        _bestSpellL = bL;
    }

    if (bR) {
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

int QWarlock::isSummoning() const {
    if (_bestSpellL && (ARR_SUMMON.indexOf(_bestSpellL->spellID()) != -1) && (_bestSpellL->turnToCast() == 1)) {
        return WARLOCK_HAND_LEFT;
    }
    if (_bestSpellR && (ARR_SUMMON.indexOf(_bestSpellR->spellID()) != -1) && (_bestSpellR->turnToCast() == 1)) {
        return WARLOCK_HAND_RIGHT;
    }
    return 0;
}

QString QWarlock::getTargetForSpell(const QSpell *spell, const QWarlock *enemy, const QList<QMonster *> &monsters) {
    if (spell->spellType() == SPELL_TYPE_CURE) {
        return "";// default target
    }
    if ((ARR_COUNTER_SPELL.indexOf(spell->spellID()) != -1) && (enemy->isSummoning() > 0)) {
        return enemy->_name;
    }
    if (spell->spellID() == SPELL_CHARM_MONSTER) {
        int max = 0;
        // summoned in this turn monsters already in "monsters" list
        /*if (enemy->_bestSpellL && (ARR_SUMMON.indexOf(enemy->_bestSpellL->spellID()) != -1)) {
            max = qMax(max, enemy->_bestSpellL->danger());
        }
        if (enemy->_bestSpellR && (ARR_SUMMON.indexOf(enemy->_bestSpellR->spellID()) != -1)) {
            max = qMax(max, enemy->_bestSpellR->danger());
        }*/
        foreach(QMonster *m, monsters) {
            if (!m->is_owner(_name) && (max < m->getStrength()) && (m->attackStrength() < m->getHp())) {
                max = m->getStrength();
            }
        }
        if (max == 0) {
            return _name;
        }/*
        if (enemy->_bestSpellL && (ARR_SUMMON.indexOf(enemy->_bestSpellL->spellID()) != -1) && (max == enemy->_bestSpellL->danger())) {
            return "LH:" + enemy->_name;
        }
        if (enemy->_bestSpellR && (ARR_SUMMON.indexOf(enemy->_bestSpellR->spellID()) != -1) && (max == enemy->_bestSpellR->danger())) {
            return "RH:" + enemy->_name;
        }*/
        foreach(QMonster *m, monsters) {
            if (!m->is_owner(_name) && (max == m->getStrength())) {
                m->setUnderControl(1);
                return m->name();
            }
        }
    }
    if (spell->spellType() == SPELL_TYPE_DAMAGE) {
        int max = 0;
        foreach(QMonster *m, monsters) {
            if (!m->underControl() && (m->attackStrength() + spell->damage() >= m->getHp()) && (max < m->getStrength())) {
                max = m->getStrength();
            }
        }
        if (max > 0) {
            foreach(QMonster *m, monsters) {
                if (!m->underControl() && (m->attackStrength() + spell->damage() >= m->getHp()) && (max == m->getStrength())) {
                    m->setAttackStrength(m->attackStrength() + spell->damage());
                    return m->name();
                }
            }
        }
    }
    if ((spell->spellType() == SPELL_TYPE_CONFUSION) && (spell->spellID() != SPELL_ANTI_SPELL)) {
        bool left_confusion = enemy->_bestSpellL && (enemy->_bestSpellL->spellType() == SPELL_TYPE_CONFUSION) && (enemy->_bestSpellL->turnToCast() == 1) && (enemy->_bestSpellL->spellID() != SPELL_ANTI_SPELL);
        bool left_paralyze = left_confusion && (ARR_PARALYZES.indexOf(enemy->_bestSpellL->spellID()) != -1);
        bool right_confusion = enemy->_bestSpellR && (enemy->_bestSpellR->spellType() == SPELL_TYPE_CONFUSION) && (enemy->_bestSpellR->turnToCast() == 1) && (enemy->_bestSpellR->spellID() != SPELL_ANTI_SPELL);
        bool right_paralyze = right_confusion && (ARR_PARALYZES.indexOf(enemy->_bestSpellR->spellID()) == -1);
        if ((left_confusion || right_confusion) && !left_paralyze && !right_paralyze) {
            return _name;
        }
    }

    return ""; // default target
}

QString QWarlock::printOrders(QMap<QString, QString> &name_to_id) const {
    QString res = QString("LH$%1#").arg(_gestureL);
    res.append(QString("RH$%1#").arg(_gestureR));
    res.append(QString("LHS$%1#").arg(_spellL));
    res.append(QString("RHS$%1#").arg(_spellR));
    res.append(QString("LHT$%1#").arg(name_to_id[_targetL]));//to id
    res.append(QString("RHT$%1#").arg(name_to_id[_targetR]));//to id

    return res;
}

/*
    post_request += "LH$"+actions.L.g+"#";
    post_request += "RH$"+actions.R.g+"#";
    console.log("prepareOrder", "point2", post_request);
    post_request += "LHS$"+getSpellNameForOrder(actions.L)+"#";
    post_request += "RHS$"+getSpellNameForOrder(actions.R)+"#";
    console.log("prepareOrder", "point3", post_request);
    post_request += "LHT$"+getSpellTargetForOrder(actions.L, battle.targetsMap)+"#";
    post_request += "RHT$"+getSpellTargetForOrder(actions.R, battle.targetsMap)+"#";
    console.log("prepareOrder", "point4", post_request);

*/

void QWarlock::targetSpell(const QWarlock *enemy, const QList<QMonster *> &monsters) {
    _targetL.clear();
    _targetR.clear();
    _spellL.clear();
    _spellR.clear();
    if (!_AI) {
        _gestureL.clear();
        _gestureR.clear();
        return;
    }
    QString tgl = _leftGestures.replace(" ", " "), tgr = _rightGestures.replace(" ", " ");
    tgl.append(_gestureL);
    tgr.append(_gestureR);
    QList<QSpell *> sl = _SpellChecker->getStriktSpellsList(tgl, tgr, false);
    /*QSpell::setOrderType(0);
    struct {
            bool operator()(const QSpell *s1, const QSpell *s2) const { return QSpell::sortDesc6(s1, s2); }
    } customOrder;
    std::sort(sl.begin(), sl.end(), customOrder);*/
    QSpell::sort(sl);

    if (sl.size() > 0) {
        foreach(QSpell *s, sl) {
            if (_spellL.isEmpty() && (s->hand() == WARLOCK_HAND_LEFT)) {
                _spellL = s->name();
                _targetL = getTargetForSpell(s, enemy, monsters);
            }
            if (_spellR.isEmpty() && (s->hand() == WARLOCK_HAND_RIGHT)) {
                _spellR = s->name();
                _targetR = getTargetForSpell(s, enemy, monsters);
            }
            delete s;
        }
        sl.clear();
    }
    if (_spellL.isEmpty()) {
        _spellL = "Default";
    }
    if (_spellR.isEmpty()) {
        _spellR = "Default";
    }
    if (_targetL.isEmpty()) {
        _targetL = "Default";
    }
    if (_targetR.isEmpty()) {
        _targetR = "Default";
    }
    qDebug() << "QWarlock::targetSpell" << _spellL << _targetL << _spellR << _targetR;
}

void QWarlock::processDecision(QWarlock *enemy, QList<QMonster *> &monsters, const QString &paralyzed, const QString &charmed) {
    qDebug() << "QWarlock::processDecision" << enemy << monsters.size() << paralyzed << charmed;
    //
    analyzeEnemy(enemy, paralyzed, charmed);
    //
    analyzeMonster(monsters, enemy);
    //
    //enemy->setSpellPriority(this, monsters);
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
    if (_AI) {
        processMonster(monsters, enemy);
    }
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
