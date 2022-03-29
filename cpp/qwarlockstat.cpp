#include "qwarlockstat.h"
#include "qwarlockutils.h"

QWarlockStat::QWarlockStat() {

}

QWarlockStat::QWarlockStat(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color)
{
    init(Name, Registered, Ladder, Melee, Played, Won, Died, Elo, Color);

}

QWarlockStat::QWarlockStat(QString Raw, bool FromIni) {
    if (FromIni) {
        parseIniAndInit(Raw);
    } else {
        parseAndInit(Raw);
    }
}

void QWarlockStat::init(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color)
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
    _active = ARR_ACTIVE_COLOR.indexOf(Color);
    if (_active == -1) {
        _active = 1;
    }
    _online = (ARR_ONLINE_COLOR.indexOf(_color) != -1) || (_lstAI.indexOf(_name.toUpper()) != -1);
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

    //qDebug() << "QWarlockStat::parseIniAndInit" << Raw << Registered << Name << Ladder << Melee << Played << Won << Died << Elo << Active;

    init(Name, Registered, Ladder, Melee, Played, Won, Died, Elo, Color);
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

    //qDebug() << "QWarlockStat::parseAndInit" << Raw << Registered << Name << Ladder << Melee << Played << Won << Died << Elo << Active << Color;

    init(Name, !Registered.isEmpty(), Ladder, Melee, Played, Won, Died, Elo, Color);
}

QString QWarlockStat::toString() const {
    return QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11").arg(boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee),
                                                     intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo), intToStr(_active))
            .arg(_color, boolToIntS(online()));
}

int QWarlockStat::active() const
{
    return _active;
}

bool QWarlockStat::online() const {
    return _online;
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
