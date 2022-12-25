#include "qwarlockstat.h"
#include "qwarlockutils.h"

QWarlockStat::QWarlockStat() {

}

QWarlockStat::QWarlockStat(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity)
{
    init(Name, Registered, Ladder, Melee, Played, Won, Died, Elo, Color, LastActivity);
}

QWarlockStat::QWarlockStat(QString Raw, bool FromIni) {
    if (FromIni) {
        parseIniAndInit(Raw);
    } else {
        parseAndInit(Raw);
    }
}

void QWarlockStat::init(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity)
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
}

void QWarlockStat::parseIniAndInit(QString Raw) {
    QStringList data = Raw.split(",");
    bool Registered = data.at(0).compare("1");
    QString Name = data.at(1);
    int Ladder = data.at(2).toInt();
    int Melee = data.at(3).toInt();
    int Played = data.at(4).toInt();
    int Won = data.at(5).toInt();
    int Died = data.at(6).toInt();
    int Elo = data.at(7).toInt();
    //int Active = data.at(8).toInt();
    QString Color = data.size() > 9 ? data.at(9) : "";
    qint64 la = data.size() > 10 ? data.at(10).toInt() : 0;
    //qDebug() << "QWarlockStat::parseIniAndInit" << Raw << Registered << Name << Ladder << Melee << Played << Won << Died << Elo << Active;

    init(Name, Registered, Ladder, Melee, Played, Won, Died, Elo, Color, la);
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

    init(Name, !Registered.isEmpty(), Ladder, Melee, Played, Won, Died, Elo, Color, la);
}

qint64 QWarlockStat::getLastActivityByColor(const QString Color) {
    qint64 curr_time = QDateTime::currentSecsSinceEpoch();
    if (Color.compare("#00FF00") == 0) {
        // 0 minutes
    } else if (Color.compare("#11EE00") == 0) {
        curr_time -= 60 * 2; // 2 minutes ago
    } else if (Color.compare("#22DD00") == 0) {
        curr_time -= 60 * 5; // 5 minutes ago
    } else if (Color.compare("#33CC00") == 0) {
        curr_time -= 60 * 15; // 15 minutes ago
    } else if (Color.compare("#669900") == 0) {
        curr_time -= 60 * 60 * 5; // 5 hours ago
    } else {
        bool ok;
        QString tmp = Color;
        tmp.replace("#", "");
        int hex = tmp.toInt(&ok, 16);
        qDebug() << "QWarlockStat::getLastActivityByColor" << Color << tmp << ok << hex << (hex <= 0xCC3300);
        if (ok && (hex <= 0xCC3300)) {
            curr_time -= 1 * 24 * 60 * 60; // more than 3 days ago
        } else {
            curr_time -= 3 * 24 * 60 * 60 + 1; // more than 3 days ago
        }
    }

    return curr_time;
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
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,0,%9,%10")
            .arg(boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee) // 1-4
            ,intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo)) //5-8
            .arg(_color, intToStr(_lastActivity)); // 9-10
}

QString QWarlockStat::toJSON() const {
    return QString("{\"r\":%1,\"n\":\"%2\",\"l\":%3,\"m\":%4,\"p\":%5,\"w\":%6,\"d\":%7,\"e\":%8,\"c\":\"%9\","
                   "\"la\":%10}")
            .arg(boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee) // 1-4
            ,intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo)) //5-8
            .arg(_color, intToStr(_lastActivity)); // 9-10
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
