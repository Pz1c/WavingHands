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

int QBattleInfo::wait_from() const
{
    return _wait_from;
}

void QBattleInfo::setWaitFrom(int newWait_from, bool only_not_set)
{
    if (only_not_set && (_wait_from != -1)) {
        return;
    }
    _wait_from = newWait_from;
}

int QBattleInfo::status() const
{
    return _status;
}

void QBattleInfo::setStatus(int newStatus, bool only_not_set)
{
    if (only_not_set && (_status != -3)) {
        return;
    }
     _status = newStatus;
}

int QBattleInfo::hint() const
{
    return _hint;
}

void QBattleInfo::setHint(int newHint, bool only_not_set)
{
    if (only_not_set && (_hint != -1)) {
        return;
    }
    _hint = newHint;
}

const QString QBattleInfo::prepareToPrint(QString str) {
    return str.replace("\n","<br>").replace("\r", "").replace('"', "&quot;");
}

const QString QBattleInfo::toJSON() {
    return QString("{\"id\":%1,\"status\":%2,\"size\":%3,\"level\":%4,\"turn\":%5,\"wait_from\":%6,\"maladroit\":%7,\"parafc\":%8,\"parafdf\":%9"
                   ",\"description\":\"%10\",\"participant\":\"%11\",\"chat\":\"%12\",\"history\":\"%13\",\"winner\":\"%14\",\"hint\":%15}")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), prepareToPrint(_participant), prepareToPrint(_chat), prepareToPrint(_history), _winner, intToStr(_hint));
}

const QString QBattleInfo::toString() {
    return QString("id#=#%1^^^status#=#%2^^^size#=#%3^^^level#=#%4^^^turn#=#%5^^^wait_from#=#%6^^^maladroit#=#%7^^^parafc#=#%8^^^"
                   "parafdf#=#%9^^^description#=#%10^^^participant#=#%11^^^chat#=#%12^^^history#=#%13^^^winner#=#%14^^^hint#=#%15")
            .arg(intToStr(_battleID),intToStr(_status),intToStr(_size),intToStr(_level),intToStr(_turn),intToStr(_wait_from),boolToStr(_maladroit),boolToStr(_parafc),boolToStr(_parafdf))
            .arg(prepareToPrint(_description), _participant, prepareToPrint(_chat), prepareToPrint(_history), _winner, intToStr(_hint));
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
            _participant = value;
        } else if (key.compare("chat") == 0) {
            _chat = value;
        } else if (key.compare("history") == 0) {
            _history = value;
        } else if (key.compare("winner") == 0) {
            _winner = value;
        } else if (key.compare("hint") == 0) {
            _hint = value.toInt();
        }
    }
}
