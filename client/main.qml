//import QtQuick 2.2
//import QtQuick.Window 2.1
//import QtQuick.Controls 1.1

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import ua.sp.warloksduel 2.0
import ua.sp.warlockdictionary 1.0

import "qrc:/js/gui_utils.js" as GUI
import "qrc:/js/main_utils.js" as MUtils
import "qrc:/js/ai_utils.js" as AI
import "qrc:/js/user_profile_utils.js" as UU
import "qrc:/js/wnd_utils.js" as WNDU
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

    Dialog {
        id: mdNoGesture
        title: warlockDictionary.getStringByCode("AreYouSure")
        //icon: StandardIcon.Warning
        property alias text: ltModal.text
        contentItem: LargeText {
            id: ltModal
            text: warlockDictionary.getStringByCode("NoGestureForTurn")
        }
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        property bool isSendOrderAction: false

        onAccepted: {
            if (isSendOrderAction) {
                BU.sendOrderEx();
            } else {
                Qt.quit();
            }
        }
    }

    property int timerCounter: 0
    Timer {
        id: tScanTimer
        interval: core.isAI ? 20000 : 60000
        running: false
        repeat: true

        onTriggered: {
            console.log("start scanning");
            core.scanState(true);
            if (--timerCounter <= 0) {
                tScanTimer.interval = core.isAI ? 10000 : 60000;
            }
        }
    }

    property alias gameCore: core
    property var gERROR: ({})
    property var gBattle: ({})

    WarlocksDuelCore {
        id: core
        onNeedLogin: getLoginFromUser()
        onErrorOccurred: showErrorMessage()
        onIsLoadingChanged: GUI.isLoadingChanged(core)
        onBattleListChanged: GUI.newBattleList(core)
        onFinishedBattleChanged: showFinishedBattle()
        onReadyBattleChanged: showReadyBattle();
        onRegisterNewUserChanged: GUI.newUserRegistered(core)
        //onOrderSubmitedChanged: MUtils.cleanOrders(core)
        onTimerStateChanged: changeTimerState()
        //onChallengeListChanged: MUtils.loadChallengesList(false)
        //onSpellListHtmlChanged: MUtils.loadSpellList(core)
        //onTopListChanged: MUtils.loadTopList(false)
        //onChallengeSubmitedChanged: MUtils.loadChallengesList(false)
        onAllowedAcceptChanged: {
            console.log("onAllowedAcceptChanged");
            //closeChild();
            //MUtils.loadChallengesList(false);
        }
        onWarlockInfoChanged: {
            console.log("onWarlockInfoChanged");
            showUserProfile(true);
        }
        onAccountMenuChanged: {
            console.log("onAccountMenuChanged");
            GUI.showLoginMenu(core.getMenuItems());
        }

        Component.onCompleted: {
            console.log("Core.completed");
            if (core.login === '') {
                showNewUserMenu();
            } else {
                core.scanState();
            }
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
    //property var  battles: MUtils.loadChallengesList(true)
    //property var  top_player: MUtils.loadTopList(true)
    //property var  spells: MUtils.default_spell_list
    property bool action_send_order: true
    property string tipTxt;
    //property bool allowCloseTip: false;
    property bool is_game_in_progress: false

    color: "#551470"

    Rectangle {
        id: rLoading
        anchors.fill: parent
        color: "#000000"
        opacity: 0.6
        z:1000

        BusyIndicator {
            id: aiLoading
            anchors.centerIn: parent
            running: rLoading.visible
            width: 0.5 * parent.width
            height: 0.5 * parent.width
        }

        MouseArea {
            anchors.fill: parent
        }
    }

    header: ToolBar {
            id: tbTop
            //anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                color: "#210430"

                ToolButton {
                    id: tbProfile
                    //text: qsTr("‹")
                    height: tbTop.height
                    width: tbTop.height
                    anchors.left: parent.left

                    onClicked: showUserProfile()
                    background: Image {
                        anchors.fill: parent
                        source: "res/trophy.png"
                    }
                }

                LargeText {
                    id: lPlayerTitle
                    text: "Hi, " + core.login
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    fontSizeMode: Text.VerticalFit
                    color: "#FEE2D6"

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
                        source: "res/menu.png"
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

    Image {
        id: iBG
        source: "res/background.png"
        anchors.fill: parent
        z: -1
    }

    Item {
        id: rBody
        anchors.fill: parent
        //color: "#210430"

        BtnBig {
            id: bbNewGame
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("NewGame")
            bg_color_active: "#551470"
            border_color_active: "#551470"
            radius: 30

            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FEE2D6" }
                GradientStop { position: 0.5; color: "#551470" }
                GradientStop { position: 1.0; color: "#FEE2D6" }
            }

            width: 0.5 * parent.width
            height: 0.1 * parent.height
            anchors.top: parent.top
            anchors.topMargin: 0.03 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                console.log("start game")
            }
        }

        BtnBig {
            id: bbNewBotGame
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("NewGameWithBot")
            transparent: true
            border.width: 0
            visible: core.allowedAdd || true

            width: 0.5 * parent.width
            height: 0.05 * parent.height
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
            //anchors.leftMargin: 0.05 * mainWindow.width
            anchors.right: parent.right
            //anchors.rightMargin: 0.05 * mainWindow.width
            visible: core.allowedAdd || true
            contentHeight: iMainScrollBody.height

            Item {
                id:iMainScrollBody
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: ltActiveBattle.height + lvActiveBattle.height + ltFinishedBattle.height + lvFinishedBattle.height

                LargeText {
                    id: ltActiveBattle
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 0.05 * mainWindow.height
                    horizontalAlignment: Text.AlignLeft
                    fontSizeMode: Text.VerticalFit
                    color: "#2DA0A5"
                    text: warlockDictionary.getStringByCode("ActiveGameTitle")
                }


                ListView {
                    id: lvActiveBattle
                    model: GUI.loadBattleList(1)
                    anchors.top: ltActiveBattle.bottom
                    anchors.topMargin: 0.01 * mainWindow.height
                    width: 0.9 * mainWindow.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: model.length * 0.08 * mainWindow.height
                    delegate: Item {
                            id: idRoot
                            width: lvActiveBattle.width
                            height: 0.08 * mainWindow.height
                            Rectangle {
                            id: rdBattleItem
                            color: "#544653"
                            radius: 30
                            anchors.fill: parent
                            anchors.topMargin: 0.01 * mainWindow.height
                            anchors.bottomMargin: 0.01 * mainWindow.height
                            /*anchors.leftMargin: 0.02 * mainWindow.width
                            anchors.rightMargin: 0.02 * mainWindow.width*/

                            LargeText {
                                id: rdbiText
                                anchors.top: rdBattleItem.top
                                anchors.bottom: rdBattleItem.bottom
                                anchors.left: rdBattleItem.left
                                anchors.leftMargin: 0.03 * parent.width
                                width: 0.90 * parent.width - parent.height
                                //anchors.rightMargin: 0.05 * parent.width
                                color: "#FEE2D6"
                                horizontalAlignment: Text.AlignLeft

                                text: lvActiveBattle.model[index].d
                            }

                            Image {
                                id: rdbifIcon
                                source: lvActiveBattle.model[index].s === 1 ? "res/circle.png" : "res/wait.png"
                                width: 0.5 * parent.height
                                height: 0.5 * parent.height
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: rdbifSign.left
                            }

                            LargeText {
                                id: rdbifSign
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                anchors.rightMargin: 0.05 * parent.width
                                width: 0.05 * parent.width
                                color: "#FEE2D6"
                                fontSizeMode: Text.VerticalFit
                                text: ">"
                            }

                            MouseArea {
                                id: maActiveBattle
                                anchors.fill: parent
                                onClicked: {
                                    console.log("getBattle", index, JSON.stringify(lvActiveBattle.model[index]));
                                    core.getBattle(lvActiveBattle.model[index].id, lvActiveBattle.model[index].s);
                                }
                            }
                        }
                    }
                }

                LargeText {
                    id: ltFinishedBattle
                    anchors.top: lvActiveBattle.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 0.05 * mainWindow.height
                    horizontalAlignment: Text.AlignLeft
                    fontSizeMode: Text.VerticalFit
                    color: "#2DA0A5"
                    text: warlockDictionary.getStringByCode("FinishedGameTitle")
                }

                ListView {
                    id: lvFinishedBattle
                    model: GUI.loadBattleList(2)
                    anchors.top: ltFinishedBattle.bottom
                    anchors.topMargin: 0.01 * mainWindow.height
                    width: 0.9 * mainWindow.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: model.length * 0.08 * mainWindow.height
                    delegate: Item {
                            id: idfRoot
                            width: lvFinishedBattle.width
                            height: 0.08 * mainWindow.height
                            Rectangle {
                            id: rdfBattleItem
                            radius: 30
                            color: "#544653"
                            anchors.fill: parent
                            anchors.topMargin: 0.01 * mainWindow.height
                            anchors.bottomMargin: 0.01 * mainWindow.height
                            /*anchors.leftMargin: 0.02 * mainWindow.width
                            anchors.rightMargin: 0.02 * mainWindow.width*/

                            LargeText {
                                id: rdbifText
                                anchors.top: rdfBattleItem.top
                                anchors.bottom: rdfBattleItem.bottom
                                anchors.left: rdfBattleItem.left
                                anchors.leftMargin: 0.03 * parent.width
                                width: 0.90 * parent.width - parent.height
                                color: "#10C9F5"
                                fontSizeMode: Text.VerticalFit
                                horizontalAlignment: Text.AlignLeft

                                text: lvFinishedBattle.model[index].d
                            }

                            Image {
                                id: rdbiIcon
                                source: lvFinishedBattle.model[index].d.indexOf(core.login) === -1 ? "res/RIP2.png" : "res/stars.png"
                                width: 0.5 * parent.height
                                height: 0.5 * parent.height
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 0.05 * parent.width
                            }

                            MouseArea {
                                id: maFinishedBattle
                                anchors.fill: parent
                                onClicked: {
                                    console.log("getFinishedBattle", index, JSON.stringify(lvFinishedBattle.model[index]));
                                    core.getBattle(lvFinishedBattle.model[index].id, lvFinishedBattle.model[index].s);
                                }
                            }
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
        console.log("Error: ", core.errorMsg)
        if (!core.isAI) {
            WNDU.showErrorWnd({text:core.errorMsg});
        }
    }

    function showFinishedBattle() {
        console.log("showFinishedBattle")
        WNDU.showErrorWnd({text:core.finishedBattle,type:1,title:"Battle #" + core.loadedBattleID});
        //MUtils.showWindow("finished_battle.qml");
    }

    function showReadyBattle() {
        var battle_str = core.battleInfo();
        console.log("showReadyBattle", battle_str);
        gBattle = JSON.parse(battle_str);
        WNDU.showBattle();
    }

    function openBattleOnline() {
        WNDU.closeChild();
        Qt.openUrlExternally(core.getOnlineUrl());
    }

    function showNewUserMenu() {
        logEvent("registration_started");
        WNDU.showNewUserMenu();
    }

    function showSpellDetails(spell_code) {
        logEvent("spell_details", {code:spell_code});
        WNDU.showErrorWnd({text:warlockDictionary.getStringByCode(spell_code + "_desc"),type:1,title:warlockDictionary.getStringByCode(spell_code)});
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


    function linkActivated(link) {
        MUtils.linkActivated(link)
    }

    function changeTimerState() {
        if (core.timerState === 1) {
            if (!tScanTimer.running) {
                tScanTimer.start();
            }
        } else {
            if (tScanTimer.running) {
                tScanTimer.stop();
            }
        }
    }

    function changeGesture(Gesture, Left) {
        MUtils.changeGesture(Gesture, Left);
    }

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
        //core = core;
        GUI.G_CORE = core;
        GUI.G_GAME_FIELD = mainWindow;
        MUtils.dict = warlockDictionary;
        MUtils.cChatMessage = warlockDictionary.getStringByCode("ChatMessage");
        MUtils.cMosterFrom = warlockDictionary.getStringByCode("MonsterFrom") + " ";
        MUtils.default_spell_list = JSON.parse(core.defaultSpellListHtml);
        logEvent("gameFieldReady");
    }

    Component.onCompleted: creationFinished()
}
