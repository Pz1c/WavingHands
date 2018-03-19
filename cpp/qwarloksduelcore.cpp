#include "qwarloksduelcore.h"

QWarloksDuelCore::QWarloksDuelCore(QObject *parent) :
    QObject(parent)
{
    _isLogined = false;
    _isLoading = false;
    _requestIdx = 0;
    loadParameters();
    applyProxySettings();

    _played = 0;
    _won = 0;
    _died = 0;
    _ladder = 0;
    _melee = 0;
    _elo = 0;

    _loadedBattleID = 0;

    _isTimerActive = false;

    WarlockDictionary = QWarlockDictionary::getInstance();

    prepareSpellHtmlList();
}

void QWarloksDuelCore::createNewChallenge(bool Fast, bool Private, bool ParaFC, bool Maladroid, int Count, int FriendlyLevel, QString Description) {
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/newchallenge")));
    QByteArray postData;
    if (Fast) {
        postData.append("fast=1&");
    }
    if (Private) {
        postData.append("private=1&");
    }
    postData.append(QString("players=%1&friendly=%2&game=1&blurb=").arg(QString::number(Count), QString::number(FriendlyLevel)));
    QString desc = Description;
    if (ParaFC) {
        desc.prepend("ParaFC ");
    }

    if (Maladroid) {
        desc.prepend("Maladroit ");
    }

    postData.append(QUrl::toPercentEncoding(desc));

    qDebug() << QString(postData);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    _reply = _nam.post(request, postData);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::regNewUser(QString Login, QString Password, QString Email) {
    _login = Login;
    _password = Password;
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/newplayer?action=new")));
    QByteArray postData;
    postData.append("name=");
    postData.append(QUrl::toPercentEncoding(_login));
    postData.append("&password=");
    postData.append(QUrl::toPercentEncoding(_password));
    postData.append("&referrer=Galbarad&email=");
    postData.append(QUrl::toPercentEncoding(Email));

    postData.append("&showemail=8&emailinform=2&icq=&aim=&website=&blurb=I+am+using+Android+application+%22"
                    "Warlock+duel%22+too+play%0D%0A+https%3A%2F%2Fplay.google.com%2Fstore%2F"
                    "apps%2Fdetails%3Fid%3Dnet.is.games.WarlockDuel+&preferfast=4&me13=1&update=1");


    qDebug() << QString(postData);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    _reply = _nam.post(request, postData);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::loginToSite() {
    if (_login.isEmpty()) {
        emit needLogin();
        return;
    }
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/login")));
    //request.setRawHeader("Cookie", "gamename=; gamepassword=");
    QByteArray postData;
    postData.append("name=");
    postData.append(QUrl::toPercentEncoding(_login));
    postData.append("&password=");
    postData.append(QUrl::toPercentEncoding(_password));

    //qDebug() << request.rawHeaderList();
    qDebug() << "Login to site: " << QString(postData);

    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    _reply = _nam.post(request, postData);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

bool QWarloksDuelCore::finishLogin(QString &Data, int StatusCode, QUrl NewUrl) {
    if (StatusCode == 200) {
        // perhapse bad login
        if (Data.indexOf("Failed Login</TITLE>") != 0) {
            _errorMsg = "Wrong login or password";
            _isLoading = false;
            emit isLoadingChanged();
            emit needLogin();
            emit errorOccurred();
            return false;
        }
    }

    if (!NewUrl.isEmpty()) {
        _isLogined = true;
        QString url = NewUrl.toString().toLower();
        qDebug() << "login sucessfull redirect to " << url;
        if (url.indexOf("https://") == -1) {
            url.prepend("https://games.ravenblack.net/").replace("//", "/").replace(":/", "://");
        }
        qDebug() << "final url " << url;
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        _reply = _nam.get(request);

        connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
        connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
        connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
        return false;
    }
    return true;
}

void QWarloksDuelCore::finishChallengeList(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishChallengeList" << StatusCode << NewUrl;
    QString list = QWarlockUtils::parseChallengesList(Data);
    if (_challengeList.compare(list) != 0) {
        _challengeList = list;
        qDebug() << list;
        emit challengeListChanged();

        prepareSpellHtmlList();
    }
}

bool QWarloksDuelCore::finishCreateChallenge(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishCreateChallenge " << StatusCode << NewUrl;
    if (NewUrl.isEmpty()) {
        _errorMsg = "<h4>Something goes wrong, can't create challenge</h4><p>" + Data + "</p>";
        emit errorOccurred();
        return false;
    } else {
        emit challengeSubmitedChanged();
    }
    return true;
}

bool QWarloksDuelCore::finishAccept(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishAccept " << StatusCode << NewUrl;
    if (NewUrl.isEmpty()) {
        _errorMsg = "Something goes wrong, can't accept challenge";
        emit errorOccurred();
        return false;
    }
    if (Data.isEmpty()) {

    }
    if (!NewUrl.isEmpty()) {
        _isLogined = true;
        QString url = NewUrl.toString().toLower();
        qDebug() << "accept sucessfull redirect to " << url;
        if (url.indexOf("http://") == -1) {
            url.prepend("https://games.ravenblack.net/").replace("//", "/").replace(":/", "://");
        }
        qDebug() << "final url " << url;
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        _reply = _nam.get(request);

        connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
        connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
        connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
    }
    return true;
}

bool QWarloksDuelCore::finishOrderSubmit(QString &Data, int StatusCode, QUrl NewUrl) {
    if (NewUrl.isEmpty()) {
        _errorMsg = "Something goes wrong, can't send battle orders";
        emit errorOccurred();
        return false;
    }
    emit orderSubmitedChanged();
    qDebug() << "finishOrderSubmit" << StatusCode << NewUrl;
    if (Data.isEmpty()) {
        qDebug() << "Data not empty";
    }

    QString url = NewUrl.toString().toLower();
    qDebug() << "login sucessfull redirect to " << url;
    if (url.indexOf("http://") == -1) {
        url.prepend("https://games.ravenblack.net/").replace("//", "/").replace(":/", "://");
    }
    qDebug() << "final url " << url;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    _reply = _nam.get(request);

    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));

    _isLoading = true;
    emit isLoadingChanged();

    return true;
}

bool QWarloksDuelCore::finishRegistration(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishRegistration" << StatusCode << NewUrl;
    if (StatusCode == 200) {
        // perhapse bad registration
        QString search1 = "<H1><A HREF=\"/\">RavenBlack Games</A></H1>";
        QString search2 = "</TD></TR></TABLE>";
        QString search3 = "<FORM ACTION=\"newplayer?action=new\" METHOD=POST>";
        int idx1 = Data.indexOf(search1);
        if (idx1 == -1) {
            _errorMsg = "Unknown error till registration";
            emit errorOccurred();
            return false;
        }
        idx1 = Data.indexOf(search2, idx1 + search1.length());
        if (idx1 == -1) {
            _errorMsg = "Unknown error till registration.";
            emit errorOccurred();
            return false;
        }
        idx1 += search2.length();
        int idx2 = Data.indexOf(search3);
        if (idx2 == -1) {
            _errorMsg = "Unknown error, till registration.";
            emit errorOccurred();
            return false;
        }

        _errorMsg = Data.mid(idx1, idx2 - idx1);
        qDebug() << "Error: " << _errorMsg;
        emit errorOccurred();
        return false;
    }

    if (!NewUrl.isEmpty()) {
        _isLogined = true;
        QString url = NewUrl.toString().toLower();
        qDebug() << "register sucessfull redirect to " << url;
        if ((url.indexOf("https://") == -1) && (url.indexOf("http://") == -1)) {
            url.prepend("https://games.ravenblack.net/").replace("//", "/").replace(":/", "://");
        }
        qDebug() << "final url " << url;
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        _reply = _nam.get(request);

        connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
        connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
        connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));

        emit registerNewUserChanged();
        return true;
    }
    return false;
}

void QWarloksDuelCore::scanState() {
    qDebug() << "scanState";
    if (!_isLogined) {
        loginToSite();
        return;
    }

    if (_elo == 0) {
        _isLoading = true;
        emit isLoadingChanged();
    }

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/player")));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::getChallengeList() {
    /*if (_elo == 0) {
        _isLoading = true;
        emit isLoadingChanged();
    }*/

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/challenges")));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::acceptChallenge(int battle_id) {
    _isLoading = true;
    emit isLoadingChanged();

    _loadedBattleID = battle_id;
    _loadedBattleType = 0;

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/accept?back=challenges&num=%1").arg(QString::number(_loadedBattleID))));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::forceSurrender(int battle_id, int turn) {
    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/warlocksubmit")));
    QByteArray postData;
    postData.append(QString("force=1&turn=%1&num=%2").arg(QString::number(turn), QString::number(battle_id)));
    qDebug() << QString(postData);
    //return;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    _reply = _nam.post(request, postData);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::sendOrders(QString orders) {
    qDebug() << "sendOrders " << orders;
    if (orders.isEmpty()) {
        _errorMsg = "Orders can't be empty";
        emit errorOccurred();
        return;
    }

    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/warlocksubmit")));
    QByteArray postData;
    postData.append(QString("turn=%1&num=%2").arg(QString::number(_loadedBattleTurn), QString::number(_loadedBattleID)));
    QStringList sl = orders.split("#"), sl1;
    QString key, value;
    foreach(QString s, sl) {
        qDebug() << "order " << s;
        if (s.isEmpty()) {
            continue;
        }
        sl1 = s.split("$");
        if (sl1.count() != 2) {
            continue;
        }
        key = sl1.at(0);
        value = sl1.at(1);
        if (key.compare("say") == 0) {
            value = value.replace("sign:dollar", "$").replace("sign:pos_number", "#");
        }
        postData.append("&");
        postData.append(QString(QUrl::toPercentEncoding(key.replace(" ", "+"))).replace("%2B", "+"));
        postData.append("=");
        postData.append(QString(QUrl::toPercentEncoding(value.replace(" ", "+"))).replace("%2B", "+"));
    }

    qDebug() << QString(postData);
    //return;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    _reply = _nam.post(request, postData);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));

    _leftGestures = "";
    _rightGestures = "";
    prepareSpellHtmlList();
}

void QWarloksDuelCore::getBattle(int battle_id, int battle_type) {
    _loadedBattleID = battle_id;
    _loadedBattleType = battle_type;
    qDebug() << "getBattle " << _loadedBattleID << " battle_type " << _loadedBattleType;
    if (!_isLogined) {
        loginToSite();
        return;
    }
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/warlocks?num=%1&full=%2").arg(QString::number(_loadedBattleID), _loadedBattleType == 2 ? "1" : "0")));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

bool QWarloksDuelCore::finishGetFinishedBattle(QString &Data) {
    qDebug() << "finishGetFinishedBattle" << _loadedBattleID << _loadedBattleType;
    QString point1 = "<A TARGET=_blank HREF=\"/rules/1/quickref.html\">Spell Reference</A></DIV>";
    QString point2 = _loadedBattleType != 0 ? "</BODY>" : "<TABLE CELLPADDING=0 CELLSPACING=0 BORDER=0 WIDTH=\"100%\">";
    int idx1 = Data.indexOf(point1);
    if (idx1 == -1) {
        _errorMsg = "Wrong battle answer";
        emit errorOccurred();
        return false;
    }
    idx1 += point1.length();
    int idx2 = Data.indexOf(point2, idx1);
    if (idx2 == -1) {
        _errorMsg = "Wrong battle answer!";
        emit errorOccurred();
        return false;
    }
    qDebug() << "finishGetFinishedBattle all fine";
    _finishedBattle = Data.mid(idx1, idx2 - idx1).replace("<a href=\"/player", "<b atr=\"")
            .replace("<A CLASS=amonoturn HREF=\"/warlocks", "<b atr=\"").replace("</A>", "</b>").replace("</a>", "</b>")
            .replace("BLOCKQUOTE", "p").replace("WIDTH=\"100%\"", "").replace("WIDTH=\"50%\"", "");

    if (Data.indexOf("<INPUT TYPE=SUBMIT VALUE=\"Force Surrender Attempt\">") != -1) {
        QString turn = QWarlockUtils::getStringFromData(Data, "<INPUT TYPE=HIDDEN NAME=force VALUE=1>", "<INPUT TYPE=HIDDEN NAME=turn VALUE=\"", "\"");
        _finishedBattle.append(QString("<br><p align=center><a href=\"/force_surrender/%1/%2\">Force Surrender Attempt</a></p><br>").arg(QString::number(_loadedBattleID), turn));
    }

    if (_loadedBattleType != 0) {
        emit finishedBattleChanged();
        return false;
    }

    QString ReadyData = Data.mid(idx2, Data.length() - idx2);
    if (ReadyData.indexOf("<FORM METHOD=POST ACTION=\"warlocksubmit\"") == -1) {
        // mean battler finished just now
        //emit finishedBattleChanged();
        return false;
    }
    return parseReadyBattle(ReadyData);
}

bool QWarloksDuelCore::parseTargetList(QString &Data) {
    if (!QWarlockUtils::parseTargetList(Data, _Targets, _errorMsg)) {
        emit errorOccurred();
        return false;
    }
    return true;
}

bool QWarloksDuelCore::prepareMonsterHtml() {
    _MonstersHtml.clear();
    foreach(QMonster *m, _Monsters) {
        _MonstersHtml.append(m->html(_login)).append("<br>");
    }

    return true;
}

bool QWarloksDuelCore::prepareWarlockHtml() {
    _WarlockHtml.clear();
    foreach(QWarlock *m, _Warlock) {
        QString ps = SpellChecker.checkSpells(m->leftGestures(), m->rightGestures(), false, true);
        if (m->name().toLower().compare(_login.toLower()) == 0) {
            _leftGestures = m->leftGestures();
            _rightGestures = m->rightGestures();
            prepareSpellHtmlList(true, true);
            _WarlockHtml.append(m->separatedString(ps) + "#;#");
        } else {
            _WarlockHtml.prepend(m->separatedString(ps) + "#;#");
        }
    }
    //_WarlockHtml.prepend("<table>").append("</table>");
    return true;
}

bool QWarloksDuelCore::parseMonsterCommand(QString &Data) {
    QStringList ml;
    foreach(QMonster *m, _Monsters) {
        if (m->owner().toLower().compare(_login.toLower()) == 0) {
            ml.append(m->name().toLower());
        }
    }
    bool is_charm_l = _leftGestures.length() >= 3 && _leftGestures.replace(" ", "").mid(_leftGestures.length() - 3, 3).compare("PSD") == 0;
    bool is_charm_r = _rightGestures.length() >= 3 && _rightGestures.replace(" ", "").mid(_rightGestures.length() - 3, 3).compare("PSD") == 0;
    return QWarlockUtils::parseMonsterCommad(Data, _monsterCommandList, _login.toLower(), ml, is_charm_l || is_charm_r);
}

bool QWarloksDuelCore::parseUnits(QString &Data) {
    foreach(QWarlock *w, _Warlock) {
        delete w;
    }

    foreach(QMonster *m, _Monsters) {
        delete m;
    }
    _Warlock.clear();
    _Monsters.clear();

    QString search1 = "<TABLE CELLPADDING=0 CELLSPACING=0 BORDER=0 WIDTH=\"100%\">";
    QString search2 = "</TABLE>";
    int idx1 = 0, idx2;
    while((idx1 = Data.indexOf(search1, idx1)) != -1) {
        idx1 += search1.length();
        idx2 = Data.indexOf(search2, idx1);
        QString data = Data.mid(idx1, idx2 - idx1);
        if (data.indexOf("/player") != -1) {
            if (!QWarlockUtils::parseWarlock(data, _Warlock, _errorMsg)) {
                emit errorOccurred();
                return false;
            }
        } else {
            if (!QWarlockUtils::parseMonster(data, _Monsters, _errorMsg)) {
                emit errorOccurred();
                return false;
            }
        }

        idx1 = idx2 + search2.length();
    }

    if (!prepareMonsterHtml()) {
        return false;
    }

    if (!prepareWarlockHtml()) {
        return false;
    }

    return true;
}

bool QWarloksDuelCore::butifyTurnMessage() {
    _finishedBattle = _finishedBattle.replace("<H2>", "").replace("</H2>", "").replace("<p>", "").replace("</p>", "")
            .replace("#FFFF88", "#F5C88E").replace("#88FFFF", "#54EBEB").replace("#88FF88", "#79D979");
    return true;
}

bool QWarloksDuelCore::parseSpecReadyBattleValues(QString &Data) {
    _charmPersonList = QWarlockUtils::getCharmedPersonList(Data);
    _paralyzeList = QWarlockUtils::getParalyseList(Data);
    _loadedBattleTurn = QWarlockUtils::getIntFromPlayerData(Data, "<INPUT TYPE=HIDDEN NAME=turn", "VALUE=", ">");
    _fire = QWarlockUtils::getStringFromData(Data, "<INPUT TYPE=CHECKBOX CLASS=check NAME=FIRE VALUE=1", ">", "<");
    _isDelay = Data.indexOf("<INPUT TYPE=RADIO CLASS=check NAME=DELAY") != -1;
    _isPermanent = Data.indexOf("<INPUT TYPE=RADIO CLASS=check NAME=PERM") != -1;
    return _loadedBattleTurn != 0;
}

bool QWarloksDuelCore::parseReadyBattle(QString &Data) {
    if (!parseTargetList(Data)) {
        return false;
    }

    if (!parseUnits(Data)) {
        return false;
    }

    if (!QWarlockUtils::parseGestures(Data, _possibleLeftGestures, _possibleRightGestures, _errorMsg)) {
        emit errorOccurred();
        return false;
    }

    if (!parseSpecReadyBattleValues(Data)) {
        _errorMsg = "Can't found current battle turn";
        emit errorOccurred();
        return false;
    }

    if (!butifyTurnMessage()) {
        return false;
    }

    if (!parseMonsterCommand(Data)) {
        return false;
    }

    prepareSpellHtmlList();

    _isTimerActive = false;
    emit timerStateChanged();
    emit readyBattleChanged();

    return true;
}


void QWarloksDuelCore::parsePlayerInfo(QString &Data) {
    _played = QWarlockUtils::getIntFromPlayerData(Data, "Played:", "<TD>", "</TD>");
    _won = QWarlockUtils::getIntFromPlayerData(Data, "Won:", "<TD>", "</TD>");
    _died = QWarlockUtils::getIntFromPlayerData(Data, "Died:", "<TD>", "</TD>");
    _ladder = QWarlockUtils::getIntFromPlayerData(Data, "Ladder Score:", "<TD>", "</TD>");
    _melee = QWarlockUtils::getIntFromPlayerData(Data, "Melee Score:", "<TD>", "</TD>");
    _elo = QWarlockUtils::getIntFromPlayerData(Data, "Elo:", "<TD>", "</TD>");
    _ready_in_battles = QWarlockUtils::getBattleList(Data, "Ready in battles:");
    _waiting_in_battles = QWarlockUtils::getBattleList(Data, "Waiting in battles:");
    _finished_battles = QWarlockUtils::getBattleList(Data, "Finished battles:");
    qDebug() << "ready: " << _ready_in_battles;
    qDebug() << "waiting: " << _waiting_in_battles;
    qDebug() << "finished: " << _finished_battles;
    if (_ready_in_battles.count() > 0) {
        _isTimerActive = false;
        emit timerStateChanged();
        getBattle(_ready_in_battles.at(0), 0);
    } else {
        _isTimerActive = true;
        emit timerStateChanged();
    }
}

bool QWarloksDuelCore::finishScan(QString &Data, int StatusCode) {
    if (StatusCode != 200) {
        // somethisn wrong try relogin
        _isLogined = false;
        loginToSite();
        _errorMsg = "Can't receive player info, try reconnect";
        emit errorOccurred();
        return false;
    }
    getChallengeList();
    parsePlayerInfo(Data);
    return true;
}


void QWarloksDuelCore::saveRequest(QString &data) {
#ifdef QT_DEBUG
    QString file_name = QString("marlocksduel_%1.html").arg(QString::number(++_requestIdx));
    QFile file(file_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << data;
    file.close();
    qDebug() << "data stored into " << file_name;
#endif
}

bool QWarloksDuelCore::processData(QString &data, int statusCode, QString url, QString new_url) {
    if (url.indexOf("/login") != -1) {
        return finishLogin(data, statusCode, new_url);
    }

    if (url.indexOf("/player") != -1) {
        finishScan(data, statusCode);
        return true;
    }

    if (url.indexOf("/warlocksubmit") != -1) {
        return !finishOrderSubmit(data, statusCode, new_url);
    }

    if (url.indexOf("/warlocks") != -1) {
        return !finishGetFinishedBattle(data);
    }

    if (url.indexOf("/newchallenge") != -1) {
        return !finishCreateChallenge(data, statusCode, new_url);
    }

    if (url.indexOf("/challenges") != -1) {
        finishChallengeList(data, statusCode, new_url);
        return false;
    }

    if (url.indexOf("/accept") != -1) {
        return finishAccept(data, statusCode, new_url);
    }

    if (url.indexOf("/newplayer") != -1) {
        return !finishRegistration(data, statusCode, new_url);
    }

    return true;
}

void QWarloksDuelCore::slotReadyRead() {
    QNetworkReply *reply = (QNetworkReply *)sender();

    if (reply->error() != QNetworkReply::NoError) {
        _isLoading = false;
        emit isLoadingChanged();
        return;
    }
    QString url = reply->url().toString();
    QString data = reply->readAll();
    qDebug() << "from url: " << url << " slotReadyRead";
    if (!data.isEmpty()) {
        saveRequest(data);
    }

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QUrl new_url;
    switch(statusCode)  {
        case 301:
        case 302:
        case 307:
            new_url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            break;
    }

    processData(data, statusCode, url, new_url.toString());
    _isLoading = false;
    emit isLoadingChanged();
}

void QWarloksDuelCore::slotError(QNetworkReply::NetworkError error) {
    _isLoading = false;
    emit isLoadingChanged();
    _errorMsg = "Network problem details: " + _reply->errorString();
    emit errorOccurred();
    qDebug() << "slotError" << error << _reply->errorString();
}

void QWarloksDuelCore::slotSslErrors(QList<QSslError> error_list) {
    _isLoading = false;
    emit isLoadingChanged();
    _errorMsg = "Sll error details: \n";
    foreach(QSslError err, error_list) {
        _errorMsg.append(err.errorString());
        _errorMsg.append("\n");
    }

    emit errorOccurred();
    qDebug() << "slotSslErrors " << error_list;
}

QString QWarloksDuelCore::getSpellList(QString left, QString right, int strikt) {
    return SpellChecker.checkSpells(left, right, strikt == 1);
}

void QWarloksDuelCore::setLogin(QString Login, QString Password) {
    _login = Login;
    _password = Password;
    saveParameters();
    loginToSite();
}

void QWarloksDuelCore::prepareSpellHtmlList(bool emit_signal, bool force_emit) {
    qDebug() << "_prevGestures" << _prevGestures << "_leftGestures" << _leftGestures;
    if (_prevGestures.compare(_leftGestures) == 0 && !_spellListHtml.isEmpty()) {
        qDebug() << "prepareSpellHtmlList exit point 1";
        if (force_emit) {
            qDebug() << "emit spellListHtmlChanged() 0";
            emit spellListHtmlChanged();
        }
        return;
    }
    _prevGestures = _leftGestures;

    if (_leftGestures.isEmpty()) {
        _spellListHtml.clear();
        if (emit_signal) {
            qDebug() << "emit spellListHtmlChanged() 2";
            emit spellListHtmlChanged();
        }
        qDebug() << "prepareSpellHtmlList exit point 2";
        return;
    }

    QStringList sl = SpellChecker.getPosibleSpellsList(_leftGestures, _rightGestures, false, true);
    if (sl.count() == 0) {
        _spellListHtml.clear();
        if (emit_signal) {
            qDebug() << "emit spellListHtmlChanged() 3";
            emit spellListHtmlChanged();
        }
        qDebug() << "prepareSpellHtmlList exit point 3";
        return;
    }
    _spellListHtml = "[";

    bool found;
    foreach(QValueName spell, SpellChecker.Spells) {
        found = false;
        foreach(QString s, sl) {
            if (s.indexOf(";" + spell.first + ";") != -1) {
                QStringList spl = s.split(";");
                int day_cnt = spl.at(2).toInt();
                QString hint = spl.at(0).compare("L") == 0 ? "Left " : "Right ";
                //hint.append(" in " + spl.at(2) + " turns");
                if (spell.first.compare("WFP") == 0 || spell.first.compare("SD") == 0) {
                    qDebug() << "day_cnt" << day_cnt;
                    qDebug() << "fp" << spell.first.mid(0, spell.first.length() - day_cnt);
                    qDebug() << "sp" << spell.first.mid(spell.first.length() - day_cnt, day_cnt);
                }
                QString g = QString("<font color=green>%1</font><font color=red>%2</font>")
                        .arg(spell.first.mid(0, spell.first.length() - day_cnt), spell.first.mid(spell.first.length() - day_cnt, day_cnt));
                if (_spellListHtml.length() > 1) {
                    _spellListHtml.append(",");
                }
                _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\"}").arg(g, hint, spell.first, WarlockDictionary->getStringByCode(spell.first)));

                found = true;
                break;
            }
        }
        if (!found) {
            if (_spellListHtml.length() > 1) {
                _spellListHtml.append(",");
            }
            _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\"}").arg(spell.first, "", spell.first, WarlockDictionary->getStringByCode(spell.first)));
        }
    }
    _spellListHtml.append("]");
    if (emit_signal) {
        qDebug() << "emit spellListHtmlChanged() 4";
        emit spellListHtmlChanged();
    }
    qDebug() << "prepareSpellHtmlList exit point 4";
}

QString QWarloksDuelCore::defaultSpellListHtml() {
    QString res = "[";
    bool first = true;
    foreach(QValueName spell, SpellChecker.Spells) {
        if (first) {
            first = false;
        } else {
            res.append(",");
        }
        res.append(QString("{\"g\":\"%1\",\"code\":\"%2\",\"n\":\"%3\",\"h\":\"\"}").arg(spell.first, spell.first, WarlockDictionary->getStringByCode(spell.first)));
    }
    res.append("]");
    return res;
    //return "<table><tr><th>Gestures</th><th>Spell name</th></tr><tr><td>cDPW</td><td>Dispel Magic</td></tr><tr><td>cSWWS</td><td>Summon Ice Elemental</td></tr><tr><td>cWSSW</td><td>Summon Fire Elemental</td></tr><tr><td>cw</td><td>Magic Mirror</td></tr><tr><td>DFFDD</td><td>Lightning Bolt</td></tr><tr><td>DFPW</td><td>Cure Heavy Wounds</td></tr><tr><td>DFW</td><td>Cure Light Wounds</td></tr><tr><td>DFWFd</td><td>Blindness</td></tr><tr><td>DPP</td><td>Amnesia</td></tr><tr><td>DSF</td><td>Confusion/Maladroitness</td></tr><tr><td>DSFFFc</td><td>Disease</td></tr><tr><td>DWFFd</td><td>Blindness</td></tr><tr><td>DWSSSP</td><td>Delay Effect</td></tr><tr><td>DWWFWD</td><td>Poison</td></tr><tr><td>FFF</td><td>Paralysis</td></tr><tr><td>WFPSFW</td><td>Summon Giant</td></tr><tr><td>FPSFW</td><td>Summon Troll</td></tr><tr><td>PSFW</td><td>Summon Ogre</td></tr><tr><td>SFW</td><td>Summon Goblin</td></tr><tr><td>FSSDD</td><td>Fireball</td></tr><tr><td>P</td><td>Shield</td></tr><tr><td>p</td><td>Surrender</td></tr><tr><td>PDWP</td><td>Remove Enchantment</td></tr><tr><td>PPws</td><td>Invisibility</td></tr><tr><td>PSDD</td><td>Charm Monster</td></tr><tr><td>PSDF</td><td>Charm Person</td></tr><tr><td>PWPFSSSD</td><td>Finger of Death</td></tr><tr><td>PWPWWc</td><td>Haste</td></tr><tr><td>SD</td><td>Magic Missile</td></tr><tr><td>SPFP</td><td>Anti-spell</td></tr><tr><td>SPFPSDW</td><td>Permanency</td></tr><tr><td>SPPc</td><td>Time Stop</td></tr><tr><td>SPPFD</td><td>Time Stop</td></tr><tr><td>SSFP</td><td>Resist Cold</td></tr><tr><td>SWD</td><td>Fear (No CFDS)</td></tr><tr><td>SWWc</td><td>Fire Storm</td></tr><tr><td>WDDc</td><td>Clap of Lightning</td></tr><tr><td>WFP</td><td>Cause Light Wounds</td></tr><tr><td>WPFD</td><td>Cause Heavy Wounds</td></tr><tr><td>WPP</td><td>Counter Spell</td></tr><tr><td>WSSc</td><td>Ice Storm</td></tr><tr><td>WWFP</td><td>Resist Heat</td></tr><tr><td>WWP</td><td>Protection</td></tr><tr><td>WWS</td><td>Counter Spell</td></tr></table>";
}

void QWarloksDuelCore::setProxySettings(QString IP, int Port, QString Username, QString Password) {
    _proxyHost = IP;
    _proxyPort = Port;
    _proxyUser = Username;
    _proxyPass = Password;
    saveParameters();
    loginToSite();
}

void QWarloksDuelCore::saveParameters() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("User");
    settings.setValue("login", _login);
    settings.setValue("password", _password);
    settings.endGroup();

    settings.beginGroup("Proxy");
    settings.setValue("host", _proxyHost);
    settings.setValue("port", _proxyPort);
    settings.setValue("user", _proxyUser);
    settings.setValue("password", _proxyPass);
    settings.endGroup();
}

void QWarloksDuelCore::loadParameters() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("User");
    _login = settings.value("login", "").toString();
    _password = settings.value("password", "").toString();
    settings.endGroup();

    settings.beginGroup("Proxy");
    _proxyHost = settings.value("host", "").toString();
    _proxyPort = settings.value("port", "").toInt();
    _proxyUser = settings.value("user", "").toString();
    _proxyPass = settings.value("password", "").toString();
    settings.endGroup();
}

void QWarloksDuelCore::applyProxySettings() {
    if (_proxyHost.isEmpty()) {
        _nam.setProxy(QNetworkProxy::NoProxy);
    } else {
        _proxy.setType(QNetworkProxy::HttpProxy);
        _proxy.setHostName(_proxyHost);
        _proxy.setPort(_proxyPort);
        if (!_proxyUser.isEmpty()) {
            _proxy.setUser(_proxyUser);
        }
        if (!_proxyPass.isEmpty()) {
            _proxy.setPassword(_proxyPass);
        }
        _nam.setProxy(_proxy);
    }
}

bool QWarloksDuelCore::isNeedLogin() {
    return !_isLogined;
}

QString QWarloksDuelCore::login() {
    return _login;
}

QString QWarloksDuelCore::password() {
    return _password;
}

QString QWarloksDuelCore::errorMsg() {
    return _errorMsg;
}

QString QWarloksDuelCore::playerInfo() {
    QString res;
    int idx;
    if (_ready_in_battles.count() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("ReadyB")));
        idx = -1;
        foreach(int i, _ready_in_battles) {
            if (++idx > 0) {
                res.append(", ");
            }
            res.append(QString("<a href=\"/ready_in_battle/%1/0\">%1</a>").arg(QString::number(i)));
        }
        res.append("</p>");
    }
    if (_waiting_in_battles.count() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("WaitB")));
        idx = -1;
        foreach(int i, _waiting_in_battles) {
            if (++idx > 0) {
                res.append(", ");
            }
            res.append(QString("<a href=\"/waiting_in_battle/%1/1\">%1</a>").arg(QString::number(i)));
        }
        res.append("</p>");
    }
    if (_finished_battles.count() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("FinishB")));
        idx = -1;
        foreach(int i, _finished_battles) {
            if (++idx > 0) {
                res.append(", ");
            }
            res.append(QString("<a href=\"/finished_battle/%1/2\">%1</a>").arg(QString::number(i)));
        }
        res.append("</p>");
    }

    res.prepend(QString("<html><h3>%1</h3><table><tr><td>%2:</td><td>%3</td></tr>\n"
                        "<tr><td>%4:</td> <td>%5</td></tr>\n"
                        "<tr><td>%6:</td> <td>%7</td></tr>\n"
                        "<tr><td>%8:</td> <td>%9</td></tr>\n"
                        "<tr><td>%10:</td> <td>%11</td></tr>\n"
                        "<tr><td>%12:</td> <td>%13</td></tr>"
                        "</table></html>")
            .arg(_login)
            .arg(WarlockDictionary->getStringByCode("Played"))
            .arg(QString::number(_played))
            .arg(WarlockDictionary->getStringByCode("Won"))
            .arg(QString::number(_won))
            .arg(WarlockDictionary->getStringByCode("Died"))
            .arg(QString::number(_died))
            .arg(WarlockDictionary->getStringByCode("LadderScore"))
            .arg(QString::number(_ladder))
            .arg(WarlockDictionary->getStringByCode("MeleeScore"))
            .arg(QString::number(_melee))
            .arg(WarlockDictionary->getStringByCode("Elo"))
            .arg(QString::number(_elo))
            );
    return res;
}

QString QWarloksDuelCore::readyInBattles() {
    QString res;
    foreach(int i, _ready_in_battles) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        res.append(QString::number(i));
    }
    return res;
}

QString QWarloksDuelCore::waitingInBattles() {
    QString res;
    foreach(int i, _waiting_in_battles) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        res.append(QString::number(i));
    }
    return res;
}

QString QWarloksDuelCore::finishedBattles() {
    QString res;
    foreach(int i, _finished_battles) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        res.append(QString::number(i));
    }
    return res;
}

QString QWarloksDuelCore::finishedBattle() {
    return _finishedBattle;
}

QString QWarloksDuelCore::proxyHost() {
    return _proxyHost;
}

int QWarloksDuelCore::proxyPort() {
    return _proxyPort;
}

QString QWarloksDuelCore::proxyUser() {
    return _proxyUser;
}

QString QWarloksDuelCore::proxyPass() {
    return _proxyPass;
}

int QWarloksDuelCore::isLoading() {
    return _isLoading ? 1 : 0;
}

int QWarloksDuelCore::loadedBattleID() {
    return _loadedBattleID;
}

QString QWarloksDuelCore::targets() {
    QString res;
    foreach(QValueName target, _Targets) {
        res.append(QString("%1;%2#").arg(target.first, target.second));
    }
    return res;
}

QString QWarloksDuelCore::warlocks() {
    qDebug() << "_WarlockHtml " << _WarlockHtml;
    return _WarlockHtml;
}

QString QWarloksDuelCore::monsters() {
    qDebug() << "_MonstersHtml " << _MonstersHtml;
    return _MonstersHtml;
}

QString QWarloksDuelCore::leftGesture() {
    return _possibleLeftGestures;
}

QString QWarloksDuelCore::rightGesture() {
    return _possibleRightGestures;
}

int QWarloksDuelCore::readyBattle() {
    return _loadedBattleType == 0 ? _loadedBattleID : 0;
}

int QWarloksDuelCore::registerNewUser() {
    return 0;
}

QString QWarloksDuelCore::monsterCommandList() {
    return _monsterCommandList;
}

QString QWarloksDuelCore::leftHand() {
    return _leftGestures;
}

QString QWarloksDuelCore::rightHand() {
    return _rightGestures;
}

int QWarloksDuelCore::orderSubmited() {
    return 0;
}

QString QWarloksDuelCore::charmPerson() {
    return _charmPersonList;
}

QString QWarloksDuelCore::paralyze() {
    return _paralyzeList;
}

int QWarloksDuelCore::timerState() {
    return _isTimerActive ? 1 : 0;
}

int QWarloksDuelCore::isDelay() {
    return _isDelay ? 1 : 0;
}

int QWarloksDuelCore::isPermanent() {
    return _isPermanent ? 1 : 0;
}

QString QWarloksDuelCore::fire() {
    return _fire;
}

int QWarloksDuelCore::challengeSubmited() {
    return 1;
}

QString QWarloksDuelCore::challengeList() {
    return _challengeList;
}

QString QWarloksDuelCore::spellListHtml() {
    //prepareSpellHtmlList(false);
    return _spellListHtml;
}
