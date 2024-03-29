#include "qwarloksduelcore.h"


QWarloksDuelCore::QWarloksDuelCore(QObject *parent, bool AsService) :
    QGameCore(parent)
{
    _isAsService = AsService;
    _isAiBusy = false;
    _lastAnyAiCall = 0;
    if (_isAsService) {
        _request_code = "ass";
    }
    QString ini_path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    qDebug() << "QWarloksDuelCore::QWarloksDuelCore" << ini_path;
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, ini_path);
    setOrganization(ORGANIZATION_NAME, _isAsService ? APPLICATION_NAME_SERVICE : APPLICATION_NAME);
    init();
    if (_isAsService) {
        _login = "";
        _password = "xsw2#EDC";
    }
    GameDictionary->setCurrentLang("en");
    SpellChecker = QWarlockSpellChecker::getInstance();
    _timerCount = 0;
    _timerInterval = 0;
    if (!_isAsService) {
        connect(&_timer, SIGNAL(timeout()), this, SLOT(timerFired()));
        connect(&_serviceTimer, SIGNAL(timeout()), this, SLOT(processServiceTimer()));
        _serviceTimer.setInterval(10000);
        _serviceTimer.start();
    }
    _isLogined = false;
    _isAI = false;
    _botIdx = 0;
    _isScanForced = false;
    _newBattle = nullptr;

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

    // AI service
    _aiCore = nullptr;

    // Google analytic setup
    if (!AsService) {
        _ga.setNam(&_nam);
        _ga.setMeasurementId("G-E8BDXXY8LP");
        _ga.setApiSecret("ySz8297sQjK94P92lNN27w");
        _ga.setClientId(_UUID);
        processNewLogin(true);
    }
}

QWarloksDuelCore::~QWarloksDuelCore() {
    saveParameters(true, true, true, true, true);

    // clean up
    QMap<int, QBattleInfo *>::iterator bii;
    for (bii = _battleInfo.begin(); bii != _battleInfo.end(); ++bii) {
        delete bii.value();
    }
    _battleInfo.clear();
    if (_newBattle) {
        delete _newBattle;
    }

    if (!_isAsService) {
        _aiThread.quit();
        _aiThread.wait();
    }

    QMap<QString, QWarlockStat *>::iterator psi;
    for (psi = _playerStats.begin(); psi != _playerStats.end(); ++psi) {
        delete psi.value();
    }
    _playerStats.clear();
}

void QWarloksDuelCore::aiCreateNewChallenge() {
    if (_ready_in_battles.count() > 0) {
        return;
    }
    if (_waiting_in_battles.count() < 3) {
        createNewChallenge(true, false, true, true, 2, 2, "Training Battle with AI Player, NO BOT allowed");
    } else {
        aiLogin();
    }
}

void QWarloksDuelCore::createNewChallenge(bool Fast, bool Private, bool ParaFC, bool Maladroid, int Count,
                                          int FriendlyLevel, QString Description, QString Warlock, int IsOnline) {
    qDebug() << "QWarloksDuelCore::createNewChallenge" << Fast << Private << ParaFC << Maladroid << Count <<
        FriendlyLevel << Description << Warlock << IsOnline;
    //Q_UNUSED(Fast);
    setIsLoading(true);

    // QNetworkRequest request;
    // request.setUrl(QUrl(QString(GAME_SERVER_URL_NEW_CHALLENGE)));
    if (!_newBattle) {
        _newBattle = new QBattleInfo();
    }
    _newBattle->setFast(Fast); // true by default
    _newBattle->setDescription(Description);
    _newBattle->setParafc(ParaFC);
    _newBattle->setParafdf(false);
    _newBattle->setMaladroit(Maladroid);
    _newBattle->setSize(Count);
    _newBattle->setLevel(FriendlyLevel);


    bool l_p = Private;
    if (!Warlock.isEmpty() && (_playerStats.contains(Warlock.toLower()) || (_playerStats.size() == 0))) {
        _inviteToBattle = Warlock;
    } else {
        _inviteToBattle.clear();
        l_p = false;
    }

    QString p_data = "fast=1&";
    if (l_p) {
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

    qDebug() << p_data;
    sendPostRequest(GAME_SERVER_URL_NEW_CHALLENGE, p_data.toUtf8());
    setTimeState(true);
}

void QWarloksDuelCore::storeFullParsedBattle(QBattleInfo *bi) {
    if (bi->status() != 2) {
        return;
    }
    QString p_data = "json=";
    p_data.append(QUrl::toPercentEncoding(bi->toString()));
    sendPostRequest(QString(GAME_SERVER_URL_STORE_FINISHED_BATTLE).arg(intToStr(bi->battleID())), p_data.toUtf8());
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

void QWarloksDuelCore::processNewLogin(bool Silent) {
    if (_login.isEmpty()) {
        _ga.setUserId("No Login");
    } else {
        _ga.setUserId(_login);
    }

    // check is exists
    for(int i = 0, Ln = _accounts.size(); i < Ln; ++i) {
        if (_accounts.at(i).first.compare(_login.toLower()) == 0) {
            return;
        }
    }

    _accounts.append(QValueName(_login.toLower(), _password));
    if (!Silent) {
        emit accountMenuChanged();
    }
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
        if (!_isAI && !_reg_in_app) {
            _exp_lv = 5;
        }
        qDebug() << "final url " << url;
        setCheckUrl(QString(GAME_SERVER_URL_GET_PROFILE).arg(_login));
        sendGetRequest(url);
        processRefferer();
        saveParameters(true, true, true, true, true);
        setTimeState(!_isAsService);
        logEvent("login", "");
        return false;
    }
    return true;
}

void QWarloksDuelCore::aiLogin() {
    qDebug() << "QWarloksDuelCore::aiLogin" << _isAI << _ready_in_battles.count() << _lstAI.count() << _botIdx << _isAsService;

    if (!_isAI && !_isAsService) {
        return;
    }
    if (_ready_in_battles.count() > 0) {
        return;
    }

    _isLogined = false;
    if (_botIdx < 0) {
        _botIdx = _lstAI.count();
    }
    if (++_botIdx >= _lstAI.count()) {
        _botIdx = 0;
    }
    _login = _lstAI.at(_botIdx);
    if (_isAsService) {
        // no timer
        scanState(true);
    }
}

void QWarloksDuelCore::callAI(QString Login, int MagicBookLevel) {
    qDebug() << "QWarloksDuelCore::callAI" << Login << MagicBookLevel;
    if (!Login.isEmpty()) {
        emit needAIAnswer(Login, MagicBookLevel);
        return;
    }
    qint64 curr_time = QDateTime::currentSecsSinceEpoch();
    if ((_lastAnyAiCall == 0) || ((curr_time - _lastAnyAiCall) >= 30)) {
        _lastAnyAiCall = curr_time;
        emit needAIAnswer("", MagicBookLevel);
    }
}

void QWarloksDuelCore::finishChallengeList(QString &Data, int StatusCode, QUrl NewUrl) {
    QList<QBattleInfo*> list = QWarlockUtils::parseChallengesList(Data);
    qDebug() << "finishChallengeList" << StatusCode << NewUrl << _isAI;// << _challengeList << list;
    QString list_str;
    bool working = false, active_challenge = false;
    int with_bot = 0;
    foreach(QBattleInfo*bi, list) {
        list_str.append(intToStr(bi->battleID())).append(";");
        if (_battleInfo.contains(bi->battleID())) {
            delete _battleInfo[bi->battleID()];
            _battleInfo.remove(bi->battleID());
        }
        //qDebug() << "finishChallengeList" << bi->toJSON(_login);
        _battleInfo.insert(bi->battleID(), bi);
        if (_isAI) {
            if (bi->for_bot() && bi->active(_login)) {
                working = aiAcceptChallenge(bi->battleID(), false) || working;
            } else if (bi->with_bot() && !bi->active(_login)) {
                with_bot = 100;
            }
        } else if (bi->for_bot() && !bi->active(_login)) {
            qDebug() << "finishChallengeList" << "emit needAIAnswer";
            callAI("", 5);
        } else if (!active_challenge && !bi->for_bot() && bi->active(_login) && (bi->size() == 2)) {
            active_challenge = true;
        }
    }

    if (!_isAI && (_challengeList.compare(list_str) != 0)) {
        _challengeList = list_str;
        //qDebug() << list;
        emit challengeListChanged();
        if (active_challenge || (_waiting_in_battles.count() + _ready_in_battles.count() == 0)) {
            generateBattleList();
        }
    } else if (_isAI && !working) {
        if (with_bot < 2) {
            aiCreateNewChallenge();
        } else if (!_isAsService) {
            aiLogin();
        } else {
            _isAiBusy = _ready_in_battles.count() > 0;
        }
    }
}

void QWarloksDuelCore::finishTopList(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishTopList" << StatusCode << NewUrl;
    QStringList sl = Data.split("\r\n");
    foreach(QString s, sl) {
        if (s.trimmed().isEmpty()) {
            continue;
        }
        QWarlockStat *ws = new QWarlockStat(s.trimmed());
        _playerStats[ws->name().toLower()] = ws;
    }
    /*int pos1 = 0, pos2;
    //QList<QWarlockStat> wsl;
    while((pos1 = Data.indexOf("<TR>", pos1)) != -1) {
        pos2 = Data.indexOf("</TR>", pos1);
        QWarlockStat ws(Data.mid(pos1, pos2 - pos1));
        _playerStats[ws.name().toLower()] = ws;
        qDebug() << "QWarloksDuelCore::finishTopList" << ws.toString();
        pos1 = pos2 + 4;
    }*/
    generateTopList();
    emit topListChanged();
}

void QWarloksDuelCore::generateTopList() {
    QMap<QString, QWarlockStat *>::iterator psi;
    QList<QWarlockStat *> wsl;
    for (psi = _playerStats.begin(); psi != _playerStats.end(); ++psi) {
        wsl.append(psi.value());
    }
    struct {
        bool operator()(const QWarlockStat *s1, const QWarlockStat *s2) const { return s1->elo() > s2->elo(); }
    } customOrderWS;
    std::sort(wsl.begin(), wsl.end(), customOrderWS);
    _topActive.clear();
    _topAll.clear();
    qint64 curr_time = QDateTime::currentSecsSinceEpoch();
    foreach(QWarlockStat *ws, wsl) {
            if (ws->ai()) {
            continue;
        }
        if (!_topAll.isEmpty()) {
            _topAll.append(",");
        }
        _topAll.append(ws->toJSON());
        qDebug() << "QWarloksDuelCore::generateTopList" << ws->name() << curr_time << ws->lastActivity() << curr_time - ws->lastActivity();
        if (curr_time - ws->lastActivity() < 3 * 24 * 60 * 60) {
            if (!_topActive.isEmpty()) {
                _topActive.append(",");
            }
            _topActive.append(ws->toJSON());
        }
    }
    _topActive.append("]").prepend("[");
    _topAll.append("]").prepend("[");
}

bool QWarloksDuelCore::finishCreateChallenge(QString &Data, int StatusCode, QUrl NewUrl) {
    qDebug() << "finishCreateChallenge " << StatusCode << NewUrl << Data << _inviteToBattle;
    if (NewUrl.isEmpty()) {
        if (_isAsService) {
            //aiLogin();
            //do nothing
        } else {
            logEvent("Game_Created_Error", QString("login;%1;").arg(_login));
            _errorMsg = "{\"type\":10}";
            emit errorOccurred();
            return false;
        }
    }


    QString url = NewUrl.toString();
    int pos = 0;
    QString created_id = QWarlockUtils::getStringFromData(url, "num", "=", "&", pos);
    int bid = created_id.toInt();
    if (bid > 0) {
        if (_newBattle) {
            _newBattle->setBattleID(bid);
            _battleInfo.insert(bid, _newBattle);
            _newBattle = nullptr;
        }
        QBattleInfo* battle_info = getBattleInfo(bid);
        battle_info->addParticipant(_login);
        battle_info->setStatus(BATTLE_INFO_STATUS_NO_START);
        if (!_inviteToBattle.isEmpty() && !created_id.isEmpty()) {
            sendGetRequest(QString(GAME_SERVER_URL_INVITE_TO_CHALLENGE).arg(_inviteToBattle, created_id));
        } else if (!_isAsService && battle_info->for_bot()) {
            qDebug() << "emit needAIAnswer";
            callAI("", 5);
        }
        QString params = QString("Id;%1;Type;%2;With;%3;Warlock;%4;IsBot;%6;").arg(created_id, battle_info->level() == 0 ? "Training" : "Scored",
                                                                                  (!_inviteToBattle.isEmpty() ? "Private warlock" : (battle_info->for_bot() ? "Bot" : "Random warlock")),
                                                                                  _login, boolToIntS(_isAI));
        logEvent("Game_Created", params);
        if ((battle_info->level() == 0) && !_event_start_training) {
            _event_start_training = true;
            logEvent("MainFunnel_Training_Game_Created", params);
        }
        if ((battle_info->level() != 0) && !_event_start_pvp) {
            _event_start_pvp = true;
            logEvent("MainFunnel_PvP_Game_Created", params);
        }
        //if ()
    }

    scanState(true);
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
    if (!NewUrl.isEmpty()) {
        QString url = NewUrl.toString().toLower();
        qDebug() << "accept sucessfull redirect to " << url;
        if (url.indexOf("http://") == -1) {
            url.prepend(GAME_SERVER_URL).replace("//", "/").replace(":/", "://");
        }
        qDebug() << "final url " << url;
        sendGetRequest(url);
        if (!_isAsService && (_loadedBattleID > 0)) {
            QBattleInfo *bi = getBattleInfo(_loadedBattleID);
            qDebug() << "QWarloksDuelCore::finishAccept" << bi->getEnemy(_login) << bi->toJSON(_login);
            if (bi->for_bot() || bi->with_bot()) {
                qDebug() << "emit needAIAnswer" << bi->getEnemy(_login);
                callAI(bi->getEnemy(_login), _exp_lv);
            }
        }
    } else {
        scanState();
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
    qDebug() << "finishOrderSubmit sucessfull redirect to" << url;
    if (url.indexOf("http://") == -1) {
        url.prepend(GAME_SERVER_URL).replace("//", "/").replace(":/", "://");
    }
    qDebug() << "final url" << url << _loadedBattleID;
    if (_loadedBattleID > 0) {
        QBattleInfo *bi = getBattleInfo(_loadedBattleID);
        qDebug() << "QWarloksDuelCore::finishOrderSubmit" << bi->getEnemy(_login) << bi->toJSON(_login);
        if (!_isAsService && (bi->for_bot() || bi->with_bot())) {
            qDebug() << "emit needAIAnswer" << bi->getEnemy(_login);
            callAI(bi->getEnemy(_login), _exp_lv);
        } else if (_isAsService) {
            qDebug() << "emit readyAIAnswer";
            emit readyAIAnswer(_loadedBattleID);
            processWarlockPut();
        }
    }

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
        _exp_lv = 1;
        emit registerNewUserChanged();
        saveParameters(false, false, true);
        return true;
    }
    return false;
}

void QWarloksDuelCore::scanState(bool Silent) {
    qDebug() << "scanState" << _isLogined << _isAI << _ready_in_battles.count() << _lstAI.count() << _botIdx << _isAsService;;
    if (!_isLogined) {
        loginToSite();
        return;
    }

    if (!Silent) {
        _isScanForced = true;
        setIsLoading(true);
        //_challengeList.clear();
    }

    sendGetRequest(GAME_SERVER_URL_PLAYER);
    logEvent(Silent ? "ListGames_Refresh_Auto" : "ListGames_Refresh_Clicked", "");
}

void QWarloksDuelCore::getChallengeList(bool Silent) {
    if (!Silent) {
        setIsLoading(true);
    }

    sendGetRequest(GAME_SERVER_URL_CHALLENGES);
}

void QWarloksDuelCore::scanTopList(bool Silent, bool ForceFull) {
    if (_isAI) {
        return;
    }
    qint64 udt = QDateTime::currentSecsSinceEpoch();
    if (udt - _lastPlayersScan > 30) {
        _lastPlayersScan = udt;
        setIsLoading(!Silent);
        sendGetRequest(QString(GAME_SERVER_URL_PLAYERS).arg(_login, ForceFull ? "1" : "0"));
    } else {
        generateTopList();
        emit topListChanged();
    }
}

bool QWarloksDuelCore::aiAcceptChallenge(int battle_id, bool changeAI) {
    if (_waiting_in_battles.count() + _ready_in_battles.count() < 5) {
        acceptChallenge(battle_id, false);
        return true;
    } else if (changeAI) {
        aiLogin();
        return true;
    }
    return false;
}

void QWarloksDuelCore::leaveBattle(int battle_id, int warlock_id) {
    setIsLoading(true);
    if (warlock_id> 0) {
        logEvent("Game_Kick_Out", QString("Id;%1;WarlockId;%2;").arg(intToStr(battle_id), intToStr(warlock_id)));
        sendGetRequest(QString(GAME_SERVER_URL_LEAVE_GAME_OTHER).arg(intToStr(battle_id), intToStr(warlock_id)));
    } else {
        logEvent("Game_Leave", QString("Id;%1;Warlock;%2;").arg(intToStr(battle_id), _login));
        sendGetRequest(QString(GAME_SERVER_URL_LEAVE_GAME).arg(intToStr(battle_id)));
    }

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
    logEvent("Game_Turn_Forced", QString("Id;%1;Turn;%2;IsBot;%3;Warlock;%4;").arg(QString::number(battle_id), QString::number(turn), boolToIntS(_isAI), _login));
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
    QString params = QString("Id;%1;Turn;%2;IsBot;%3;Warlock;%4;").arg(QString::number(_loadedBattleID), QString::number(_loadedBattleTurn), boolToIntS(_isAI), _login);
    logEvent("Game_Turn_Submitted", params);
    if ((_loadedBattleTurn == 1) && !_event_submit_turn) {
        _event_submit_turn = true;
        logEvent("MainFunnel_Submit_Turn", params);
    }
    if ((_loadedBattleTurn == 5) && !_event_submit_turn5) {
        _event_submit_turn5 = true;
        logEvent("MainFunnel_Submit_Turn_5", params);
    }
    //prepareSpellHtmlList();
}

void QWarloksDuelCore::getBattle(int battle_id, int battle_type, bool silent) {
    _loadedBattleID = battle_id;
    _loadedBattleType = battle_type;
    _loadedBattleSilent = silent;
    qDebug() << "getBattle " << _loadedBattleID << " battle_type " << _loadedBattleType << _isLogined << "silent" << _loadedBattleSilent;
    if (!_isLogined) {
        loginToSite();
        return;
    }
    if (_loadedBattleType == 2) {
        QBattleInfo* bi = getBattleInfo(_loadedBattleID);
        if (bi->fullParsed()) {
            _finishedBattle = bi->getFinishedBattleInfo(_login);
            emit finishedBattleChanged();
            return;
        }
    }

    setIsLoading(!silent);
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
    int idx_wait = Data.indexOf("Your orders are in for this turn.");
    int idx_action = Data.indexOf("<FORM METHOD=POST ACTION=\"warlocksubmit\" OnSubmit=");
    qDebug() << "parseBattleDescription" << _loadedBattleID << _loadedBattleType << idx_action << idx_wait;
    if ((_loadedBattleType == 2) && (Data.indexOf(QString("Battle %1 does not exist.").arg(intToStr(_loadedBattleID))) != -1)) {
        return -2;
    }
    int prev_battle_type = _loadedBattleType;

    if ((_loadedBattleType == 0) && (idx_action != -1)) {
        _loadedBattleType = 1;
    } else if (_loadedBattleType == 1) {
        if (idx_wait != -1) {
            _loadedBattleType = 0;
        } else if (idx_action == -1) {
            _loadedBattleType = 2;
        }
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
        QString winner = QWarlockUtils::getFinishedBattleDescription(Data);
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

void QWarloksDuelCore::processSpellBookLevelAfterBattle(QBattleInfo *bi) {
    if (bi->level() == 0) {
        switch(++_play_training_game) {
        case 1:
            logEvent("MainFunnel_Training_Game_Ended", "");
            break;
        case 3:
            logEvent("MainFunnel_3_Training_Game_Ended", "");
            break;
        case 10:
            logEvent("MainFunnel_10_Training_Games_Ende", "");
            break;
        }
    } else {
        switch(++_play_pvp_game) {
        case 1:
            logEvent("MainFunnel_PvP_Game_Ended", "");
            break;
        case 3:
            logEvent("MainFunnel_3_PvP_Game_Ended", "");
            break;
        case 10:
            logEvent("MainFunnel_10_PvP_Games_Ende", "");
            break;
        }
    }
    if (bi->isWinner(_login)) {
        if (bi->with_bot()) {
            switch(++_win_vs_bot) {
            case 0:
                break;
            case 1:
            case 2:
                setSBL(3);
                break;
            default:
                setSBL(_win_vs_bot >= 10 ? 5 : 4);
            }
        } else {
            ++_win_vs_warlock;
            setSBL(5);
        }
    }
}

bool QWarloksDuelCore::finishGetFinishedBattle(QString &Data) {
    qDebug() << "finishGetFinishedBattle" << _loadedBattleID << _loadedBattleType;
    QBattleInfo *battleInfo = getBattleInfo(_loadedBattleID);
    if ((_loadedBattleType == 2) && (Data.indexOf("id#=#") == 0)) {
        QBattleInfo *bi = new QBattleInfo(Data);
        if (bi->battleID() != _loadedBattleID) {
            delete bi;
            _finishedBattle = "Sorry, wrong answer, please contact with viskgameua@gmail.com";
        } else {
            _battleInfo[_loadedBattleID] = bi;
            delete battleInfo;
            battleInfo = bi;
            _finishedBattle = battleInfo->getFullHist(_login);
        }
        if (!_loadedBattleSilent) {
            emit finishedBattleChanged();
        }
        return false;
    }

    int old_state = _loadedBattleType;
    _loadedBattleType = parseBattleDescription(Data);
    if (_loadedBattleType == -2) { // finished but deleted
        if (_finished_battles.indexOf(_loadedBattleID) != -1) {
            _finished_battles.removeAt(_finished_battles.indexOf(_loadedBattleID));
        }
        if (!_loadedBattleSilent) {
            _finishedBattle = "Sorry, but you battle already deleted from game server and we not store it on archive server";
            emit finishedBattleChanged();
        }
        return false;
    } else if (_loadedBattleType == -1) { // unstarted
        QWarlockUtils::parseUnstartedBattle(Data, battleInfo);
        if (!_loadedBattleSilent) {
            _finishedBattle = QString("{\"type\":12,\"d\":\"\",\"id\":%1,\"battle_data\":%2}").arg(intToStr(_loadedBattleID), battleInfo->toJSON(_login));
            emit finishedBattleChanged();
        }
        return false;
    }

    if ((old_state == 1) && (_loadedBattleType == 2)) {
        qDebug() << "finishGetFinishedBattle" << "check rate us" << _finished_battles.count();
        if (!_loadedBattleSilent && !_rateus && battleInfo->isWinner(_login)) {
            bool vsbot = battleInfo->with_bot();
            if (!vsbot || (_finished_battles.count() >= 7)) {
                _rateus = true;
                _errorMsg = "{\"type\":14,\"id\":-1,\"action\":\"rate_us\"}";
                emit errorOccurred();
            }
        }

        // to get full history, but not only last turn
        getBattle(_loadedBattleID, 2, _loadedBattleSilent);
        scanState(true);
        return false;
    }

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
            battleInfo->parseAllTurns(Data, _login, false);
        } else if (_loadedBattleType == -1) {
            _finishedBattle = QString("{\"type\":12,\"d\":\"\",\"id\":%1,\"battle_data\":%2}").arg(intToStr(_loadedBattleID), battleInfo->toJSON(_login));
        } else if (_loadedBattleType == 2) {
            battleInfo->parseAllTurns(Data, _login, true);
            _finishedBattle = battleInfo->getFinishedBattleInfo(_login);
            qDebug() << "after battleInfo->getFullHist" << _login << battleInfo->winner() << battleInfo->isWinner(_login) << battleInfo->with_bot();
            processSpellBookLevelAfterBattle(battleInfo);
#ifndef _DEBUG
            storeFullParsedBattle(battleInfo);
#endif
            QString params = QString("Id;%1;Players;%2;Winner;%3;").arg(intToStr(_loadedBattleID), battleInfo->getInListParticipant(_login, true), battleInfo->winner());
            logEvent("Game_End", params);
        }
        if (!_loadedBattleSilent) {
            emit finishedBattleChanged();
        }
        return false;
    }

    int idx3 = Data.indexOf("<TABLE CELLPADDING=0 CELLSPACING=0 BORDER=0 WIDTH=\"100%\">", idx1);
    if (idx3 == -1) {
        if (!_loadedBattleSilent) {
            _errorMsg = "Wrong battle answer!!!";
            emit errorOccurred();
        }
        return false;
    }

    QString ReadyData = Data.mid(idx3, Data.length() - idx3);
    if (ReadyData.indexOf("<FORM METHOD=POST ACTION=\"warlocksubmit\"") == -1) {
        // mean battle finished just now
        _errorMsg = "Looks like battle already finished, please wait for refresh";
        qDebug() << _errorMsg << ReadyData;
        if (!_loadedBattleSilent) {
            emit errorOccurred();
        }
        return false;
    }
    bool res = parseReadyBattle(ReadyData);
    if (!_loadedBattleSilent && res) {
        qDebug() << "Check spellbook levelup" << _loadedBattleTurn << battleInfo->with_bot();

        if (_loadedBattleTurn == 1) {
            QString params = QString("Id;%1;Players;%2;").arg(intToStr(_loadedBattleID), battleInfo->getInListParticipant(_login, true));
            logEvent("Game_Started", params);
            if ((battleInfo->level() == 0) && !_event_start_training) {
                _event_start_training = true;
                logEvent("MainFunnel_Training_Game_Started", params);
            }
            if ((battleInfo->level() != 0) && !_event_start_pvp) {
                _event_start_pvp = true;
                logEvent("MainFunnel_PvP_Game_Started", params);
            }
        } else if (_loadedBattleTurn >= 15) {
            if (battleInfo->with_bot()) {
                setSBL(2);
            } else {
                setSBL(5);
            }
        }
        setPossibleSpell(Data);

        calcBattleDecision();

        if (!prepareMonsterHtml()) {
            return false;
        }

        if (!prepareWarlockHtml()) {
            return false;
        }

        prepareBattleChatAndHistory(Data);

        if (_isAI) {
            sendOrders(prepareBattleOrders());
        } else {
            setTimeState(false);
            emit readyBattleChanged();
        }
    }
    return res;
}

QString QWarloksDuelCore::prepareBattleOrders() {
    QString res = "say$";
    QString msg = "";
    QString msg_code = QString("BotSay_%1").arg(intToStr(_loadedBattleTurn));
    msg = GameDictionary->getStringByCode(msg_code);
    if (msg_code.compare(msg) == 0) {
        // not found
        msg.clear();
    }
    res.append(msg.replace("$", "sign:dollar").replace("#", "sign:pos_number"));
    res.append("#");
    res.append(_player->printOrders(_WarlockID));
    foreach(QMonster *m, _Monsters) {
        res.append(QString("%1$%2#").arg(_WarlockID[m->name()].replace(" ", "+"), _WarlockID[m->newTarget()].replace(" ", "+")));
    }
    qDebug() << "QWarloksDuelCore::prepareBattleOrders" << _paralyzeList << _charmPersonList;
    foreach(QWarlock *w, _Warlock) {
        qDebug() << "QWarloksDuelCore::prepareBattleOrders" << w->name() << w->id() << _WarlockID[w->name()];
        if (_paralyzeList.indexOf(w->id() + ";") != -1) {
            res.append(QString("PARALYZE%1$%2#").arg(w->id(), w->forcedHand() ? "LH" : "RH"));
        }
        if (_charmPersonList.indexOf(w->id() + ";") != -1) {
            res.append(QString("DIRECTHAND%1$%2#").arg(w->id(), w->forcedHand() ? "LH" : "RH"));
            res.append(QString("DIRECTGESTURE%1$-#").arg(w->id()));
        }
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
    int chat_cnt = 0;
    foreach(QString ss, slC) {
        if (ss.indexOf(" says ") != -1) {
            if (!chat_msg.isEmpty()) {
                chat_msg.append("<br>");
            }
            chat_msg.append(ss);
            if (ss.indexOf(_login, Qt::CaseInsensitive) == -1) {
                ++chat_cnt;
            }
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
    // fix for issue #201
    if (battleInfo->with_bot()) {
        chat_cnt = 0;
    }
    _chat = intToStr(chat_cnt);
}

void QWarloksDuelCore::setPossibleSpell(const QString &Data) {
    if (_isAsService) qDebug() << "QWarloksDuelCore::setPossibleSpell";
    _player = nullptr;
    _enemy = nullptr;
    foreach(QWarlock *w, _Warlock) {
        if (_isAsService) qDebug() << "QWarloksDuelCore::setPossibleSpell" << w->name() << w->player() << _player << _enemy;
        if (w->player()) {
            _player = w;
            w->setMagicBookLevel(_exp_lv);
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
    if (_isAsService) qDebug() << "QWarloksDuelCore::setPossibleSpell" << _player << _enemy;
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
        _WarlockID[_Targets.at(i).second] = _Targets.at(i).first; // name to id
        //_WarlockID[_Targets.at(i).second] = _Targets.at(i).first; // id to name
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
    QBattleInfo *bi = getBattleInfo(_loadedBattleID);
    bi->cleanParticipant();
    int size = 0;
    foreach(QWarlock *m, _Warlock) {
        bool Player = m->name().compare(_login, Qt::CaseInsensitive) == 0;
        ++size;
        bi->addParticipant(m->name());
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
    bi->setSize(size);
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
    if (_isTimerActive && !_isAsService) {
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
    qDebug() << "QWarloksDuelCore::generateBattleList";
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
        _battleList.append(QString("{\"id\":%1,\"s\":1,\"d\":\"%2\",\"el\":\"%3\"}").arg(intToStr(bid), battle_info->getInListDescription(_login), battle_info->getEnemy(_login)));
    }

    QString wait_str = "";
    for(i = _waiting_in_battles.size()- 1; i >= 0; --i) {
        bid = _waiting_in_battles.at(i);
        battle_info = getBattleInfo(bid);
        if (battle_info->status() != BATTLE_INFO_STATUS_NO_START) {
            battle_info->setStatus(BATTLE_INFO_STATUS_WAIT);
        }
        d = battle_info->getInListDescription(_login);
        if (battle_info->status() == 0) {
            if (first) {
                first = false;
            } else {
                _battleList.append(",");
            }
            _battleList.append(QString("{\"id\":%1,\"s\":0,\"d\":\"%2\",\"el\":\"%3\"}").arg(intToStr(bid), d, battle_info->getEnemy(_login)));
        } else {
           if (!wait_str.isEmpty()) {
               wait_str.append(",");
           }
           wait_str.append(QString("{\"id\":%1,\"s\":-1,\"d\":\"%3\"}").arg(intToStr(bid), d));
        }
    }
    if (!wait_str.isEmpty()) {
        if (first) {
           first = false;
        } else {
            _battleList.append(",");
        }
        _battleList.append(wait_str);
    }

    int active_battle_cnt = _waiting_in_battles.size() + _ready_in_battles.size();
    QString chalenge_str;
    bool with_challenge = false, with_any_challenge = false;
    if (active_battle_cnt < 5) {
        QStringList sl = _challengeList.split(";");
        foreach (QString s, sl) {
            bid = s.toInt();
            if (bid <= 0){
                continue;
            }
            battle_info = getBattleInfo(bid);
            if ((battle_info->level() == BATTLE_INFO_LEVEL_LADDER) || battle_info->with_bot() ||
                 (battle_info->status() != BATTLE_INFO_STATUS_NO_START) ||
                 (battle_info->size() != 2) || !battle_info->active(_login)) {
                continue;
            }
            if (with_any_challenge) {
                chalenge_str.append(",");
            } else {
                with_any_challenge = true;
            }

            QString enemy = battle_info->getEnemy(_login).toLower();
            if (!with_challenge && _playerStats.contains(enemy)) {
                if (qAbs(_playerStats[enemy]->elo() - _elo) <= 150) {
                    with_challenge = true;
                }
            }
            QString ddd = battle_info->description();
            ddd = ddd.replace("Maladroit", "", Qt::CaseInsensitive)
                    .replace("ParaFC", "", Qt::CaseInsensitive)
                    .replace("Created with android app Warlock's Duel.", "", Qt::CaseInsensitive)
                    .replace("NO BOT", "", Qt::CaseInsensitive);
            chalenge_str.append(QString("{\"id\":%1,\"s\":4,\"d\":\"%2 challenge by %3\",\"dt\":\"%4\",\"el\":\"%5\"}").
                               arg(intToStr(bid), battle_info->level() == BATTLE_INFO_LEVEL_FRIENDLY ? "Open" : "Practice", battle_info->getEnemy(_login), ddd, battle_info->getEnemy(_login)));
        }
    }
    if (/*with_any_challenge && */!with_challenge && (active_battle_cnt < 3)) {
        QMap<QString, QWarlockStat *>::iterator psi;
        QList<QString> awailable_walocks;
        qint64 curr_time = QDateTime::currentSecsSinceEpoch();
        for (psi = _playerStats.begin(); psi != _playerStats.end(); ++psi) {
            if (!psi.value()->ai() && (curr_time - psi.value()->lastActivity() <= 3 * 24 * 60 * 60) &&
                (psi.key().compare(_login.toLower()) != 0) && (qAbs(psi.value()->elo() - _elo) <= 150)) {
                awailable_walocks.append(psi.key());
            }
        }
        if (awailable_walocks.size() > 0) {
            int idx = QWarlockUtils::getRand(0, awailable_walocks.size());
            if (first) {
                first = false;
            } else {
                _battleList.append(",");
            }
            QString enemy = _playerStats[awailable_walocks.at(idx)]->name();
            _battleList.append(QString("{\"id\":0,\"s\":5,\"d\":\"New Match: %1\",\"dt\":\"Players we match you have played recently and are more or less your level\",\"el\":\"%3\"}").
                               arg(enemy, enemy));
        }
    }
    if (!chalenge_str.isEmpty()) {
        if (first) {
            //first = false;
        } else {
            _battleList.append(",");
        }
        _battleList.append(chalenge_str);
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
    // ForceBattleList == true - mean scan just after battle orders submit
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

    if (!_allowedAccept && ((_finished_battles.count() > 0) || (_played > 0))) {
        _allowedAccept = true;
        _allowedAdd = true;
        emit allowedAcceptChanged();
    }
    QBattleInfo *battle_info;
    if (!_isAI && ForceBattleList && (_ready_in_battles.count() == 0) && (QGameUtils::getTs20021DIffInSec(_timeAskForNotification) >= 31 * 24 * 60 * 60)
            && !checkIsNotificationGranted()) {
        battle_info = getBattleInfo(_loadedBattleID);
        if ((battle_info->turn() > 0) && !battle_info->with_bot()) {
            _timeAskForNotification = QGameUtils::getCurrTimestamp2021();
            _errorMsg = "{\"type\":201,\"id\":-1}";
            emit errorOccurred();
        }
        ///
    }

    qDebug() << "ready: " << _ready_in_battles;
    qDebug() << "waiting: " << _waiting_in_battles;
    qDebug() << "finished: " << _finished_battles;
    //qDebug() << "shown: " << _shown_battles;
    emit playerInfoChanged();
    if (_ready_in_battles.count() > 0) {
        setTimeState(false);
        if (_isAI) {
            _isAiBusy = true;
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

    //if (!changed) {
        foreach(int bid, _ready_in_battles) {
            battle_info = getBattleInfo(bid);
            battle_info->setStatus(BATTLE_INFO_STATUS_READY);
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
        //int curre_time_spec = static_cast<int>(QDateTime::currentSecsSinceEpoch() - SECONDS_AT_20210901);
        bool ask_ai = false;
        foreach(int bid, _waiting_in_battles) {
            battle_info = getBattleInfo(bid);
            if (battle_info->status() != BATTLE_INFO_STATUS_NO_START) {
                battle_info->setStatus(BATTLE_INFO_STATUS_WAIT);
            }
            if (!changed && (old_wait.indexOf(bid) == -1)) {
                changed = true;
                //break;
            }
            qDebug() << "finishScan waiting" << battle_info->toJSON(_login);
            if (!_isAsService && !ask_ai && (battle_info->for_bot() || battle_info->with_bot())) {
                QString enemy = battle_info->getEnemy(_login);
                if (_lstAI.indexOf(enemy.toUpper()) != -1) {
                    ask_ai = true;
                    qDebug() << "emit needAIAnswer"  << enemy;
                    callAI(battle_info->getEnemy(_login), _exp_lv);
                }
            }

            if (_isAI && (_ready_in_battles.size() == 0) && battle_info->canForceSurrendering()) {
                if (battle_info->turn() > 0) {
                    battle_info->setWaitFrom(battle_info->wait_from() + 30 * 60);
                    forceSurrender(bid, battle_info->turn());
                } else if (!ask_ai) {
                    ask_ai = true;
                    getBattle(bid, 0);
                }
            }

            if (!_isAI && (_ready_in_battles.size() == 0)) {
                if ((battle_info->status() == BATTLE_INFO_STATUS_WAIT) && battle_info->canForceSurrendering(7 * 24 * 60 * 60) && (battle_info->turn() > 0)) {
                    battle_info->setWaitFrom(battle_info->wait_from() + 30 * 60);
                    forceSurrender(bid, battle_info->turn());
                }

                if ((battle_info->status() == BATTLE_INFO_STATUS_NO_START)) {
                    if (battle_info->isInviteRejected() && battle_info->canForceSurrendering(10 * 60)) {
                        leaveBattle(bid);
                    } else if (!battle_info->isInviteRejected() && battle_info->canForceSurrendering(24 * 60 * 60)) {
                        battle_info->setWaitFrom(-2);
                        getBattle(bid, 0, true);
                    }
                }

            }
        }
        if (!_isAI && !ask_ai) {
            qDebug() << "emit needAIAnswer any";
            callAI("", 5);
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
    foreach(int bid, old_fin) {
        if (_finished_battles.indexOf(bid) == -1) {
            _finished_battles.append(bid);
        }
    }

    // try to find refferrer
    processRefferer();

    if (changed || ForceBattleList) {
        generateBattleList();
    }
    saveParameters();
}

bool QWarloksDuelCore::checkIsNotificationGranted() {
    int allowed = 0;
#ifdef Q_OS_ANDROID
    QJniObject val = QJniObject::fromString("");
    QJniObject string = QJniObject::callStaticObjectMethod("org/qtproject/example/androidnotifier/NotificationClient", "isNotificationAllowed",
                                                           "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                                                           QNativeInterface::QAndroidApplication::context(), val.object<jstring>());
    allowed = string.toString().toInt();
    qDebug() << "checkIsNotificationGranted" << allowed;
#endif
    return allowed == 1;
}

void QWarloksDuelCore::processRefferer() {
    if (_isAsService || _isAI) {
        return;
    }
    QString finished_battles;
    foreach(int fid, _finished_battles) {
        if (!finished_battles.isEmpty()) {
            finished_battles.append(",");
        }
        finished_battles.append(intToStr(fid));
    }

    QString full_reff;
    #ifdef Q_OS_ANDROID
    QJniObject val = QJniObject::fromString(finished_battles);
    QJniObject string = QJniObject::callStaticObjectMethod("org/qtproject/example/androidnotifier/NotificationClient", "get_refferer",
                                                           "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                                                           QNativeInterface::QAndroidApplication::context(), val.object<jstring>());
    full_reff = string.toString();
    qDebug() << "CheckReffereef" << full_reff;
    #endif
    if (full_reff.isEmpty()) {
        return;
    }
    QString reff;
    int idx1 = full_reff.indexOf("referrer=");
    if (idx1 != -1) {
        idx1 += 9;
        reff = "create_battle," + full_reff.mid(idx1);
    } else {
        reff = full_reff;
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
        getChallengeList(true);
    }
    if (!_isAI && !_isAsService) {
        if (QDateTime::currentSecsSinceEpoch() - _lastPlayersScan >= 1 * 60) {
            scanTopList(false);
        }
    }
    return true;
}

bool QWarloksDuelCore::finishScanWarlock(QString &Data) {
    qDebug() << "QWarloksDuelCore::finishScanWarlock" << Data;
    int pos = 0;
    bool registered = Data.indexOf("<img width=\"12\" height=\"12\" src=\"/img/reg.png\" alt=\"Registered!\">") != -1;
    bool mobile = Data.indexOf("I am using Android application") != -1;
    QString login = QWarlockUtils::getStringFromData(Data, "Info for", " ", " ", pos);
    QString lastActive = QWarlockUtils::getStringFromData(Data, "Last Active:</TD>", "<TD>", " ago", pos);
    _warlockId = QWarlockUtils::getStringFromData(Data, "<INPUT TYPE=HIDDEN NAME=rcpt VALUE", "=", ">", pos);
    int played = QWarlockUtils::getIntFromPlayerData(Data, "Played:", "<TD>", "</TD>", pos);
    int won = QWarlockUtils::getIntFromPlayerData(Data, "Won:", "<TD>", "</TD>", pos);
    int died = QWarlockUtils::getIntFromPlayerData(Data, "Died:", "<TD>", "</TD>", pos);
    int ladder = QWarlockUtils::getIntFromPlayerData(Data, "Ladder Score:", "<TD>", "</TD>", pos);
    int melee = QWarlockUtils::getIntFromPlayerData(Data, "Melee Score:", "<TD>", "</TD>", pos);
    int elo = QWarlockUtils::getIntFromPlayerData(Data, "Elo:", "<TD>", "</TD>", pos);

//    QList<int> ready_in_battles = QWarlockUtils::getBattleList(Data, "Ready in battles:");
//    QList<int> waiting_in_battles = QWarlockUtils::getBattleList(Data, "Waiting in battles:");
//    QList<int> finished_battles = QWarlockUtils::getBattleList(Data, "Finished battles:");

    QString ll = login.toLower();
    if (_playerStats.contains(ll)) {
        _playerStats[ll]->setRegistered(registered);
        _playerStats[ll]->setLadder(ladder);
        _playerStats[ll]->setMelee(melee);
        _playerStats[ll]->setPlayed(played);
        _playerStats[ll]->setWon(won);
        _playerStats[ll]->setDied(died);
        _playerStats[ll]->setElo(elo);
        _playerStats[ll]->setLastActivity(QWarlockUtils::parseLastActivity(lastActive));
        _playerStats[ll]->setMobile(mobile);
    } else {
        // QWarlockStat(QString Name, bool Registered, int Ladder, int Melee, int Played, int Won, int Died, int Elo, QString Color, qint64 LastActivity, bool Mobile);
        _playerStats[ll] = new QWarlockStat(login, registered, ladder, melee, played, won, died, elo, "#FF0000", QWarlockUtils::parseLastActivity(lastActive), mobile);
    }
    QWarlockStat *wi = _playerStats[ll];
    wi->setWarlockId(_warlockId.toInt());
    //            ltTitle.text = warlock_data.name;
    //            ltLast.text = UPU.getLastActivityText(warlock_data.last_activity);
    //            ltScore.text = "Score: " + warlock_data.elo;
    //            ltWon.text = "Won: " + warlock_data.won;
    //            ltLost.text = "Lost: " + (warlock_data.played - warlock_data.won);
    //            ltPlayed.text = "Played: " + warlock_data.played;
    //            bbBtn2.visible = !warlock_data.isPlayer;
    //            bbAction.text = warlock_data.isPlayer ? "Hall of Fame" : "Challenge";
    //            ltTitleOnline.visible = warlock_data.online;
    _errorMsg = QString("{\"type\":15,\"name\":\"%1\",\"last_activity\":%2,\"elo\":%3,\"won\":%4,\"played\":%5,\"isPlayer\":false,\"online\":%6}")
                    .arg(login, intToStr(wi->lastActivity()), intToStr(wi->elo()),intToStr(won),intToStr(played),intToStr(wi->online()));
    emit errorOccurred();

    /*_warlockInfo.clear();
    int idx;
    if (ready_in_battles.count() > 0) {
        _warlockInfo.append(QString("<h4>%1: </h4><p>").arg(GameDictionary->getStringByCode("ReadyB")));
        idx = -1;
        foreach(int i, ready_in_battles) {
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
        foreach(int i, waiting_in_battles) {
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
        foreach(int i, finished_battles) {
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
            ).append("</html>");*/


    //emit warlockInfoChanged();
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

    if (url.indexOf("robot_gateway/wh/top") != -1) {
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

    if (url.indexOf("robot_gateway/wh/warlock_get") != -1) {
        processWarlockGet(data);
        return true;
    }

    if ((url.indexOf("robot_gateway/wh/") != -1) && ((url.indexOf("store_json") != -1) || (url.indexOf("warlock_put") != -1))) {
        return true;
    }

    // battle parsing there
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

    if (/*(url.indexOf("/refuse") != -1) ||*/ (url.indexOf("/leave") != -1)) {
        scanState();
        return false;
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

    setIsLoading(false);
    processData(data, _httpResponceCode, url, new_url.toString());
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
    return SpellChecker->getSpellBook(_isParaFDF, true, !_isAI);
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
                _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\",\"sbl\":%5}").arg(g, hint, spell->gesture(), GameDictionary->getStringByCode(spell->gesture()), intToStr(spell->spellBookLevel())));

                found = true;
                break;
            }
        }
        if (!found) {
            if (_spellListHtml.length() > 1) {
                _spellListHtml.append(",");
            }
            _spellListHtml.append(QString("{\"g\":\"%1\",\"h\":\"%2\",\"code\":\"%3\",\"n\":\"%4\",\"sbl\":%5}").arg(spell->gesture(), "", spell->gesture(), GameDictionary->getStringByCode(spell->gesture()), intToStr(spell->spellBookLevel())));
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
}

void QWarloksDuelCore::saveGameParameters() {
    //settings->remove("");
    settings->setValue("app_version", APPLICATION_VERSION);
    settings->setValue("login", _login);
    settings->setValue("password", _password);
    settings->setValue("reg_in_app", _reg_in_app);
    settings->setValue("exp_lv", _exp_lv);
    settings->setValue("allowed_add", _allowedAdd);
    settings->setValue("allowed_accept", _allowedAccept);
    //settings->setValue("accounts", accountToString());
    settings->setValue("finished_battles", finishedBattles());
    settings->setValue("played", _played);
    settings->setValue("won", _won);
    settings->setValue("died", _died);
    settings->setValue("ladder", _ladder);
    settings->setValue("melee", _melee);
    settings->setValue("elo", _elo);
    settings->setValue("win_vs_bot", _win_vs_bot);
    settings->setValue("win_vs_warlock", _win_vs_warlock);
    settings->setValue("play_vs_bot", _play_training_game);
    settings->setValue("play_vs_warlock", _play_pvp_game);
    settings->setValue("hint1", _hint1);
    settings->setValue("show_hint", _show_hint);
    settings->setValue("feedback", _feedback);
    settings->setValue("rate_us", _rateus);
    settings->setValue("reffrer_processed", _process_refferer);
    settings->setValue("last_players_scan", _lastPlayersScan);
    settings->setValue("ts_notification_ask", _timeAskForNotification);
    settings->setValue("event_start_training", _event_start_training);
    settings->setValue("event_start_pvp", _event_start_pvp);
    settings->setValue("event_submit_turn", _event_submit_turn);
    settings->setValue("event_submit_turn5", _event_submit_turn5);

    settings->beginWriteArray("battle_info");
    QMap<int, QBattleInfo *>::iterator bii;
    int i = 0;
    for (bii = _battleInfo.begin(); bii != _battleInfo.end(); ++bii, ++i) {
        settings->setArrayIndex(i);
        settings->setValue("k", bii.key());
        settings->setValue("v",  bii.value()->toString());
    }
    settings->endArray();

    QMap<QString, QWarlockStat *>::iterator psi;
    settings->beginWriteArray("ps");
    i = 0;
    for (psi = _playerStats.begin(); psi != _playerStats.end(); ++psi, ++i) {
        settings->setArrayIndex(i);
        settings->setValue("n", psi.key());
        settings->setValue("v", psi.value()->toString());
    }
    settings->endArray();
}

void QWarloksDuelCore::loadGameParameters() {
    _appVersion = settings->value("app_version", "0").toInt();
    _login = settings->value("login", "").toString();
    _password = settings->value("password", "").toString();
    _reg_in_app = settings->value("reg_in_app", "false").toBool();
    _exp_lv = settings->value("exp_lv", "1").toInt();
    if (_exp_lv < 1) {
        _exp_lv = 1;
    }
    if (!_reg_in_app) {
        _exp_lv = 5;
    }
    if (_appVersion < APPLICATION_VERSION_INT) {
        qDebug() << "QWarloksDuelCore::loadGameParameters" << _appVersion << "<" << APPLICATION_VERSION_INT << "clean all settings";
        settings->clear();
    }
    _played = settings->value("played", "0").toInt();
    _won = settings->value("won", "0").toInt();
    _died = settings->value("died", "0").toInt();
    _ladder = settings->value("ladder", "0").toInt();
    _melee = settings->value("melee", "0").toInt();
    _elo = settings->value("elo", "1500").toInt();
    _win_vs_bot = settings->value("win_vs_bot", "0").toInt();
    _win_vs_warlock = settings->value("win_vs_warlock", "0").toInt();
    _play_training_game = settings->value("play_vs_bot", "0").toInt();
    _play_pvp_game = settings->value("play_vs_warlock", "0").toInt();
    _allowedAdd = settings->value("allowed_add", "true").toBool();
    _allowedAccept = settings->value("allowed_accept", "true").toBool();
    _hint1 = settings->value("hint1", "0").toInt();
    _show_hint = settings->value("show_hint", "false").toBool();
    _feedback = settings->value("feedback", "false").toBool();
    _rateus = settings->value("rate_us", "false").toBool();
    _process_refferer = settings->value("reffrer_processed", "false").toBool();
    _lastPlayersScan = settings->value("last_players_scan", "0").toInt();
    _event_start_training = settings->value("event_start_training", "false").toBool();
    _event_start_pvp = settings->value("event_start_pvp", "false").toBool();
    _event_submit_turn = settings->value("event_submit_turn", "false").toBool();
    _event_submit_turn5 = settings->value("event_submit_turn5", "false").toBool();
    _timeAskForNotification = settings->value("ts_notification_ask", "0").toInt();

    //accountsFromString(settings->value("accounts", _login.toLower() + "&" + _password).toString());

    int size  = settings->beginReadArray("battle_info");
    int battle_id;
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        battle_id = settings->value("k").toInt();
        QBattleInfo *bi = new QBattleInfo(settings->value("v").toString());
        _battleInfo.insert(battle_id, bi);
    }
    settings->endArray();

    size = settings->beginReadArray("ps");
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        _playerStats[settings->value("n").toString()] = new QWarlockStat(settings->value("v").toString(), true);
    }
    settings->endArray();
    if (_playerStats.size() > 0) {
        generateTopList();
    }
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
    return QString("{\"name\":\"%1\",\"played\":%2,\"won\":%3,\"died\":%4,\"ladder\":%5,\"melee\":%6,\"elo\":%7,\"feedback\":%8,\"rate_us\":%9,"
                   "\"finished_game_count\":%10,\"sbl\":%11,\"win_vs_bot\":%12,\"win_vs_warlock\":%13}")
            .arg(_login, intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_ladder), intToStr(_melee), intToStr(_elo), boolToStr(_feedback), boolToStr(_rateus))
            .arg(intToStr(_play_training_game + _play_pvp_game), intToStr(_exp_lv), intToStr(_win_vs_bot), intToStr(_win_vs_warlock));
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
    QStringList sl = _challengeList.split(";");
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
    res.append("]");

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

QString QWarloksDuelCore::getTopList(bool ShowAll) {
    return ShowAll ? _topAll : _topActive;
}

QString QWarloksDuelCore::getHintArray(int hint_id) {
    QString res = "[", hint, hint_code, hint_icon, hint_icon_code;
    bool first = true, search = true;
    int hint_idx = 0;
    while(search) {
        hint_code = QString("hint_%1_%2").arg(intToStr(hint_id), intToStr(++hint_idx));
        hint_icon_code = QString("%1_icon").arg(hint_code);
        hint = GameDictionary->getStringByCode(hint_code);
        if (hint.compare(hint_code) == 0) {
            search = false;
        } else {
            hint_icon = GameDictionary->getStringByCode(hint_icon_code);
            if (hint_icon.compare(hint_icon_code) == 0) {
                hint_icon.clear();
            }

            res.append(QString("%1{\"txt\":\"%2\",\"icon\":\"%3\"}").arg(first ? "" : ",", hint, hint_icon));
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

QString QWarloksDuelCore::battleReadOnly(int BattleID) {
    qDebug() << "QWarloksDuelCore::battleReadOnly" << BattleID << _loadedBattleID;
    QBattleInfo *battle_info = getBattleInfo(_loadedBattleID);
    QString res = battle_info->fullJSON();
    return res;
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
    QString tmpLT = battle_info->getTurnInfo(_loadedBattleTurn, _login);

    return QString("{\"id\":%1,\"is_fdf\":%2,\"fire\":\"%3\",\"permanent\":%4,\"delay\":%5,\"paralyze\":\"%6\",\"charm\":\"%7\","
                   "\"rg\":\"%8\",\"lg\":\"%9\",\"prg\":\"%10\",\"plg\":\"%11\",\"monster_cmd\":\"%12\",\"monsters\":%13,\"warlocks\":%14,"
                   "\"targets\":\"%15\",\"chat\":%16,\"is_fc\":%17,\"paralyzed_hand\":%18,\"hint\":%19,\"msg\":\"%20\","
                   "\"battle_hist\":\"%21\",\"battle_chat\":\"%22\",\"turn_num\":%23,\"with_bot\":%24,\"last_turn_hist\":%25,"
                   "\"maladroit\":%26}")
            .arg(intToStr(_loadedBattleID), boolToIntS(_isParaFDF), _fire, boolToIntS(_isPermanent), boolToIntS(_isDelay)) // 1-5
            .arg(_paralyzeList, _charmPersonList, _rightGestures, _leftGestures, _possibleRightGestures, _possibleLeftGestures) // 6-11
            .arg(_monsterCommandList, _MonstersHtml, _WarlockHtml, tmp_trg, _chat,  boolToStr(_isParaFC), _paralyzedHands, hint, msg) // 12 - 20
            .arg(tmpBH, tmpBC, intToStr(_loadedBattleTurn), boolToStr(battle_info->with_bot()), tmpLT, boolToStr(_isMaladroit)); // 21-26
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
        stmp = _playerStats[clean_login]->toString();
    } else {
        stmp = QString("0,%1,0,0,0,0,0,1500,#000000,0,0,0").arg(DirtyLogin ? clean_login : WarlockName);
    }
    // boolToIntS(_registered), _name, intToStr(_ladder), intToStr(_melee), intToStr(_played), intToStr(_won), intToStr(_died), intToStr(_elo), intToStr(_active)
    QStringList sltmp = stmp.split(",");
    return QString("{\"registered\":%1,\"name\":\"%2\",\"elo\":%3,\"played\":%4,\"won\":%5,\"died\":%6,\"found\":%7,\"last_activity\":%8,\"is_bot\":%9}").
            arg(sltmp.at(0), sltmp.at(1), sltmp.at(7), sltmp.at(4), sltmp.at(5), sltmp.at(6), boolToStr(found), sltmp.at(9), sltmp.at(11));
}

QString QWarloksDuelCore::findWarlockByName(const QString &warlockName) {
    QList<QWarlockStat *> wsl;
    qint64 curr_time = QDateTime::currentSecsSinceEpoch();
    QMap<QString, QWarlockStat *>::iterator psi;
    for (psi = _playerStats.begin(); psi != _playerStats.end(); ++psi) {
        if (warlockName.isEmpty()) {
            if (curr_time - psi.value()->lastActivity() <= 10 * 24 * 60 * 60) {
                // active no more than 10 days ago
                wsl.append(psi.value());
            }
        } else {
            if (_login.compare(psi.value()->name(), Qt::CaseInsensitive) == 0) {
                continue;
            }
            if (psi.value()->name().indexOf(warlockName, Qt::CaseInsensitive) == 0) {
                // start with
                wsl.append(psi.value());
            }
        }
    }


    if (warlockName.isEmpty()) {
        struct {
            bool operator()(const QWarlockStat *s1, const QWarlockStat *s2) const { return s1->elo() > s2->elo(); }
        } customOrderWS;
        std::sort(wsl.begin(), wsl.end(), customOrderWS);
        QList<QWarlockStat *> wsl2;
        for (int i = 0, Ln = wsl.size(); i < Ln; ++i) {
            if (_login.compare(wsl.at(i)->name(), Qt::CaseInsensitive) == 0) {
                for(int j = -4; j < 0; ++j) {
                    if (i + j >= 0) {
                        wsl2.append(wsl.at(i + j));
                    }
                }
                for(int j = 1; j < 5; ++j) {
                    if (i + j < wsl.size()) {
                        wsl2.append(wsl.at(i + j));
                    }
                }
                wsl.clear();
                wsl.append(wsl2);
                break;
            }
        }
    } else {
        struct {
            bool operator()(const QWarlockStat *s1, const QWarlockStat *s2) const { return s1->name().compare(s2->name(), Qt::CaseInsensitive) > 0; }
        } customOrderWS;
        std::sort(wsl.begin(), wsl.end(), customOrderWS);
    }
    QString res = "[";
    bool first = !warlockName.isEmpty();
    if (warlockName.isEmpty()) {
        res.append("{\"n\":\"Suggested\",\"e\":0,\"rt\":1}");
    }
    foreach(QWarlockStat *ws, wsl) {
        QString name;
        if (warlockName.isEmpty()) {
            name = ws->name();
        } else {
            name = "<b>" + ws->name().mid(0, warlockName.length()) + "</b>" + ws->name().mid(warlockName.length());
        }
        res.append(QString("%1{\"n\":\"%2\",\"e\":%3,\"l\":\"%4\",\"rt\":0}").arg(first ? "" : ",", name, warlockName.isEmpty() ? intToStr(ws->elo()) : "0", ws->name()));
        if (first) {first = false;}
    }
    if (warlockName.isEmpty()) {
        res.append(",{\"n\":\"View More\",\"e\":0,\"rt\":2}");
    }
    res.append("]");
    return res;
}

void QWarloksDuelCore::askPermissions() {
    qDebug() << "QWarloksDuelCore::askPermission";
    if (_isAsService || _isAI) {
        return;
    }
#ifdef Q_OS_ANDROID
    QJniObject javaNotification = QJniObject::fromString("");
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/androidnotifier/NotificationClient",
        "askPermission",
        "(Landroid/content/Context;Ljava/lang/String;)V",
        QNativeInterface::QAndroidApplication::context(),
        javaNotification.object<jstring>());
#else
    //_errorMsg = QString("{\"id\":-1,\"type\":18,\"action\":\"play_with_friends\",\"link\":\"%1\"}").arg(s);
    emit errorOccurred();
#endif
}

void QWarloksDuelCore::showNotification(const QString &msg) {
    qDebug() << "QWarloksDuelCore::showNotification" << msg;
    if (_isAsService || _isAI) {
        return;
    }
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
    _reg_in_app = false;
    _exp_lv = 5;
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

void QWarloksDuelCore::setupAIServer() {
    if (!_aiCore && !_isAsService && !_isAI) {
        qDebug() << "QWarloksDuelCore::timerFired" << "set AI SERVICE";
        _aiCore = new QWarloksDuelCore(nullptr, true);
        _aiCore->moveToThread(&_aiThread);
        connect(&_aiThread, &QThread::finished, _aiCore, &QObject::deleteLater);
        connect(this, &QWarloksDuelCore::needAIAnswer, _aiCore, &QWarloksDuelCore::doAIAnswer);
        connect(_aiCore, &QWarloksDuelCore::readyAIAnswer, this, &QWarloksDuelCore::checkAIAnswer);
        _aiThread.start();
    }
}

void QWarloksDuelCore::timerFired() {
    qDebug() << "QWarloksDuelCore::timerFired" << _timerCount << _isAsService << _login;
    if ((_timerCount > 0) && (--_timerCount <= 0)) {
        _timer.setInterval((_isAI && !_isAsService) ? 30000 : 60000);
    }
    //if (_login.isEmpty())
    scanState(true);
    saveParameters(false, false, true, false, false);
}

void QWarloksDuelCore::processServiceTimer() {
    //uint curr_time = QDateTime::currentSecsSinceEpoch();
    //qDebug() << "QWarloksDuelCore::processServiceTimer" << _isAsService << curr_time << _isAI;
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
    if (_isAsService || _isAI) {
        return;
    }
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

void QWarloksDuelCore::doAIAnswer(QString Login, int MagicBookLevel) {
    qDebug() << "QWarloksDuelCore::doAIAnswer" << Login << _isAsService << MagicBookLevel;
    //if (!Login.isEmpty()) {
    //    _botIdx = _lstAI.indexOf(Login.toUpper()) - 1;
    //}
    //aiLogin();
    if (_isAiBusy && Login.isEmpty()) {
        return;
    }
    _isAiBusy = !Login.isEmpty();
    _exp_lv = MagicBookLevel;
    sendGetRequest(QString(GAME_SERVER_URL_WARLOCK_GET).arg(Login));
}

void QWarloksDuelCore::checkAIAnswer(int battle_id) {
    qDebug() << "QWarloksDuelCore::checkAIAnswer" << _isAsService << battle_id;
    if (battle_id > 0) {
        QBattleInfo *bi = getBattleInfo(battle_id);
        if (bi->with_bot()) {
            scanState(true);
        }
    }
}

void QWarloksDuelCore::processWarlockPut() {
    QString data = "json=";
    foreach (int bid, _waiting_in_battles) {
        QBattleInfo *bi = getBattleInfo(bid);
        if ((bi->status() != BATTLE_INFO_STATUS_WAIT)) {
            continue;
        }
        data.append(QUrl::toPercentEncoding(bi->toString(true)));
        data.append(QUrl::toPercentEncoding("#SPLIT_POINT#"));
    }
    sendPostRequest(QString(GAME_SERVER_URL_WARLOCK_PUT).arg(_login), data.toUtf8());
}

void QWarloksDuelCore::processWarlockGet(QString &Data) {
    if (Data.indexOf("#TO_OFTEN#") != -1) {
        _isAiBusy = false;
        return;
    }
    QStringList sl = Data.split("#SPLIT_POINT#");
    QString Login = sl.at(0);
    _botIdx = _lstAI.indexOf(Login, Qt::CaseInsensitive) - 1;
    qDebug() << "QWarloksDuelCore::processWarlockGet" << Login << _botIdx;
    for(int i = 1; i < sl.size(); ++i) {
        if (sl.at(i).indexOf("id#=#") == -1) {
            continue;
        }
        QBattleInfo *bi = new QBattleInfo(sl.at(i));
        if (bi->battleID() > 0) {
            if (_battleInfo.contains(bi->battleID())) {
                _battleInfo[bi->battleID()]->setWaitFrom(bi->wait_from());
                _battleInfo[bi->battleID()]->setTurn(bi->turn());
            } else {
                _battleInfo.insert(bi->battleID(), bi);
                continue;
            }
        }

        delete bi;
    }

    aiLogin();
}

int QWarloksDuelCore::getBotBattle() {
    if (_isAsService) {
        return 0;
    }
    QBattleInfo *battle_info;
    foreach(int bid, _ready_in_battles) {
        battle_info = getBattleInfo(bid);
        if (battle_info->for_bot() || battle_info->with_bot()) {
            return bid;
        }
    }
    foreach(int bid, _waiting_in_battles) {
        battle_info = getBattleInfo(bid);
        if ((battle_info->status() != 0) && (battle_info->status() != 1) && (battle_info->status() != -1)) {
            continue;
        }
        if (battle_info->for_bot() || battle_info->with_bot()) {
            QString enemy = battle_info->getEnemy(_login);
            if (_lstAI.indexOf(enemy.toUpper()) != -1) {
                qDebug() << "emit needAIAnswer"  << enemy;
                callAI(battle_info->getEnemy(_login), _exp_lv);
                return bid;
            }
        }
    }


    return 0;
}

void QWarloksDuelCore::setSBL(int NewLevel) {
    if (_exp_lv < NewLevel) {
        _exp_lv = NewLevel;
        logEvent("level_up", QString("level;%1;").arg(intToStr(_exp_lv)));
        emit playerInfoChanged();
    }
}

void QWarloksDuelCore::setUserProperties(const QString &OS, const QString &Screen, const QString &Locale) {
    qDebug() << "QWarloksDuelCore::setUserProperties" << OS << Locale << Screen;
    _ga.setUserProperties(OS, Locale, Screen);
}

void QWarloksDuelCore::logEvent(QString EventName, QString EventParams) {
    qDebug() << "QWarloksDuelCore::logEvent" << EventName << EventParams << _isAI << _isAsService;
    if (_isAsService) {
        return;
    }
    QString final_param = EventParams;
    if (final_param.indexOf("Warlock;") == -1) {
        final_param.append(QString("Warlock;%1;").arg(_login));
    }
    if ((final_param.indexOf("Id;") == -1) && (_loadedBattleType == 1)) {
        final_param.append(QString("Id;%1;").arg(intToStr(_loadedBattleID)));
    }
    if ((final_param.indexOf("Turn;") == -1) && (_loadedBattleType == 1)) {
        final_param.append(QString("Turn;%1;").arg(intToStr(_loadedBattleTurn)));
    }
    if (final_param.indexOf("Spellbook;") == -1) {
        final_param.append(QString("Spellbook;%1;").arg(intToStr(_exp_lv)));
    }
    _ga.sendEvent(EventName, final_param);
}
