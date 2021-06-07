#include "qwarlock.h"

QWarlock::QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player, bool isAI) :
    _scared(0), _confused(0), _charmed(0), _paralized(0), _shield(0), _coldproof(0), _fireproof(0), _hp(0), _poison(0), _desease(0), _amnesia(0),
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
    _desease = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Desease", "(", ")", pos, true));
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

    return QString("{\"name\":\"%1\",\"status\":\"%2\",\"L\":\"%3\",\"R\":\"%4\",\"spells\":%5,\"player\":%6,\"bsL\":%7,\"bsR\":%8,\"hp\":%9,\"scared\":%10,"
                   "\"confused\":%11,\"charmed\":%12,\"paralized\":%13,\"shield\":%14,\"coldproof\":%15,\"fireproof\":%16,\"poison\":%17,\"desease\":%18,"
                   "\"amnesia\":%19,\"maladroit\":%20,\"summon_left\":%21,\"summon_right\":%22,\"active\":%23,\"mshield\":%24,\"delay\":%25,\"time_stop\":%26,"
                   "\"haste\":%27,\"permanency\":%28,\"blindness\":%29,\"invisibility\":%30,\"plg\":\"%31\",\"prg\":\"%32\",\"charm_left\":%33,\"charm_right\":%34,"
                   "\"lgL\":\"%35\",\"lgR\":\"%36\",\"smcL\":%37,\"smcR\":%38}").
            arg(_name, _status, _leftGestures, _rightGestures, res, boolToStr(_player), sbsL, sbsR, intToStr(_hp)).
            arg(intToStr(_scared), intToStr(_confused), intToStr(_charmed), intToStr(_paralized), intToStr(_shield),
                intToStr(_coldproof), intToStr(_fireproof), intToStr(_poison), intToStr(_desease)).
            arg(intToStr(_amnesia), intToStr(_maladroit), intToStr(summon_left), intToStr(summon_right), _active ? "1" : "0").
            arg(intToStr(_mshield), intToStr(_delay), intToStr(_time_stop), intToStr(_haste), intToStr(_permanency), intToStr(_blindness), intToStr(_invisibility),
                _possibleLeftGestures, _possibleRightGestures, boolToStr(charmL), boolToStr(charmR)).
            arg(_leftGestures.trimmed().right(1), _rightGestures.trimmed().right(1), intToStr(spell_max_pass_left), intToStr(spell_max_pass_right));
}

bool QWarlock::player() const
{
    return _player;
}

void QWarlock::breakEnemySpell(QSpell *spell) {
    qDebug() << spell->json();
}

void QWarlock::setAntispell(QWarlock *enemy) {
    QSpell *esL = enemy->_bestSpellL;
    QSpell *esR = enemy->_bestSpellR;
    bool left_first = esL && (!esR || (esL->priority() > esR->priority()));
    if (left_first && esL) {
        breakEnemySpell(esL);
    }
    if (esR) {
        breakEnemySpell(esR);
    }
    if (!left_first && esL) {
        breakEnemySpell(esL);
    }
}

void QWarlock::setSpellPriority(const QWarlock *enemy, const QList<QMonster *> &monsters) {
    int _coldproof_in = 999, _fireproof_in = 999;
    int under_attack = 0, paralysis_left = 3, paralysis_right = 3;
    foreach(QMonster *m,  monsters) {
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
            if (_desease >= spell->turnToCast()) {
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
            if ((_desease > 0) && (_desease >= spell->turnToCast())) {
                spell->changePriority(5);
            }
            break;
        case SPELL_REMOVE_ENCHANTMENT:
        case SPELL_DISPEL_MAGIC:
            if ((qMin(_desease, _poison) > 0) && (qMin(_desease, _poison) >= spell->turnToCast())) {
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
}

void QWarlock::setPossibleSpells(const QList<QSpell *> &possibleSpells, const QWarlock *enemy, const QList<QMonster *> &monsters)
{
    qDebug() << "QWarlock::setPossibleSpells";
    _possibleSpells = possibleSpells;
    _bestSpellL = nullptr;
    _bestSpellR = nullptr;
    //logSpellList(possibleSpells, "QWarlock::setPossibleSpells before");
    setSpellPriority(enemy, monsters);
    //logSpellList(possibleSpells, "QWarlock::setPossibleSpells after");
    /*if (enemy) {
        setAntispell(enemy);
    }*/
    checkSpells();
}

void QWarlock::checkSpells() {
    qDebug() << "QWarlock::checkSpells" << _name;
    logSpellList(_possibleSpells, "QWarlock::checkSpells");

    if (_maladroit > 0) {
        QStringList pg;
        pg << "W" << "S" << "D" << "C" << "F";
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
