#include "qwarloksduelcore.h"


QWarloksDuelCore::QWarloksDuelCore(QObject *parent, bool AsService) :
    QGameCore(parent)
{
    _isAsService = AsService;
    if (_isAsService) {
        _request_code = "ass";
    }
    QString ini_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    qDebug() << "QWarloksDuelCore::QWarloksDuelCore" << ini_path;
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, ini_path);
    setOrganization(ORGANIZATION_NAME, APPLICATION_NAME);
    init();
    GameDictionary->setCurrentLang("en");
    SpellChecker = QWarlockSpellChecker::getInstance();
    _timerCount = 0;
    _timerInterval = 0;
    connect(&_timer, SIGNAL(timeout()), this, SLOT(timerFired()));
    connect(&_serviceTimer, SIGNAL(timeout()), this, SLOT(processServiceTimer()));
    _serviceTimer.setInterval(10000);
    _serviceTimer.start();

    //_lstAI << "CONSTRUCT" << "EARTHGOLEM" << "IRONGOLEM";
    _isLogined = false;
    _isAI = false;
    _botIdx = 0;
    _isScanForced = false;

    _played = 0;
    _won = 0;
    _died = 0;
    _ladder = 0;
    _melee = 0;
    _elo = 0;
    //_feedback = true;
    //_rateus = true;

    _loadedBattleID = 0;

    _isParaFDF = false;
    _isParaFC = false;
    _isTimerActive = false;
    _isMaladroit = false;

    _nam.setRedirectPolicy(QNetworkRequest::ManualRedirectPolicy);

    prepareSpellHtmlList();
}

QWarloksDuelCore::~QWarloksDuelCore() {
    saveParameters(true, true, true, true, true);

    // clean up
    QMap<int, QBattleInfo *>::iterator bii;
    for (bii = _battleInfo.begin(); bii != _battleInfo.end(); ++bii) {
        delete bii.value();
    }
}

void QWarloksDuelCore::aiCreateNewChallenge() {
    int wib_cnt = _waiting_in_battles.count();
    int rib_cnt = _ready_in_battles.count();
    if (wib_cnt + rib_cnt < 5) {
        createNewChallenge(true, false, true, true, 2, 2, "Training Battle with AI Player, NO BOT allowed");
    } else {
        aiLogin();
    }
}

void QWarloksDuelCore::createNewChallenge(bool Fast, bool Private, bool ParaFC, bool Maladroid, int Count, int FriendlyLevel, QString Description, QString Warlock) {
    qDebug() << "QWarloksDuelCore::createNewChallenge" << Fast << Private << ParaFC << Maladroid << Count << FriendlyLevel << Description << Warlock;
    //Q_UNUSED(Fast);
    setIsLoading(true);

    // QNetworkRequest request;
    // request.setUrl(QUrl(QString(GAME_SERVER_URL_NEW_CHALLENGE)));
    QString p_data = "fast=1&";
    if (Private) {
        p_data.append("private=1&");
    }
    p_data.append(QString("players=%1&friendly=%2&game=1&blurb=").arg(QString::number(Count), QString::number(FriendlyLevel)));
    QString desc = Description;
    if (ParaFC) {
        desc.prepend("ParaFC ");
    }
    if (Maladroid) {
        desc.prepend("Maladroit ");
    }
    p_data.append(QUrl::toPercentEncoding(desc));
    if (!Warlock.isEmpty() && _playerStats.contains(Warlock.toLower())) {
        _inviteToBattle = Warlock;
    } else {
        _inviteToBattle.clear();
    }

    qDebug() << p_data;
    sendPostRequest(GAME_SERVER_URL_NEW_CHALLENGE, p_data.toUtf8());
    setTimeState(true);
}

void QWarloksDuelCore::sendMessage(const QString &Msg) {
    setIsLoading(true);

    QString postData;
    postData.append(QString("rcpt=%1&message=").arg(_warlockId));
    postData.append(QUrl::toPercentEncoding(Msg));

    qDebug() << QString(postData);
    sendPostRequest(GAME_SERVER_URL_SENDMESS, postData.toUtf8());
}

void QWarloksDuelCore::regNewUser(const QString &Login, const QString &Email, const QString &Pass) {
    _login = Login;
    _password = Pass.isEmpty() ? QGameUtils::rand(10) : Pass;
    setIsLoading(true);

    QString postData;
    postData.append("name=");
    postData.append(QUrl::toPercentEncoding(_login));
    postData.append("&password=");
    postData.append(QUrl::toPercentEncoding(_password));
    //postData.append("&referrer=Galbarad&email=");
    //postData.append(QUrl::toPercentEncoding(Email));

    postData.append("&icq=&aim=&website=&blurb=I+am+using+Android+application+%22"
                    "Warlock+duel%22+too+play+net.is.games.WarlockDuel&preferfast=4&me13=1&update=1");


    qDebug() << postData << Email;
    sendPostRequest(GAME_SERVER_URL_NEW_PLAYER, postData.toUtf8());
}

void QWarloksDuelCore::loginToSite() {
    if (_login.isEmpty()) {
        emit needLogin();
        return;
    }

    setIsLoading(true);

    QString postData;
    postData.append("name=");
    postData.append(QUrl::toPercentEncoding(_login));
    postData.append("&password=");
    postData.append(QUrl::toPercentEncoding(_password));

    qDebug() << "Login to site: " << postData;

    _waiting_in_battles.clear();
    _ready_in_battles.clear();
    sendPostRequest(GAME_SERVER_URL_LOGIN, postData.toUtf8());
}

void QWarloksDuelCore::processNewLogin() {
    // check is exists
    for(int i = 0, Ln = _accounts.size(); i < Ln; ++i) {
        if (_accounts.at(i).first.compare(_login.toLower()) == 0) {
            return;
        }
    }

    _accounts.append(QValueName(_login.toLower(), _password));
    emit accountMenuChanged();
}

QString QWarloksDuelCore::accountToString() {
    QString result;
    for(int i = 0, Ln = _accounts.size(); i < Ln; ++i) {
        result.append(QString("%1&%2&&").arg(_accounts.at(i).first, _accounts.at(i).second));
    }
    return result;
}

void QWarloksDuelCore::accountsFromString(QString acc) {
    //qDebug() << "QWarloksDuelCore::accountsFromString" << acc;
    _accounts.clear();
    QStringList sl1 = acc.split("&&"), sl2;
    foreach(QString s, sl1) {
        sl2 = s.split("&");
        //qDebug() << "QWarloksDuelCore::accountsFromString" << sl2;
        if (sl2.size() == 2) {
            _accounts.append(QValueName(sl2.at(0), sl2.at(1)));
            //qDebug() << "QWarloksDuelCore::accountsFromString" << sl2.at(0) << sl2.at(1);
        }
    }
    if (_accounts.size() > 1) {
        emit accountMenuChanged();
    }
}

QString QWarloksDuelCore::accountMenu() {
    QString result = "{\"lst\":[";
    for(int i = 0, Ln = _accounts.size(); i < Ln; ++i) {
        result.append(QString("\"%1\",").arg(_accounts.at(i).first));
    }
    result.append("\" \"]}");
    return result;
}

void QWarloksDuelCore::autoLogin(int Idx) {
    if ((Idx < 0) || (Idx >= _accounts.size())) {
        return;
    }

    QValueName lp = _accounts.at(Idx);
    setLogin(lp.first, lp.second);
}

bool QWarloksDuelCore::finishLogin(QString &Data, int StatusCode, QUrl NewUrl) {
    if (StatusCode == 200) {
        // perhapse bad login
        if (Data.indexOf("Failed Login</TITLE>") != 0) {
            _errorMsg = "Wrong login or password";
            setIsLoading(false);
            emit needLogin();
            emit errorOccurred();
            return false;
        }
    }

    if (!NewUrl.isEmpty()) {
        _isLogined = true;
        emit loginChanged();
        processNewLogin();
        _isAI = _lstAI.indexOf(_login.toUpper()) != -1;
        QString url = NewUrl.toString().toLower();
        qDebug() << "login sucessfull redirect to " << url;
        if (url.indexOf("https://") == -1) {
            url.prepend(GAME_SERVER_URL).replace("//", "/").replace(":/", "://");
        }
        qDebug() << "final url " << url;
        setCheckUrl(QString(GAME_SERVER_URL_GET_PROFILE).arg(_login));
        sendGetRequest(url);
        processRefferer();
        saveParameters(true, true, true, true, true);
        return false;
    }
    return true;
}

void QWarloksDuelCore::aiLogin() {
    qDebug() << "QWarloksDuelCore::aiLogin" << _isAI << _ready_in_battles.count() << _lstAI.count() << _botIdx;

    if (!_isAI) {
        return;
    }

    if (_ready_in_battles.count() == 0) {
        _isLogined = false;
        if (++_botIdx >= _lstAI.count()) {
            _botIdx = 0;
        }
        _login = _lstAI.at(_botIdx);
        //_challengeList.clear();
    }
}

void QWarloksDuelCore::finishChallengeList(QString &Data, int StatusCode, QUrl NewUrl) {
    QList<QBattleInfo*> list = QWarlockUtils::parseChallengesList(Data);
    qDebug() << "finishChallengeList" << StatusCode << NewUrl;// << _challengeList << list;
    QString list_str;
    foreach(QBattleInfo*bi, list) {
        list_str.append(intToStr(bi->battleID())).append(";");
        if (_battleInfo.contains(bi->battleID())) {
            delete _battleInfo[bi->battleID()];
            _battleInfo.remove(bi->battleID());
        }
        _battleInfo.insert(bi->battleID(), bi);
    }

    if (_challengeList.compare(list_str) != 0) {
        _challengeList = list_str;
        qDebug() << list;
        emit challengeListChanged();
    } else if (_isAI) {
        aiLogin();
    }
}

void QWarloksDuelCore::finishTopList(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishTopList" << StatusCode << NewUrl;
    int pos1 = Data.lastIndexOf("<TD CLASS=darkbg>"), pos2;
    while((pos1 = Data.indexOf("<TR>", pos1)) != -1) {
        pos2 = Data.indexOf("</TR>", pos1);
        QWarlockStat ws(Data.mid(pos1, pos2 - pos1));
        _playerStats[ws.name().toLower()] = ws;
        qDebug() << "QWarloksDuelCore::finishTopList" << ws.toString();
        pos1 = pos2 + 4;
    }

    //QString list = QWarlockUtils::parseTopList(Data);
    /*if (_topList.compare(list) != 0) {
        _topList = list;
        qDebug() << list;
        emit topListChanged();
    }*/
}

bool QWarloksDuelCore::finishCreateChallenge(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishCreateChallenge " << StatusCode << NewUrl << Data << _inviteToBattle;
    if (NewUrl.isEmpty()) {
        _errorMsg = "{\"type\":10}";
        emit errorOccurred();
        return false;
    }

    QString url = NewUrl.toString();
    int pos = 0;
    QString created_id = QWarlockUtils::getStringFromData(url, "num", "=", "&", pos);
    int bid = created_id.toInt();
    if (bid > 0) {
        QBattleInfo *battle_info = getBattleInfo(bid);
        battle_info->addParticipant(_login);
        battle_info->setStatus(BATTLE_INFO_STATUS_NO_START);

        if (!_inviteToBattle.isEmpty() && !created_id.isEmpty()) {
           sendGetRequest(QString(GAME_SERVER_URL_INVITE_TO_CHALLENGE).arg(_inviteToBattle, created_id));
        }
    }
    return true;
}

bool QWarloksDuelCore::finishAccept(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishAccept " << StatusCode << NewUrl;
    if (NewUrl.isEmpty()) {
        //bool battle_is_full = Data.indexOf("That battle is full") != 1;
        bool is_type_10 = Data.indexOf("Unregistered players may not be in more than 5 games at once") != -1;//&& !battle_is_full;
        _errorMsg = QString("{\"type\":%1,\"d\":\"%2\"}").arg(is_type_10 ? "10" : "11", is_type_10 ? "" : Data);
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
            url.prepend(GAME_SERVER_URL).replace("//", "/").replace(":/", "://");
        }
        qDebug() << "final url " << url;
        sendGetRequest(url);
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
    qDebug() << "finishOrderSubmit sucessfull redirect to " << url;
    if (url.indexOf("http://") == -1) {
        url.prepend(GAME_SERVER_URL).replace("//", "/").replace(":/", "://");
    }
    qDebug() << "final url " << url;
    sendGetRequest(url);
    setIsLoading(true);

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
            url.prepend(GAME_SERVER_URL).replace("//", "/").replace(":/", "://");
        }
        qDebug() << "final url " << url;
        sendGetRequest(url);
        _allowedAccept = false;
        _allowedAdd = false;
        _reg_in_app = true;
        _show_hint = true;
        emit registerNewUserChanged();
        saveParameters(false, false, true);
        return true;
    }
    return false;
}

void QWarloksDuelCore::scanState(bool Silent) {
    qDebug() << "scanState";
    if (!_isLogined) {
        loginToSite();
        return;
    }

    if (!Silent) {
        _isScanForced = true;
        setIsLoading(true);
    }

    sendGetRequest(GAME_SERVER_URL_PLAYER);
}

void QWarloksDuelCore::getChallengeList(bool Silent) {
    if (!Silent) {
        setIsLoading(true);
    }

    sendGetRequest(GAME_SERVER_URL_CHALLENGES);
}

void QWarloksDuelCore::getTopList() {
    setIsLoading(true);
    qint64 udt = QDateTime::currentSecsSinceEpoch();
    sendGetRequest(QString(GAME_SERVER_URL_PLAYERS).arg((udt - _lastPlayersScan > (3 * 24 * 60 - 15) * 60) ? '1' : '0'));
    _lastPlayersScan = udt;
}

void QWarloksDuelCore::aiAcceptChallenge(int battle_id) {
    if (_waiting_in_battles.count() + _ready_in_battles.count() < 5) {
        acceptChallenge(battle_id, false);
    } else {
        aiLogin();
    }
}

void QWarloksDuelCore::leaveBattle(int battle_id) {
    setIsLoading(true);
    sendGetRequest(QString(GAME_SERVER_URL_LEAVE_GAME).arg(intToStr(battle_id)));
}

void QWarloksDuelCore::acceptChallenge(int battle_id, bool from_card) {
    qDebug() << "QWarloksDuelCore::acceptChallenge" << battle_id << from_card;
    setIsLoading(true);

    _loadedBattleID = battle_id;
    _loadedBattleType = 0;

    sendGetRequest(QString(GAME_SERVER_URL_ACCEPT_CHALLENGE).arg((from_card ? "player" : "challenges"), QString::number(_loadedBattleID)));
}

void QWarloksDuelCore::rejectChallenge(int battle_id) {
    setIsLoading(true);

    _loadedBattleID = battle_id;
    _loadedBattleType = 0;

    sendGetRequest(QString(GAME_SERVER_URL_REFUSE_CHALLENGE).arg(QString::number(_loadedBattleID)));
}

void QWarloksDuelCore::deleteMsg(QString msg_from) {
    setIsLoading(true);

    sendGetRequest(QString(GAME_SERVER_URL_DELLMESS).arg(msg_from));
}

void QWarloksDuelCore::forceSurrender(int battle_id, int turn) {
    _loadedBattleID = battle_id;
    _loadedBattleType = 1;
    QString postData;
    postData.append(QString("force=1&turn=%1&num=%2").arg(QString::number(turn), QString::number(battle_id)));
    qDebug() << postData;
    sendPostRequest(GAME_SERVER_URL_SUBMIT, postData.toUtf8());
}

void QWarloksDuelCore::sendOrders(QString orders) {
    qDebug() << "sendOrders " << orders;
    if (orders.isEmpty()) {
        _errorMsg = "Orders can't be empty";
        emit errorOccurred();
        return;
    }
    setIsLoading(true);

    QString postData;
    postData.append(_extraOrderInfo);
    //postData.append(QString("turn=%1&num=%2").arg(QString::number(_loadedBattleTurn), QString::number(_loadedBattleID)));
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

    qDebug() << "QWarloksDuelCore::sendOrders" << postData;
    sendPostRequest(GAME_SERVER_URL_SUBMIT, postData.toUtf8());
    _leftGestures = "";
    _rightGestures = "";
    //prepareSpellHtmlList();
}

void QWarloksDuelCore::getBattle(int battle_id, int battle_type) {
    _loadedBattleID = battle_id;
    _loadedBattleType = battle_type;
    qDebug() << "getBattle " << _loadedBattleID << " battle_type " << _loadedBattleType << _isLogined;
    if (!_isLogined) {
        loginToSite();
        return;
    }
    setIsLoading(true);

    sendGetRequest(QString(_loadedBattleType == 2 ? GAME_SERVER_URL_GET_FINISHED_BATTLE : GAME_SERVER_URL_GET_BATTLE).arg(QString::number(_loadedBattleID)));
}

void QWarloksDuelCore::getWarlockInfo(const QString & Login) {
    qDebug() << "getWarlockInfo" << Login;
    setIsLoading(true);

    sendGetRequest(QString(GAME_SERVER_URL_GET_PROFILE).arg(Login));
}

QBattleInfo *QWarloksDuelCore::getBattleInfo(int battleId) {
    if (!_battleInfo.contains(battleId)) {
        _battleInfo.insert(battleId, new QBattleInfo());
        _battleInfo[battleId]->setBattleID(battleId);
        _battleInfo[battleId]->addParticipant(_login);
    }
    return _battleInfo[battleId];
}

int QWarloksDuelCore::parseBattleDescription(QString &Data) {
    int idx_action = Data.indexOf("<FORM METHOD=POST ACTION=\"warlocksubmit\" OnSubmit=");
    qDebug() << "parseBattleDescription" << _loadedBattleID << _loadedBattleType << idx_action;
    if ((_loadedBattleType == 2) && (Data.indexOf(QString("Battle %1 does not exist.").arg(intToStr(_loadedBattleID))) != -1)) {
        return -2;
    }
    int prev_battle_type = _loadedBattleType;

    if ((_loadedBattleType == 0) && (idx_action != -1)) {
        _loadedBattleType = 1;
    } else if ((_loadedBattleType == 1) && (idx_action == -1)) {
        _loadedBattleType = 2;
    }

    int res = _loadedBattleType;
    QBattleInfo *battleInfo = getBattleInfo(_loadedBattleID);
    QString search_key = "<a href=\"/player";
    if (_loadedBattleType == 0) {
        int idx = Data.indexOf("has not yet begun.");
        if (idx != -1) {
            // unstarted
            res = -1;
            search_key = "<A HREF=\"/player";
            int last_found = 0;
            int need = QWarlockUtils::getIntFromPlayerData(Data, "Battle for", " ", "<HR>", last_found);
            battleInfo->setSize(need);
        } else {
            // wait
            res = 0;
        }
    } else if (_loadedBattleType == 1) {
        // ready
        res = 1;
    } else if ((_loadedBattleType == 2) && (prev_battle_type != 2)) {
        // finished
        res = 2;
        QString winner = QWarlockUtils::getFinishedBattleDescription(Data, _login);
        battleInfo->setWinner(winner);
    }
    int idx = 0;
    while(idx != -1) {
        QString wl = QWarlockUtils::getStringFromData(Data, search_key, "/", ".html", idx);
        if (!wl.isEmpty()) {
            battleInfo->addParticipant(wl);
        }
    }

    qDebug() << res;
    battleInfo->setStatus(res);
    return res;
}

bool QWarloksDuelCore::finishGetFinishedBattle(QString &Data) {
    qDebug() << "finishGetFinishedBattle" << _loadedBattleID << _loadedBattleType;
    int old_state = _loadedBattleType;
    _loadedBattleType = parseBattleDescription(Data);
    QBattleInfo *battleInfo = getBattleInfo(_loadedBattleID);
    if (_loadedBattleType == -2) { // finished but deleted
        if (_finished_battles.indexOf(_loadedBattleID) != -1) {
            _finished_battles.removeAt(_finished_battles.indexOf(_loadedBattleID));
        }
        /*if (_shown_battles.indexOf(intToStr(_loadedBattleID)) != -1) {
            _shown_battles.removeAt(_shown_battles.indexOf(intToStr(_loadedBattleID)));
        }*/
        _finishedBattle = "Sorry, but you battle already deleted from game server and we not store it on archive server";
        emit finishedBattleChanged();
        return false;
    } else if (_loadedBattleType == -1) { // unstarted
        _finishedBattle = QString("{\"type\":12,\"d\":\"%1\",\"id\":%2}").arg(battleInfo->getInListDescription(_login.toLower()), intToStr(_loadedBattleID));
        emit finishedBattleChanged();
        return false;
    }

    if ((old_state == 1) && (_loadedBattleType == 2)) {
        //QString d = _battleDesc[_loadedBattleID].toUpper();
        qDebug() << "finishGetFinishedBattle" << "check rate us" << _finished_battles.count();
        if (!_rateus && battleInfo->isWinner(_login)) {
            bool vsbot = battleInfo->withBot();
            if (!vsbot || (_finished_battles.count() >= 7)) {
                _rateus = true;
                _errorMsg = "{\"type\":14,\"id\":-1,\"action\":\"rate_us\"}";
                emit errorOccurred();
            }
        }
        // to get full history, but not only last turn
        getBattle(_loadedBattleID, 2);
        scanState(true);
        return false;
    }

    int ccnt = 0, idxc = 0;
    while((idxc = Data.indexOf(" says ", idxc)) != -1) {
        idxc += 6;
        ++ccnt;
    }
    _chat = intToStr(ccnt);

    _isParaFDF = Data.indexOf("(ParaFDF)") != -1;
    _isParaFC = Data.indexOf("(ParaFC)") != -1;
    _isMaladroit = Data.indexOf("(Maladroit)") != -1;
    QString point1 = "<A TARGET=_blank HREF=\"/rules/1/quickref.html\">Spell Reference</A></DIV>";
    QString point2 = _loadedBattleType != 1 ? "</BODY>" : "<FORM METHOD=POST ACTION=\"warlocksubmit\" OnSubmit=";
    int idx1 = Data.indexOf(point1);
    if (idx1 == -1) {
        if (_loadedBattleType == 1) {
            _loadedBattleType = 2; // battle already finish
            point2 = "</BODY>";
            idx1 = Data.indexOf(point1);
        }
        if (idx1 == -1) {
            _errorMsg = "Wrong battle answer!";
            emit errorOccurred();
            return false;
        }
    }
    idx1 += point1.length();
    int idx2 = Data.indexOf(point2, idx1);
    if (idx2 == -1) {
        _errorMsg = "Wrong battle answer!!";
        emit errorOccurred();
        return false;
    }

    qDebug() << "finishGetFinishedBattle all fine" << _loadedBattleType << point2 << idx1 << idx2;
    _finishedBattle = Data.mid(idx1, idx2 - idx1).replace("<a href=\"/player", "<b atr=\"")
            .replace("<A CLASS=amonoturn HREF=\"/warlocks", "<b atr=\"").replace("</A>", "</b>").replace("</a>", "</b>")
            .replace("BLOCKQUOTE", "p").replace("WIDTH=\"100%\"", "").replace("WIDTH=\"50%\"", "");

    int Pos = 0;
    QString ForceSurrenderTurn = "0";
    if (Data.indexOf("<INPUT TYPE=SUBMIT VALUE=\"Force Surrender Attempt\">") != -1) {
        ForceSurrenderTurn = QWarlockUtils::getStringFromData(Data, "<INPUT TYPE=HIDDEN NAME=force VALUE=1>", "<INPUT TYPE=HIDDEN NAME=turn VALUE=\"", "\"", Pos);
        _finishedBattle = _finishedBattle.mid(0, _finishedBattle.indexOf("<FORM ACTION=") - 1);
        if (_isAI) {
            forceSurrender(_loadedBattleID, ForceSurrenderTurn.toInt());
            return false;
        }
        //_finishedBattle.append(QString("<br><p align=center><a href=\"/force_surrender/%1/%2\">Force Surrender Attempt</a></p><br>").arg(QString::number(_loadedBattleID), turn));
    }

    butifyTurnMessage(_finishedBattle, _loadedBattleType == 1);


    if (_loadedBattleType != 1) {
        qDebug() << "battle is not ready end there" << _loadedBattleType;
        int idx = _finishedBattle.indexOf("Your orders are in for this turn.");
        if (idx != -1) {
            idx += 33;
            _finishedBattle = _finishedBattle.mid(idx, _finishedBattle.length() - idx).replace("<FONT COLOR=\"#AAAAAA\">", "").replace("</FONT>", "")
                    .replace("<BR><BR>", "").replace("\n", " ").replace("RH:", "Right hand:").replace("LH:", "Left hand:");
            _finishedBattle = QString("{\"type\":8,\"d\":\"%1\",\"fst\":%2,\"id\":%3}").arg(_finishedBattle, ForceSurrenderTurn, intToStr(_loadedBattleID));
        } else if (_loadedBattleType == -1) {
            _finishedBattle = QString("{\"type\":12,\"d\":\"%1\",\"id\":%2}").arg(battleInfo->getInListDescription(_login.toLower()), intToStr(_loadedBattleID));
        } else if (_loadedBattleType == 2) {
            _finishedBattle = _finishedBattle/*.replace('"', "''")*/.replace("\n", " ");
            _finishedBattle = QWarlockUtils::parseBattleHistory(_finishedBattle, battleInfo->getInListDescription(_login.toLower()), _loadedBattleID);
        }
        emit finishedBattleChanged();
        return false;
    }

    int idx3 = Data.indexOf("<TABLE CELLPADDING=0 CELLSPACING=0 BORDER=0 WIDTH=\"100%\">", idx1);
    if (idx3 == -1) {
        _errorMsg = "Wrong battle answer!!!";
        emit errorOccurred();
        return false;
    }

    QString ReadyData = Data.mid(idx3, Data.length() - idx3);
    if (ReadyData.indexOf("<FORM METHOD=POST ACTION=\"warlocksubmit\"") == -1) {
        // mean battle finished just now
        _errorMsg = "Looks like battle already finished, please wait for refresh";
        qDebug() << _errorMsg << ReadyData;
        emit errorOccurred();
        return false;
    }
    bool res = parseReadyBattle(ReadyData);
    if (res) {
        setPossibleSpell(Data);

        calcBattleDecision();

        if (!prepareMonsterHtml()) {
            return false;
        }

        if (!prepareWarlockHtml()) {
            return false;
        }

        prepareBattleChatAndHistory(Data);

        setTimeState(false);
        emit readyBattleChanged();
    }
    return res;
}

void QWarloksDuelCore::prepareBattleChatAndHistory(QString &Data) {
    qDebug() << "QWarloksDuelCore::prepareBattleChatAndHistory" << _loadedBattleTurn << _loadedBattleID;// << _battleChat[_loadedBattleID].size() << _battleHistory[_loadedBattleID].size();
    int cidx1 = Data.indexOf("</U></H2><BR><BLOCKQUOTE>");
    if (cidx1 == -1) {
        return;
    }
    cidx1 += 25;
    int cidx2 = Data.indexOf("</BLOCKQUOTE>", cidx1);
    if (cidx2 == -1) {
        return;
    }

    QString turn_hist = Data.mid(cidx1, cidx2 - cidx1).replace("\n", " ");
    butifyTurnMessage(turn_hist);

    //qDebug() << "QWarloksDuelCore::prepareBattleChatAndHistory" << turn_hist;
    QString chat_msg;
    QStringList slC = turn_hist.split("<BR>");
    QString short_hist;
    foreach(QString ss, slC) {
        if (ss.indexOf(" says ") != -1) {
            if (!chat_msg.isEmpty()) {
                chat_msg.append("<br>");
            }
            chat_msg.append(ss);
        } else {
            if (!short_hist.isEmpty()) {
                short_hist.append("<br>");
            }
            short_hist.append(ss);
        }
    }

    QBattleInfo *battleInfo = getBattleInfo(_loadedBattleID);
    battleInfo->addChat(_loadedBattleTurn, chat_msg);
    battleInfo->addHistory(_loadedBattleTurn, short_hist);
}

void QWarloksDuelCore::setPossibleSpell(const QString &Data) {
    _player = nullptr;
    _enemy = nullptr;
    foreach(QWarlock *w, _Warlock) {
        if (w->player()) {
            _player = w;
        } else if (!_enemy) {
            _enemy = w;
        }
        w->setIsParaFDF(_isParaFDF);
        w->setIsParaFC(_isParaFC);
        w->setIsMaladroit(_isMaladroit);
        if ((w->paralized() > 0) && (Data.indexOf(QString("%1's left hand is paralysed.").arg(w->name())) != -1)) {
            w->setParalyzedHand(WARLOCK_HAND_LEFT);
        }
        if ((w->paralized() > 0) && (Data.indexOf(QString("%1's right hand is paralysed.").arg(w->name())) != -1)) {
            w->setParalyzedHand(WARLOCK_HAND_RIGHT);
        }
        if (_WarlockID.contains(w->name())) {
            w->setId(_WarlockID[w->name()]);
        }
        w->setPossibleSpells(SpellChecker->getSpellsList(w));
    }

    //_enemy->setPossibleSpells(SpellChecker->getSpellsList(_enemy, false), _player, _Monsters);
    //_player->setPossibleSpells(SpellChecker->getSpellsList(_player, false));
}

// AI that calculate best gestures hidden there
void QWarloksDuelCore::calcBattleDecision() {
    if (!_player || !_enemy) {
        return;
    }
    //try {
        //_enemy->processDecision(_player, _Monsters);
        _player->processDecision(_enemy, _Monsters, _paralyzeList, _charmPersonList);
    //} catch(const std::exception& e) {
    //    qDebug() << "QWarloksDuelCore::calcBattleDecision" << e.what();
    //}

}

bool QWarloksDuelCore::parseTargetList(QString &Data) {
    if (!QWarlockUtils::parseTargetList(Data, _Targets, _errorMsg)) {
        emit errorOccurred();
        return false;
    }
    //foreach(QValueName vn, _Targets) {
    for(int i = 0, Ln = _Targets.size(); i < Ln; ++i) {
        _WarlockID[_Targets.at(i).second] = _Targets.at(i).first;
    }
    qDebug() << "QWarloksDuelCore::parseTargetList" << _Targets << _WarlockID;
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
        bool Player = m->name().compare(_login, Qt::CaseInsensitive) == 0;
        //QString ps = SpellChecker.checkSpells(m->leftGestures(), m->rightGestures(), false, !Player);
        if (Player) {
            _leftGestures = m->leftGestures();
            _rightGestures = m->rightGestures();
            //prepareSpellHtmlList(true, true);
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
    return QWarlockUtils::parseMonsterCommad(Data, _Monsters);
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

    return true;
}

bool QWarloksDuelCore::butifyTurnMessage(QString &str, bool CleanGestures) {
    str = str.replace("<H2>", "").replace("</H2>", "").replace("<p>", "").replace("</p>", "")
            .replace("#FFFF88", "#F5C88E").replace("#88FFFF", "#54EBEB").replace("#88FF88", "#79D979");
    int idx = str.indexOf("<FONT CLASS=monoturn>");
    if (CleanGestures && (idx != -1)) {
        QString part2 = str.mid(idx).replace("<FONT CLASS=monoturn>Turn:</FONT>", "<!--");
        part2 = part2.replace("<FONT CLASS=monoturn>", "<!--").replace("</FONT>", "-->");
        str = str.mid(0, idx).append(part2);
    }
    return true;
}

bool QWarloksDuelCore::parseSpecReadyBattleValues(QString &Data) {
    _charmPersonList = QWarlockUtils::getCharmedPersonList(Data);
    _paralyzeList = QWarlockUtils::getParalyseList(Data);
    int Pos = 0;
    _loadedBattleTurn = QWarlockUtils::getIntFromPlayerData(Data, "<INPUT TYPE=HIDDEN NAME=turn", "VALUE=", ">", Pos, true);
    _fire = QWarlockUtils::getStringFromData(Data, "<INPUT TYPE=CHECKBOX CLASS=check NAME=FIRE VALUE=1", ">", "<", Pos, true);
    _isDelay = Data.indexOf("<INPUT TYPE=RADIO CLASS=check NAME=DELAY") != -1;
    _isPermanent = Data.indexOf("<INPUT TYPE=RADIO CLASS=check NAME=PERM") != -1;
    //_isParaFDF = QWarlockUtils::getStringFromData(Data, "<U", ">", "<").indexOf("(ParaFDF)") != -1;
    _extraOrderInfo.clear();
    _paralyzedHands.clear();
    int idx1 = 0, idx2, idx3, idx4;
    while((idx1 = Data.indexOf("<INPUT TYPE=HIDDEN NAME=", idx1)) != -1) {
        idx1 += 24;
        idx2 = Data.indexOf(" ", idx1);
        QString par_name = Data.mid(idx1, idx2 - idx1).replace('"', "");
        idx3 = Data.indexOf("VALUE=", idx2) + 6;
        idx4 = Data.indexOf(">", idx3);
        QString par_val = Data.mid(idx3, idx4 - idx3).replace('"', "");
        qDebug() << "found hidden value" << par_name << par_val;
        idx1 = idx4;
        if (!_extraOrderInfo.isEmpty()) {
            _extraOrderInfo.append("&");
        }
        _extraOrderInfo.append(QString("%1=%2").arg(par_name, par_val));
        if (par_name.indexOf("PARALYZE") == 0) {
            if (!_paralyzedHands.isEmpty()) {
                _paralyzedHands.append(",");
            }
            _paralyzedHands.append(QString("[%1,\"%2\"]").arg(par_name.mid(8), par_val));
        }
    }
    _paralyzedHands.append("]").prepend("[");
    qDebug() << "QWarloksDuelCore::parseSpecReadyBattleValues" << _isParaFDF << _loadedBattleTurn << _extraOrderInfo << _paralyzedHands;

    return _loadedBattleTurn != 0;
}

bool QWarloksDuelCore::parseReadyBattle(QString &Data) {
    qDebug() << "QWarloksDuelCore::parseReadyBattle";
    if (!parseTargetList(Data)) {
        _errorMsg = "Can't parse targets";
        emit errorOccurred();
        return false;
    }

    if (!QWarlockUtils::parseGestures(Data, _possibleLeftGestures, _possibleRightGestures, _errorMsg)) {
        _errorMsg = "Can't parse gestures";
        emit errorOccurred();
        return false;
    }

    if (!parseUnits(Data)) {
        _errorMsg = "Can't parse units";
        emit errorOccurred();
        return false;
    }

    if (!parseSpecReadyBattleValues(Data)) {
        _errorMsg = "Can't found current battle turn";
        emit errorOccurred();
        return false;
    }

    if (!parseMonsterCommand(Data)) {
        _errorMsg = "Can't parse monsters command";
        emit errorOccurred();
        return false;
    }

    //prepareSpellHtmlList();

    return true;
}


void QWarloksDuelCore::setTimeState(bool State) {
    //timer.s
    _isTimerActive = State;
    if ((_isTimerActive && _timer.isActive()) || !_isTimerActive) {
        _timer.stop();
    }
    if (_isTimerActive) {
        _timer.start(_isAI ? 30000 : 60000);
    }
    //emit timerStateChanged();
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
    qDebug() << "QWarloksDuelCore::parseChallendge" << Data;
    QWarlockUtils::parsePersonalChallenge(Data, _challenge);
    qDebug() << "QWarloksDuelCore::parseChallendge" << _challenge;
}

void QWarloksDuelCore::generateBattleList() {
    _battleList = "[[" + _challenge.join(",");
    bool first = _challenge.isEmpty();
    QBattleInfo *battle_info;
    QString d;
    int bid, i;
    for(i = _ready_in_battles.size() - 1; i >= 0; --i) {
        bid = _ready_in_battles.at(i);
        if (first) {
            first = false;
        } else {
            _battleList.append(",");
        }
        battle_info = getBattleInfo(bid);
        battle_info->setStatus(BATTLE_INFO_STATUS_READY);
        _battleList.append(QString("{\"id\":%1,\"s\":1,\"d\":\"%2\"}").arg(intToStr(bid), battle_info->getInListDescription(_login)));
    }
    QString wait_str = "";
    for(i = _waiting_in_battles.size()- 1; i >= 0; --i) {
        bid = _waiting_in_battles.at(i);
        battle_info = getBattleInfo(bid);
        if (battle_info->status() != BATTLE_INFO_STATUS_NO_START) {
            battle_info->setStatus(BATTLE_INFO_STATUS_WAIT);
        }
        d = battle_info->getInListDescription(_login);//QWarlockUtils::getBattleShortTitle(_battleDesc[bid], _battleState[bid], bid);
        if (battle_info->status() == 0) {
            if (first) {
                first = false;
            } else {
                _battleList.append(",");
            }
            _battleList.append(QString("{\"id\":%1,\"s\":0,\"d\":\"%3\"}").arg(intToStr(bid), d));
        } else {
           if (!wait_str.isEmpty()) {
               wait_str.append(",");
           }
           wait_str.append(QString("{\"id\":%1,\"s\":-1,\"d\":\"%3\"}").arg(intToStr(bid), d));
        }
    }
    if (!wait_str.isEmpty()) {
        if (!first) {
            _battleList.append(",");
        }
        _battleList.append(wait_str);
    }

    _battleList.append("],[");
    first = true;
    for(i = _finished_battles.size() - 1; i >= 0; --i) {
        bid = _finished_battles.at(i);
        if (first) {
            first = false;
        } else {
            _battleList.append(",");
        }
        battle_info = getBattleInfo(bid);
        if (battle_info->status() != BATTLE_INFO_STATUS_DELETED) {
            battle_info->setStatus(BATTLE_INFO_STATUS_FINISHED);
        }
        d = battle_info->getInListDescription(_login);
        _battleList.append(QString("{\"id\":%1,\"s\":2,\"d\":\"%2\"}").arg(intToStr(bid), d));
    }
    _battleList.append("]]");
    emit battleListChanged();
}

void QWarloksDuelCore::parsePlayerInfo(QString &Data, bool ForceBattleList) {
    qDebug() << "QWarloksDuelCore::parsePlayerInfo" << _login;
    QList<int> old_read(_ready_in_battles), old_wait(_waiting_in_battles), old_fin(_finished_battles);
    _challenge.clear();
    int Idx = 0;
    _played = QWarlockUtils::getIntFromPlayerData(Data, "Played:", "<TD>", "</TD>", Idx);
    _won = QWarlockUtils::getIntFromPlayerData(Data, "Won:", "<TD>", "</TD>", Idx);
    _died = QWarlockUtils::getIntFromPlayerData(Data, "Died:", "<TD>", "</TD>", Idx);
    _ladder = QWarlockUtils::getIntFromPlayerData(Data, "Ladder Score:", "<TD>", "</TD>", Idx);
    _melee = QWarlockUtils::getIntFromPlayerData(Data, "Melee Score:", "<TD>", "</TD>", Idx);
    _elo = QWarlockUtils::getIntFromPlayerData(Data, "Elo:", "<TD>", "</TD>", Idx);
    _ready_in_battles = QWarlockUtils::getBattleList(Data, "Ready in battles:");
    _waiting_in_battles = QWarlockUtils::getBattleList(Data, "Waiting in battles:");
    _finished_battles = QWarlockUtils::getBattleList(Data, "Finished battles:");
    Idx = 0;
    QString challendge = QWarlockUtils::getStringFromData(Data, "Challenged to battles:</TD>", "<TD>", "</TD>", Idx);
    if (!challendge.isEmpty()) {
        parseChallendge(challendge);
    }
    parseMessages(Data);

    if (_isAsService) {
        if (_ready_in_battles.count() > 0) {
            showNotification("Your turn, open the game list");
        }
        if (!challendge.isEmpty()) {
            showNotification("You got an invite, see invitation");
        }

        return;
    }

    if (!_allowedAccept && ((_finished_battles.count() > 0) || (_played > 0))) {
        _allowedAccept = true;
        _allowedAdd = true;
        emit allowedAcceptChanged();
    }

    qDebug() << "ready: " << _ready_in_battles;
    qDebug() << "waiting: " << _waiting_in_battles;
    qDebug() << "finished: " << _finished_battles;
    //qDebug() << "shown: " << _shown_battles;
    emit playerInfoChanged();
    if (_ready_in_battles.count() > 0) {
        setTimeState(false);
        if (_isAI) {
            getBattle(_ready_in_battles.at(0), 0);
        }
    } else /* if (new_fb_id > 0) {
        setTimeState(false);
        getBattle(new_fb_id, 2);
    }  else */ {
        setTimeState(true);
    }
    qDebug() << old_read <<  _ready_in_battles << old_wait << _waiting_in_battles;// << new_fb_id;
    bool changed = _ready_in_battles.size() > 0;
    if (_isScanForced) {
        changed = true;
        _isScanForced = false;
    }
    QBattleInfo *battle_info;
    //if (!changed) {
        foreach(int bid, _ready_in_battles) {
            battle_info = getBattleInfo(bid);
            battle_info->setStatus(BATTLE_INFO_STATUS_READY);
            //battle_info->setWaitFrom(-1);
            //_battleWait.remove(bid);
            //_battleState[bid] = 1;
            if (!changed && (old_read.indexOf(bid) == -1)) {
                changed = true;
                //break;
            }
        }
    //}
    if (!changed) {
        foreach(int bid, old_read) {
            if (_ready_in_battles.indexOf(bid) == -1) {
                changed = true;
                break;
            }
        }
    }
    //if (!changed) {
        int curre_time_spec = static_cast<int>(QDateTime::currentSecsSinceEpoch() - SECONDS_AT_20210901);
        foreach(int bid, _waiting_in_battles) {
            battle_info = getBattleInfo(bid);
            if (battle_info->status() == BATTLE_INFO_STATUS_READY) {
                battle_info->setStatus(BATTLE_INFO_STATUS_WAIT);
            }
            if (!changed && (old_wait.indexOf(bid) == -1)) {
                changed = true;
                //break;
            }

            if (_isAI && (_ready_in_battles.size() == 0) && battle_info->canForceSurrendering()) {
                battle_info->setWaitFrom(battle_info->wait_from() + 30 * 60);
                forceSurrender(bid, battle_info->turn());
            }
        }
    //}
    if (!changed) {
        foreach(int bid, old_wait) {
            if (_waiting_in_battles.indexOf(bid) == -1) {
                changed = true;
                break;
            }
        }
    }
    if (!changed) {
        foreach(int bid, _finished_battles) {
            if (old_fin.indexOf(bid) == -1) {
                changed = true;
                break;
            }
        }
    }
    foreach(int bid, _finished_battles) {
        battle_info = getBattleInfo(bid);
        if (battle_info->status() < BATTLE_INFO_STATUS_FINISHED) {
            battle_info->setStatus(BATTLE_INFO_STATUS_FINISHED);
        }
    }

    // try to find refferrer
    processRefferer();

    if (changed || ForceBattleList) {
        generateBattleList();
    }
    saveParameters();
}

void QWarloksDuelCore::processRefferer() {
    if (_isAsService) {
        return;
    }
    QString reff;
    #ifdef Q_OS_ANDROID
    QJniObject val = QJniObject::fromString("Try to get reffereerrre");
    QJniObject string = QJniObject::callStaticObjectMethod("org/qtproject/example/androidnotifier/NotificationClient", "get_refferer",
                                                           "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                                                           QNativeInterface::QAndroidApplication::context(), val.object<jstring>());
    reff = string.toString();
    #endif
    qDebug() << "CheckReffereef" << reff;
    if (reff.isEmpty()) {
        return;
    }
    QStringList sl = reff.split(",");
    if (sl.size() != 3) {
        return;
    }
    if (sl.at(0).compare("create_battle") == 0) {
        int bfl = sl.at(1).indexOf("vf") != -1 ? 2 : 1;
        createNewChallenge(true, true, true, true, 2, bfl, "Join to fun)", sl.at(2));
    } else if (sl.at(0).compare("show_battle") == 0) {
        int type = sl.at(2).toInt();
        if (type > 1) {
            // 2 ready, 3 finished
            getBattle(sl.at(1).toInt(), type - 1);
        }
    }
}

bool QWarloksDuelCore::finishScan(QString &Data, bool ForceBattleList) {
    parsePlayerInfo(Data, ForceBattleList);
    if (!ForceBattleList) {
        getChallengeList();
    }
    if (!_isAI && !_isAsService) {
        if (QDateTime::currentSecsSinceEpoch() - _lastPlayersScan > 10 * 60) {
            getTopList();
        }
    }
    return true;
}

bool QWarloksDuelCore::finishScanWarlock(QString &Data) {
    qDebug() << "QWarloksDuelCore::finishScanWarlock" << Data;
    int pos = 0;
    QString login = QWarlockUtils::getStringFromData(Data, "Info for", " ", " ", pos);
    _warlockId = QWarlockUtils::getStringFromData(Data, "<INPUT TYPE=HIDDEN NAME=rcpt VALUE", "=", ">", pos);
    int played = QWarlockUtils::getIntFromPlayerData(Data, "Played:", "<TD>", "</TD>", pos);
    int won = QWarlockUtils::getIntFromPlayerData(Data, "Won:", "<TD>", "</TD>", pos);
    int died = QWarlockUtils::getIntFromPlayerData(Data, "Died:", "<TD>", "</TD>", pos);
    int ladder = QWarlockUtils::getIntFromPlayerData(Data, "Ladder Score:", "<TD>", "</TD>", pos);
    int melee = QWarlockUtils::getIntFromPlayerData(Data, "Melee Score:", "<TD>", "</TD>", pos);
    int elo = QWarlockUtils::getIntFromPlayerData(Data, "Elo:", "<TD>", "</TD>", pos);
    QList<int> ready_in_battles = QWarlockUtils::getBattleList(Data, "Ready in battles:");
    QList<int> waiting_in_battles = QWarlockUtils::getBattleList(Data, "Waiting in battles:");
    QList<int> finished_battles = QWarlockUtils::getBattleList(Data, "Finished battles:");

    _warlockInfo.clear();
    int idx;
    if (ready_in_battles.count() > 0) {
        _warlockInfo.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("ReadyB")));
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
        _warlockInfo.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("WaitB")));
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
        _warlockInfo.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("FinishB")));
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
            .arg(login, GameDictionary->getStringByCode("Played"), QString::number(played), GameDictionary->getStringByCode("Won"), QString::number(won),
                 GameDictionary->getStringByCode("Died"), QString::number(died), GameDictionary->getStringByCode("LadderScore"), QString::number(ladder))
            .arg(GameDictionary->getStringByCode("MeleeScore"), QString::number(melee), GameDictionary->getStringByCode("Elo"), QString::number(elo))
            ).append("</html>");

    emit warlockInfoChanged();
    return true;
}

bool QWarloksDuelCore::processData(QString &data, int statusCode, QString url, QString new_url) {
    qDebug() << "QWarloksDuelCore::processData" << statusCode << url << new_url;

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
            finishScan(data, (url.indexOf("mess=Orders") != -1));
        }
        return true;
    }

    if (url.indexOf("/warlocksubmit") != -1) {
        return !finishOrderSubmit(data, statusCode, new_url);
    }

    if ((url.indexOf("/warlocks") != -1) || (url.indexOf("/inf/spellcaster/") != -1) || (url.indexOf("robot_gateway/wh/") != -1)) {
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

void QWarloksDuelCore::setCredentialToAuth(QAuthenticator *authenticator) {
    if (!_proxyUser.isEmpty()) {
        authenticator->setUser(_proxyUser);
    }
    if (!_proxyPass.isEmpty()) {
        authenticator->setPassword(_proxyPass);
    }
}

void QWarloksDuelCore::onAuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator) {
    qDebug() << "QWarloksDuelCore::onAuthenticationRequired" << reply->url();
    setCredentialToAuth(authenticator);
}

void QWarloksDuelCore::onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator) {
    qDebug() << "QWarloksDuelCore::onProxyAuthenticationRequired" << proxy.hostName();
    setCredentialToAuth(authenticator);
}

void QWarloksDuelCore::slotReadyRead() {
    //QGameCore::slotReadyRead();
    QNetworkReply *reply = (QNetworkReply *)sender();
    QString url = reply->url().toString();
    QString data = reply->readAll();
    _httpResponceCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    saveRequest(data);

    QUrl new_url;
    if ((_httpResponceCode >= 500) && (_httpResponceCode < 600))  {
        QTimer::singleShot(5000, this, SLOT(resendLastRequest));
        return;
    } else if ((_httpResponceCode >= 300) && (_httpResponceCode < 400))  {
        new_url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    }

    if (!new_url.isEmpty() && (url.indexOf("/logout") != -1)) {
        setIsLoading(false);
        emit needLogin();
        return;
    }

    if (url.indexOf("/chalplayer") != -1) {
        setIsLoading(false);
        scanState();
        return;
    }

    processData(data, _httpResponceCode, url, new_url.toString());
    setIsLoading(false);
}

void QWarloksDuelCore::slotError(QNetworkReply::NetworkError error) {
    setIsLoading(false);
    _errorMsg = "Network problem details: " + _reply->errorString();
    emit errorOccurred();
    qDebug() << "slotError" << error << _reply->errorString();
}

void QWarloksDuelCore::slotSslErrors(QList<QSslError> error_list) {
    //QSslSocket::ignoreSslErrors();
    setIsLoading(false);
    _errorMsg = "Sll error details: \n";
    foreach(QSslError err, error_list) {
        _errorMsg.append(err.errorString());
        _errorMsg.append("\n");
    }

    emit errorOccurred();
    qDebug() << "slotSslErrors " << error_list;
}

QString QWarloksDuelCore::getSpellList(QString left, QString right, bool Enemy) {
    return SpellChecker->checkSpells(left, right, Enemy);
}
QString QWarloksDuelCore::getSpellBook() {
    bool separate_spellbook = !_isAI && _reg_in_app && (_exp_lv < 1);
    if (separate_spellbook) {
        QSpell::setOrderType(1);
    }
    QString res = SpellChecker->getSpellBook(_isParaFDF, true, !_isAI);
    if (separate_spellbook) {
        QSpell::setOrderType(0);
    }
    return res;
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

    QList<QSpell *> sl = SpellChecker->getPosibleSpellsList(_leftGestures, _rightGestures, WARLOCK_PLAYER, _possibleLeftGestures.indexOf("As Right") != -1 ? _possibleRightGestures : _possibleLeftGestures, _possibleRightGestures, _isParaFDF);
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
    foreach(QSpell *spell, SpellChecker->Spells) {
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
                _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\"}").arg(g, hint, spell->gesture(), GameDictionary->getStringByCode(spell->gesture())));

                found = true;
                break;
            }
        }
        if (!found) {
            if (_spellListHtml.length() > 1) {
                _spellListHtml.append(",");
            }
            _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\"}").arg(spell->gesture(), "", spell->gesture(), GameDictionary->getStringByCode(spell->gesture())));
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
    foreach(QSpell *spell, SpellChecker->Spells) {
        if (first) {
            first = false;
        } else {
            res.append(",");
        }
        res.append(QString("{\"g\":\"%1\",\"code\":\"%2\",\"n\":\"%3\",\"h\":\"\"}").arg(spell->gesture(), spell->gesture(), GameDictionary->getStringByCode(spell->gesture())));
    }
    res.append("]");
    return res;
    //return "<table><tr><th>Gestures</th><th>Spell name</th></tr><tr><td>cDPW</td><td>Dispel Magic</td></tr><tr><td>cSWWS</td><td>Summon Ice Elemental</td></tr><tr><td>cWSSW</td><td>Summon Fire Elemental</td></tr><tr><td>cw</td><td>Magic Mirror</td></tr><tr><td>DFFDD</td><td>Lightning Bolt</td></tr><tr><td>DFPW</td><td>Cure Heavy Wounds</td></tr><tr><td>DFW</td><td>Cure Light Wounds</td></tr><tr><td>DFWFd</td><td>Blindness</td></tr><tr><td>DPP</td><td>Amnesia</td></tr><tr><td>DSF</td><td>Confusion/Maladroitness</td></tr><tr><td>DSFFFc</td><td>Disease</td></tr><tr><td>DWFFd</td><td>Blindness</td></tr><tr><td>DWSSSP</td><td>Delay Effect</td></tr><tr><td>DWWFWD</td><td>Poison</td></tr><tr><td>FFF</td><td>Paralysis</td></tr><tr><td>WFPSFW</td><td>Summon Giant</td></tr><tr><td>FPSFW</td><td>Summon Troll</td></tr><tr><td>PSFW</td><td>Summon Ogre</td></tr><tr><td>SFW</td><td>Summon Goblin</td></tr><tr><td>FSSDD</td><td>Fireball</td></tr><tr><td>P</td><td>Shield</td></tr><tr><td>p</td><td>Surrender</td></tr><tr><td>PDWP</td><td>Remove Enchantment</td></tr><tr><td>PPws</td><td>Invisibility</td></tr><tr><td>PSDD</td><td>Charm Monster</td></tr><tr><td>PSDF</td><td>Charm Person</td></tr><tr><td>PWPFSSSD</td><td>Finger of Death</td></tr><tr><td>PWPWWc</td><td>Haste</td></tr><tr><td>SD</td><td>Magic Missile</td></tr><tr><td>SPFP</td><td>Anti-spell</td></tr><tr><td>SPFPSDW</td><td>Permanency</td></tr><tr><td>SPPc</td><td>Time Stop</td></tr><tr><td>SPPFD</td><td>Time Stop</td></tr><tr><td>SSFP</td><td>Resist Cold</td></tr><tr><td>SWD</td><td>Fear (No CFDS)</td></tr><tr><td>SWWc</td><td>Fire Storm</td></tr><tr><td>WDDc</td><td>Clap of Lightning</td></tr><tr><td>WFP</td><td>Cause Light Wounds</td></tr><tr><td>WPFD</td><td>Cause Heavy Wounds</td></tr><tr><td>WPP</td><td>Counter Spell</td></tr><tr><td>WSSc</td><td>Ice Storm</td></tr><tr><td>WWFP</td><td>Resist Heat</td></tr><tr><td>WWP</td><td>Protection</td></tr><tr><td>WWS</td><td>Counter Spell</td></tr></table>";
}

void QWarloksDuelCore::saveGameParameters() {
    //settings->remove("");
    settings->setValue("login", _login);
    settings->setValue("password", _password);
    settings->setValue("reg_in_app", _reg_in_app);
    settings->setValue("exp_lv", _exp_lv);
    settings->setValue("allowed_add", _allowedAdd);
    settings->setValue("allowed_accept", _allowedAccept);
    settings->setValue("accounts", accountToString());
    settings->setValue("finished_battles", finishedBattles());
    //settings->setValue("shown_battles", _shown_battles);
    settings->setValue("played", _played);
    settings->setValue("won", _won);
    settings->setValue("died", _died);
    settings->setValue("ladder", _ladder);
    settings->setValue("melee", _melee);
    settings->setValue("elo", _elo);
    settings->setValue("hint1", _hint1);
    settings->setValue("show_hint", _show_hint);
    settings->setValue("feedback", _feedback);
    settings->setValue("rate_us", _rateus);
    settings->setValue("reffrer_processed", _process_refferer);
    settings->setValue("last_players_scan", _lastPlayersScan);

    settings->beginWriteArray("battle_info");
    QMap<int, QBattleInfo *>::iterator bii;
    int i = 0;
    for (bii = _battleInfo.begin(); bii != _battleInfo.end(); ++bii, ++i) {
        settings->setArrayIndex(i);
        settings->setValue("k", bii.key());
        settings->setValue("v",  bii.value()->toString());
    }
    settings->endArray();
}

void QWarloksDuelCore::loadGameParameters() {
    _login = settings->value("login", "").toString();
    _password = settings->value("password", "").toString();
    _reg_in_app = settings->value("reg_in_app", "false").toBool();
    _exp_lv = settings->value("exp_lv", "0").toInt();
    _played = settings->value("played", "0").toInt();
    _won = settings->value("won", "0").toInt();
    _died = settings->value("died", "0").toInt();
    _ladder = settings->value("ladder", "0").toInt();
    _melee = settings->value("melee", "0").toInt();
    _elo = settings->value("elo", "1500").toInt();
    _allowedAdd = settings->value("allowed_add", "true").toBool();
    _allowedAccept = settings->value("allowed_accept", "true").toBool();
    _hint1 = settings->value("hint1", "0").toInt();
    _show_hint = settings->value("show_hint", "false").toBool();
    _feedback = settings->value("feedback", "false").toBool();
    _rateus = settings->value("rate_us", "false").toBool();
    _process_refferer = settings->value("reffrer_processed", "false").toBool();
    _lastPlayersScan = settings->value("last_players_scan", "0").toInt();
    accountsFromString(settings->value("accounts", _login.toLower() + "&" + _password).toString());

    int size  = settings->beginReadArray("battle_info");
    int battle_id;
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        battle_id = settings->value("k").toInt();
        _battleInfo.insert(battle_id, new QBattleInfo(settings->value("v").toString()));
    }
    settings->endArray();
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

QString QWarloksDuelCore::playerJson() {
    return QString("{\"name\":\"%1\",\"played\":%2,\"won\":%3,\"died\":%4,\"ladder\":%5,\"melee\":%6,\"elo\":%7,\"feedback\":%8,\"rate_us\":%9,\"finished_game_count\":%10}")
            .arg(_login, intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_ladder), intToStr(_melee), intToStr(_elo), boolToStr(_feedback), boolToStr(_rateus))
            .arg(intToStr(_finished_battles.count()));
}

QString QWarloksDuelCore::playerInfo() {
    QString res;
    int idx;
    if (_msg.size() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("Messages")));
        foreach(QString s, _msg) {
            res.append(QString("<p>%1</p>").arg(s));
        }
    }
    /*if (_challenge.size() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("ChallengeB")));
        QMapIterator<int, QString> i(_challenge);
        while (i.hasNext()) {
            i.next();
            res.append(QString("<p>%1 <a href=\"/challenge/%2/accept\">Accept</a> <a href=\"/challenge/%3/reject\">Reject</a></p>").arg(i.value(), intToStr(i.key()), intToStr(i.key())));
        }
    }*/
    if (_ready_in_battles.count() > 0) {
        res.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("ReadyB")));
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
        res.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("WaitB")));
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
        res.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("FinishB")));
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
            .arg(_login, GameDictionary->getStringByCode("Played"), QString::number(_played), GameDictionary->getStringByCode("Won"), QString::number(_won),
                 GameDictionary->getStringByCode("Died"), QString::number(_died), GameDictionary->getStringByCode("LadderScore"), QString::number(_ladder))
            .arg(GameDictionary->getStringByCode("MeleeScore"), QString::number(_melee), GameDictionary->getStringByCode("Elo"), QString::number(_elo))
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

int QWarloksDuelCore::loadedBattleID() {
    return _loadedBattleID;
}

int QWarloksDuelCore::readyBattle() {
    return _loadedBattleType == 0 ? _loadedBattleID : 0;
}

int QWarloksDuelCore::registerNewUser() {
    return 0;
}

int QWarloksDuelCore::orderSubmited() {
    return 0;
}

int QWarloksDuelCore::timerState() {
    return _isTimerActive ? 1 : 0;
}

int QWarloksDuelCore::challengeSubmited() {
    return 1;
}

QString QWarloksDuelCore::challengeList() {
    QString res = "[";
    bool first = true;
    QStringList sl = _challengeList.split(",");
    foreach(QString s, sl) {
        int bid = s.toInt();
        if ((bid <= 0) || !_battleInfo.contains(bid)) {
            continue;
        }
        if (first) {
            first = false;
        } else {
            res.append(",");
        }
        res.append(_battleInfo[bid]->toJSON(_login));
    }

    return res;
}

QString QWarloksDuelCore::topList() {
    return _topList;
}

QString QWarloksDuelCore::spellListHtml() {
    //prepareSpellHtmlList(false);
    return _spellListHtml;
}

QString QWarloksDuelCore::getOnlineUrl(int battle_id) {
    int bit = battle_id == 0 ? _loadedBattleID : battle_id;
    return QString(GAME_SERVER_URL_GET_FINISHED_BATTLE).arg(intToStr(bit));
}

QString QWarloksDuelCore::battleList() {
    return _battleList;
}

QString QWarloksDuelCore::getHintArray(int hint_id) {
    QString res = "[", hint, hint_code;
    bool first = true, search = true;
    int hint_idx = 0;
    while(search) {
        hint_code = QString("hint_%1_%2").arg(intToStr(hint_id), intToStr(++hint_idx));
        hint = GameDictionary->getStringByCode(hint_code);
        if (hint.compare(hint_code) == 0) {
            search = false;
        } else {
            res.append(QString("%1\"%2\"").arg(first ? "" : ",", hint));
        }
        if (first) {
            first = false;
        }
    }
    return res.append("]");
}

QString QWarloksDuelCore::getBattleHint(QBattleInfo *battle_info) {
    QString hint, hint_code;
    if (battle_info->hint() > 0) {
        //hint_code = QString("hint_%1_%2").arg(intToStr(hint_id), intToStr(battle_turn));
        //hint = GameDictionary->getStringByCode(hint_code);
        return battle_info->turn() != 1 ? "[]" : getHintArray(battle_info->hint());
    } else if (!_show_hint) {
        return "[]";
    }


    while(true) {
        hint_code = QString("hint_%1_1").arg(intToStr(++_hint1));
        hint = GameDictionary->getStringByCode(hint_code);
        if (hint.compare(hint_code) == 0) {
            _show_hint = false;
            return "[]";
        } else {
            battle_info->setHint(_hint1);
            return getHintArray(_hint1);
        }
    }
}

QString QWarloksDuelCore::battleInfo() {
    QString tmp_trg;
    for(int i = 0, Ln = _Targets.size(); i < Ln; ++i) {
        tmp_trg.append(QString("%1;%2#").arg(_Targets.at(i).first, _Targets.at(i).second));
    }
    QBattleInfo *battle_info = getBattleInfo(_loadedBattleID);
    battle_info->setTurn(_loadedBattleTurn);

    QString hint = getBattleHint(battle_info);
    QString msg = "";
    if (_isAI) {
        QString msg_code = QString("BotSay_%1").arg(intToStr(_loadedBattleTurn));
        msg = GameDictionary->getStringByCode(msg_code);
        if (msg_code.compare(msg) == 0) {
            // not found
            msg.clear();
        }
    }
    QString tmpBH = battle_info->getHistory();
    QString tmpBC = battle_info->getChat();

    return QString("{\"id\":%1,\"is_fdf\":%2,\"fire\":\"%3\",\"permanent\":%4,\"delay\":%5,\"paralyze\":\"%6\",\"charm\":\"%7\","
                   "\"rg\":\"%8\",\"lg\":\"%9\",\"prg\":\"%10\",\"plg\":\"%11\",\"monster_cmd\":\"%12\",\"monsters\":%13,\"warlocks\":%14,"
                   "\"targets\":\"%15\",\"chat\":%16,\"is_fc\":%17,\"paralyzed_hand\":%18,\"hint\":%19,\"msg\":\"%20\",\"battle_hist\":\"%21\",\"battle_chat\":\"%22\"}")
            .arg(intToStr(_loadedBattleID), boolToIntS(_isParaFDF), _fire, boolToIntS(_isPermanent), boolToIntS(_isDelay)) // 1-5
            .arg(_paralyzeList, _charmPersonList, _rightGestures, _leftGestures, _possibleRightGestures, _possibleLeftGestures) // 6-11
            .arg(_monsterCommandList, _MonstersHtml, _WarlockHtml, tmp_trg, _chat,  boolToStr(_isParaFC), _paralyzedHands, hint, msg) // 12 - 20
            .arg(tmpBH, tmpBC); // 21-22
}

void QWarloksDuelCore::setParamValue(const QString &Parameter, const QString &Value) {
    if (Parameter.compare("feedback") == 0) {
        _feedback = Value.compare("true") == 0;
    } else if (Parameter.compare("rate_us") == 0) {
        _rateus = Value.compare("true") == 0;
    }
}

QString QWarloksDuelCore::getWarlockStats(const QString &WarlockName, bool DirtyLogin) {
    QString clean_login = WarlockName.toLower();
    if (DirtyLogin && (clean_login.indexOf("(") != -1)) {
        clean_login = clean_login.mid(0, clean_login.indexOf("(") - 1);
    }
    QString stmp;
    bool found = _playerStats.contains(clean_login);
    if (found) {
        stmp = _playerStats[clean_login].toString();
    } else {
        stmp = QString("0,%1,0,0,0,0,0,1500,0").arg(DirtyLogin ? clean_login : WarlockName);
    }
    // boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee), intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo), intToStr(_active)
    QStringList sltmp = stmp.split(",");
    return QString("{\"registered\":%1,\"name\":\"%2\",\"elo\":%3,\"played\":%4,\"won\":%5,\"died\":%6,\"found\":%7}").
            arg(sltmp.at(0), sltmp.at(1), sltmp.at(7), sltmp.at(4), sltmp.at(5), sltmp.at(6), boolToStr(found));
}

void QWarloksDuelCore::showNotification(const QString &msg) {
    qDebug() << "QWarloksDuelCore::showNotification" << msg;
    #ifdef Q_OS_ANDROID
    QJniObject javaNotification = QJniObject::fromString(msg);
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/androidnotifier/NotificationClient",
        "notify",
        "(Landroid/content/Context;Ljava/lang/String;)V",
        QNativeInterface::QAndroidApplication::context(),
        javaNotification.object<jstring>());
    #else
    //_errorMsg = QString("{\"id\":-1,\"type\":18,\"action\":\"play_with_friends\",\"link\":\"%1\"}").arg(s);
    emit errorOccurred();
    #endif
}

void QWarloksDuelCore::getSharableLink(const QString &game_level) {
    QString s = QString("https://play.google.com/store/apps/details?id=net.is.games.WarlocksDuel&utm_source=invite_from_app&referrer=%1,%2").arg(game_level, _login);
    qDebug() << "QWarloksDuelCore::getSharableLink" << s;
    #ifdef Q_OS_ANDROID
    QJniObject javaNotification = QJniObject::fromString(s);
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/androidnotifier/NotificationClient",
        "share",
        "(Landroid/content/Context;Ljava/lang/String;)V",
        QNativeInterface::QAndroidApplication::context(),
        javaNotification.object<jstring>());
    #else
    QClipboard *cp = QGuiApplication::clipboard();
    if (cp) {
        cp->setText(s);
    }
    _errorMsg = QString("{\"id\":-1,\"type\":18,\"action\":\"play_with_friends\",\"link\":\"%1\"}").arg(s);
    emit errorOccurred();
    #endif
}

void QWarloksDuelCore::logout() {
    _isLogined = false;
    _login.clear();
    _password.clear();

    _finished_battles.clear();
    _exp_lv = 0;
    _played = 0;
    _won = 0;
    _died = 0;
    _ladder = 0;
    _melee = 0;
    _elo = 1500;
    _hint1 = 0;
    _show_hint = true;
    _feedback = false;
    _rateus = false;

    settings->beginGroup("Game");
    settings->remove("");
    settings->endGroup();

    setCheckUrl("");

    saveParameters();
    sendGetRequest(GAME_SERVER_URL_LOGOUT);
}

void QWarloksDuelCore::setTimerInterval(int count, int msec) {
    qDebug() << "QWarloksDuelCore::setTimerInterval" << count << msec;
    if (count > 0) {
        _timerCount = count;
    }
    if (msec > 0) {
        _timer.setInterval(msec);
    }
    setTimeState(true);
    if (msec == 0) {
        timerFired();
    }
}

void QWarloksDuelCore::timerFired() {
    qDebug() << "QWarloksDuelCore::timerFired" << _timerCount;
    if ((_timerCount > 0) && (--_timerCount <= 0)) {
        _timer.setInterval((_isAI && !_isAsService) ? 30000 : 60000);
    }
    scanState(true);
    saveParameters(false, false, true, false, false);
    //settings->sync();
}

void QWarloksDuelCore::processServiceTimer() {
    uint curr_time = QDateTime::currentSecsSinceEpoch();
    qDebug() << "QWarloksDuelCore::processServiceTimer" << _isAsService << curr_time << _isAI;
    if (_isAI || _isAsService) {
        _serviceTimer.stop();
        return;
    }
    setCheckUrl(_isLogined ? QString(GAME_SERVER_URL_GET_PROFILE).arg(_login) : "");
    if (_isLogined) {
        processRefferer();
    }
}

void QWarloksDuelCore::setCheckUrl(const QString &check_url) {
    #ifdef Q_OS_ANDROID
    QJniObject javaNotification = QJniObject::fromString(check_url);
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/androidnotifier/NotificationClient",
        "setCheckUrl",
        "(Landroid/content/Context;Ljava/lang/String;)V",
        QNativeInterface::QAndroidApplication::context(),
        javaNotification.object<jstring>());
    #endif
}
