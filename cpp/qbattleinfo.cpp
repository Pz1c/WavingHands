#include "qbattleinfo.h"
#include "qwarlockutils.h"

QBattleInfo::QBattleInfo()
{
    //qDebug() << "QBattleInfo::QBattleInfo";
    init();
}

QBattleInfo::QBattleInfo(const QString &battle_info) {
    init();
    parseString(battle_info);
}

void QBattleInfo::init() {
    _battleID = 0;
    _status = -3;
    _size = 0; // 2-6
    _level = -1; // 0 Ladder, 1 Friendly, 2 V.Friendly
    _turn = 0;
    _wait_from = -1;
    _hint = -1;
    _maladroit = false;
    _parafc = false;
    _parafdf = false;
    _fast = false;
    _for_bot = false;
    _with_bot = false;
    _fullParsed = false;
}

const QString &QBattleInfo::winner() const
{
    return _winner;
}

void QBattleInfo::setWinner(const QString &newWinner)
{
    _winner = newWinner.toLower();
}

int QBattleInfo::size() const
{
    return _size;
}

void QBattleInfo::setSize(int newSize)
{
    _size = newSize;
}

int QBattleInfo::battleID() const
{
    return _battleID;
}

void QBattleInfo::setBattleID(int newBattleID)
{
    _battleID = newBattleID;
}

int QBattleInfo::wait_from() const
{
    return _wait_from;
}

void QBattleInfo::setWaitFrom(int newWait_from)
{
    _wait_from = newWait_from;
}

int QBattleInfo::status() const
{
    return _status;
}

void QBattleInfo::setStatus(int newStatus)
{
     if (_status == newStatus) {
         if ((newStatus == BATTLE_INFO_STATUS_WAIT) && (_wait_from <= 0)) {
             _wait_from = static_cast<int>(QDateTime::currentSecsSinceEpoch() - SECONDS_AT_20210901);
         }
         return;
     }
    if (newStatus == BATTLE_INFO_STATUS_READY) {
        _wait_from = -1;
    }
    if ((_status == BATTLE_INFO_STATUS_READY) && (newStatus == BATTLE_INFO_STATUS_WAIT)) {
        _wait_from = static_cast<int>(QDateTime::currentSecsSinceEpoch() - SECONDS_AT_20210901);
    }
     _status = newStatus;
}

bool QBattleInfo::canForceSurrendering() const {
    int curr_time = static_cast<int>(QDateTime::currentSecsSinceEpoch() - SECONDS_AT_20210901);
    return ((_wait_from > 0) && ((curr_time - _wait_from) > 75 * 60 * 60));
}

int QBattleInfo::hint() const
{
    return _hint;
}

void QBattleInfo::setHint(int newHint)
{
    _hint = newHint;
}

void QBattleInfo::cleanParticipant() {
    _participant.clear();
    _challenged.clear();
    _with_bot = false;
}

void QBattleInfo::addParticipant(const QString &login, bool Challenged) {
    QString clean_login = login;
    int idx1 = clean_login.indexOf("(");
    if (idx1 != -1) {
        clean_login = clean_login.mid(0, idx1);
    }
    if (Challenged) {
        if (_challenged.indexOf(clean_login, Qt::CaseInsensitive) == -1) {
            _challenged.append(clean_login);
        }
    } else {
        if (_participant.indexOf(clean_login, Qt::CaseInsensitive) == -1) {
            _participant.append(clean_login);
        }
        _with_bot = _with_bot || (_lstAI.indexOf(clean_login.toUpper()) != -1);
    }
}

QString QBattleInfo::containParticipant(const QString& line) {
    qDebug() << "QBattleInfo::containParticipant" << line;
    foreach(QString lp, _participant) {
        qDebug() << "QBattleInfo::containParticipant" << lp;
        if (line.toLower().indexOf(lp.toLower()) != -1) {
            qDebug() << "QBattleInfo::containParticipant" << "true";
            return lp;
        }
    }
    return "";
}

QString QBattleInfo::getEnemy(const QString &Login) const {
    QString tmp = Login.toLower();
    foreach(QString lp, _participant) {
        if (tmp.compare(lp.toLower()) != 0) {
            return lp;
        }
    }
    return "";
}

bool QBattleInfo::fullParsed() const
{
    return _fullParsed;
}

const QString &QBattleInfo::description() const
{
    return _description;
}

void QBattleInfo::setDescription(const QString &newDescription)
{
    _description = newDescription;
    _for_bot = newDescription.indexOf("TRANING BOT ONLY") != -1;
}

bool QBattleInfo::with_bot() const
{
    return _with_bot;
}

bool QBattleInfo::for_bot() const
{
    return _for_bot;
}

void QBattleInfo::setForBot(bool newFor_bot)
{
    _for_bot = newFor_bot;
}

bool QBattleInfo::fast() const
{
    return _fast;
}

void QBattleInfo::setFast(bool newFast)
{
    _fast = newFast;
}

bool QBattleInfo::parafdf() const
{
    return _parafdf;
}

void QBattleInfo::setParafdf(bool newParafdf)
{
    _parafdf = newParafdf;
}

bool QBattleInfo::parafc() const
{
    return _parafc;
}

void QBattleInfo::setParafc(bool newParafc)
{
    _parafc = newParafc;
}

bool QBattleInfo::maladroit() const
{
    return _maladroit;
}

void QBattleInfo::setMaladroit(bool newMaladroit)
{
    _maladroit = newMaladroit;
}

int QBattleInfo::level() const
{
    return _level;
}

void QBattleInfo::setLevel(int newLevel)
{
    _level = newLevel;
}

int QBattleInfo::turn() const
{
    return _turn;
}

void QBattleInfo::setTurn(int newTurn)
{
    _turn = newTurn;
}

/*
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
*/

QString QBattleInfo::getInListDescription(const QString &Login) const  {
    if (_status == -3) {
        return QString("Battle #%1 no info").arg(intToStr(_battleID));
    } else if (_status == -2) {
        return QString("Battle #%1 deleted").arg(intToStr(_battleID));
    } else if (_status == -1) {
        return "Waiting to start...";
        //return QString("Waiting to start...").arg(intToStr(_battleID));
    } else if (_status == 0) {
        if (_size == _participant.size()) {
            // looks like already started
            QString enemy = getEnemy(Login);
            if (enemy.isEmpty()) {
                return QString("Waiting opponent #%1").arg(intToStr(_battleID));
            } else {
                return QString("Waiting for %1").arg(enemy);
            }
        } else {
            // not started perhaps
            return "Waiting to start...";
            //return QString("Waiting opponent #%1").arg(intToStr(_battleID));
        }
    } else if (_status == 1) {
        QString enemy = getEnemy(Login);
        if (enemy.isEmpty()) {
            return QString("Ready #%1").arg(intToStr(_battleID));
        } else {
            return QString("Ready vs. %1").arg(enemy);
        }
    } else if (_status == 2) {
        QString enemy = getEnemy(Login.toLower());
        if (enemy.isEmpty()) {
            if (_winner.isEmpty()) {
                return QString("Finished #%1").arg(intToStr(_battleID));
            } else if (_winner.compare(" ") == 0) {
                return QString("Draw #%1").arg(intToStr(_battleID));
            } else if (_winner.compare(Login.toLower()) == 0) {
                return QString("Won #%1").arg(intToStr(_battleID));
            } else {
                return QString("Lose #%1").arg(intToStr(_battleID));
            }
        } else {
            if (_winner.isEmpty()) {
                return QString("Finished vs. %1").arg(enemy);
            } else if (_winner.compare(" ") == 0) {
                return QString("Draw vs. %1").arg(enemy);
            } else if (_winner.compare(Login.toLower()) == 0) {
                return QString("Won vs. %1").arg(enemy);
            } else {
                return QString("Lose vs. %1").arg(enemy);
            }
        }
    }

    return QString("Battle #%1").arg(intToStr(_battleID));
}

bool QBattleInfo::active(const QString &login) const {
    QString check = login.toLower();
    foreach(QString s, _participant) {
        if (check.compare(s.toLower()) == 0) {
            return false;
        }
    }
    return true;
}

bool QBattleInfo::isWinner(const QString &Login) const {
    return _winner.compare(Login.toLower()) == 0;
}

void QBattleInfo::addChat(int battle_turn, const QString &chat_msg) {
    qDebug() << "QBattleInfo::addChat" << battle_turn << chat_msg << _chat.size() << _chat;
    if (_turn < battle_turn) {
        _turn = battle_turn;
    }
    while(_chat.count() <= battle_turn) {
        _chat.append("");
    }
    _chat.replace(battle_turn, chat_msg);
}

QString QBattleInfo::getChat() const {
    QString tmpBC;
    int turn_idx = -1;
    foreach(QString s, _chat) {
        if (++turn_idx == 0) {
            //continue;
        }
        if (s.isEmpty()) {
            continue;
        }
        tmpBC.prepend(prepareToPrint(s));
        tmpBC.prepend(QString("<p><font color=&quot;#10C9F5&quot; >Turn %1</font></p>").arg(intToStr(turn_idx)));
    }
    return tmpBC;
}

void QBattleInfo::addHistory(int battle_turn, const QString &hist_msg) {
    qDebug() << "QBattleInfo::addHistory" << battle_turn << hist_msg << _history;
    if (_turn < battle_turn) {
        _turn = battle_turn;
    }
    while(_history.size() <= battle_turn) {
        _history.append("");
    }
    _history.replace(battle_turn, hist_msg);
}

QString QBattleInfo::getHistory() const {
    QString tmpBH;
    int turn_idx = -1;
    foreach(QString s, _history) {
        if (++turn_idx == 0) {
            //continue;
        }
        tmpBH.prepend(prepareToPrint(s));
        tmpBH.prepend(QString("<p><font color=&quot;#10C9F5&quot; size=+1>Turn %1</font></p>").arg(intToStr(turn_idx)));
    }
    return tmpBH;
}

QString QBattleInfo::prepareToPrint(QString str) const {
    return str.replace("\n","<br>").replace("\r", "").replace('"', "&quot;");
}

QString QBattleInfo::getFullHist(const QString& Login) const {
    QString fh, tmp;
    if (!_description.isEmpty()) {
        fh.append("<p><font color=&quot;#10C9F5&quot; size=+1>Turn 0</font></p>");
        fh.append(_description);
    }
    for (int i = 1, Ln = _history.size(); i < Ln; ++i) {
        tmp.clear();
        tmp.append(QString("<p><font color=&quot;#10C9F5&quot; size=+1>Turn %1</font></p>").arg(intToStr(i)));
        tmp.append(prepareToPrint(_chat.at(i)));
        if (!_chat.at(i).isEmpty() && !_history.at(i).isEmpty()) {
            tmp.append("<br>");
        }
        tmp.append(prepareToPrint(_history.at(i)));
        fh.prepend(tmp);
    }
    return QString("{\"type\":9,\"d\":\"%1\",\"id\":%2,\"t\":\"%3\",\"st\":\"%4\"}").arg(fh, intToStr(_battleID), getInListDescription(Login), _sub_title);
}

QString QBattleInfo::toJSON(const QString &Login) const {
    return QString("{\"id\":%1,\"status\":%2,\"size\":%3,\"level\":%4,\"turn\":%5,\"wait_from\":%6,\"maladroit\":%7,\"parafc\":%8,\"parafdf\":%9"
                   ",\"description\":\"%10\",\"participant\":\"%11\",\"chat\":\"%12\",\"history\":\"%13\",\"winner\":\"%14\",\"hint\":%15,"
                   "\"fast\":%16,\"with_bot\":%17,\"for_bot\":%18,\"active\":%19,\"need\":%20,\"player\":\"%21\",\"challenged\":\"%22\"}")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), prepareToPrint(_participant.join(",")), prepareToPrint(_chat.join("#END_TURN#")), prepareToPrint(_history.join("#END_TURN#")), _winner, intToStr(_hint))
            .arg(boolToStr(_fast), boolToStr(_with_bot), boolToStr(_for_bot), boolToStr(active(Login)), intToStr(_size - _participant.size()), Login, _challenged.join(","));
}

QString QBattleInfo::toString(bool Short) const {
    QString tmp_chat = Short ? "" : prepareToPrint(_chat.join("#END_TURN#"));
    QString tmp_hist = Short ? "" : prepareToPrint(_history.join("#END_TURN#"));
    return QString("id#=#%1^^^status#=#%2^^^size#=#%3^^^level#=#%4^^^turn#=#%5^^^wait_from#=#%6^^^maladroit#=#%7^^^parafc#=#%8^^^"
                   "parafdf#=#%9^^^description#=#%10^^^participant#=#%11^^^chat#=#%12^^^history#=#%13^^^winner#=#%14^^^hint#=#%15^^^"
                   "fast#=#%16^^^with_bot#=#%17^^^for_bot#=#%18^^^full_parsed#=#%19^^^sub_title#=#%20^^^challenged#=#%21")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), prepareToPrint(_participant.join(",")), tmp_chat, tmp_hist, _winner, intToStr(_hint))
            .arg(boolToStr(_fast), boolToStr(_with_bot), boolToStr(_for_bot), boolToStr(_fullParsed), _sub_title, _challenged.join(","));
}

void QBattleInfo::parseString(const QString &battle_info) {
    QStringList sl1 = battle_info.split("^^^"), sl2;
    QString key, value;
    foreach(QString si, sl1) {
        sl2 = si.split("#=#");
        if (sl2.size() != 2) {
            continue;
        }
        key = sl2[0];
        value = sl2[1];
        if (key.compare("id") == 0) {
            _battleID = value.toInt();
        } else if (key.compare("status") == 0) {
            _status = value.toInt();
        } else if (key.compare("size") == 0) {
            _size = value.toInt();
        } else if (key.compare("level") == 0) {
            _level = value.toInt();
        } else if (key.compare("turn") == 0) {
            _turn = value.toInt();
        } else if (key.compare("wait_from") == 0) {
            _wait_from = value.toInt();
        } else if (key.compare("maladroit") == 0) {
            _maladroit = value.compare("true") == 0;
        } else if (key.compare("parafc") == 0) {
            _parafc = value.compare("true") == 0;
        } else if (key.compare("parafdf") == 0) {
            _parafdf = value.compare("true") == 0;
        } else if (key.compare("description") == 0) {
            _description = value;
        } else if (key.compare("participant") == 0) {
            _participant = value.split(",");
        } else if (key.compare("chat") == 0) {
            _chat = value.split("#END_TURN#");
        } else if (key.compare("history") == 0) {
            _history = value.split("#END_TURN#");
        } else if (key.compare("winner") == 0) {
            _winner = value;
        } else if (key.compare("hint") == 0) {
            _hint = value.toInt();
        } else if (key.compare("fast") == 0) {
            _fast = value.compare("true") == 0;
        } else if (key.compare("with_bot") == 0) {
            _with_bot = value.compare("true") == 0;
        } else if (key.compare("for_bot") == 0) {
            _for_bot = value.compare("true") == 0;
        } else if (key.compare("full_parsed") == 0) {
            _fullParsed = value.compare("true") == 0;
        } else if (key.compare("sub_title") == 0) {
            _sub_title = value;
        } else if (key.compare("challenged") == 0) {
            _challenged = value.split(",");
        }
    }
}

void QBattleInfo::parseAllTurns(QString& Data) {
    _fullParsed = true;
    cleanParticipant();
    int idx = Data.indexOf("<TABLE CELLPADDING=0 CELLSPACING=0 BORDER=0 WIDTH="), last_idx = idx, first_idx = Data.indexOf("<U>Turn");
    while (true) {
        QString warlock = QWarlockUtils::getStringFromData(Data, "<a href=\"/player/", ">", "<", idx);
        if (warlock.isEmpty()) {
            break;
        }
        addParticipant(warlock);
    }
    setSize(_participant.size());
    QString hist = Data.mid(first_idx, last_idx - first_idx).replace("<a href=\"/player", "<b atr=\"")
        .replace("<A CLASS=amonoturn HREF=\"/warlocks", "<b atr=\"").replace("</A>", "</b>").replace("</a>", "</b>")
        .replace("BLOCKQUOTE", "p").replace("WIDTH=\"100%\"", "").replace("WIDTH=\"50%\"", "").replace("<H2>", "").replace("</H2>", "")
        .replace("<p>", "").replace("</p>", "").replace("#FFFF88", "#F5C88E").replace("#88FFFF", "#54EBEB").replace("#88FF88", "#79D979");
    QWarlockUtils::parseBattleHistory(hist, this);
}
void QBattleInfo::setSubTitle(const QString &newSub_title)
{
    _sub_title = newSub_title;
}
