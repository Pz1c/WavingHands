#include "qwarlockutils.h"
#include "qwarlockdictionary.h"
#include "qwarlock.h"
#include "qmonster.h"
#include "qbattleinfo.h"


QWarlockUtils::QWarlockUtils()
{
}


QList<int> QWarlockUtils::getBattleList(QString &Data, QString Search) {
    QList<int> res_list;
    int idx1 = Data.indexOf(Search);
    if (idx1 == -1) {
        qDebug() << "QWarlockUtils::getBattleList" << "return -1";
        return res_list;
    }
    idx1 += Search.length();
    int idx2 = Data.indexOf("<TABLE", idx1);
    if (idx2 == -1) {
        qDebug() << "QWarlockUtils::getBattleList" << "return -2";
        return res_list;
    }
    idx2 += 7;
    int idx3 = Data.indexOf("</TABLE", idx2);
    if (idx3 == -1) {
        qDebug() << "QWarlockUtils::getBattleList" << "return -3";
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

    qDebug() << "QWarlockUtils::getBattleList" << Search << res_list;
    return res_list;
}


QString QWarlockUtils::getStringFromData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd, int &Pos, bool NoChangePos) {
    //qDebug() << "QWarlockUtils::getStringFromData" << (Data.length() > 60 ? Data.left(60) : Data) << Search << ValueBegin << ValueEnd << Pos << NoChangePos;
    int old_pos = Pos, idx1 = 0, idx2 = 0, idx3 = -1;
    if (Pos == -1) {
        Pos = 0;
    }
    do {
        idx1 = Data.indexOf(Search, Pos);
        if (idx1 == -1) {
            break;
        }
        idx1 += Search.length();

        idx2 = ValueBegin.isEmpty() ? idx1 : Data.indexOf(ValueBegin, idx1);
        if (idx2 == -1) {
            break;
        }

        idx2 += ValueBegin.length();

        idx3 = -1;
        QStringList ve = ValueEnd.split("#;#");
        foreach(QString v, ve) {
            int idx = Data.indexOf(v, idx2);
            if (idx == -1) {
                continue;
            }
            if ((idx3 == -1) || (idx < idx3)) {
                idx3 = idx;
            }
        }
        if (idx3 == -1) {
            idx3 = Data.length();
        }
    } while(false);
    if (NoChangePos) {
        Pos = old_pos;
    } else {
        Pos = idx3;
    }
    if (idx3 != -1) {
        QString res = Data.mid(idx2, idx3 - idx2);
        //qDebug() << "QWarlockUtils::getStringFromData" << idx1 << idx2 << idx3 << res;
        return res;
    } else {
        //qDebug() << "QWarlockUtils::getStringFromData not found" << idx1 << idx2 << idx3;
        return "";
    }
}

int QWarlockUtils::getIntFromPlayerData(QString &Data, QString Search, QString ValueBegin, QString ValueEnd, int &Pos, bool NoChangePos) {
    QString res = getStringFromData(Data, Search, ValueBegin, ValueEnd, Pos, NoChangePos);
    bool ok;
    int int_res = res.toInt(&ok, 10);
    int final_res = ok ? int_res : 0;
    //qDebug() << "QWarlockUtils::getIntFromPlayerData" << res << int_res << ok << final_res;
    return final_res;
}


bool QWarlockUtils::parseTargetList(QString &Data, QList<QValueName>& _Targets, QString &error) {
    //qDebug() << "parseTargetList: " << Data;
    _Targets.clear();
    _Targets = getSelectOptionFromData(Data, "<SELECT NAME=\"LHT\">", "</SELECT>", "<OPTION VALUE=\"", "</OPTION>", error);
    return error.isEmpty();
}

bool QWarlockUtils::parseMonster(QString &Data, QList<QMonster *> &result, QString &error) {
    error.clear();
    int pos = 0;
    QString name = getStringFromData(Data, "WIDTH=\"50%\"", ">", "</TD", pos);
    QString state = getStringFromData(Data, "CLASS=lightbg>", "", "</TD", pos);
    QString owner = getStringFromData(Data, "Owned by:", " ", "</TD", pos);
    QString attack = getStringFromData(Data, "Attacking:", " ", "</TD", pos);
    QMonster *res = new QMonster(name, state, owner, attack);
    result.append(res);
    return true;
}

bool QWarlockUtils::parseWarlock(QString &Data, QList<QWarlock *> &result, QString &error, const QString &player) {
    error.clear();
    int pos = 0;
    QString name = getStringFromData(Data, "WIDTH=\"50%\"", "html\">", "</A>", pos);
    pos = 0;
    QString state = getStringFromData(Data, "a href=\"/player/", "<TD CLASS=lightbg>", "</TD", pos);
    QString lh = getStringFromData(Data, "LH:</FONT>", "<FONT CLASS=monoturn>", "</FONT>", pos).replace("&nbsp;", " ").replace("&gt;", ">");
    QString rh = getStringFromData(Data, "RH:</FONT>", "<FONT CLASS=monoturn>", "</FONT>", pos).replace("&nbsp;", " ").replace("&gt;", ">");
    qDebug() << "QWarlockUtils::parseWarlock" << name << player;
    QWarlock *res = new QWarlock(name, state, lh, rh, name.compare(player, Qt::CaseInsensitive) == 0, _lstAI.indexOf(name.toUpper()) != -1);
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
    for (int i = 0, Ln = g.size(); i < Ln; ++i) {
        left.append(QString("%1;%2#").arg(g[i].first.replace("&gt;", ">"), g[i].second.replace("&gt;", ">")));
    }
    g.clear();
    g = getSelectOptionFromData(Data, "<SELECT NAME=RH>", "</SELECT>", "<OPTION VALUE=\"", "</OPTION>", error);
    if (!error.isEmpty()) {
        return false;
    }
    for (int i = 0, Ln = g.size(); i < Ln; ++i) {
        right.append(QString("%1;%2#").arg(g[i].first.replace("&gt;", ">"), g[i].second.replace("&gt;", ">")));
    }
    return true;
}

void QWarlockUtils::appendSeparatedList(QString &list, const QString &data, const QString &separator) {
    if (!list.isEmpty()) {
        list.append(separator);
    }
    list.append(data);
}

bool QWarlockUtils::parseMonsterCommad(QString &Data, QList<QMonster *> &result) {
    qDebug() << "QWarlockUtils::parseMonsterCommad" << result.size();
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
        QString monster_id = Data.mid(idx1, idx2 - idx1), owner;
        bool just_created = (monster_name.indexOf("LH:") != -1) || (monster_name.indexOf("RH:") != -1);
        if (just_created) {
            // fix
            qDebug() << "monster_name before fix" << monster_name;
            owner = monster_name.right(monster_name.length() - monster_name.indexOf(":") - 1);
            monster_name = monster_id.left(3) + owner;
        } else {
            continue;
        }
        qDebug() << "monster_name" << monster_name << "monster_id" << monster_id << result.count();
        idx1 = idx2 + search2.length();
        result.append(new QMonster(monster_name, "", owner, ""));
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
    bool first = true;
    int cnt = 0;
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx1 = Data.indexOf(">", idx1);
        idx2 = Data.indexOf(search2, ++idx1);
        str = Data.mid(idx1, idx2 - idx1).replace("</A> ", "");
        //qDebug() << "Found login: " << str;
        if (first) {
            first = false;
        } else {
            res.append(",");
        }
        res.append(str);
        idx1 = idx2;
        ++cnt;
    }
    res.append("#!#").replace(", #!#", "#!#");

    // need X more -- search X
    QString search3 = "Need ";
    idx1 = Data.indexOf(search3);
    idx1 += search3.length();
    idx2 = Data.indexOf(" ", idx1);
    str = Data.mid(idx1, idx2 - idx1);
    cnt += str.toInt();
    //qDebug() << "Need more meat: " << str;
    res.append(str);
    res.append("#!#");
    res.append(intToStr(cnt));
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
    int idx1 = Data.indexOf(">");
    if (idx1 == -1) {
        res.append(Data);
    } else {
        ++idx1;
        res.append(Data.mid(idx1, Data.length() - idx1));
    }
    res = res.replace('"', "&quot;").replace("\r", "").replace("\n","<br>")/*.replace("parafc", "", Qt::CaseInsensitive).replace("maladroit", "", Qt::CaseInsensitive)*/.trimmed();
    return res;
}

QString QWarlockUtils::parseChallengePart3(QString &Data) {
    QString marker1 = "num=";
    int idx1 = Data.indexOf(marker1);
    idx1 += marker1.length();
    int idx2 = Data.indexOf("\"", idx1);
    return Data.mid(idx1, idx2 - idx1);
}

QBattleInfo * QWarlockUtils::parseChallenge(QString &Data) {
    //qDebug() << "parseChallenge" << Data;
    QBattleInfo *res = nullptr;
    //bool is_active = Data.indexOf(">Accept</A>") != -1;
    QString search1 = "<TD";
    QString search2 = "</TD>";
    QStringList tmp, logins;
    QString description, battle_id;
    int friendly = -1, total_count = 0;
    bool parafdf = false, parafc = false, maladroit = false, fast = false;
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
                logins = tmp.at(0).split(",");
                //need_more = tmp.at(1).toInt();
                /*if (need_more > 1) {
                    logins.append(QString(QWarlockDictionary::getInstance()->getStringByCode("NeedMore")).arg(intToStr(need_more)));
                }*/
                total_count = tmp.at(2).toInt();
                break;
            case 1:
                challenge_part = parseChallengePart1(part);
                tmp = challenge_part.split("#!#");
                fast = tmp.at(0).toInt() == 1;
                friendly = tmp.at(1).toInt();
                break;
            case 2:
                parafc = part.indexOf("ParaFC", 0, Qt::CaseInsensitive) != -1;
                maladroit = part.indexOf("Maladroit", 0, Qt::CaseInsensitive) != -1;
                parafdf = part.indexOf("ParaFDF", 0, Qt::CaseInsensitive) != -1;
                description = parseChallengeDescription(part);
                break;
            case 3:
                battle_id = parseChallengePart3(part);
                break;
        }
        //res.append(challenge_part);
    }
    bool for_bot = (friendly == 2) && (total_count == 2) && (description.indexOf("NO BOT") == -1);
    //bool with_bot = (friendly == 2) && (total_count == 2) && (description.indexOf("Training Battle with AI Player") != -1);
    int bid = battle_id.toInt();
    if (bid > 0) {
        res = new QBattleInfo();
        res->setBattleID(bid);
        res->setStatus(BATTLE_INFO_STATUS_NO_START);
        res->setLevel(friendly);
        res->setSize(total_count);
        res->setFast(fast);
        res->setMaladroit(maladroit);
        res->setParafc(parafc);
        res->setParafdf(parafdf);
        res->setDescription(description);
        res->setForBot(for_bot);
        //res->setWithBot(with_bot);
        foreach(QString l, logins) {
            res->addParticipant(l);
        }
    }
    return res;
}

QList<QBattleInfo *> QWarlockUtils::parseChallengesList(QString &Data) {
    qDebug() << "parseChallengesList";
    QList<QBattleInfo *> res;
    QString search1 = "<TR><TD><A HREF=\"/player";
    QString search2 = "</TR> <TR><TD COLSPAN=4><HR></TD></TR>";
    int idx1 = 0, idx2;

    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += 4; // !! remove <TR> only
        idx2 = Data.indexOf(search2, idx1);
        QString challenge = Data.mid(idx1, idx2 - idx1);

        QBattleInfo *parsed = parseChallenge(challenge);
        if (parsed != nullptr) {
            res.append(parsed);
        }
    }
    return res;
}

QString QWarlockUtils::parseTopList(QString &Data) {
    qDebug() << "parseTopList";

    QString data = Data.replace("<IMG WIDTH=12 HEIGHT=12 SRC=\"/img/reg.png\" ALT=\"Registered!\" />&nbsp;", "");
    QString res = "[";
    QString search1 = "<TD><A HREF=\"/player/";
    QString search2 = "<TD STYLE=\"background-color:";
    int idx1 = 0, idx2;
    int count = 0;

    while((idx1 = data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx2 = data.indexOf(search2, idx1);
        QString player = data.mid(idx1, idx2 - idx1);

        QString parsed = parsePlayerTop(player);
        if (!parsed.isEmpty()) {
            if (++count > 1) {
                res.append(",");
            }
            res.append(parsed);
        }
    }
    return res.append("]");
}

QString QWarlockUtils::parsePlayerTop(QString &Data) {
    //qDebug() << "parseChallenge";
    QString res;
    QString search1 = "<TD ALIGN=CENTER>";
    QString search2 = "</TD>";
    QString login, ladder, melee, elo, played, won, died;
    int idx1 = 0, idx2, idx = 0;
    idx2 = Data.indexOf(".");
    login = Data.mid(0, idx2);
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx2 = Data.indexOf(search2, idx1);
        QString part = Data.mid(idx1, idx2 - idx1);
        switch(++idx) {
            case 0:
                login = part;
                break;
            case 1:
                ladder = part;
                break;
            case 2:
                melee = part;
                break;
            case 3:
                played = part;
                break;
            case 4:
                won = part;
                break;
            case 5:
                died = part;
                break;
            case 6:
                elo = part;
                break;
        }
        //res.append(challenge_part);
    }
    //res.append("#&#");

    res = QString("{\"login\":\"%1\",\"ladder\":%2,\"melee\":\"%3\",\"played\":%4,\"won\":%5,\"dead\":%6,\"elo\":%7}")
            .arg(login, ladder, melee, played, won, died, elo);
    //qDebug() << res;
    return res;
}


int QWarlockUtils::strValueToInt(QString val) {
    if (val.compare("permanent") == 0) {
        return 999;
    } else {
        return val.toInt();
    }
}

QString QWarlockUtils::getFinishedBattleDescription(const QString &Data) {
    QString res, winner, enemy, tmp;
    int idx1 = 0, idx2;//, idx3, count = 0, dead = 0, surrender = 0;
    idx1 = Data.indexOf(" is victorious!");
    if (idx1 != -1) {
        idx2 = idx1 - 13; // max login length is 10 + 3 for <b>
        idx2 = Data.indexOf("<B>", idx2);
        idx2 += 3;
        winner = Data.mid(idx2, idx1 - idx2);
        return winner;
    } else {
        return " ";
    }
}

QString QWarlockUtils::getBattleShortTitle(const QString &Title, int State, int BattleID) {
    if (Title.isEmpty()) {
        switch(State) {
        case -2: return QString("Battle #%1 deleted").arg(intToStr(BattleID));
        case -1: return "Waiting to start...";//QString("#%1 waiting to start...").arg(intToStr(BattleID));
        case 0: return QString("Waiting opponent #%1...").arg(intToStr(BattleID));
        case 1: return QString("Battle #%1 is ready").arg(intToStr(BattleID));
        case 2: return QString("Battle #%1 finished").arg(intToStr(BattleID));
        default: return QString("Battle #%1 in %2 state").arg(intToStr(BattleID), intToStr(State));
        }
    } else {
        switch(State) {
        //case -2: return QString("Battle #%1 deleted").arg(intToStr(BattleID));
        case -1: return "Waiting to start...";//QString("Not started, %1").arg(Title);
        case 0: return QString("Waiting for %1...").arg(Title);
        //case 1: return QString("Battle #%1 is ready").arg(Title);
        //case 2: return QString("Battle #%1 finished").arg(Title);
        default: return Title;
        }
    }
}

void QWarlockUtils::parseBattleTurn(QString &turn_text, QBattleInfo* bi, int current_turn, bool last_turn) {
    qDebug() << "QWarlockUtils::parseBattleTurn" << current_turn << last_turn;
    QString result_c, result_h, line, def_color = "#FEE2D6", current_color = def_color, sub_line, ssln, sssline, last_line, red_desc;
    int idx1;
    bool first_turn = current_turn == 0;
    QStringList lst = turn_text.split("<FONT "), sslines, ssslines;
    for (int i = 0, Ln = lst.length(); i < Ln; ++i) {
        line = lst.at(i).trimmed();
        //console.log("wnd_spell.processTurnMessage", "main", line, i, Ln, curr_turn, last_turn);
        if (line.isEmpty()) {
            continue;
        }
        idx1 = line.indexOf("COLOR=\"");
        if (idx1 != -1) {
            idx1 += 16;
            current_color = line.mid(idx1 - 9, 7);
        } else {
            idx1 = 0;
            current_color = def_color;
        }
        sub_line = line.mid(idx1);
        //console.log("wnd_spell.processTurnMessage", "sub", current_color, sub_line);
        sslines = sub_line.split("</FONT>");
        for(int j = 0, LnJ = sslines.length(); j < LnJ; ++j) {
            ssln = sslines.at(j).trimmed();
            if (ssln.isEmpty()) {
                continue;
            }
            ssslines = ssln.split("<BR>");
            for(int k = 0, LnK = ssslines.length(); k < LnK; ++k) {
                sssline = ssslines.at(k).trimmed().replace("<p>", "").replace("</p>", "");
                if (sssline.isEmpty()) {
                    continue;
                }
                if ((current_color.compare("#CCCCCC") != 0) || first_turn) {
                    if (sssline.indexOf(" says ") != -1) {
                        if (!result_c.isEmpty()) {
                            result_c.append("<br>");
                        }
                        result_c.append(QString("<font color=&quot;%1&quot;>%2</font>").arg(current_color, sssline.replace('"', "&quot;")));
                    } else {
                        if (!result_h.isEmpty()) {
                            result_h.append("<br>");
                        }
                        result_h.append(QString("<font color=&quot;%1&quot;>%2</font>").arg(current_color, sssline.replace('"', "&quot;")));
                    }
                } else {
                    continue;
                }
                if (last_turn) {
                    last_line = sssline;
                }
                if (last_turn && (current_color.compare("#FF6666") == 0) && !bi->containParticipant(sssline).isEmpty()) {
                    red_desc.append(sssline);
                    red_desc.append("<br>");
                }
            }
            current_color = def_color;
        }
    }
    if (last_turn && !last_line.isEmpty()) {
        QString sub_title = last_line;
        if (!red_desc.isEmpty()) {
            sub_title.append("<br>").append(red_desc);
        }
        bi->setSubTitle(sub_title);
        if (last_line.indexOf("is victorious") != -1) {
            bi->setWinner(bi->containParticipant(last_line));
        }
    }
    bi->addChat(current_turn, result_c);
    bi->addHistory(current_turn, result_h);
    //return result;
}

void QWarlockUtils::parseBattleHistory(QString &history, QBattleInfo* bi) {
    bool first = true;
    QString l_title, turn_txt;
    QString d, line1;
    QStringList lst1 = history.replace("<TABLE CELLPADDING=0 CELLSPACING=0 BORDER=0 ><TR>", "<U>Turn").split("<U>Turn");
    int i = lst1.size(), idx1, last_turn, curr_turn;
    foreach(const QString &s1, lst1) {
        --i;
        line1 = s1.trimmed();
        if (line1.isEmpty()) {
            continue;
        }
        if (first) {
            last_turn = line1.mid(0, line1.indexOf(" ")).trimmed().toInt();

            first = false;
            continue;
        }
        if ((i == 0) && (line1.indexOf("monoturn") != -1)) {
            continue;
        }

        idx1 = line1.indexOf("</U>");
        if (idx1 == -1) {
            continue;
        }

        curr_turn = line1.mid(0, idx1).toInt();
        idx1 += 4;
        turn_txt = line1.mid(idx1);
        parseBattleTurn(turn_txt, bi, curr_turn, curr_turn == last_turn);
    }

    //return QString("{\"type\":9,\"d\":\"%1\",\"id\":%2,\"t\":\"%3\",\"st\":\"%4\"}").arg(d, intToStr(bi->battleID()), bi->getInListDescription(login), l_title);
}

int QWarlockUtils::getStrengthByMonsterName(QString val) {
    if (val.indexOf("Goblin") != -1) {
        return 1;
    } else if (val.indexOf("Ogre") != -1) {
        return 2;
    } else if (val.indexOf("Trol") != -1) {
        return 3;
    } else if (val.indexOf("Giant") != -1) {
        return 4;
    } else if (val.indexOf("Fire") != -1) {
        return 3;
    } else if (val.indexOf("Ice") != -1) {
        return 3;
    } else {
        return 0;
    }
}

void QWarlockUtils::parsePersonalChallenge(const QString &Data, QStringList &result) {
    int idx1 = 0, idx2, battle_id;
    QString tmp, desc, logins, fast, level, parafc, maladroit, fl, need;

    QStringList sl = Data.split("<A HREF=\"/warlocks?num=");
    foreach(QString s, sl) {
        qDebug() << "QWarlockUtils::parsePersonalChallenge" << s;
        if (s.isEmpty()) {
            continue;
        }
        idx1 = s.indexOf('"');
        tmp = s.mid(0, idx1);
        qDebug() << "QWarlockUtils::parsePersonalChallenge" << tmp << idx1;
        battle_id = tmp.toInt();
        idx1 = s.indexOf("</A>. ", idx1);
        idx1 += 6;
        idx2 = s.indexOf("<A HREF", idx1);
        desc = s.mid(idx1, idx2 - idx1);
        fast = desc.indexOf("Fast") != -1 ? "1" : "0";
        if (desc.indexOf("Very") != -1) {
            level = "Training game";
            fl = "2";
        } else if (desc.indexOf("Friendly") != -1) {
            level = "Friendly";
            fl = "1";
        } else {
            level = "Ladder";
            fl = "0";
        }
        idx1 = s.indexOf("Waiting: ", idx2);
        logins.clear();
        while((idx1 = s.indexOf("<A HREF=\"/player/", idx1)) != -1) {
            idx1 += 17;
            idx2 = s.indexOf('.', idx1);
            tmp = s.mid(idx1, idx2 - idx1);
            if (!logins.isEmpty()) {
                logins.append(",");
            }
            logins.append(tmp);
        }
        if (battle_id > 0) {
            QStringList lg = logins.split(",");
            result.append(QString("{\"id\":%1,\"s\":3,\"d\":\"Challenged by %2\",\"dt\":\"%3\",\"el\":\"%4\"}").arg(intToStr(battle_id), lg.at(0).trimmed(), desc, lg.at(0).trimmed()));
        }
    }
}

void QWarlockUtils::parseUnstartedBattle(QString &Data, QBattleInfo *bi) {
    int idx1 = 0, idx2, idx3, idx4, idx5;
    bool challenged;
    // parse title
    QString title = getStringFromData(Data, "<H2><U", ">", "</U>", idx1);
    bi->setMaladroit(title.indexOf("(Maladroit)") != -1);
    bi->setParafc(title.indexOf("(ParaFC)") != -1);
    bi->setParafdf(title.indexOf("(ParaFDF)") != -1);
    if (title.indexOf("Very") != -1) {
        bi->setLevel(2);
    } else if (title.indexOf("Friendly") != -1) {
        bi->setLevel(1);
    } else {
        bi->setLevel(0);
    }
    // parse paricipant
    bi->cleanParticipant();
    qDebug() << "QWarlockUtils::parseUnstartedBattle" << idx1;
    while ((idx1 = Data.indexOf("<A HREF=\"/player/", idx1)) != -1) {
        idx1 += 17;
        idx2 = Data.indexOf('.', idx1);
        QString tmp_login = Data.mid(idx1, idx2 - idx1);
        idx3 = Data.indexOf("<BR>", idx2);
        idx4 = Data.indexOf("&reject=", idx2);
        challenged = (idx4 != -1) && (idx4 < idx3);
        if (challenged) {
            idx4 += 8;
            idx5 = Data.indexOf('"', idx4);
            QString w_id = Data.mid(idx4, idx5 - idx4);
            tmp_login.append(":");
            tmp_login.append(w_id);
        }
        idx1 = idx3;
        bi->addParticipant(tmp_login, challenged);
        qDebug() << "QWarlockUtils::parseUnstartedBattle" << idx1 << idx2 << tmp_login << challenged;
    }

    // parse description
    QString description = getStringFromData(Data, "<TD ALIGN=CENTER>", "<BR>", "</TD>", idx2);
    QStringList sl1 = description.split("<HR>");
    bi->setFast(sl1.at(0).indexOf("fast", Qt::CaseInsensitive) != -1);
    if (sl1.size() > 1) {
        bi->setDescription(sl1.at(1));
    }
    QStringList sl2 = sl1.at(0).split(" ");
    QString size_s = sl2.last();
    bool ok;
    int size_i = size_s.toInt(&ok);
    if (ok) {
        bi->setSize(size_i);
    }
}

int QWarlockUtils::getRand(int Min, int Max) {
    return QRandomGenerator::global()->bounded(Min, Max);
}
