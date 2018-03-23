#include "qwarlockutils.h"
#include "qwarlockdictionary.h"

QWarlockUtils::QWarlockUtils()
{
}


QList<int> QWarlockUtils::getBattleList(QString &Data, QString Search) {
    QList<int> res_list;
    int idx1 = Data.indexOf(Search);
    if (idx1 == -1) {
        return res_list;
    }
    idx1 += Search.length();
    int idx2 = Data.indexOf("<TABLE", idx1);
    if (idx2 == -1) {
        return res_list;
    }
    idx2 += 7;
    int idx3 = Data.indexOf("</TABLE", idx2);
    if (idx3 == -1) {
        return res_list;
    }
    int idx4 = idx2;
    QString battle_id;
    while((idx4 = Data.indexOf("HREF=\"/warlocks?num=", idx4)) != -1 && idx4 < idx3) {
        idx4 += 19;
        battle_id.clear();
        while(++idx4 < idx3) {
            QChar a = Data.at(idx4);
            if (a.isDigit()) {
                battle_id.append(a);
            } else {
                bool *ok = new bool();
                int int_res = battle_id.toInt(ok, 10);
                if (*ok) {
                    res_list.append(int_res);
                }

                break;
            }
        }
    }

    return res_list;
}


QString QWarlockUtils::getStringFromData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd) {
    int idx1 = Data.indexOf(Search);
    if (idx1 == -1) {
        return 0;
    }
    idx1 += Search.length();

    int idx2 = ValueBegin.isEmpty() ? idx1 : Data.indexOf(ValueBegin, idx1);
    if (idx2 == -1) {
        return 0;
    }

    idx2 += ValueBegin.length();
    int idx3 = Data.indexOf(ValueEnd, idx2);
    if (idx3 == -1) {
        return 0;
    }
    QString res = Data.mid(idx2, idx3 - idx2);
    qDebug() << "getStringFromData " << Search << " res: " << res;
    return res;
}

int QWarlockUtils::getIntFromPlayerData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd) {
    QString res = getStringFromData(Data, Search, ValueBegin, ValueEnd);
    bool *ok = new bool();
    int int_res = res.toInt(ok, 10);
    return *ok ? int_res : 0;
}


bool QWarlockUtils::parseTargetList(QString &Data, QList<QValueName>& _Targets, QString &error) {
    //qDebug() << "parseTargetList: " << Data;
    _Targets.clear();
    _Targets = getSelectOptionFromData(Data, "<SELECT NAME=\"LHT\">", "</SELECT>", "<OPTION VALUE=\"", "</OPTION>", error);
    return error.isEmpty();
}

bool QWarlockUtils::parseMonster(QString &Data, QList<QMonster *> &result, QString &error) {
    error.clear();
    QString name = getStringFromData(Data, "WIDTH=\"50%\"", ">", "</TD");
    QString state = getStringFromData(Data, "CLASS=lightbg>", "", "</TD");
    QString owner = getStringFromData(Data, "Owned by:", " ", "</TD");
    QString attack = getStringFromData(Data, "Attacking:", " ", "</TD");
    QMonster *res = new QMonster(name, state, owner, attack);
    result.append(res);
    return true;
}

bool QWarlockUtils::parseWarlock(QString &Data, QList<QWarlock *> &result, QString &error, const QString &player) {
    error.clear();
    QString name = getStringFromData(Data, "WIDTH=\"50%\"", "html\">", "</A>");
    QString state = getStringFromData(Data, "a href=\"/player/", "<TD CLASS=lightbg>", "</TD");
    QString lh = getStringFromData(Data, "LH:</FONT>", "<FONT CLASS=monoturn>", "</FONT>").replace("&nbsp;", " ").replace("&gt;", ">");
    QString rh = getStringFromData(Data, "RH:</FONT>", "<FONT CLASS=monoturn>", "</FONT>").replace("&nbsp;", " ").replace("&gt;", ">");
    QWarlock *res = new QWarlock(name, state, lh, rh, name.toLower().indexOf(player) != -1);
    result.append(res);
    return true;
}

QList<QValueName> QWarlockUtils::getSelectOptionFromData(QString &Data, QString SearchBegin, QString SearchEnd, QString ValueBegin, QString ValueEnd, QString &error) {
    QList<QValueName> res;
    error.clear();
    int idx1 = Data.indexOf(SearchBegin);
    if (idx1 == -1) {
        error = "Can't found target list, try to reload";
        return res;
    }
    idx1 += SearchBegin.length();

    int idx2 = Data.indexOf(SearchEnd, idx1);
    if (idx2 == -1) {
        error = "Can't found target list, try to reload.";
        return res;
    }
    int idx3 = idx1, idx4, idx5;
    QString value, name;
    while((idx3 = Data.indexOf(ValueBegin, idx3)) != -1 && idx3 < idx2) {
        idx3 += ValueBegin.length();
        idx4 = Data.indexOf('"', idx3);
        value = Data.mid(idx3, idx4 - idx3);
        idx4 += 2;
        idx5 = Data.indexOf(ValueEnd, idx4);
        name = Data.mid(idx4, idx5 - idx4);

        res.append(QValueName(value, name));

        idx3 = idx5 + ValueEnd.length();
    }

    return res;
}

bool QWarlockUtils::parseGestures(QString &Data, QString &left, QString &right, QString &error) {
    left.clear();
    right.clear();
    QList<QValueName> g;
    g = getSelectOptionFromData(Data, "<SELECT NAME=LH>", "</SELECT>", "<OPTION VALUE=\"", "</OPTION>", error);
    if (!error.isEmpty()) {
        return false;
    }
    foreach(QValueName v, g) {
        left.append(QString("%1;%2#").arg(v.first.replace("&gt;", ">"), v.second.replace("&gt;", ">")));
    }
    g.clear();
    g = getSelectOptionFromData(Data, "<SELECT NAME=RH>", "</SELECT>", "<OPTION VALUE=\"", "</OPTION>", error);
    if (!error.isEmpty()) {
        return false;
    }
    foreach(QValueName v, g) {
        right.append(QString("%1;%2#").arg(v.first.replace("&gt;", ">"), v.second.replace("&gt;", ">")));
    }
    return true;
}

bool QWarlockUtils::parseMonsterCommad(QString &Data, QString &result, QString owner, QStringList &monsters, bool IsCharm) {
    result.clear();
    qDebug() << "parseMonsterCommad";
    QString search1 = "<TR><TD COLSPAN=3>Direct ";
    QString search2 = " to attack:";
    QString search3 = "<SELECT NAME=\"";
    int idx1 = 0, idx2;
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx2 = Data.indexOf(search2, idx1);
        QString monster_name = Data.mid(idx1, idx2 - idx1);
        idx1 = Data.indexOf(search3, idx2);
        idx1 += search3.length();
        idx2 = Data.indexOf("\"", idx1);
        QString monster_id = Data.mid(idx1, idx2 - idx1);
        qDebug() << "monster_name" << monster_name;
        qDebug() << "monster_id" << monster_id;
        if (IsCharm) {
            result.append(QString("%1,%2;").arg(monster_id, monster_name));
        } else if (((monster_name.indexOf("LH:") != -1) || (monster_name.indexOf("RH:") != -1)) && monster_name.toLower().indexOf(owner) != -1) {
            result.append(QString("%1,%2;").arg(monster_id, monster_name));
        } else if (monsters.count() > 0) {
            foreach(QString m, monsters) {
                if (m.compare(monster_name.toLower()) == 0) {
                    result.append(QString("%1,%2;").arg(monster_id, monster_name));
                    break;
                }
            }
        }
        idx1 = idx2 + search2.length();
    }
    return true;
}

QString QWarlockUtils::getCharmedPersonList(QString &Data) {
    qDebug() << "getCharmedPersonList";
    QString res;
    QString search1 = "<SELECT NAME=\"DIRECTHAND";
    QString search2 = "\"";
    int idx1 = 0, idx2;
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx2 = Data.indexOf(search2, idx1);
        QString target_id = Data.mid(idx1, idx2 - idx1);
        qDebug() << "target_id" << target_id;
        res.append(target_id + ";");
    }
    return res;
}

QString QWarlockUtils::getParalyseList(QString &Data) {
    //qDebug() << "getParalyseList";
    QString res;
    QString search1 = "<SELECT NAME=\"PARALYZE";
    QString search2 = "\"";
    int idx1 = 0, idx2;
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx2 = Data.indexOf(search2, idx1);
        QString target_id = Data.mid(idx1, idx2 - idx1);
        //qDebug() << "target_id" << target_id;
        res.append(target_id + ";");
    }
    return res;
}

QString QWarlockUtils::parseChallengePart0(QString &Data) {
    QString res;
    QString search1 = "<A HREF=\"/player/";
    QString search2 = "<BR>";
    QString str;

    // search login
    int idx1 = 0, idx2;
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx1 = Data.indexOf(">", idx1);
        idx2 = Data.indexOf(search2, ++idx1);
        str = Data.mid(idx1, idx2 - idx1).replace("</A> ", "");
        //qDebug() << "Found login: " << str;
        res.append(str + ", ");
        idx1 = idx2;
    }
    res.append("#!#").replace(", #!#", "#!#");

    // need X more -- search X
    QString search3 = "Need ";
    idx1 = Data.indexOf(search3);
    idx1 += search3.length();
    idx2 = Data.indexOf(" ", idx1);
    str = Data.mid(idx1, idx2 - idx1);
    //qDebug() << "Need more meat: " << str;
    res.append(str);
    res.append("#!#");

    return res;
}

QString QWarlockUtils::parseChallengePart1(QString &Data) {
    QString res;
    res.append(Data.indexOf("Fast") == -1 ? "0#!#" : "1#!#");
    if (Data.indexOf("V.F") != -1) {
        res.append("2#!#");
    } else if (Data.indexOf("Friendly") != -1) {
        res.append("1#!#");
    } else {
        res.append("0#!#");
    }
    return res;
}

QString QWarlockUtils::parseChallengeDescription(QString &Data) {
    QString res;
    //res.append(Data.toLower().indexOf("maladroit") == -1 ? "0#!#" : "1#!#");
    //res.append(Data.toLower().indexOf("parafc") == -1 ? "0#!#" : "1#!#");
    int idx1 = 0;
    idx1 = Data.indexOf(">");
    if (idx1 == -1) {
        res.append(Data);
    } else {
        res.append(Data.mid(++idx1, Data.length() - idx1));
    }
    res = res.replace("parafc", "", Qt::CaseInsensitive).replace("maladroit", "", Qt::CaseInsensitive).trimmed();
    return res;
}

QString QWarlockUtils::parseChallengePart3(QString &Data) {
    int idx1 = Data.indexOf(";num=");
    idx1 += 5;
    int idx2 = Data.indexOf("\"", idx1);
    return Data.mid(idx1, idx2 - idx1);
}

QString QWarlockUtils::parseChallenge(QString &Data) {
    //qDebug() << "parseChallenge";
    QString res;
    if (Data.indexOf(">Accept</A>") == -1) {
        return res;
    }
    QString search1 = "<TD";
    QString search2 = "</TD>";
    QStringList tmp;
    QString logins, description, battle_id;
    int need_more = 0, fast = 0, friendly = -1, parafc = 0, maladroit = 0;
    int idx1 = 0, idx2, idx = -1;
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx2 = Data.indexOf(search2, idx1);
        QString part = Data.mid(idx1, idx2 - idx1);
        QString challenge_part;
        switch(++idx) {
            case 0:
                challenge_part = parseChallengePart0(part);
                tmp = challenge_part.split("#!#");
                logins = tmp.at(0);
                need_more = tmp.at(1).toInt();
                if (need_more > 1) {
                    logins.append(QString(QWarlockDictionary::getInstance()->getStringByCode("NeedMore")).arg(QString::number(need_more)));
                }
                break;
            case 1:
                challenge_part = parseChallengePart1(part);
                tmp = challenge_part.split("#!#");
                fast = tmp.at(0).toInt();
                friendly = tmp.at(1).toInt();
                break;
            case 2:
                parafc = part.indexOf("ParaFC", 0, Qt::CaseInsensitive) == -1 ? 0 : 1;
                maladroit = part.indexOf("Maladroit", 0, Qt::CaseInsensitive) == -1 ? 0 : 1;
                description = parseChallengeDescription(part);
                break;
            case 3:
                battle_id = parseChallengePart3(part);
                break;
        }
        //res.append(challenge_part);
    }
    //res.append("#&#");
    QString level;
    switch (friendly) {
    case 0:
        level = need_more > 1 ? QWarlockDictionary::getInstance()->getStringByCode("Melee"): QWarlockDictionary::getInstance()->getStringByCode("Ladder");
        break;
    case 1:
        level = QWarlockDictionary::getInstance()->getStringByCode("Friendly");
        break;
    case 2:
        level = QWarlockDictionary::getInstance()->getStringByCode("VFriendly");
        break;
    }
    res = QString("{\"logins\":\"%1\",\"fast\":%2,\"level\":\"%3\",\"parafc\":%4,\"maladroit\":%5,\"desc\":\"%6\",\"battle_id\":%7}")
            .arg(logins,
                 QString::number(fast),
                 level,
                 QString::number(parafc),
                 QString::number(maladroit),
                 description, battle_id);
    //qDebug() << res;
    return res;
}

QString QWarlockUtils::parseChallengesList(QString &Data) {
    //qDebug() << "parseChallengesList";

    QString res = "[";
    QString search1 = "<TR><TD><A HREF=\"/player/";
    QString search2 = "</TR> <TR><TD COLSPAN=4><HR></TD></TR>";
    int idx1 = 0, idx2;
    int count = 0;

    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += 4;
        idx2 = Data.indexOf(search2, idx1);
        QString challenge = Data.mid(idx1, idx2 - idx1);

        QString parsed = parseChallenge(challenge);
        if (!parsed.isEmpty()) {
            if (++count > 1) {
                res.append(",");
            }
            res.append(parsed);
        }
    }
    return res.append("]");
}
