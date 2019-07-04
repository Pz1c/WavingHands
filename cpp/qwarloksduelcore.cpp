#include "qwarloksduelcore.h"

QWarloksDuelCore::QWarloksDuelCore(QObject *parent) :
    QObject(parent)
{
    _lstAI << "CONSTRUCT" << "EARTHGOLEM" << "IRONGOLEM";
    _isLogined = false;
    _isLoading = false;
    _isAI = false;
    _botIdx = 0;
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

    _isParaFDF = false;
    _isTimerActive = false;

    WarlockDictionary = QWarlockDictionary::getInstance();

    prepareSpellHtmlList();
}

QWarloksDuelCore::~QWarloksDuelCore() {
    saveParameters();
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

void QWarloksDuelCore::sendMessage(const QString &Msg) {
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/sendmess")));
    QByteArray postData;
    postData.append(QString("rcpt=%1&message=").arg(_warlockId));
    postData.append(QUrl::toPercentEncoding(Msg));

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

    postData.append("&showemail=8&icq=&aim=&website=&blurb=I+am+using+Android+application+%22"
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
        emit loginChanged();
        _isAI = _lstAI.indexOf(_login.toUpper()) != -1;
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

void QWarloksDuelCore::finishTopList(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishTopList" << StatusCode << NewUrl;
    QString list = QWarlockUtils::parseTopList(Data);
    if (_topList.compare(list) != 0) {
        _topList = list;
        qDebug() << list;
        emit topListChanged();
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

        _allowedAccept = false;
        _allowedAdd = false;
        emit registerNewUserChanged();
        saveParameters();
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

void QWarloksDuelCore::getTopList() {
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/players")));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::acceptChallenge(int battle_id, bool from_card) {
    _isLoading = true;
    emit isLoadingChanged();

    _loadedBattleID = battle_id;
    _loadedBattleType = 0;

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/accept?back=%1&num=%2").arg((from_card ? "player" : "challenges"), QString::number(_loadedBattleID))));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::rejectChallenge(int battle_id) {
    _isLoading = true;
    emit isLoadingChanged();

    _loadedBattleID = battle_id;
    _loadedBattleType = 0;

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/refuse?back=player&num=%1").arg(QString::number(_loadedBattleID))));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

void QWarloksDuelCore::deleteMsg(QString msg_from) {
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/delmess?from=%1").arg(msg_from)));

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

    qDebug() << "QWarloksDuelCore::sendOrders" << QString(postData);
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

void QWarloksDuelCore::getWarlockInfo(const QString & Login) {
    qDebug() << "getWarlockInfo" << Login;
    _isLoading = true;
    emit isLoadingChanged();

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://games.ravenblack.net/player/%1.html").arg(Login)));

    _reply = _nam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(slotReadyRead()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

bool QWarloksDuelCore::finishGetFinishedBattle(QString &Data) {
    qDebug() << "finishGetFinishedBattle" << _loadedBattleID << _loadedBattleType;
    _isParaFDF = Data.indexOf("(ParaFDF)") != -1;
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

    butifyTurnMessage();

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
        if (!_MonstersHtml.isEmpty()) {
            _MonstersHtml.append(",");
        }
        _MonstersHtml.append(m->json(_login));
    }
    _MonstersHtml.append("]").prepend("[");
    return true;
}

bool QWarloksDuelCore::prepareWarlockHtml() {
    qDebug() << "QWarloksDuelCore::prepareWarlockHtml";
    _WarlockHtml.clear();
    foreach(QWarlock *m, _Warlock) {
        bool Player = m->name().toLower().compare(_login.toLower()) == 0;
        //QString ps = SpellChecker.checkSpells(m->leftGestures(), m->rightGestures(), false, !Player);
        if (Player) {
            _leftGestures = m->leftGestures();
            _rightGestures = m->rightGestures();
            prepareSpellHtmlList(true, true);
        }
        if (!_WarlockHtml.isEmpty()) {
            _WarlockHtml.append(",");
        }
        _WarlockHtml.append(m->separatedString());
        qDebug() << "added" << m->name();
    }
    _WarlockHtml.prepend("[").append("]");
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
            if (!QWarlockUtils::parseWarlock(data, _Warlock, _errorMsg, _login.toLower())) {
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

    QWarlock *enemy = nullptr;
    foreach(QWarlock *m, _Warlock) {
        m->setIsParaFDF(_isParaFDF);
        if (m->player()) {
            m->setPossibleGestures(_possibleLeftGestures, _possibleRightGestures);
        } else {
            enemy = m;
        }
        QList<QSpell *> sl = SpellChecker.getSpellsList(m);
        m->setPossibleSpells(sl, m->player() ? enemy : nullptr, _Monsters);
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
    //_isParaFDF = QWarlockUtils::getStringFromData(Data, "<U", ">", "<").indexOf("(ParaFDF)") != -1;
    qDebug() << "QWarloksDuelCore::parseSpecReadyBattleValues" << _isParaFDF << _loadedBattleTurn;
    return _loadedBattleTurn != 0;
}

bool QWarloksDuelCore::parseReadyBattle(QString &Data) {
    if (!parseTargetList(Data)) {
        return false;
    }

    if (!QWarlockUtils::parseGestures(Data, _possibleLeftGestures, _possibleRightGestures, _errorMsg)) {
        emit errorOccurred();
        return false;
    }

    if (!parseUnits(Data)) {
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

    setTimeState(false);
    emit readyBattleChanged();

    return true;
}


void QWarloksDuelCore::setTimeState(bool State) {
    _isTimerActive = State;
    emit timerStateChanged();
}

int QWarloksDuelCore::getLoadedBattleTurn()
{
    return _loadedBattleTurn;
}

void QWarloksDuelCore::parseMessages(QString &Data) {
    int idx1 = 0, idx2;
    QString tmp;
    _msg.clear();
    while((idx1 = Data.indexOf("<TABLE><TR><TD CLASS=lightbg>", idx1)) != -1) {
        idx1 += 29;
        idx2 = Data.indexOf("</TD></TR></TABLE>", idx1);
        tmp = Data.mid(idx1, idx2 - idx1).replace("</TD></TR><TR><TD>", " ");
        _msg.append(tmp);
        idx1 = idx2;
    }
}

void QWarloksDuelCore::parseChallendge(QString &Data) {
    int idx1 = 0, idx2, idx3 = 0, idx4, battle_id;
    QString tmp, desc;
    while((idx1 = Data.indexOf("<A HREF=\"/warlocks?num=", idx1)) != -1) {
        if (idx3 == 0) {
            idx3 = Data.indexOf("</TD></TR>", idx1);
        } else if (idx1 > idx3) {
            break;
        }
        idx1 += 23;
        idx2 = Data.indexOf('"', idx1);
        tmp = Data.mid(idx1, idx2 - idx1);
        battle_id = tmp.toInt();
        idx1 = Data.indexOf("</A>. ", idx2);
        idx1 += 6;
        idx2 = Data.indexOf("<A HREF", idx1);
        desc = Data.mid(idx1, idx2 - idx1);
        idx1 = Data.indexOf("Waiting: ", idx2);
        idx2 = Data.indexOf("<A HREF=\"/warlocks?num=", idx1);
        idx4 = Data.indexOf("</TD></TR>", idx1);
        idx2 = ((idx2 != -1) && (idx2 < idx3)) ? idx2 : idx4;
        tmp = Data.mid(idx1, idx2 - idx1).replace("<BR>", " ");
        desc.append(tmp);
        idx1 = idx2 - 1;
        if (battle_id > 0) {
            _challenge[battle_id] = desc;
        }
    }
}

void QWarloksDuelCore::parsePlayerInfo(QString &Data) {
    qDebug() << "QWarloksDuelCore::parsePlayerInfo";
    _challenge.clear();
    _played = QWarlockUtils::getIntFromPlayerData(Data, "Played:", "<TD>", "</TD>");
    _won = QWarlockUtils::getIntFromPlayerData(Data, "Won:", "<TD>", "</TD>");
    _died = QWarlockUtils::getIntFromPlayerData(Data, "Died:", "<TD>", "</TD>");
    _ladder = QWarlockUtils::getIntFromPlayerData(Data, "Ladder Score:", "<TD>", "</TD>");
    _melee = QWarlockUtils::getIntFromPlayerData(Data, "Melee Score:", "<TD>", "</TD>");
    _elo = QWarlockUtils::getIntFromPlayerData(Data, "Elo:", "<TD>", "</TD>");
    _ready_in_battles = QWarlockUtils::getBattleList(Data, "Ready in battles:");
    _waiting_in_battles = QWarlockUtils::getBattleList(Data, "Waiting in battles:");
    _finished_battles = QWarlockUtils::getBattleList(Data, "Finished battles:");
    QString challendge = QWarlockUtils::getStringFromData(Data, "Challenged to battles:</TD>", "<TD>", "</TD>");
    if (!challendge.isEmpty()) {
        parseChallendge(challendge);
    }
    parseMessages(Data);

    if (!_allowedAccept && ((_finished_battles.count() > 0) || (_played > 0))) {
        _allowedAccept = true;
        _allowedAdd = true;
        emit allowedAcceptChanged();
    }
    int new_fb_id = 0;
    if ((_ready_in_battles.count() == 0) && (_finished_battles.count() > 0)) {
        QString sbid;
        foreach(int fbid, _finished_battles) {
            sbid = QString::number(fbid);
            if (_shown_battles.indexOf(sbid) == -1) {
                _shown_battles.append(sbid);
                new_fb_id = fbid;
                break;
            }
        }
    }
    qDebug() << "ready: " << _ready_in_battles;
    qDebug() << "waiting: " << _waiting_in_battles;
    qDebug() << "finished: " << _finished_battles;
    qDebug() << "shown: " << _shown_battles;
    if (_ready_in_battles.count() > 0) {
        setTimeState(false);
        getBattle(_ready_in_battles.at(0), 0);
    } else if (new_fb_id > 0) {
        setTimeState(false);
        getBattle(new_fb_id, 2);
    } else {
        setTimeState(true);
    }
    saveParameters();
}

bool QWarloksDuelCore::finishScan(QString &Data) {
    parsePlayerInfo(Data);
    if (_isAI) {
        _challengeList = "[]";
        if (_ready_in_battles.count() == 0) {
            _isLogined = false;
            if (++_botIdx >= _lstAI.count()) {
                _botIdx = 0;
            }
            _login = _lstAI.at(_botIdx);
        }
    } else {
        getChallengeList();
    }
    return true;
}

bool QWarloksDuelCore::finishScanWarlock(QString &Data) {
    qDebug() << "QWarloksDuelCore::finishScanWarlock" << Data;
    QString login = QWarlockUtils::getStringFromData(Data, "Info for", " ", " ");
    _warlockId = QWarlockUtils::getStringFromData(Data, "<INPUT TYPE=HIDDEN NAME=rcpt VALUE", "=", ">");
    int played = QWarlockUtils::getIntFromPlayerData(Data, "Played:", "<TD>", "</TD>");
    int won = QWarlockUtils::getIntFromPlayerData(Data, "Won:", "<TD>", "</TD>");
    int died = QWarlockUtils::getIntFromPlayerData(Data, "Died:", "<TD>", "</TD>");
    int ladder = QWarlockUtils::getIntFromPlayerData(Data, "Ladder Score:", "<TD>", "</TD>");
    int melee = QWarlockUtils::getIntFromPlayerData(Data, "Melee Score:", "<TD>", "</TD>");
    int elo = QWarlockUtils::getIntFromPlayerData(Data, "Elo:", "<TD>", "</TD>");
    QList<int> ready_in_battles = QWarlockUtils::getBattleList(Data, "Ready in battles:");
    QList<int> waiting_in_battles = QWarlockUtils::getBattleList(Data, "Waiting in battles:");
    QList<int> finished_battles = QWarlockUtils::getBattleList(Data, "Finished battles:");

    _warlockInfo.clear();
    int idx;
    if (ready_in_battles.count() > 0) {
        _warlockInfo.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("ReadyB")));
        idx = -1;
        foreach(int i, _ready_in_battles) {
            if (++idx > 0) {
                _warlockInfo.append(", ");
            }
            _warlockInfo.append(QString("<a href=\"/battle/%1/0\">%1</a>").arg(QString::number(i)));
        }
        _warlockInfo.append("</p>");
    }
    if (waiting_in_battles.count() > 0) {
        _warlockInfo.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("WaitB")));
        idx = -1;
        foreach(int i, _waiting_in_battles) {
            if (++idx > 0) {
                _warlockInfo.append(", ");
            }
            _warlockInfo.append(QString("<a href=\"/battle/%1/1\">%1</a>").arg(QString::number(i)));
        }
        _warlockInfo.append("</p>");
    }
    if (finished_battles.count() > 0) {
        _warlockInfo.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("FinishB")));
        idx = -1;
        foreach(int i, _finished_battles) {
            if (++idx > 0) {
                _warlockInfo.append(", ");
            }
            _warlockInfo.append(QString("<a href=\"/battle/%1/2\">%1</a>").arg(QString::number(i)));
        }
        _warlockInfo.append("</p>");
    }

    _warlockInfo.prepend(QString("<html><h3>%1</h3><table><tr><td>%2:</td><td>%3</td></tr>\n"
                        "<tr><td>%4:</td> <td>%5</td></tr>\n"
                        "<tr><td>%6:</td> <td>%7</td></tr>\n"
                        "<tr><td>%8:</td> <td>%9</td></tr>\n"
                        "<tr><td>%10:</td> <td>%11</td></tr>\n"
                        "<tr><td>%12:</td> <td>%13</td></tr>"
                        "</table>")
            .arg(login)
            .arg(WarlockDictionary->getStringByCode("Played"))
            .arg(QString::number(played))
            .arg(WarlockDictionary->getStringByCode("Won"))
            .arg(QString::number(won))
            .arg(WarlockDictionary->getStringByCode("Died"))
            .arg(QString::number(died))
            .arg(WarlockDictionary->getStringByCode("LadderScore"))
            .arg(QString::number(ladder))
            .arg(WarlockDictionary->getStringByCode("MeleeScore"))
            .arg(QString::number(melee))
            .arg(WarlockDictionary->getStringByCode("Elo"))
            .arg(QString::number(elo))
            ).append("</html>");

    emit warlockInfoChanged();
    return true;
}


void QWarloksDuelCore::saveRequest(QString &data) {
#ifdef QT_DEBUG
    QString file_name = QString("warlocksduel_%1.html").arg(QString::number(++_requestIdx));
    QFile file(file_name);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << data;
    file.close();
    qDebug() << "data stored into " << file_name;
#endif
}

bool QWarloksDuelCore::processData(QString &data, int statusCode, QString url, QString new_url) {
    if (url.indexOf("delmess?from") != -1) {
        scanState();
        return true;
    }

    if (url.indexOf("/login") != -1) {
        return finishLogin(data, statusCode, new_url);
    }

    if (url.indexOf("/players") != -1) {
        finishTopList(data, statusCode, new_url);
        return false;
    }

    if (url.indexOf("/player") != -1) {
        if (statusCode != 200) {
            _isLogined = false;
            loginToSite();
            _errorMsg = "Can't receive player info, try reconnect";
            emit errorOccurred();
            return false;
        }
        if (url.indexOf(".html") != -1) {
            finishScanWarlock(data);
        } else {
            finishScan(data);
        }
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
    _isLoading = false;
    processData(data, statusCode, url, new_url.toString());
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

QString QWarloksDuelCore::getSpellList(QString left, QString right, bool Enemy) {
    return SpellChecker.checkSpells(left, right, Enemy);
}
QString QWarloksDuelCore::getSpellBook() {
    return SpellChecker.getSpellBook(_isParaFDF);
}

void QWarloksDuelCore::setLogin(QString Login, QString Password) {
    _login = Login;
    _password = Password;
    _finished_battles.clear();
    saveParameters();
    loginToSite();
}

void QWarloksDuelCore::prepareSpellHtmlList(bool emit_signal, bool force_emit) {
    qDebug() << "QWarloksDuelCore::prepareSpellHtmlList" << "_prevGestures" << _prevGestures << "_leftGestures" << _leftGestures;
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

    QList<QSpell *> sl = SpellChecker.getPosibleSpellsList(_leftGestures, _rightGestures, WARLOCK_PLAYER, _possibleLeftGestures.indexOf("As Right") != -1 ? _possibleRightGestures : _possibleLeftGestures, _possibleRightGestures, _isParaFDF);
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
    foreach(QSpell *spell, SpellChecker.Spells) {
        qDebug() << "spell" << spell->gesture();
        if (!spell->active()) {
            continue;
        }
        found = false;
        foreach(QSpell *s, sl) {
            if (!s->possibleCast()) {
                continue;
            }
            if (s->spellID() == spell->spellID()) {

                int day_cnt = s->turnToCast();
                QString hint = s->hand() == WARLOCK_HAND_LEFT ? "Left " : "Right ";
                qDebug() << "s" << spell->gesture() << hint;
                //hint.append(" in " + spl.at(2) + " turns");
                if (spell->gesture().compare("WFP") == 0 || spell->gesture().compare("SD") == 0) {
                    qDebug() << "day_cnt" << day_cnt;
                    qDebug() << "fp" << spell->gesture().mid(0, spell->gesture().length() - day_cnt);
                    qDebug() << "sp" << spell->gesture().mid(spell->gesture().length() - day_cnt, day_cnt);
                }
                QString g = QString("<font color=green>%1</font><font color=red>%2</font>")
                        .arg(spell->gesture().mid(0, spell->gesture().length() - day_cnt), spell->gesture().mid(spell->gesture().length() - day_cnt, day_cnt));
                if (_spellListHtml.length() > 1) {
                    _spellListHtml.append(",");
                }
                _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\"}").arg(g, hint, spell->gesture(), WarlockDictionary->getStringByCode(spell->gesture())));

                found = true;
                break;
            }
        }
        if (!found) {
            if (_spellListHtml.length() > 1) {
                _spellListHtml.append(",");
            }
            _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\"}").arg(spell->gesture(), "", spell->gesture(), WarlockDictionary->getStringByCode(spell->gesture())));
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
    foreach(QSpell *spell, SpellChecker.Spells) {
        if (first) {
            first = false;
        } else {
            res.append(",");
        }
        res.append(QString("{\"g\":\"%1\",\"code\":\"%2\",\"n\":\"%3\",\"h\":\"\"}").arg(spell->gesture(), spell->gesture(), WarlockDictionary->getStringByCode(spell->gesture())));
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
    settings.setValue("allowed_add", _allowedAdd);
    settings.setValue("allowed_accept", _allowedAccept);
    settings.endGroup();

    settings.beginGroup("Proxy");
    settings.setValue("host", _proxyHost);
    settings.setValue("port", _proxyPort);
    settings.setValue("user", _proxyUser);
    settings.setValue("password", _proxyPass);
    settings.endGroup();

    settings.beginGroup("Game");
    settings.setValue("finished_battles", finishedBattles());
    settings.setValue("shown_battles", _shown_battles);
    settings.endGroup();
}

void QWarloksDuelCore::loadParameters() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, APPLICATION_NAME);
    settings.beginGroup("User");
    _login = settings.value("login", "").toString();
    _password = settings.value("password", "").toString();
    _allowedAdd = settings.value("allowed_add", "true").toBool();
    _allowedAccept = settings.value("allowed_accept", "true").toBool();
    settings.endGroup();

    settings.beginGroup("Proxy");
    _proxyHost = settings.value("host", "").toString();
    _proxyPort = settings.value("port", "").toInt();
    _proxyUser = settings.value("user", "").toString();
    _proxyPass = settings.value("password", "").toString();
    settings.endGroup();

    settings.beginGroup("Game");
    QStringList fbl = settings.value("finished_battles", "").toString().split(",");
    foreach(QString fb, fbl) {
        int fbid = fb.toInt();
        if ((fbid == 0) || (_finished_battles.indexOf(fbid) != -1)) {
            continue;
        }
        _finished_battles.append(fbid);
    }
    _shown_battles = settings.value("shown_battles", "").toStringList();
    if ((_shown_battles.isEmpty() || ((_shown_battles.size() == 1) && (_shown_battles.at(0).isEmpty()))) && !fbl.isEmpty()) {
        _shown_battles.append(fbl);
    }
    settings.endGroup();
}

bool QWarloksDuelCore::allowedAccept()
{
    return _allowedAccept;
}

QString QWarloksDuelCore::warlockInfo()
{
    return _warlockInfo;
}


bool QWarloksDuelCore::allowedAdd()
{
    return _allowedAdd;
}

void QWarloksDuelCore::applyProxySettings() {
    if (_proxyHost.isEmpty()) {
        _nam.setProxy(QNetworkProxy::NoProxy);
    } else {
        _proxy.setType(QNetworkProxy::HttpProxy);
        _proxy.setHostName(_proxyHost);
        _proxy.setPort((quint16)_proxyPort);
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

bool QWarloksDuelCore::isAI() {
    return _isAI;
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
    if (_msg.size() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("Messages")));
        foreach(QString s, _msg) {
            res.append(QString("<p>%1</p>").arg(s));
        }
    }
    if (_challenge.size() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("ChallengeB")));
        QMapIterator<int, QString> i(_challenge);
        while (i.hasNext()) {
            i.next();
            res.append(QString("<p>%1 <a href=\"/challenge/%2/accept\">Accept</a> <a href=\"/challenge/%3/reject\">Reject</a></p>").arg(i.value(), intToStr(i.key()), intToStr(i.key())));
        }
    }
    if (_ready_in_battles.count() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(WarlockDictionary->getStringByCode("ReadyB")));
        idx = -1;
        foreach(int i, _ready_in_battles) {
            if (++idx > 0) {
                res.append(", ");
            }
            res.append(QString("<a href=\"/battle/%1/0\">%1</a>").arg(QString::number(i)));
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
            res.append(QString("<a href=\"/battle/%1/1\">%1</a>").arg(QString::number(i)));
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
            res.append(QString("<a href=\"/battle/%1/2\">%1</a>").arg(QString::number(i)));
        }
        res.append("</p>");
    }

    res.prepend(QString("<html><h3>%1</h3><table><tr><td>%2:</td><td>%3</td></tr>\n"
                        "<tr><td>%4:</td> <td>%5</td></tr>\n"
                        "<tr><td>%6:</td> <td>%7</td></tr>\n"
                        "<tr><td>%8:</td> <td>%9</td></tr>\n"
                        "<tr><td>%10:</td> <td>%11</td></tr>\n"
                        "<tr><td>%12:</td> <td>%13</td></tr>"
                        "</table>")
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
            ).append("</html>");
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

int QWarloksDuelCore::isParaFDF() {
    return _isParaFDF ? 1 : 0;
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

QString QWarloksDuelCore::topList() {
    return _topList;
}

QString QWarloksDuelCore::spellListHtml() {
    //prepareSpellHtmlList(false);
    return _spellListHtml;
}
