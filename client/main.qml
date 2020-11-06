//import QtQuick 2.2
//import QtQuick.Window 2.1
//import QtQuick.Controls 1.1

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import ua.sp.warloksduel 1.8
import ua.sp.warlockdictionary 1.0

import "qrc:/js/main_utils.js" as MUtils
import "qrc:/js/ai_utils.js" as AI
import "qrc:/js/user_profile_utils.js" as UU
import "qrc:/js/wnd_utils.js" as WNDU
import "qrc:/js/gui_utils.js" as GUI
import "qrc:/qml/windows"
import "qrc:/qml/components"
import "qrc:/qml"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 507
    height: 900

    Keys.onPressed: {
        console.log("loader.KEY_PRESSED: " + event.key)
        if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
            console.log("loader.BACK_KEY_PRESSED: " + event.key)
            event.accepted = true;
            WNDU.processEscape();
        }
    }

    WarlocksDuelCore {
        id: core
        onNeedLogin: getLoginFromUser()
        onErrorOccurred: showErrorMessage()
        onIsLoadingChanged: MUtils.isLoadingChanged()
        onFinishedBattleChanged: showFinishedBattle()
        onReadyBattleChanged: showReadyBattle();
        onRegisterNewUserChanged: GUI.newUserRegistered()
        onOrderSubmitedChanged: MUtils.cleanOrders()
        onTimerStateChanged: changeTimerState()
        onChallengeListChanged: MUtils.loadChallengesList(false)
        onSpellListHtmlChanged: MUtils.loadSpellList()
        onTopListChanged: MUtils.loadTopList(false)
        onChallengeSubmitedChanged: MUtils.loadChallengesList(false)
        onAllowedAcceptChanged: {
            console.log("onAllowedAcceptChanged");
            closeChild();
            MUtils.loadChallengesList(false);
        }
        onWarlockInfoChanged: {
            console.log("onWarlockInfoChanged");
            showUserProfile(true);
        }
        onAccountMenuChanged: {
            console.log("onAccountMenuChanged");
            MUtils.showLoginMenu(core.getMenuItems());
        }

        Component.onCompleted: {
            console.log("Core.completed");
        }
    }

    AnalyticItem {
        id: analytics
        userId: //take uuid
            core.uuid
    }

    property var warlockDictionary: WarlockDictionary
    property real height_koeff: 1//rMain.height / 800
    property bool exit_on_back_button: true
    property var  battles: MUtils.loadChallengesList(true)
    property var  top_player: MUtils.loadTopList(true)
    property var  spells: MUtils.default_spell_list
    property bool action_send_order: true
    property string tipTxt;
    property bool allowCloseTip: false;
    property bool is_game_in_progress: false

    color: "#551470"

    header: ToolBar {
            id: tbTop
            //anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                color: "#551470"

                ToolButton {
                    id: tbProfile
                    //text: qsTr("‹")
                    height: tbTop.height
                    width: tbTop.height
                    anchors.left: parent.left

                    onClicked: showUserProfile()
                    background: Image {
                        anchors.fill: parent
                        source: "res/user.png"
                    }
                }

                LargeText {
                    id: lPlayerTitle
                    text: "Hi, " + core.login
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    fontSizeMode: Text.VerticalFit
                    color: "#FEE206"

                    anchors.left: tbProfile.right
                    anchors.right: tbMenu.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }

                ToolButton {
                    id: tbMenu
                    height: tbTop.height
                    width: tbTop.height
                    anchors.right: parent.right
                    onClicked: dMenu.open()
                    background: Image {
                        anchors.fill: parent
                        source: "res/list.png"
                    }
                }
        }
    }

    Drawer {
        id: dMenu
        width: 0.66 * mainWindow.width
        height: mainWindow.height
        edge: Qt.RightEdge

        Label {
            text: "Content goes here!"
            anchors.centerIn: parent
        }
    }

    Rectangle {
        id: rBody
        anchors.fill: parent
        color: "#210430"

        BtnBig {
            id: bbNewGame
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("NewGame")
            bg_color_active: "#551470"
            border_color_active: "#551470"

            width: 0.5 * parent.width
            height: 0.05 * parent.height
            anchors.top: parent.top
            anchors.topMargin: 0.03 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                console.log("start game")
            }
        }

        BtnBig {
            id: bbNewBotGame
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("NewGameWithBot")
            transparent: true
            border.width: 0
            visible: core.allowedAdd || true

            width: 0.5 * parent.width
            height: 0.03 * parent.height
            anchors.top: bbNewGame.bottom
            anchors.topMargin: 0.01 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                console.log("start game with bot")
            }
        }

        ScrollView {
            id: iBattleList
            anchors.top: bbNewBotGame.bottom
            anchors.topMargin: 0.01 * parent.height
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            visible: core.allowedAdd || true

            ListView {
                id: lvBattle
                model: GUI.loadChallengesList()
                delegate: Item {
                        id: idRoot
                        width: mainWindow.width
                        height: 0.1 * mainWindow.height
                        Rectangle {
                        id: rdBattleItem
                        color: "darkgrey"
                        radius: 20
                        border.width: 1
                        border.color: lvBattle.model[index].s === 1 ? "green" : "darkgrey"
                        anchors.fill: parent
                        anchors.topMargin: 0.01 * mainWindow.height
                        anchors.bottomMargin: 0.01 * mainWindow.height
                        anchors.leftMargin: 0.02 * mainWindow.width
                        anchors.rightMargin: 0.02 * mainWindow.width

                        LargeText {
                            id: rdbiText
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 0.05 * parent.width
                            anchors.right: rdbiIcon.left
                            anchors.rightMargin: 0.05 * parent.width
                            color: "#2DA0A5"
                            text: lvBattle.model[index].d ? lvBattle.model[index].d : "Battle #" + lvBattle.model[index].id
                        }

                        Image {
                            id: rdbiIcon
                            source: "res/fight.png"
                            width: parent.height
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                        }
                    }
                }
            }
        }
    }

    function addDuel() {
        console.log("add duel");
        if (core.allowedAdd) {
            MUtils.showWindow("new_challenge.qml");
        } else {
            showTipMessage(warlockDictionary.getStringByCode("WaitTrainingBattle"), true);
        }
    }

    function openRecords() {
        console.log("openRecords");
        core.getTopList();
        MUtils.showTop();
    }

    function refreshData() {
        console.log("user start scanning");
        core.scanState()
    }

    function showMainMenu() {
        //WNDU.showMainMenu();
        dMenu.open();
    }

    function showUserProfile(other) {
        console.log("showUserProfile", other);
        Qt.show_info_self = other === true ? false : true;
        WNDU.showProfileWindow();
    }

    function getLoginFromUser(real_login) {
        if (real_login) {
            WNDU.showLogin();
        } else {
            showNewUserMenu();
        }
    }

    function showProxySettings(child_wnd) {
        MUtils.showWindow("edit_network.qml", child_wnd);
    }

    function showLangSettings(child_wnd) {
        MUtils.showWindow("edit_lang.qml", child_wnd);
    }

    function showRegisterUser(child_wnd) {
        console.log("showNewUserMenu")
        if (child_wnd) {
            child_wnd.destroy()
        }

        MUtils.showNewUserWnd();
    }

    function closeUserProfile() {
        WNDU.closeChild();
        if (MUtils.wndUP) {
            MUtils.wndUP.destroy();
            MUtils.wndUP = null;
        }
    }

    function showTipMessage(TipTxt, allowClose) {
        console.log("Tip: ", TipTxt, allowClose);
        allowCloseTip = allowClose ? true : false;
        WNDU.showErrorWnd({text:TipTxt,type:1});
    }

    function showErrorMessage() {
        console.log("Error: ", Qt.core.errorMsg)
        if (!core.isAI) {
            WNDU.showErrorWnd({text:Qt.core.errorMsg});
        }
    }

    function showFinishedBattle() {
        console.log("showFinishedBattle")
        WNDU.showErrorWnd({text:Qt.core.finishedBattle,type:1});
        //MUtils.showWindow("finished_battle.qml");
    }

    function showReadyBattle() {
        console.log("showReadyBattle", allowCloseTip);
        if (allowCloseTip) {
            closeChild();
        }
        MUtils.showReadyBattle();
    }

    function openBattleOnline() {
        WNDU.closeChild();
        Qt.openUrlExternally(core.getOnlineUrl());
    }

    function showNewUserMenu() {
        logEvent("registration_started");
        WNDU.showNewUserMenu();
        //MUtils.showWindow("new_user_menu.qml", child_wnd);
    }

    function showSpellDetails(spell_code) {
        WNDU.showErrorWnd({text:getSpellDescription(spell_code),type:1});
        //MUtils.showWindow("spell_details.qml");
    }

    function hideNewUserMenu() {
        console.log("hideNewUserMenu")
        MUtils.hideNewUserWnd()
    }

    function processEscape() {
        WNDU.processEscape();
    }

    function storeWnd(wnd) {
        //WNDU.wndTmp = wnd;
    }

    function closeChild() {
        WNDU.closeChild();
    }

    function getSpellDescription(currSpell) {
        return "<h2>" + warlockDictionary.getStringByCode(currSpell) + "</h2><p>" + warlockDictionary.getStringByCode(currSpell + "_desc") + "</p>"
    }

    function linkActivated(link) {
        MUtils.linkActivated(link)
    }

    function changeTimerState() {
        if (Qt.core.timerState === 1) {
            if (!tScanTimer.running) {
                tScanTimer.start()
            }
        } else {
            if (tScanTimer.running) {
                tScanTimer.stop()
            }
        }
    }

    function changeGesture(Gesture, Left) {
        MUtils.changeGesture(Gesture, Left);
    }

    /*function changeLanguage(new_lang, child_wnd) {
        if (child_wnd) {
            child_wnd.destroy();
        }
        warlockDictionary.setCurrentLang(new_lang)
        setNewLanguage()
    }

    function setNewLanguage() {
        MUtils.default_spell_list = JSON.parse(core.defaultSpellListHtml);
        Qt.core.prepareSpellHtmlList(1==1, 1==1);
        var tmp_l_arr = [warlockDictionary.getStringByCode("Neither"),
                         warlockDictionary.getStringByCode("Left"),
                         warlockDictionary.getStringByCode("Right"),
                         warlockDictionary.getStringByCode("Two-handed")]
        tLHGLabel.text = warlockDictionary.getStringByCode("LeftHand")
        tRHGLabel.text = warlockDictionary.getStringByCode("RightHand")
        tDelayLabel.text = warlockDictionary.getStringByCode("DelaySpell")
        cbDelay.model = tmp_l_arr
        tPermanentLabel.text = warlockDictionary.getStringByCode("MakeSpellPermanent")
        cbPermanent.model = tmp_l_arr
        tChatMessage.text = warlockDictionary.getStringByCode("ChatMessage")
        labelDoit.text = warlockDictionary.getStringByCode("Submit")
        MUtils.cChatMessage = warlockDictionary.getStringByCode("ChatMessage")
        MUtils.cMosterFrom = warlockDictionary.getStringByCode("MonsterFrom") + " "
    }*/

    function logEvent(event_name, params) {
        if (core.isAI) {
            return;
        }

        if (!params) {
            params = {};
        }

        console.log("logEvent", event_name, JSON.stringify(params));
        analytics.logEvent(event_name, params);
    }

    function loadingStop() {
        MUtils.hideLoading();
    }

    function creationFinished() {
        console.log("main.creationFinished");
        Qt.core = core;
        Qt.mainWindow = mainWindow;
        Qt.gameField = mainWindow;
        Qt.height_coeff = height_koeff;
        Qt.dictionary = warlockDictionary;
        MUtils.dict = warlockDictionary;
        MUtils.cChatMessage = warlockDictionary.getStringByCode("ChatMessage");
        MUtils.cMosterFrom = warlockDictionary.getStringByCode("MonsterFrom") + " ";
        MUtils.default_spell_list = JSON.parse(core.defaultSpellListHtml);
        showNewUserMenu();
        /*if (core.login === '') {
            showNewUserMenu();
        } else {
            core.scanState();
        }*/
        /*MUtils.cleanOrders();
        MUtils.showList();
        MUtils.showLoginMenu(core.accountMenu);
        */
        //tipTxt = warlockDictionary.getStringByCode("JustRegistered");
        //showTipMessage(true);
        logEvent("gameFieldReady");
    }

    Component.onCompleted: creationFinished()
}
