#ifndef QWARLOKSDUELCORE_H
#define QWARLOKSDUELCORE_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QNetworkProxy>
#include <QList>
#include <QPair>
#include <QStandardPaths>
#include <QClipboard>
#include <QGuiApplication>
#ifdef Q_OS_ANDROID
#include <QJniObject>
#endif
#include <QTimer>
#include <QThread>

#include <qcore.h>
#include <qgoogleanalytics.h>
//#include "qwarlockutils.h"
#include "qwarlockspellchecker.h"
#include "qwarlockdictionary.h"
#include "qgameconstant.h"
#include "qwarlock.h"
#include "qwarlockstat.h"
#include "qbattleinfo.h"


class QWarloksDuelCore : public QGameCore
{
    Q_OBJECT
    Q_PROPERTY(bool isNeedLogin READ isNeedLogin NOTIFY needLogin)
    Q_PROPERTY(QString login READ login NOTIFY loginChanged)
    Q_PROPERTY(QString password READ password NOTIFY passwordChanged)
    Q_PROPERTY(QString errorMsg READ errorMsg NOTIFY errorOccurred)
    Q_PROPERTY(QString playerInfo READ playerInfo NOTIFY playerInfoChanged)
    Q_PROPERTY(QString readyInBattles READ readyInBattles NOTIFY readyInBattlesChanged)
    Q_PROPERTY(QString waitingInBattles READ waitingInBattles NOTIFY waitingInBattlesChanged)
    Q_PROPERTY(QString finishedBattles READ finishedBattles NOTIFY finishedBattlesChanged)
    Q_PROPERTY(QString finishedBattle READ finishedBattle NOTIFY finishedBattleChanged)
    Q_PROPERTY(int readyBattle READ readyBattle NOTIFY readyBattleChanged)
    Q_PROPERTY(int loadedBattleID READ loadedBattleID NOTIFY loadedBattleIDChanged)
    Q_PROPERTY(int registerNewUser READ registerNewUser NOTIFY registerNewUserChanged)
    Q_PROPERTY(int orderSubmited READ orderSubmited NOTIFY orderSubmitedChanged)
    Q_PROPERTY(int challengeSubmited READ challengeSubmited NOTIFY challengeSubmitedChanged)
    Q_PROPERTY(int timerState READ timerState NOTIFY timerStateChanged)
    Q_PROPERTY(QString challengeList READ challengeList NOTIFY challengeListChanged)
    Q_PROPERTY(QString spellListHtml READ spellListHtml NOTIFY spellListHtmlChanged)
    Q_PROPERTY(QString defaultSpellListHtml READ defaultSpellListHtml NOTIFY defaultSpellListHtmlChanged)
    Q_PROPERTY(QString topList READ topList NOTIFY topListChanged)
    Q_PROPERTY(bool isAI READ isAI NOTIFY isAIChanged)
    Q_PROPERTY(bool allowedAccept READ allowedAccept NOTIFY allowedAcceptChanged)
    Q_PROPERTY(bool allowedAdd READ allowedAdd NOTIFY allowedAddChanged)
    Q_PROPERTY(QString warlockInfo READ warlockInfo NOTIFY warlockInfoChanged)
    Q_PROPERTY(QString accountMenu READ accountMenu NOTIFY accountMenuChanged)
    Q_PROPERTY(QString battleList READ battleList NOTIFY battleListChanged)

public:
    explicit QWarloksDuelCore(QObject *parent = nullptr, bool AsService = false);
    ~QWarloksDuelCore();

    bool isNeedLogin();
    QString login();
    QString password();
    QString errorMsg();
    QString playerInfo();
    QString readyInBattles();
    QString waitingInBattles();
    QString finishedBattles();
    QString finishedBattle();
    int readyBattle();
    int loadedBattleID();
    int registerNewUser();
    int orderSubmited();
    int timerState();
    int challengeSubmited();
    QString challengeList();
    QString spellListHtml();
    QString defaultSpellListHtml();
    QString topList();
    bool isAI();
    bool allowedAdd();
    bool allowedAccept();
    QString warlockInfo();
    QString accountMenu();
    QString battleList();


signals:
    void needLogin();
    void loginChanged();
    void passwordChanged();
    void errorOccurred();
    void playerInfoChanged();
    void readyInBattlesChanged();
    void waitingInBattlesChanged();
    void finishedBattlesChanged();
    void finishedBattleChanged();
    void readyBattleChanged();
    void loadedBattleIDChanged();
    void registerNewUserChanged();
    void orderSubmitedChanged();
    void timerStateChanged();
    void challengeSubmitedChanged();
    void challengeListChanged();
    void spellListHtmlChanged();
    void defaultSpellListHtmlChanged();
    void isAIChanged();
    void allowedAddChanged();
    void allowedAcceptChanged();
    void topListChanged();
    void warlockInfoChanged();
    void accountMenuChanged();
    void battleListChanged();
    void needAIAnswer(QString Login, int MagicBookLevel);
    void readyAIAnswer(int battle_id);

public slots:

    QString getOnlineUrl(int battle_id = 0);
    void scanState(bool Silent = false);
    void getChallengeList(bool Silent = false);
    void scanTopList(bool Silent = false, bool ForceFull = false);
    void acceptChallenge(int battle_id, bool from_card = false);
    bool aiAcceptChallenge(int battle_id, bool changeAI = true);
    void rejectChallenge(int battle_id);
    void deleteMsg(QString msg_from);
    void forceSurrender(int battle_id, int turn);
    void sendOrders(QString orders);
    void setLogin(QString Login, QString Password);
    void createNewChallenge(bool Fast, bool Private, bool ParaFC, bool Maladroid, int Count, int FriendlyLevel, QString Description, QString Warlock = "");
    void aiCreateNewChallenge();
    void regNewUser(const QString &Login, const QString &Email, const QString &Pass = "");
    void getBattle(int battle_id, int battle_type);
    void getWarlockInfo(const QString &Login);
    void sendMessage(const QString &Msg);
    void autoLogin(int Idx);
    void aiLogin();
    void logout();
    void leaveBattle(int battle_id, int warlock_id = 0);
    void setParamValue(const QString &Parameter, const QString &Value);
    QString getWarlockStats(const QString &WarlockName, bool DirtyLogin = false);
    void getSharableLink(const QString &game_level = "vf");
    void setTimerInterval(int count, int msec);
    int getBotBattle();
    void setUserProperties(const QString &OS, const QString &Screen, const QString &Locale);
    void logEvent(QString EventName, QString EventParams);


    void slotReadyRead() override;
    void slotError(QNetworkReply::NetworkError error) override;
    void slotSslErrors(QList<QSslError> error_list) override;

    void onAuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);
    void onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    QString getSpellList(QString left, QString right, bool Enemy);
    QString getSpellBook();
    int getLoadedBattleTurn();
    QString battleInfo();
    QString playerJson();
    QString getTopList(bool ShowAll = false);

    void prepareSpellHtmlList(bool emit_signal = true, bool force_emit = false);
    void setupAIServer();

    void setSBL(int NewLevel);
    // https://github.com/Pz1c/WavingHands/issues/268
    void showNotification(const QString &msg);
protected slots:
    void loginToSite();
    void timerFired();
    void processServiceTimer();
    void doAIAnswer(QString Login, int MagicBookLevel);
    void checkAIAnswer(int battle_id);

protected:
    bool processData(QString &Data, int StatusCode, QString url, QString new_url);
    bool finishLogin(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishRegistration(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishOrderSubmit(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishCreateChallenge(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishAccept(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishScan(QString &Data, bool ForceBattleList = false);
    bool finishScanWarlock(QString &Data);
    bool finishGetFinishedBattle(QString &Data);
    void finishChallengeList(QString &Data, int StatusCode, QUrl NewUrl);
    void finishTopList(QString &Data, int StatusCode, QUrl NewUrl);
    bool butifyTurnMessage(QString &str, bool CleanGestures = false);
    bool parseTargetList(QString &Data);
    bool parseUnits(QString &Data);
    bool parseMonsterCommand(QString &Data);
    bool prepareMonsterHtml();
    bool prepareWarlockHtml();
    bool parseSpecReadyBattleValues(QString &Data);
    void setPossibleSpell(const QString &Data);
    void calcBattleDecision();
    void prepareBattleChatAndHistory(QString &Data);
    void processRefferer();
    void setCheckUrl(const QString &check_url);

    bool parseReadyBattle(QString &Data);
    void parsePlayerInfo(QString &Data, bool ForceBattleList = false);
    void parseChallendge(QString &Data);
    void parseMessages(QString &Data);

    //void setOrganization() override;
    void saveGameParameters() override;
    void loadGameParameters() override;

    void setTimeState(bool State);

    void setCredentialToAuth(QAuthenticator *authenticator);

    void processNewLogin(bool Silent = false);
    QString accountToString();
    void accountsFromString(QString acc);

    void generateBattleList();
    int parseBattleDescription(QString &Data);

    void generateTopList();

    QString getHintArray(int hint_id);
    QString getBattleHint(QBattleInfo *battle_info);

    QBattleInfo *getBattleInfo(int battleId);

    void storeFullParsedBattle(QBattleInfo *bi);
    QString prepareBattleOrders();

    void processWarlockGet(QString &Data);
    void processWarlockPut();
    void callAI(QString Login, int MagicBookLevel);
private:
    // user login
    bool _isLogined;
    bool _isAI;
    bool _allowedAdd;
    bool _allowedAccept;
    int _botIdx;
    QString _login;
    QString _password;
    QString _errorMsg;
    QStringList _activeBattle;

    // user statistic
    int _played;
    int _won;
    int _died;
    int _ladder;
    int _melee;
    int _elo;
    int _exp_lv;
    int _hint1;
    bool _feedback;
    bool _rateus;
    bool _show_hint;
    bool _reg_in_app;
    bool _process_refferer;
    bool _event_start_training;
    bool _event_start_pvp;
    bool _event_submit_turn;
    bool _event_submit_turn5;
    QList<int> _ready_in_battles;
    QList<int> _waiting_in_battles;
    QList<int> _finished_battles;
    //QStringList _shown_battles;
    QString _finishedBattle;
    QStringList _challenge;
    int _win_vs_bot;
    int _win_vs_warlock;
    int _play_training_game;
    int _play_pvp_game;
    //QMap<int, QString> _battleDesc;
    //QMap<int, int> _battleHint;
    //QMap<int, int> _battleState; // -2 deleted, -1 not started, 0 wait, 1 ready, 2 finished
    //QMap<int, int> _battleWait;
    QStringList _msg;
    QList<QValueName> _accounts;
    qint64 _lastPlayersScan;
    QMap<QString, QWarlockStat> _playerStats;
    //QMap<int, QStringList> _battleHistory;
    //QMap<int, QStringList> _battleChat;
    QString _inviteToBattle;
    QMap<int, QBattleInfo *> _battleInfo;
    QBattleInfo* _newBattle;

    // current battle
    int _loadedBattleID;
    int _loadedBattleType;
    int _loadedBattleTurn;
    QList<QValueName> _Targets;
    QMap<QString, QString> _WarlockID;
    QList<QMonster *> _Monsters;
    QString _MonstersHtml;
    QList<QWarlock *> _Warlock;
    QString _WarlockHtml;
    QString _possibleLeftGestures;
    QString _possibleRightGestures;
    QString _leftGestures;
    QString _rightGestures;
    QString _monsterCommandList;
    QString _charmPersonList;
    QString _paralyzeList;
    QString _extraOrderInfo;
    QString _paralyzedHands;
    bool _isDelay;
    bool _isPermanent;
    bool _isParaFDF;
    bool _isParaFC;
    bool _isMaladroit;
    QString _fire;
    QString _challengeList;
    QString _battleList;
    QString _spellListHtml;
    QString _prevGestures;
    QString _topList;
    QString _warlockInfo;
    QString _warlockId;
    QString _chat;
    QWarlock *_enemy;
    QWarlock *_player;
    QString _topActive;
    QString _topAll;

    // management
    bool _isTimerActive;
    bool _isScanForced;
    bool _isAsService;
    bool _isAiBusy;
    qint64 _lastAnyAiCall;

    // Spell checker
    QWarlockSpellChecker *SpellChecker;

    //time
    int _timerCount;
    int _timerInterval;
    QTimer _timer;
    QTimer _serviceTimer;

    // AI service
    QWarloksDuelCore *_aiCore;
    QThread _aiThread;

    //GA4
    QGoogleAnalytics _ga;

};

#endif // QWARLOKSDUELCORE_H
