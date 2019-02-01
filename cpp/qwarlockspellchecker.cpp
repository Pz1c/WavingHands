#include "qwarlockspellchecker.h"

QWarlockSpellChecker::QWarlockSpellChecker(QObject *parent) :
    QObject(parent)
{
    Spells.append(new QSpell(0,"cDPW","Dispel Magic", SPELL_TYPE_REMOVE_ENCHANTMENT,10,1,10));
    Spells.append(new QSpell(1,"cSWWS","Summon Ice Elemental", SPELL_TYPE_ELEMENTAL,14,0,15));
    Spells.append(new QSpell(2,"cWSSW","Summon Fire Elemental", SPELL_TYPE_ELEMENTAL,15,1,15));
    Spells.append(new QSpell(3,"cw","Magic Mirror", SPELL_TYPE_MAGIC_SHIELD,10,0,10));
    Spells.append(new QSpell(4,"DFFDD","Lightning Bolt", SPELL_TYPE_DAMAGE,11,1,16));
    Spells.append(new QSpell(5,"DFPW","Cure Heavy Wounds", SPELL_TYPE_CURE,10,1,10));
    Spells.append(new QSpell(6,"DFW","Cure Light Wounds", SPELL_TYPE_CURE,10,0,10));
    Spells.append(new QSpell(7,"DFWFd","Blindness", SPELL_TYPE_CONFUSION,13,0,15));
    Spells.append(new QSpell(8,"DPP","Amnesia", SPELL_TYPE_CONFUSION,16,1,12));
    Spells.append(new QSpell(9,"DSF","Confusion/Maladroitness", SPELL_TYPE_CONFUSION,16,5,12));
    Spells.append(new QSpell(SPELL_DISEASE,"DSFFFc","Disease", SPELL_TYPE_POISON,14,1,17));
    Spells.append(new QSpell(11,"DWFFd","Blindness", SPELL_TYPE_CONFUSION,13,0,15));
    Spells.append(new QSpell(12,"DWSSSP","Delay Effect", SPELL_TYPE_SPEC,10,0,10));
    Spells.append(new QSpell(13,"DWWFWD","Poison", SPELL_TYPE_POISON,13,1,18));
    Spells.append(new QSpell(SPELL_PARALYSIS,"FFF","Paralysis", SPELL_TYPE_CONFUSION,15,2,12));
    Spells.append(new QSpell(15,"WFPSFW","Summon Giant", SPELL_TYPE_SUMMON_MONSTER,11,4,14));
    Spells.append(new QSpell(16,"FPSFW","Summon Troll", SPELL_TYPE_SUMMON_MONSTER,12,3,13));
    Spells.append(new QSpell(17,"PSFW","Summon Ogre", SPELL_TYPE_SUMMON_MONSTER,13,2,12));
    Spells.append(new QSpell(18,"SFW","Summon Goblin", SPELL_TYPE_SUMMON_MONSTER,14,1,11));
    Spells.append(new QSpell(19,"FSSDD","Fireball", SPELL_TYPE_DAMAGE,13,1,15));
    Spells.append(new QSpell(20,"P","Shield", SPELL_TYPE_SHIELD,10,0,10));
    Spells.append(new QSpell(21,"PDWP","Remove Enchantment", SPELL_TYPE_REMOVE_ENCHANTMENT,10,0,13));
    Spells.append(new QSpell(22,"PPws","Invisibility", SPELL_TYPE_CONFUSION,13,0,13));
    Spells.append(new QSpell(23,"PSDD","Charm Monster", SPELL_TYPE_CHARM_MONSTER,10,0,12));
    Spells.append(new QSpell(24,"PSDF","Charm Person", SPELL_TYPE_CONFUSION,15,4,13));
    Spells.append(new QSpell(25,"PWPFSSSD","Finger of Death", SPELL_TYPE_DAMAGE,11,0,20));
    Spells.append(new QSpell(26,"PWPWWc","Haste", SPELL_TYPE_HASTLE,12,0,10));
    Spells.append(new QSpell(27,"SD","Magic Missile", SPELL_TYPE_DAMAGE,10,0,10));
    Spells.append(new QSpell(28,"SPFP","Anti-spell", SPELL_TYPE_CONFUSION,13,0,14));
    Spells.append(new QSpell(29,"SPFPSDW","Permanency", SPELL_TYPE_SPEC,13,0,16));
    Spells.append(new QSpell(30,"SPPc","Time Stop", SPELL_TYPE_HASTLE,13,0,10));
    Spells.append(new QSpell(31,"SPPFD","Time Stop", SPELL_TYPE_HASTLE,13,0,10));
    Spells.append(new QSpell(32,"SSFP","Resist Cold", SPELL_TYPE_RESIST,14,1,10));
    Spells.append(new QSpell(33,"SWD","Fear (No CFDS)", SPELL_TYPE_CONFUSION,15,0,12));
    Spells.append(new QSpell(34,"SWWc","Fire Storm", SPELL_TYPE_MASSIVE,13,1,14));
    Spells.append(new QSpell(35,"WDDc","Clap of Lightning", SPELL_TYPE_DAMAGE,14,0,14));
    Spells.append(new QSpell(36,"WFP","Cause Light Wounds", SPELL_TYPE_DAMAGE,14,0,12));
    Spells.append(new QSpell(37,"WPFD","Cause Heavy Wounds", SPELL_TYPE_DAMAGE,13,0,13));
    Spells.append(new QSpell(38,"WPP","Counter Spell", SPELL_TYPE_MAGIC_SHIELD,10,0,10));
    Spells.append(new QSpell(39,"WSSc","Ice Storm", SPELL_TYPE_MASSIVE,10,0,14));
    Spells.append(new QSpell(40,"WWFP","Resist Heat", SPELL_TYPE_RESIST,10,4,10));
    Spells.append(new QSpell(41,"WWP","Protection", SPELL_TYPE_SHIELD,10,1,10));
    Spells.append(new QSpell(42,"WWS","Counter Spell", SPELL_TYPE_MAGIC_SHIELD,10,1,10));
    Spells.append(new QSpell(43,"p","Surrender", SPELL_TYPE_SPEC,-10,0,0));
    Spells.append(new QSpell(44,">","Stab", SPELL_TYPE_STAB,-1,0,0));
    Spells.append(new QSpell(SPELL_DISEASE_FDF,"DSFDFc","Disease", SPELL_TYPE_POISON,14,1,17));
    Spells.append(new QSpell(SPELL_PARALYSIS_FDF,"FDF","Paralysis", SPELL_TYPE_CONFUSION,15,2,12));
    Spells.append(new QSpell(SPELL_PARALYSIS_FDFD,"FDFD","Paralysis", SPELL_TYPE_CONFUSION,15,2,12));
}

bool QWarlockSpellChecker::checkSpellChar(QChar left, QChar right, QChar spell) {
    if (left != spell.toUpper()) {
        return false;
    }

    if (spell.isLower() && left != right) {
        return false;
    }

    return true;
}

bool QWarlockSpellChecker::checkStriktSpell(QString left, QString right, QString spell) {
    int Ln = spell.length();
    if (left.length() < Ln) {
        return false;
    }

    for (int i = 0; i < Ln; ++i) {
        if (!checkSpellChar(left.at(i), right.at(i), spell.at(i))) {
            return false;
        }
    }

    return true;
}

int QWarlockSpellChecker::checkSpellPosible(QString left, QString right, QString spell, QString possible_left, QString possible_right) {
    int Ln = spell.length() - 1, GLn = left.length() < right.length() ? left.length() : right.length();
    if (Ln == 0) {
        return 0;
    }
    QString work_spell = spell.left(Ln);
    qDebug() << "checkSpellPosible " << spell << " work spell " << work_spell << " left " << left << " right" << right << "possible_left" << possible_left << "possible_right" << possible_right;
    bool perhapse;
    int spell_idx;
    QChar spell_char;
    int res = 0;
    for (int i = 0; i < Ln; ++i) {
        perhapse = true;
        spell_idx = -1;
        for (int j = Ln - i; j > 0; --j) {
            spell_char = work_spell.at(++spell_idx);
            if ((GLn - j < 0) || !checkSpellChar(left.at(GLn - j), right.at(GLn - j), spell_char)) {
                perhapse = false;
                break;
            }
        }
        if (perhapse) {
            res = i + 1;
            break;
        }
    }
    if (res > 0) {
        QString g = spell.mid(spell.length() - res, 1);
        if (possible_left.indexOf(g.toUpper()) == -1) {
            res = spell.length() + 1;
        } else if ((g.toUpper().compare(g) != 0) && (possible_right.indexOf(g.toUpper()) == -1)) {
            res = spell.length() + 1;
        }
    }
    return res;
}

void QWarlockSpellChecker::checkHandOnSpell(QList<QSpell *> &Result, QSpell *Spell, QString left, QString right, int Hand, bool Enemy, QString possible_left, QString possible_right) {
    QString gestures = Spell->gesture(), w_left, w_right;
    int Ln = gestures.length();
    w_left = left.right(Ln);
    w_right = right.right(Ln);
    int l_turn_to_spell = checkSpellPosible(w_left, w_right, gestures, possible_left, possible_right);
    if (l_turn_to_spell > 0) {
        qDebug() << "left checkSpellPosible (" << w_left << ", " << w_right << ") " << gestures << l_turn_to_spell;
    } else {
        l_turn_to_spell = Spell->gesture().length();
    }
   // QSpell(QSpell *Spell, int Hand, int TurnToCast, bool Enemy = false);
    bool add = true;
    QSpell *hand_spell = new QSpell(Spell, Hand, l_turn_to_spell, Enemy);
    if (Result.count() > 0) {
        foreach(QSpell *spell, Result) {
            bool same_hand = spell->hand() == hand_spell->hand();
            bool same_spell = spell->spellID() == hand_spell->spellID();
            bool summon_moster_spell = (spell->spellType() == SPELL_TYPE_SUMMON_MONSTER) && (hand_spell->spellType() == SPELL_TYPE_SUMMON_MONSTER);
            bool priority_higer = spell->priority() < hand_spell->priority();
            if (same_hand && (same_spell || (summon_moster_spell && priority_higer && (spell->turnToCast() == hand_spell->turnToCast())))) {
                add = false;
                if (priority_higer) {
                    spell->setPriority(hand_spell->priority());
                }
                break;
            }
        }
    }
    if (add) {
        Result.append(hand_spell);
    } else {
        delete hand_spell;
    }
}

QList<QSpell *> QWarlockSpellChecker::getPosibleSpellsList(QString left, QString right, bool Enemy, QString possible_left, QString possible_right, bool IsFDF) {
    qDebug() << "QWarlockSpellChecker::getPosibleSpellsList" << left << right << Enemy;
    QList<QSpell *> res;

    Spells.at(SPELL_DISEASE)->setActive(!IsFDF);

    Spells.at(SPELL_PARALYSIS)->setActive(!IsFDF);
    Spells.at(SPELL_DISEASE_FDF)->setActive(IsFDF);
    Spells.at(SPELL_PARALYSIS_FDF)->setActive(IsFDF);
    Spells.at(SPELL_PARALYSIS_FDFD)->setActive(IsFDF);

    foreach(QSpell *vn, Spells) {
        if (!vn->active()) {
            continue;
        }
        checkHandOnSpell(res, vn, left, right, WARLOCK_HAND_LEFT, Enemy, possible_left, possible_right);
        checkHandOnSpell(res, vn, right, left, WARLOCK_HAND_RIGHT, Enemy, possible_right, possible_left);
    }

    return res;
}

QList<QSpell *> QWarlockSpellChecker::getStriktSpellsList(QString left, QString right, bool Enemy) {
    qDebug() << "QWarlockSpellChecker::getStriktSpellsList" << left << right << Enemy;
    QString gestures, w_left, w_right;
    QList<QSpell *> res;

    foreach(QSpell *vn, Spells) {
        gestures = vn->gesture();
        int Ln = gestures.length();
        w_left = left.right(Ln);
        w_right = right.right(Ln);

        if (checkStriktSpell(w_left, w_right, gestures)) {
            res.append(new QSpell(vn, WARLOCK_HAND_LEFT, 0, Enemy));
        }

        if (checkStriktSpell(w_right, w_left, gestures)) {
            res.append(new QSpell(vn, WARLOCK_HAND_RIGHT, 0, Enemy));
        }
    }

    return res;
}

QList<QSpell *> QWarlockSpellChecker::getSpellsList(QWarlock *warlock) {
    // QString Left, QString Right, bool strikt, bool Enemy
    qDebug() << "QWarlockSpellChecker::getSpellsList" << warlock->separatedString();
    QString left = warlock->leftGestures().replace(" ", "");
    QString right = warlock->rightGestures().replace(" ", "");
    QString possible_left = warlock->possibleLeftGestures();
    QString possible_right = warlock->possibleRightGestures();
    QList<QSpell *> sl = getPosibleSpellsList(left, right, !warlock->player(), possible_left, possible_right, warlock->isParaFDF());
    qDebug() << "before sort" << sl;
    qSort(sl.begin(), sl.end(), QSpell::sortDesc);
    qDebug() << "after sort" << sl;
    return sl;
}

QString QWarlockSpellChecker::checkSpells(QString Left, QString Right, bool Enemy) {
    qDebug() << "QWarlockSpellChecker::checkSpells" << Left << Right;
    QList<QSpell *> sl = getStriktSpellsList(Left.replace(" ", ""), Right.replace(" ", ""), Enemy);
    qSort(sl.begin(), sl.end(), QSpell::sortDesc);

    QString res;
    foreach(QSpell *s, sl) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        res.append(s->json());
        delete s;
    }
    res.prepend("[").append("]");

    //qDebug() << "spellCheckFinish result " << res;
    return res;
}

QString QWarlockSpellChecker::getSpellBook(bool IsFDF) {
    qDebug() << "QWarlockSpellChecker::getSpellBook" << IsFDF;
    QString res;

    Spells.at(SPELL_DISEASE)->setActive(!IsFDF);
    Spells.at(SPELL_PARALYSIS)->setActive(!IsFDF);
    Spells.at(SPELL_DISEASE_FDF)->setActive(IsFDF);
    Spells.at(SPELL_PARALYSIS_FDF)->setActive(IsFDF);
    Spells.at(SPELL_PARALYSIS_FDFD)->setActive(IsFDF);

    foreach(QSpell *vn, Spells) {

        if (!res.isEmpty()) {
            res.append(",");
        }
        res.append(vn->json());
    }
    res.append("]").prepend("[");

    return res;
}
