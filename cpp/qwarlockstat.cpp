#include "qwarlockstat.h"
#include "qwarlockutils.h"

QWarlockStat::QWarlockStat() {

}

QWarlockStat::QWarlockStat(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity, bool Mobile)
{
    init(Name, Registered, Ladder, Melee, Played, Won, Died, Elo, Color, LastActivity, Mobile);
}

QWarlockStat::QWarlockStat(QString Raw, bool FromIni) {
    if (FromIni) {
        parseIniAndInit(Raw);
    } else if (Raw.indexOf("END_ROW") != -1) {
        parseSiteAndInit(Raw);
    } else {
        parseAndInit(Raw);
    }
}

void QWarlockStat::init(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity, bool Mobile, int warlockId)
{
    _name = Name;
    _registered = Registered;
    _ladder = Ladder;
    _melee = Melee;
    _played = Played;
    _won = Won;
    _died = Died;
    _elo = Elo;
    _color = Color;
    if (LastActivity != 0) {
        _lastActivity = LastActivity;
    } else {
        _lastActivity = 0;
    }
    _ai = (_lstAI.indexOf(_name.toUpper()) != -1);
    _mobile = Mobile;
    _warlockId = warlockId;
}

void QWarlockStat::parseSiteAndInit(QString Raw) {
    QStringList data = Raw.split(",");
    bool Registered = data.at(0).compare("1") == 0;
    QString Name = data.at(1);
    int Ladder = data.at(2).toInt();
    int Melee = data.at(3).toInt();
    int Played = data.at(4).toInt();
    int Won = data.at(5).toInt();
    int Died = data.at(6).toInt();
    int Elo = data.at(7).toInt();
    QString Color = data.at(8);
    bool Mobile = data.at(10).toInt() == 1;
    qint64 la = QDateTime::currentSecsSinceEpoch() - data.at(11).toInt();
    qDebug() << "QWarlockStat::parseSiteAndInit" << Raw << Registered << Name << Ladder << Melee << Played << Won << Died << Elo << la;

    init(Name, Registered, Ladder, Melee, Played, Won, Died, Elo, Color, la, Mobile);
}


//QString QWarlockStat::toString() const {
//    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11")
//        .arg(boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee) // 1-4
//             ,intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo)) //5-8
//        .arg(_color, intToStr(_lastActivity), boolToIntS(_mobile)); // 9-11


void QWarlockStat::parseIniAndInit(QString Raw) {
    QStringList data = Raw.split(",");
    bool Registered = data.at(0).compare("1") == 0;
    QString Name = data.at(1);
    int Ladder = data.at(2).toInt();
    int Melee = data.at(3).toInt();
    int Played = data.at(4).toInt();
    int Won = data.at(5).toInt();
    int Died = data.at(6).toInt();
    int Elo = data.at(7).toInt();
    QString Color = data.at(8);
    qint64 la = data.at(9).toInt();
    bool Mobile = data.at(10).toInt() == 1;
    int id = data.at(11).toInt();
    qDebug() << "QWarlockStat::parseIniAndInit" << Raw << Registered << Name << Ladder << Melee << Played << Won << Died << Elo << la << id;

    init(Name, Registered, Ladder, Melee, Played, Won, Died, Elo, Color, la, Mobile, id);
}

void QWarlockStat::parseAndInit(QString Raw) {
    int pos = 0;
    QString Registered = QWarlockUtils::getStringFromData(Raw, "/img/reg.png", "ALT=\"", "!", pos, true);
    QString Name = QWarlockUtils::getStringFromData(Raw, "<A HREF=", "/player/", ".html", pos);
    int Ladder = QWarlockUtils::getIntFromPlayerData(Raw, "<TD ALIGN", ">", "<", pos);
    int Melee = QWarlockUtils::getIntFromPlayerData(Raw, "<TD ALIGN", ">", "<", pos);
    int Played = QWarlockUtils::getIntFromPlayerData(Raw, "<TD ALIGN", ">", "<", pos);
    int Won = QWarlockUtils::getIntFromPlayerData(Raw, "<TD ALIGN", ">", "<", pos);
    int Died = QWarlockUtils::getIntFromPlayerData(Raw, "<TD ALIGN", ">", "<", pos);
    int Elo = QWarlockUtils::getIntFromPlayerData(Raw, "<TD ALIGN", ">", "<", pos);
    QString Color = QWarlockUtils::getStringFromData(Raw, "<TD STYLE=", "background-color:", ";", pos);
    qint64 la = getLastActivityByColor(Color);
    //qDebug() << "QWarlockStat::parseAndInit" << Raw << Registered << Name << Ladder << Melee << Played << Won << Died << Elo << Active << Color;

    init(Name, !Registered.isEmpty(), Ladder, Melee, Played, Won, Died, Elo, Color, la, false);
}

qint64 QWarlockStat::getLastActivityByColor(const QString Color) {
    qint64 curr_time = QDateTime::currentSecsSinceEpoch();
    int dec_sec;
    if (MAP_COLOR_TO_SECONDS.contains(Color)) {
        dec_sec = MAP_COLOR_TO_SECONDS[Color];
    } else {
        dec_sec = 20000100;
    }
    qDebug() << "QWarlockStat::getLastActivityByColor" << _name << Color << curr_time << dec_sec << curr_time - dec_sec;
    return curr_time - dec_sec;
}

void QWarlockStat::setMobile(bool newMobile)
{
    _mobile = newMobile;
}

void QWarlockStat::setLastActivity(qint64 newLastActivity)
{
    _lastActivity = newLastActivity;
}

void QWarlockStat::setElo(int newElo)
{
    _elo = newElo;
}

void QWarlockStat::setDied(int newDied)
{
    _died = newDied;
}

void QWarlockStat::setWon(int newWon)
{
    _won = newWon;
}

void QWarlockStat::setPlayed(int newPlayed)
{
    _played = newPlayed;
}

void QWarlockStat::setMelee(int newMelee)
{
    _melee = newMelee;
}

void QWarlockStat::setLadder(int newLadder)
{
    _ladder = newLadder;
}

void QWarlockStat::setRegistered(bool newRegistered)
{
    _registered = newRegistered;
}

int QWarlockStat::warlockId() const
{
    return _warlockId;
}

void QWarlockStat::setWarlockId(int newWarlockId)
{
    _warlockId = newWarlockId;
}

bool QWarlockStat::mobile() const
{
    return _mobile;
}

bool QWarlockStat::ai() const
{
    return _ai;
}

qint64 QWarlockStat::lastActivity() const
{
    return _lastActivity;
}

QString QWarlockStat::toString() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12")
            .arg(boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee) // 1-4
                 ,intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo)) //5-8
            .arg(_color, intToStr(_lastActivity), boolToIntS(_mobile), intToStr(_warlockId)); // 9-12
}

QString QWarlockStat::toJSON() const {
    qint64 curr_time = QDateTime::currentSecsSinceEpoch();
    QString active = boolToIntS(curr_time - _lastActivity <= 300);
    return QString("{\"r\":%1,\"n\":\"%2\",\"l\":%3,\"m\":%4,\"p\":%5,\"w\":%6,\"d\":%7,\"e\":%8,\"c\":\"%9\","
                   "\"la\":%10,\"mob\":%11,\"a\":%12,\"id\":%13}")
            .arg(boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee) // 1-4
            ,intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo)) //5-8
            .arg(_color, intToStr(_lastActivity), boolToIntS(_mobile), active, intToStr(_warlockId)); // 9-15
}


bool QWarlockStat::online() const {
    return _ai || (QDateTime::currentSecsSinceEpoch() - _lastActivity <= 5 * 60);
}

int QWarlockStat::elo() const
{
    return _elo;
}

int QWarlockStat::died() const
{
    return _died;
}

int QWarlockStat::won() const
{
    return _won;
}

int QWarlockStat::played() const
{
    return _played;
}

int QWarlockStat::melee() const
{
    return _melee;
}

int QWarlockStat::ladder() const
{
    return _ladder;
}

bool QWarlockStat::registered() const
{
    return _registered;
}

const QString &QWarlockStat::name() const
{
    return _name;
}
