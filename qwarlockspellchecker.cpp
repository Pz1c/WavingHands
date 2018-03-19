#include "qwarlockspellchecker.h"

QWarlockSpellChecker::QWarlockSpellChecker(QObject *parent) :
    QObject(parent)
{
    Spells.append(QPair<QString, QString>("cDPW","Dispel Magic"));
    Spells.append(QPair<QString, QString>("cSWWS","Summon Ice Elemental"));
    Spells.append(QPair<QString, QString>("cWSSW","Summon Fire Elemental"));
    Spells.append(QPair<QString, QString>("cw","Magic Mirror"));
    Spells.append(QPair<QString, QString>("DFFDD","Lightning Bolt"));
    Spells.append(QPair<QString, QString>("DFPW","Cure Heavy Wounds"));
    Spells.append(QPair<QString, QString>("DFW","Cure Light Wounds"));
    Spells.append(QPair<QString, QString>("DFWFd","Blindness"));
    Spells.append(QPair<QString, QString>("DPP","Amnesia"));
    Spells.append(QPair<QString, QString>("DSF","Confusion/Maladroitness"));
    Spells.append(QPair<QString, QString>("DSFFFc","Disease"));
    Spells.append(QPair<QString, QString>("DWFFd","Blindness"));
    Spells.append(QPair<QString, QString>("DWSSSP","Delay Effect"));
    Spells.append(QPair<QString, QString>("DWWFWD","Poison"));
    Spells.append(QPair<QString, QString>("FFF","Paralysis"));
    Spells.append(QPair<QString, QString>("WFPSFW","Summon Giant"));
    Spells.append(QPair<QString, QString>("FPSFW","Summon Troll"));
    Spells.append(QPair<QString, QString>("PSFW","Summon Ogre"));
    Spells.append(QPair<QString, QString>("SFW","Summon Goblin"));
    Spells.append(QPair<QString, QString>("FSSDD","Fireball"));
    Spells.append(QPair<QString, QString>("P","Shield"));
    Spells.append(QPair<QString, QString>("PDWP","Remove Enchantment"));
    Spells.append(QPair<QString, QString>("PPws","Invisibility"));
    Spells.append(QPair<QString, QString>("PSDD","Charm Monster"));
    Spells.append(QPair<QString, QString>("PSDF","Charm Person"));
    Spells.append(QPair<QString, QString>("PWPFSSSD","Finger of Death"));
    Spells.append(QPair<QString, QString>("PWPWWc","Haste"));
    Spells.append(QPair<QString, QString>("SD","Magic Missile"));
    Spells.append(QPair<QString, QString>("SPFP","Anti-spell"));
    Spells.append(QPair<QString, QString>("SPFPSDW","Permanency"));
    Spells.append(QPair<QString, QString>("SPPc","Time Stop"));
    Spells.append(QPair<QString, QString>("SPPFD","Time Stop"));
    Spells.append(QPair<QString, QString>("SSFP","Resist Cold"));
    Spells.append(QPair<QString, QString>("SWD","Fear (No CFDS)"));
    Spells.append(QPair<QString, QString>("SWWc","Fire Storm"));
    Spells.append(QPair<QString, QString>("WDDc","Clap of Lightning"));
    Spells.append(QPair<QString, QString>("WFP","Cause Light Wounds"));
    Spells.append(QPair<QString, QString>("WPFD","Cause Heavy Wounds"));
    Spells.append(QPair<QString, QString>("WPP","Counter Spell"));
    Spells.append(QPair<QString, QString>("WSSc","Ice Storm"));
    Spells.append(QPair<QString, QString>("WWFP","Resist Heat"));
    Spells.append(QPair<QString, QString>("WWP","Protection"));
    Spells.append(QPair<QString, QString>("WWS","Counter Spell"));
    Spells.append(QPair<QString, QString>("p","Surrender"));
    Spells.append(QPair<QString, QString>(">","Stab"));
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

int QWarlockSpellChecker::checkSpellPosible(QString left, QString right, QString spell) {
    int Ln = spell.length() - 1, GLn = left.length() < right.length() ? left.length() : right.length();
    if (Ln == 0) {
        return 0;
    }
    QString work_spell = spell.left(Ln);
    //qDebug() << "checkSpellPosible " << spell << " work spell " << work_spell << " left " << left << " right" << right;
    bool perhapse;
    int spell_idx;
    QChar spell_char;
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
            return i + 1;
        }
    }
    return 0;
}

QStringList QWarlockSpellChecker::getPosibleSpellsList(QString left, QString right) {
    QString gestures, spell_name, w_left, w_right;
    QStringList res_left;
    QStringList res_right;
    QString spell_template = "%1;%2;%3;%4;";
    //bool only_best = false;

    foreach(QValueName vn, Spells) {
        gestures = vn.first;
        spell_name = vn.second;
        int Ln = gestures.length();
        if (left.length() > Ln) {
            w_left = left.mid(left.length() - Ln, Ln);
            w_right = right.mid(right.length() - Ln, Ln);
        } else {
            w_left = left;
            w_right = right;
        }

        int l_turn_to_spell = checkSpellPosible(w_left, w_right, gestures);
        if (l_turn_to_spell > 0) {
            qDebug() << "left checkSpellPosible (" << w_left << ", " << w_right << ") " << gestures << l_turn_to_spell;
        }

        int r_turn_to_spell = checkSpellPosible(w_right, w_left, gestures);
        if (r_turn_to_spell > 0) {
            qDebug() << "right checkSpellPosible (" << w_right << ", " << w_left << ") " << gestures << r_turn_to_spell;
        }

        //if ((l_turn_to_spell > 0) && (!only_best || (r_turn_to_spell == 0) || (l_turn_to_spell < r_turn_to_spell))) {
        if (l_turn_to_spell > 0) {
            if (res_left.count() == 0) {
                res_left.append(QString(spell_template).arg("L", spell_name, QString::number(l_turn_to_spell), gestures));
            } else {
                bool check_summon = gestures.length() >= 3 && gestures.mid(gestures.length() - 3, 3).compare("SFW") == 0;
                bool add = true;
                foreach(QString s, res_left) {
                    if (s.compare(spell_name) == 0) {
                        add = false;
                        break;
                    }
                    if (check_summon && s.indexOf("Summon") != -1 && s.indexOf("Ice") == -1 && s.indexOf("Fire") == -1 && s.at(s.length() - 1) == '1') {
                        add = false;
                        break;
                    }
                }
                if (add) {
                    res_left.append(QString(spell_template).arg("L", spell_name, QString::number(l_turn_to_spell), gestures));
                }
            }
        }

        if ((r_turn_to_spell > 0) && (!only_best || (l_turn_to_spell == 0) || (r_turn_to_spell <= l_turn_to_spell))) {
            if (res_right.count() == 0) {
              res_right.append(QString(spell_template).arg("R", spell_name, QString::number(r_turn_to_spell), gestures));
            } else {
                bool add = true;
                bool check_summon = gestures.length() >= 3 && gestures.mid(gestures.length() - 3, 3).compare("SFW") == 0;
                foreach(QString s, res_right) {
                    if (s.compare(spell_name) == 0) {
                        add = false;
                        break;
                    }
                    if (check_summon && s.indexOf("Summon") != -1 && s.indexOf("Ice") == -1 && s.indexOf("Fire") == -1 && s.at(s.length() - 1) == '1') {
                        add = false;
                        break;
                    }
                }
                if (add) {
                    res_right.append(QString(spell_template).arg("R", spell_name, QString::number(r_turn_to_spell), gestures));
                }
            }
        }
    }
    //qDebug() << "spellCheckFinish prepare result";
    QStringList res;
    if (res_left.count() > 0) {
        foreach(QString s, res_left) {
            res.append(s);
        }
    }
    if (res_right.count() > 0) {
        foreach(QString s, res_right) {
            res.append(s);
        }
    }
    //qDebug() << "spellCheckFinish result " << res;
    return res;
}

QStringList QWarlockSpellChecker::getStriktSpellsList(QString left, QString right) {
    QString gestures, spell_name, w_left, w_right;
    QStringList res_left;
    QStringList res_right;

    foreach(QValueName vn, Spells) {
        gestures = vn.first;
        spell_name = vn.second;
        int Ln = gestures.length();
        if (left.length() > Ln) {
            w_left = left.mid(left.length() - Ln, Ln);
            w_right = right.mid(right.length() - Ln, Ln);
        } else {
            w_left = left;
            w_right = right;
        }

        if (checkStriktSpell(w_left, w_right, gestures) && (res_left.indexOf(spell_name) == -1)) {
            res_left.append(spell_name);
        }

        if (checkStriktSpell(w_right, w_left, gestures) && (res_right.indexOf(spell_name) == -1)) {
            res_right.append(spell_name);
        }
    }

    //qDebug() << "spellCheckFinish prepare result";
    QStringList res;
    if (res_left.count() > 0) {
        foreach(QString s, res_left) {
            res.append("L;" + s);
        }
    }
    if (res_right.count() > 0) {
        foreach(QString s, res_right) {
            res.append("R;" + s);
        }
    }
    //qDebug() << "spellCheckFinish result " << res;
    return res;
}

QString QWarlockSpellChecker::checkSpells(QString Left, QString Right, bool strikt) {
    qDebug() << "QWarlockSpellChecker::checkSpells" << Left << Right << strikt;
    QString left = Left.replace(" ", "");
    QString right = Right.replace(" ", "");
    QStringList sl;
    if (strikt) {
        sl = getStriktSpellsList(left, right);
    } else {
        sl = getPosibleSpellsList(left, right);
    }
    if (sl.count() == 0) {
        return "";
    }
    QString res;
    foreach(QString s, sl) {
        res.append(s + "#");
    }

    //qDebug() << "spellCheckFinish result " << res;
    return res;
}
