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
     _status = newStatus;
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
    if (_participant.indexOf(login.toLower()) == -1) {
        _participant.append(login.toLower());
    }
}

const QString QBattleInfo::getEnemy(const QString &Login) {
    foreach(QString lp, _participant) {
        if (lp.compare(Login) != 0) {
            return lp;
        }
    }
    return "";
}

const QString QBattleInfo::getInListDescription(const QString &Login) {
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
            return QString("Waiting opponent #%1").arg(intToStr(_battleID));
        }
    } else if (_status == 1) {
        QString enemy = getEnemy(Login);
        if (enemy.isEmpty()) {
            return QString("Ready #%1").arg(intToStr(_battleID));
        } else {
            return QString("Ready vs. %1").arg(enemy);
        }
    } else if (_status == 2) {
        QString enemy = getEnemy(Login);
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

const bool QBattleInfo::isWinner(const QString &Login) {
    return _winner.compare(Login.toLower());
}

const bool QBattleInfo::withBot() {
    foreach(QString bot, _lstAI) {
        if (_participant.indexOf(bot.toLower()) == 0) {
            return true;
        }
    }
    return false;
}

const QString QBattleInfo::prepareToPrint(QString str) {
    return str.replace("\n","<br>").replace("\r", "").replace('"', "&quot;");
}

const QString QBattleInfo::toJSON() {
    return QString("{\"id\":%1,\"status\":%2,\"size\":%3,\"level\":%4,\"turn\":%5,\"wait_from\":%6,\"maladroit\":%7,\"parafc\":%8,\"parafdf\":%9"
                   ",\"description\":\"%10\",\"participant\":\"%11\",\"chat\":\"%12\",\"history\":\"%13\",\"winner\":\"%14\",\"hint\":%15}")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), prepareToPrint(_participant.join(",")), prepareToPrint(_chat.join("#END_TURN#")), prepareToPrint(_history.join("#END_TURN#")), _winner, intToStr(_hint));
}

const QString QBattleInfo::toString() {
    return QString("id#=#%1^^^status#=#%2^^^size#=#%3^^^level#=#%4^^^turn#=#%5^^^wait_from#=#%6^^^maladroit#=#%7^^^parafc#=#%8^^^"
                   "parafdf#=#%9^^^description#=#%10^^^participant#=#%11^^^chat#=#%12^^^history#=#%13^^^winner#=#%14^^^hint#=#%15")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), prepareToPrint(_participant.join(",")), prepareToPrint(_chat.join("#END_TURN#")), prepareToPrint(_history.join("#END_TURN#")), _winner, intToStr(_hint));
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
        }
    }
}
