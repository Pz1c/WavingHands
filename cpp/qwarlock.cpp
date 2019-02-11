#include "qwarlock.h"

QWarlock::QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player) :
    _scared(0), _confused(0), _charmed(0), _paralized(0), _shield(0), _coldproof(0), _fireproof(0), _hp(0), _poison(0), _desease(0), _amnesia(0),
    _maladroit(0), _bestSpellL(nullptr), _bestSpellR(nullptr)
{
    _name = Name;
    _status = Status;
    _leftGestures = LeftGestures;
    _rightGestures = RightGestures;
    _player = Player;
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
    _possibleLeftGestures = left;
    _possibleRightGestures = right;
    if (_possibleLeftGestures.indexOf("As Right") != -1) {
        _possibleLeftGestures = _possibleRightGestures;
    }
    qDebug() << "QWarlock::setPossibleGestures" << left << right << _possibleLeftGestures << _possibleRightGestures;
    //*/
}

void QWarlock::parseStatus() {
    _hp = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Health", ":", " #;#)"));
    _scared = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Fear", "(", ")"));
    _confused = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Confused", "(", ")"));
    _charmed = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Charmed", "(", ")"));
    _paralized = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Paralize", "(", ")"));
    _shield = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Shield", "(", ")"));
    _coldproof = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Coldproof", "(", ")"));
    _fireproof = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Fireproof", "(", ")"));
    _poison = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Poison", "(", ")"));
    _desease = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Desease", "(", ")"));
    _amnesia = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Forgetful", "(", ")"));
    _maladroit = QWarlockUtils::strValueToInt(QWarlockUtils::getStringFromData(_status, "Maladroit", "(", ")"));
}

void QWarlock::checkPossibleGesture() {
    if (_amnesia > 0) {
        _possibleLeftGestures = _leftGestures.right(1);
        _possibleRightGestures = _rightGestures.right(1);
    } else if (_scared > 0) {
        _possibleLeftGestures = "W,S";
        _possibleRightGestures = "W,S";
    } else if (_maladroit > 0) {
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
    QString l = _leftGestures.right(WARLOCK_GESTURE_LENGTH);
    QString r = _rightGestures.right(WARLOCK_GESTURE_LENGTH);
    int summon_left = SPELL_ID_MAX, summon_right = SPELL_ID_MAX;
    QString res;
    foreach(QSpell *s, _possibleSpells) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        if ((s->spellType() == SPELL_TYPE_SUMMON_MONSTER) && (s->turnToCast() == 1)) {
            if  (s->hand() == WARLOCK_HAND_LEFT) {
                summon_left = qMin(summon_left, s->spellID());
            } else {
                summon_right = qMin(summon_right, s->spellID());
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
                   "\"amnesia\":%19,\"maladroit\":%20,\"summon_left\":%21,\"summon_right\":%22}").
            arg(_name, _status, l, r, res, _player ? "true" : "false", sbsL, sbsR, intToStr(_hp)).
            arg(intToStr(_scared), intToStr(_confused), intToStr(_charmed), intToStr(_paralized), intToStr(_shield), intToStr(_coldproof), intToStr(_fireproof), intToStr(_poison), intToStr(_desease)).
            arg(intToStr(_amnesia), intToStr(_maladroit), intToStr(summon_left), intToStr(summon_right));
}

bool QWarlock::player() const
{
    return _player;
}

void QWarlock::breakEnemySpell(QSpell *spell) {

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
    foreach(QSpell *spell, _possibleSpells) {
        switch(spell->spellID()) {
            case SPELL_RESIST_COLD:
                _coldproof_in = spell->turnToCast();
                break;
            case SPELL_RESIST_HEAT:
                _fireproof_in = spell->turnToCast();
                break;

        }
    }

    foreach(QMonster *m,  monsters) {
        // TODO add monster processing
    }

    foreach(QSpell *spell, _possibleSpells) {
        switch(spell->spellType()) {
        case SPELL_TYPE_MASSIVE:
        case SPELL_TYPE_ELEMENTAL:
            if (((spell->level() == 0) && (_coldproof == 0) && (_coldproof_in - 1 > spell->turnToCast())) ||
                ((spell->level() == 1) && (_fireproof == 0) && (_fireproof_in - 1 > spell->turnToCast()))) {
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
        case SPELL_REMOVE_ENCHANTMENT:
        case SPELL_DISPEL_MAGIC:
            if ((_desease > _poison ? _desease : _poison) >= spell->turnToCast()) {
                spell->changePriority(5);
            }
            break;
        }
    }

    qSort(_possibleSpells.begin(), _possibleSpells.end(), QSpell::sortDesc);
}

void QWarlock::setPossibleSpells(const QList<QSpell *> &possibleSpells, const QWarlock *enemy, const QList<QMonster *> &monsters)
{
    qDebug() << "QWarlock::setPossibleSpells" << possibleSpells;
    _possibleSpells = possibleSpells;
    _bestSpellL = nullptr;
    _bestSpellR = nullptr;
    setSpellPriority(enemy, monsters);
    /*if (enemy) {
        setAntispell(enemy);
    }*/
    checkSpells();
}

void QWarlock::checkSpells() {
    qDebug() << "QWarlock::checkSpells" << _name << _possibleSpells;
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
