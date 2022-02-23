#include "qbattleinfo.h"

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

void QBattleInfo::addParticipant(const QString &login) {
    QString clean_login = login;
    int idx1 = clean_login.indexOf("(");
    if (idx1 != -1) {
        clean_login = clean_login.mid(0, idx1 - 1);
    }

    if (_participant.indexOf(clean_login, Qt::CaseInsensitive) == -1) {
        _participant.append(clean_login);
    }
}

QString QBattleInfo::getEnemy(const QString &Login) const {
    foreach(QString lp, _participant) {
        if (lp.compare(Login, Qt::CaseInsensitive) != 0) {
            return lp;
        }
    }
    return "";
}

const QString &QBattleInfo::description() const
{
    return _description;
}

void QBattleInfo::setDescription(const QString &newDescription)
{
    _description = newDescription;
}

bool QBattleInfo::with_bot() const
{
    return _with_bot;
}

void QBattleInfo::setWithBot(bool newWith_bot)
{
    _with_bot = newWith_bot;
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
            QString enemy = getEnemy(Login.toLower());
            if (enemy.isEmpty()) {
                return QString("Waiting opponent #%1").arg(intToStr(_battleID));
            } else {
                return QString("Waiting for %1").arg(enemy);
            }
        } else {
            // not started perhaps
            return QString("Waiting opponent #%1").arg(intToStr(_battleID));
        }
    } else if (_status == 1) {
        QString enemy = getEnemy(Login.toLower());
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
            } else if (_winner.compare("no") == 0) {
                return QString("Draw #%1").arg(intToStr(_battleID));
            } else if (Login.compare(_winner, Qt::CaseInsensitive) == 0) {
                return QString("Win #%1").arg(intToStr(_battleID));
            } else {
                return QString("Lose #%1").arg(intToStr(_battleID));
            }
        } else {
            if (_winner.isEmpty()) {
                return QString("Finished vs. %1").arg(enemy);
            } else if (_winner.compare("no") == 0) {
                return QString("Draw vs. %1").arg(enemy);
            } else if (Login.compare(_winner, Qt::CaseInsensitive) == 0) {
                return QString("Win vs. %1").arg(enemy);
            } else {
                return QString("Lose vs. %1").arg(enemy);
            }
        }
    }

    return QString("Battle #%1").arg(intToStr(_battleID));
}

bool QBattleInfo::active(const QString &login) const {
    return _participant.indexOf(login, Qt::CaseInsensitive) == -1;
}

bool QBattleInfo::isWinner(const QString &Login) const {
    return _winner.compare(Login.toLower());
}

bool QBattleInfo::withBot() const {
    foreach(QString bot, _lstAI) {
        if (_participant.indexOf(bot, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
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
            continue;
        }
        if (s.isEmpty()) {
            continue;
        }
        tmpBC.append(QString("<p><font color=&quot;#10C9F5&quot; >Turn %1</font></p>").arg(intToStr(turn_idx)));
        tmpBC.append(prepareToPrint(s));
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
            continue;
        }
        tmpBH.append(QString("<p><font color=&quot;#10C9F5&quot; size=+1>Turn %1</font></p>").arg(intToStr(turn_idx)));
        tmpBH.append(prepareToPrint(s));
    }
    return tmpBH;
}

QString QBattleInfo::prepareToPrint(QString str) const {
    return str.replace("\n","<br>").replace("\r", "").replace('"', "&quot;");
}

QString QBattleInfo::toJSON(const QString &Login) const {
    return QString("{\"id\":%1,\"status\":%2,\"size\":%3,\"level\":%4,\"turn\":%5,\"wait_from\":%6,\"maladroit\":%7,\"parafc\":%8,\"parafdf\":%9"
                   ",\"description\":\"%10\",\"participant\":\"%11\",\"chat\":\"%12\",\"history\":\"%13\",\"winner\":\"%14\",\"hint\":%15,"
                   "\"fast\":%16,\"with_bot\":%17,\"for_bot\":%18,\"active\":%19}")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), prepareToPrint(_participant.join(",")), prepareToPrint(_chat.join("#END_TURN#")), prepareToPrint(_history.join("#END_TURN#")), _winner, intToStr(_hint))
            .arg(boolToStr(_fast), boolToStr(_with_bot), boolToStr(_for_bot), boolToStr(active(Login)));
}

QString QBattleInfo::toString(const QString &Login) const {
    return QString("id#=#%1^^^status#=#%2^^^size#=#%3^^^level#=#%4^^^turn#=#%5^^^wait_from#=#%6^^^maladroit#=#%7^^^parafc#=#%8^^^"
                   "parafdf#=#%9^^^description#=#%10^^^participant#=#%11^^^chat#=#%12^^^history#=#%13^^^winner#=#%14^^^hint#=#%15^^^"
                   "fast#=#%16^^^with_bot#=#%17^^^for_bot#=#%18^^^active#=#%19")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), prepareToPrint(_participant.join(",")), prepareToPrint(_chat.join("#END_TURN#")), prepareToPrint(_history.join("#END_TURN#")), _winner, intToStr(_hint))
            .arg(boolToStr(_fast), boolToStr(_with_bot), boolToStr(_for_bot), boolToStr(active(Login)));
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
        }
    }
}
