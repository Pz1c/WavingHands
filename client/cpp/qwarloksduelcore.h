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
#include <qcore.h>

//#include "qwarlockutils.h"
#include "qwarlockspellchecker.h"
#include "qwarlockdictionary.h"
#include "qgameconstant.h"


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
    Q_PROPERTY(QString targets READ targets NOTIFY targetsChanged)
    Q_PROPERTY(QString warlocks READ warlocks NOTIFY warlocksChanged)
    Q_PROPERTY(QString monsters READ monsters NOTIFY monstersChanged)
    Q_PROPERTY(QString leftGesture READ leftGesture NOTIFY leftGestureChanged)
    Q_PROPERTY(QString rightGesture READ rightGesture NOTIFY rightGestureChanged)
    Q_PROPERTY(int registerNewUser READ registerNewUser NOTIFY registerNewUserChanged)
    Q_PROPERTY(QString monsterCommandList READ monsterCommandList NOTIFY monsterCommandListChanged)
    Q_PROPERTY(QString leftHand READ leftHand NOTIFY leftHandChanged)
    Q_PROPERTY(QString rightHand READ rightHand NOTIFY rightHandChanged)
    Q_PROPERTY(int orderSubmited READ orderSubmited NOTIFY orderSubmitedChanged)
    Q_PROPERTY(int challengeSubmited READ challengeSubmited NOTIFY challengeSubmitedChanged)
    Q_PROPERTY(QString charmPerson READ charmPerson NOTIFY charmPersonChanged)
    Q_PROPERTY(QString paralyze READ paralyze NOTIFY paralyzeChanged)
    Q_PROPERTY(int timerState READ timerState NOTIFY timerStateChanged)
    Q_PROPERTY(int isDelay READ isDelay NOTIFY isDelayChanged)
    Q_PROPERTY(int isPermanent READ isPermanent NOTIFY isPermanentChanged)
    Q_PROPERTY(int isParaFDF READ isParaFDF NOTIFY isParaFDFChanged)
    Q_PROPERTY(QString fire READ fire NOTIFY fireChanged)
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
    explicit QWarloksDuelCore(QObject *parent = nullptr);
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
    QString targets();
    QString warlocks();
    QString monsters();
    QString leftGesture();
    QString rightGesture();
    int registerNewUser();
    QString monsterCommandList();
    QString leftHand();
    QString rightHand();
    int orderSubmited();
    QString charmPerson();
    QString paralyze();
    int timerState();
    int isDelay();
    int isPermanent();
    int isParaFDF();
    QString fire();
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
    QString getOnlineUrl(int battle_id = 0);
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
    void targetsChanged();
    void warlocksChanged();
    void monstersChanged();
    void leftGestureChanged();
    void rightGestureChanged();
    void registerNewUserChanged();
    void monsterCommandListChanged();
    void leftHandChanged();
    void rightHandChanged();
    void orderSubmitedChanged();
    void charmPersonChanged();
    void paralyzeChanged();
    void timerStateChanged();
    void isDelayChanged();
    void fireChanged();
    void isPermanentChanged();
    void isParaFDFChanged();
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


public slots:

    void scanState(bool Silent = false);
    void getChallengeList(bool Silent = false);
    void getTopList();
    void acceptChallenge(int battle_id, bool from_card = false);
    void rejectChallenge(int battle_id);
    void deleteMsg(QString msg_from);
    void forceSurrender(int battle_id, int turn);
    void sendOrders(QString orders);
    void setLogin(QString Login, QString Password);
    void createNewChallenge(bool Fast, bool Private, bool ParaFC, bool Maladroid, int Count, int FriendlyLevel, QString Description);
    void regNewUser(QString Login, QString Email);
    void getBattle(int battle_id, int battle_type);
    void getWarlockInfo(const QString &Login);
    void sendMessage(const QString &Msg);
    void autoLogin(int Idx);

    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors(QList<QSslError> error_list);
    void onAuthenticationRequired(QNetworkReply* reply, QAuthenticator* authenticator);
    void onProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    QString getSpellList(QString left, QString right, bool Enemy);
    QString getSpellBook();
    int getLoadedBattleTurn();

    void prepareSpellHtmlList(bool emit_signal = true, bool force_emit = false);
protected slots:
    void loginToSite();

protected:
    bool processData(QString &Data, int StatusCode, QString url, QString new_url);
    bool finishLogin(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishRegistration(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishOrderSubmit(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishCreateChallenge(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishAccept(QString &Data, int StatusCode, QUrl NewUrl);
    bool finishScan(QString &Data);
    bool finishScanWarlock(QString &Data);
    bool finishGetFinishedBattle(QString &Data);
    void finishChallengeList(QString &Data, int StatusCode, QUrl NewUrl);
    void finishTopList(QString &Data, int StatusCode, QUrl NewUrl);
    bool butifyTurnMessage();
    bool parseTargetList(QString &Data);
    bool parseUnits(QString &Data);
    bool parseMonsterCommand(QString &Data);
    bool prepareMonsterHtml();
    bool prepareWarlockHtml();
    bool parseSpecReadyBattleValues(QString &Data);

    bool parseReadyBattle(QString &Data);
    void parsePlayerInfo(QString &Data);
    void parseChallendge(QString &Data);
    void parseMessages(QString &Data);

    void setOrganization() override;
    void saveGameParameters() override;
    void loadGameParameters() override;

    void setTimeState(bool State);

    void setCredentialToAuth(QAuthenticator *authenticator);

    void processNewLogin();
    QString accountToString();
    void accountsFromString(QString acc);

    void generateBattleList();
    int parseBattleDescription(QString &Data);

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
    QStringList _lstAI;
    QStringList _activeBattle;

    // user statistic
    int _played;
    int _won;
    int _died;
    int _ladder;
    int _melee;
    int _elo;
    bool _reg_in_app;
    int _exp_lv;
    QList<int> _ready_in_battles;
    QList<int> _waiting_in_battles;
    QList<int> _finished_battles;
    QStringList _shown_battles;
    QString _finishedBattle;
    QMap<int, QString> _challenge;
    QMap<int, QString> _battleDesc;
    QStringList _msg;
    QList<QValueName> _accounts;

    // current battle
    int _loadedBattleID;
    int _loadedBattleType;
    int _loadedBattleTurn;
    QList<QValueName> _Targets;
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
    bool _isDelay;
    bool _isPermanent;
    bool _isParaFDF;
    QString _fire;
    QString _challengeList;
    QString _battleList;
    QString _spellListHtml;
    QString _prevGestures;
    QString _topList;
    QString _warlockInfo;
    QString _warlockId;

    // management
    bool _isTimerActive;

    // Spell checker
    QWarlockSpellChecker SpellChecker;
};

#endif // QWARLOKSDUELCORE_H
