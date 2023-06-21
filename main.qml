import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import ua.sp.warloksduel 2.0
import ua.sp.warlockdictionary 1.0
//import ua.sp.GoogleAnalytics 1.0

import "qrc:/js/game_constant.js" as GC
import "qrc:/js/gui_utils.js" as GUI
//import "qrc:/js/ga.js" as GA
//import "qrc:/js/main_utils.js" as MUtils
//import "qrc:/js/ai_utils.js" as AI
import "qrc:/js/user_profile_utils.js" as UU
import "qrc:/js/wnd_utils.js" as WNDU
import "qrc:/qml/windows"
import "qrc:/qml/components"
import "qrc:/qml"


ApplicationWindow {
    id: mainWindow
    visible: true
    width: 410//270
    height: 720//600
    flags: /*Qt.FramelessWindowHint//|*/Qt.Window
    color: "#551470"
    visibility: isMobile() ? Window.FullScreen : Window.Windowed
    property real ratioObject: 1
    property real ratioFont: 1
    property string afterCloseAction: ""
    property alias gameCore: core
    property alias mainContainer: iWndContainer
    property alias mainHeader: tbTop
    property var gERROR: ({})
    property var gBattle: ({})
    property int playerSpellbookLevel: 6
    property var warlockDictionary: WarlockDictionary
    //property var googleAnalytic: GoogleAnalytics
    property real height_koeff: 1
    property bool exit_on_back_button: true
    property bool action_send_order: true
    property string tipTxt;
    //property bool allowCloseTip: false;
    property bool is_game_in_progress: false
    property alias keyListener: iWndContainer

    property int realScreenWidth: 0
    property int realScreenHeight: 0
    property real calculatedRatio: 0
    property real calculatedRatioFont: 0

    Dialog {
        id: mdNoGesture
        title: warlockDictionary.getStringByCode("AreYouSure")
        anchors.centerIn: parent
        //icon: StandardIcon.Warning
        property alias text: ltModal.text
        contentItem: LargeText {
            id: ltModal
            text: warlockDictionary.getStringByCode("NoGestureForTurn")
        }
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel

        property int dialogType: 0

        onAccepted: {
            switch(dialogType) {
            case 0:
                logEvent("app_closed", {});
                return Qt.quit();
            case 1: return confirmOrdersEx();
            case 2: return joinBattleDialogResult(true);
            case 3: return core.logout();
            }
        }

        onRejected: {
            if (dialogType == 2) {
                joinBattleDialogResult(false);
            }
        }
    }

    WarlocksDuelCore {
        id: core
        onNeedLogin: getLoginFromUser()
        onErrorOccurred: showErrorMessage()
        onIsLoadingChanged: GUI.isLoadingChanged()
        onBattleListChanged: GUI.newBattleList()
        onFinishedBattleChanged: showFinishedBattle()
        onReadyBattleChanged: showReadyBattle();
        onRegisterNewUserChanged: GUI.newUserRegistered()
        //onTimerStateChanged: changeTimerState()
        onChallengeListChanged: GUI.loadChallengeList()
        onAllowedAcceptChanged: {
            console.log("onAllowedAcceptChanged");
        }
        onWarlockInfoChanged: {
            console.log("onWarlockInfoChanged");
            showUserProfile(true);
        }
        onPlayerInfoChanged: GUI.userProfileChanged()
        onLoginChanged: {
            closeChild(true);
            //showAfterRegWnd();
        }
        onTopListChanged: {
                if (needTopList === 1) {
                    needTopList = 0;
                    showHallOfFameWindow();
                }
            }
        Component.onCompleted: {
            console.log("Core.completed");
            core.setupAIServer();
            if (core.login === '') {
                tbTop.visible = false;
                rBody.visible = false;
                showNewUserMenu();
                //showAfterRegWnd();
            } else {
                //core.scanState();
                logEvent("auto_login_start", {});
                core.setTimerInterval(1, 0);
                GUI.prepareLoginMenu(core.accountMenu);
            }
        }
    }

    /*AnalyticItem {
        id: analytics
        userId: //take uuid
            core.uuid
    }*/

    Item {
        id: iWndContainer
        anchors.fill: parent
        z: 500
        focus: true

        Keys.onBackPressed: processEscape();
        Keys.onEscapePressed: processEscape();
        Keys.onCancelPressed: processEscape();
        Keys.onPressed: function (event) {logKeyEvent(event)};

        //Keys.on
    }

    function logKeyEvent(event) {
        console.log("LOG_KEY_EVENT", JSON.stringify(event));
    }

    Rectangle {
        id: rLoading
        anchors.fill: parent
        color: "#000000"
        opacity: 0.6
        z:1000
        visible: false

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

    Drawer {
        id: dMenu
        width: 516 * ratioObject
        height: mainWindow.height
        edge: Qt.LeftEdge

        Rectangle {
            id: dMenuBG
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#544653" }
                GradientStop { position: 1.0; color: "#210430" }
            }

            Image {
                id: iBGimg
                source: "res/stars_bg.png"
                anchors.fill: parent
            }

            z: -1
        }

        Item {
            id: dmITop
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 188 * ratioObject
            z: 10

            Text {
                id: dmIHi
                anchors.top: parent.top
                anchors.topMargin: 108 * ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 48 * ratioObject
                anchors.right: parent.right
                font.pixelSize: 42 * ratioFont
                color: "#E7FFFF"
                text: "Hi, " + core.login
            }

            Rectangle {
                id: dmRLine
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 2 * ratioObject
                color: "#E7FFFF"
            }
        }

        ScrollView {
            id: iMenuItems
            anchors.top: dmITop.bottom
            anchors.topMargin: 24 * ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 48 * ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 48 * ratioObject
            anchors.bottom: bbNewGameWithFriend.top
            anchors.bottomMargin: 24 * ratioObject
            contentWidth: -1
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            z: 9


            ListView {
                id: lvmItem
                model: GUI.getMainMenuList()

                delegate: Item {
                        id: idmiRoot
                        width: 420 * ratioObject
                        height: 74 * ratioObject

                        Item {
                            id: rdmiItem
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            //anchors.bottomMargin: 12 * ratioObject

                            LargeText {
                                id: rdbiText
                                anchors.verticalCenter: rdmiItem.verticalCenter
                                height: 48 * ratioObject
                                font.pixelSize: 28 * ratioFont
                                anchors.left: rdmiItem.left
                                //anchors.leftMargin: 36 * ratioObject
                                anchors.right: rmiiLink.left
                                anchors.rightMargin: 5 * ratioObject
                                color: "#A8F4F4"
                                horizontalAlignment: Text.AlignLeft
                                text: warlockDictionary.getStringByCode(lvmItem.model[index].t)
                            }

                            LargeText {
                                id: rmiiLink
                                anchors.verticalCenter: rdmiItem.verticalCenter
                                height: 48 * ratioObject
                                width: 10 * ratioObject
                                font.pixelSize: 28 * ratioFont
                                anchors.right: rdmiItem.right
                                anchors.rightMargin: 18 * ratioObject
                                color: "#A8F4F4"
                                horizontalAlignment: Text.AlignLeft
                                text: ">"
                            }

                            MouseArea {
                                id: maMainMenuItem
                                anchors.fill: parent
                                onClicked: {
                                    console.log("MenuItem", index, JSON.stringify(lvmItem.model[index]));
                                    GUI.mainMenuAction(lvmItem.model[index].c);
                                    dMenu.close();
                                }
                            }
                    }
                }
            }


        }

        BtnBig {
            id: bbNewGameWithFriend
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("NewGameWithFriend")
            bg_color_active: "#551470"
            border_color_active: "#551470"
            radius: 30

            gradient: Gradient {
                GradientStop { position: 0.0 ; color: "#905B93" }
                GradientStop { position: 0.75; color: "#551470" }
                GradientStop { position: 1.0 ; color: "#551470" }
            }

            width: 300 * ratioObject
            height: 64 * ratioObject
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 84 * ratioObject
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                callInviteFriends();
                dMenu.close();
            }
        }


    }

    Image {
        id: iBG
        source: "res/background.png"
        anchors.fill: parent
        z: -1
    }

    Rectangle {
            id: tbTop
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 84 * ratioObject
            color: "#210430"
            //opacity: 0.39

            ToolButton {
                id: tbMenu
                anchors.left: parent.left
                anchors.leftMargin: 18 * ratioObject
                anchors.top: parent.top
                anchors.topMargin: 18 * ratioObject
                height: 48 * ratioObject
                width: 48 * ratioObject

                onClicked: {
                    dMenu.open();
                    logEvent("ListGames_Menu_Clicked");
                }
                background: Image {
                    anchors.fill: parent
                    source: "res/menu.png"
                }
            }

            ToolButton {
                id: tbProfile
                anchors.left: tbMenu.right
                anchors.leftMargin: 18 * ratioObject
                anchors.top: parent.top
                anchors.topMargin: 18 * ratioObject
                height: 48 * ratioObject
                width: 48 * ratioObject

                onClicked: {
                    logEvent("ListGames_Spellbook_Clicked");
                    showWndSpellbook();
                }
                background: Image {
                    anchors.fill: parent
                    source: "res/spellbook.png"
                }
            }

            LargeText {
                id: lPlayerTitle
                text: "Hi " + core.login
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.VerticalFit
                color: "#FEE2D6"
                font.pixelSize: 28 * ratioFont

                anchors.left: tbProfile.right
                anchors.right: tbScoreText.left
                height: 48 * ratioObject
                anchors.verticalCenter: parent.verticalCenter
            }

            LargeText {
                id: tbScoreText
                text: "1500"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.VerticalFit
                color: "#FEE2D6"
                font.pixelSize: 28 * ratioFont

                anchors.right: tbScoreIcon.left
                height: 48 * ratioObject
                width: 64 * ratioObject
                anchors.verticalCenter: parent.verticalCenter

                onClicked: {
                    logEvent("ListGames_Score_Clicked");
                    GUI.mainMenuAction("player_score");
                }
            }



            ToolButton {
                id: tbScoreIcon
                anchors.right: parent.right
                anchors.rightMargin: 18 * ratioObject
                anchors.top: parent.top
                anchors.topMargin: 18 * ratioObject
                height: 48 * ratioObject
                width: 48 * ratioObject

                onClicked: {
                    GUI.mainMenuAction("player_score");
                    //showWndSpellbook();
                }
                background: Image {
                    anchors.fill: parent
                    source: "res/trophy.png"
                }
            }
    }

    Item {
        id: rBody
        anchors.top: tbTop.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        //color: "#210430"

        ScrollView {
            id: iBattleList
            anchors.top: parent.top
            //anchors.topMargin: 130 * ratioObject
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 24 * ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 10 * ratioObject
            visible: true//core.allowedAdd || true
            contentHeight: iMainScrollBody.height
            contentWidth: -1

            Item {
                id: iMainScrollBody
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: ((144 + 130) * ratioObject)+ bbNewGame.height + ltActiveBattle.height + lvActiveBattle.height + ltFinishedBattle.height + lvFinishedBattle.height

                BtnBig {
                    id: bbNewGame
                    text_color: "#ABF4F4"
                    text: warlockDictionary.getStringByCode("NewGame")
                    bg_color_active: "#551470"
                    border_color_active: "#551470"
                    radius: 30

                    gradient: Gradient {
                        GradientStop { position: 0.0 ; color: "#905B93" }
                        GradientStop { position: 0.75; color: "#551470" }
                        GradientStop { position: 1.0 ; color: "#551470" }
                    }

                    width: 366 * ratioObject
                    height: 96 * ratioObject
                    anchors.top: parent.top
                    anchors.topMargin: 144 * ratioObject
                    anchors.horizontalCenter: parent.horizontalCenter

                    onClicked: {
                        console.log("start game btn 1");
                        logEvent("New_game_button", {});
                        WNDU.showNewGame();
                        //GUI.startGame(0);
                    }
                }

                LargeText {
                    id: ltActiveBattle
                    anchors.top: bbNewGame.bottom
                    anchors.topMargin: 130 * ratioObject
                    anchors.left: parent.left
                    //anchors.right: parent.right
                    height: 48 * ratioObject
                    width: 0.5 * parent.width
                    font.pixelSize: 28 * ratioFont
                    horizontalAlignment: Text.AlignLeft
                    fontSizeMode: Text.VerticalFit
                    color: "#2DA0A5"
                    text: warlockDictionary.getStringByCode("ActiveGameTitle")
                }

                LargeText {
                    id: ltRefresh
                    anchors.top: bbNewGame.bottom
                    anchors.topMargin: 130 * ratioObject
                    anchors.left: ltActiveBattle.right
                    anchors.right: parent.right
                    height: 48 * ratioObject
                    font.pixelSize: 28 * ratioFont
                    font.underline: true
                    horizontalAlignment: Text.AlignRight
                    fontSizeMode: Text.VerticalFit
                    color: "#A8F4F4"
                    text: warlockDictionary.getStringByCode("Refresh")

                    onClicked: {
                        console.log("try refresh state");
                        WNDU.closeChilds();
                        core.scanState(0);
                    }
                }

                ListView {
                    id: lvActiveBattle
                    model: GUI.loadBattleList(1)
                    anchors.top: ltActiveBattle.bottom
                    anchors.topMargin: 6 * ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    //width: 0.9 * mainWindow.width
                    //anchors.horizontalCenter: parent.horizontalCenter
                    height: model.length * 96 * ratioObject

                    delegate: Item {
                            id: idRoot
                            width: lvActiveBattle.width
                            height: 96 * ratioObject
                            Item {
                                id: rdBattleItem
                                z: 10
                                //color: "#544653"
                                //opacity: 0.39
                                //radius: 30
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 12 * ratioObject

                                Text {
                                    id: rdbiText2
                                    anchors.verticalCenter: rdBattleItem.verticalCenter
                                    //height: 48 * ratioObject
                                    font.pixelSize: 28 * ratioFont
                                    anchors.left: rdBattleItem.left
                                    anchors.leftMargin: 36 * ratioObject
                                    //anchors.right: rdbifIcon.left
                                    //anchors.rightMargin: 5 * ratioObject
                                    color: "#FEE2D6"
                                    horizontalAlignment: Text.AlignLeft

                                    text: lvActiveBattle.model[index].d
                                }
                                Text {
                                    id: rdbiText3
                                    anchors.baseline: rdbiText2.baseline// rdBattleItem.verticalCenter
                                    //height: 48 * ratioObject
                                    font.pixelSize: 21 * ratioFont
                                    anchors.left: rdbiText2.right
                                    anchors.leftMargin: 5 * ratioObject
                                    anchors.right: rdbifIcon.left
                                    anchors.rightMargin: 5 * ratioObject
                                    color: "#2DA0A5"
                                    font.italic: true
                                    horizontalAlignment: Text.AlignLeft
                                    visible: lvActiveBattle.model[index].online

                                    text: "ONLINE"
                                }

                                Rectangle {
                                    id: rdbifIcon
                                    width: 24 * ratioObject
                                    height: 24 * ratioObject
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: rdbiLink.right
                                    anchors.rightMargin: 36 * ratioObject
                                    visible: (lvActiveBattle.model[index].s === 1) || (lvActiveBattle.model[index].s === 3) || (lvActiveBattle.model[index].s === 5)
                                    radius: width / 2
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: lvActiveBattle.model[index].s === 5 ? "#34ab9d" : "#9DFFD3" }
                                        GradientStop { position: 1.0; color: lvActiveBattle.model[index].s === 5 ? "#3a9aa5" : "#9CDFFF" }
                                    }
                                }

                                LargeText {
                                    id: rdbiLink
                                    anchors.verticalCenter: rdBattleItem.verticalCenter
                                    height: 48 * ratioObject
                                    width: 10 * ratioObject
                                    font.pixelSize: 28 * ratioFont
                                    anchors.right: rdBattleItem.right
                                    anchors.rightMargin: 18 * ratioObject
                                    color: "#FEE2D6"
                                    horizontalAlignment: Text.AlignLeft
                                    text: ">"
                                }

                                MouseArea {
                                    id: maActiveBattle
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("getBattle", index, JSON.stringify(lvActiveBattle.model[index]));
                                        if (lvActiveBattle.model[index].s >= 3) {
                                            var obj = JSON.parse(JSON.stringify(lvActiveBattle.model[index]));
                                            obj.type = 17;
                                            obj.action = lvActiveBattle.model[index].s === 5 ? "force_challenge" : "private_challenge";
                                            showErrorWnd(obj);
                                        } else {
                                            core.getBattle(lvActiveBattle.model[index].id, Math.max(lvActiveBattle.model[index].s, 0));
                                            if (lvActiveBattle.model[index].s === 1) {
                                            logEvent("Game_Ready_Clicked", {Id:lvActiveBattle.model[index].id});
                                            } else if (lvActiveBattle.model[index].s === 0) {
                                                logEvent("Game_Waiting_Clicked", {Id:lvActiveBattle.model[index].id});
                                            } else if (lvActiveBattle.model[index].s === -1) {
                                                logEvent("Game_Pending_Clicked", {Id:lvActiveBattle.model[index].id});
                                            }
                                        }
                                    }
                                }
                        }

                        Rectangle {
                            id: rdBattleItemBg
                            color: "#544653"
                            opacity: 0.39
                            radius: 30
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 12 * ratioObject
                        }
                    }
                }

                LargeText {
                    id: ltFinishedBattle
                    anchors.top: lvActiveBattle.bottom
                    anchors.topMargin: 6 * ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 48 * ratioObject
                    font.pixelSize: 28 * ratioFont
                    horizontalAlignment: Text.AlignLeft
                    fontSizeMode: Text.VerticalFit
                    color: "#2DA0A5"
                    text: warlockDictionary.getStringByCode("FinishedGameTitle")
                    visible: lvFinishedBattle.model.length > 0
                }

                ListView {
                    id: lvFinishedBattle
                    model: GUI.loadBattleList(2)
                    anchors.top: ltFinishedBattle.bottom
                    //anchors.topMargin: 0.01 * mainWindow.height
                    width: 0.9 * mainWindow.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: model.length * 96 * ratioObject
                    delegate: Item {
                            id: idfRoot
                            width: lvFinishedBattle.width
                            height: 96 * ratioObject
                            Item {
                            id: rdfBattleItem
                            //radius: 30
                            //color: "#544653"
                            //opacity: 0.39
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 12 * ratioObject

                            LargeText {
                                id: rdbifText
                                anchors.verticalCenter: rdfBattleItem.verticalCenter
                                anchors.left: rdfBattleItem.left
                                anchors.leftMargin: 36 * ratioObject
                                anchors.right: rdbiIcon.left
                                anchors.rightMargin: 18 * ratioObject
                                height: 48 * ratioObject
                                font.pixelSize: 28 * ratioFont

                                color: "#10C9F5"
                                fontSizeMode: Text.VerticalFit
                                horizontalAlignment: Text.AlignLeft

                                text: lvFinishedBattle.model[index].d
                            }

                            Image {
                                id: rdbiIcon
                                source: lvFinishedBattle.model[index].d.indexOf("Won ") !== 0 ? "res/RIP2.png" : "res/stars.png"
                                width: 24 * ratioObject
                                height: 24 * ratioObject
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 18 * ratioObject
                            }

                            MouseArea {
                                id: maFinishedBattle
                                anchors.fill: parent
                                onClicked: {
                                    console.log("getFinishedBattle", index, JSON.stringify(lvFinishedBattle.model[index]));
                                    core.getBattle(lvFinishedBattle.model[index].id, lvFinishedBattle.model[index].s);
                                    logEvent("Game_Finished_Clicked", {Id:lvFinishedBattle.model[index].id});
                                }
                            }
                        }

                        Rectangle {
                            id: rdfBattleItemBG
                            radius: 30
                            color: "#544653"
                            opacity: 0.39
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 12 * ratioObject
                        }
                    }
                }
            }
        }
    }

    function refreshData() {
        console.log("user start scanning");
        core.scanState()
    }

    function showMainMenu() {
        dMenu.open();
    }

    function showUserProfile(other) {
        console.log("showUserProfile", other);
        gERROR.show_info_self = other === true ? false : true;
        WNDU.showProfileWindow();
    }

    property int startRegisterFlowIndex: 0
    function getLoginFromUser(real_login) {
        if (real_login) {
            logEvent("user_login_start");
            startRegisterFlowIndex = 1; // login
            WNDU.showRegisterFlow();
        } else {
            showNewUserMenu();
        }
    }

    function showTipMessage(TipTxt, allowClose) {
        console.log("Tip: ", TipTxt, allowClose);
        //allowCloseTip = allowClose ? true : false;
        //showErrorWnd({text:TipTxt,type:1});
    }

    function showErrorWnd(data) {
        WNDU.showErrorWnd(data);
    }

    function showSearchWarlockWnd(battle_friendly_level) {
        WNDU.showErrorWnd({type:101,bfl:battle_friendly_level,close_all:1});
        //WNDU.showSearchWarlockWindow();
    }

    function showErrorMessage() {
        var err_msg = core.errorMsg;
        console.log("Error: ", err_msg);
        if (!core.isAI) {
            if (err_msg.indexOf("{") === -1) {
                showErrorWnd({text:err_msg});
            } else {
                var obj = JSON.parse(err_msg);
                if (obj.type === 14) {
                    // rate us action
                    afterCloseAction = "rateus";
                    return ;
                } else if (obj.type === 22) {
                    afterCloseAction = "spellbook_levelup";
                    return ;
                }

                showErrorWnd(obj);
            }
        }
    }

    function showBattleChat(chat_history) {
        console.log("showBattleChat");
        showErrorWnd({type:3,text:chat_history, title: "Battle #" + gBattle.id + " chat", msg: gBattle.actions.C, read_only: gBattle.read_only});
    }

    function storeBattleChatMsg(msg) {
        gBattle.actions.C = msg;
        //processEscape();
    }

    function showFinishedBattle() {
        var bit = core.loadedBattleID;
        var txt = core.finishedBattle;
        logEvent("showFinishedBattle", {battle_id:bit});
        console.log(txt);

        if (txt.indexOf("{") === 0) {
            var err = JSON.parse(txt);
            if (!err.type) {
                err.type = 7;
            }
            showErrorWnd(err);
        } else {
            showErrorWnd({text:txt,type:2,title:"Battle #" + bit});
        }
        core.scanState(1);
    }

    function showAfterRegWnd() {
        WNDU.showAfterRegistrationWindow();
    }

    function afterRegAction() {
        processEscape();
        //GUI.startGameWithPlayerEx(true);
        GUI.startGameWithBotEx(true);
    }

    function showReadyBattle() {
        if (WNDU.isSomeWndOpenned()) {
            return;
        }

        var battle_str = core.battleInfo();
        console.log("showReadyBattle", battle_str);
        gBattle = JSON.parse(battle_str);
        gBattle.read_only = false;
        WNDU.showBattle();
    }

    function openBattleOnline() {
        WNDU.closeChild();
        Qt.openUrlExternally(core.getOnlineUrl());
    }

    function showNewUserMenu() {
        logEvent("registration_started");
        //WNDU.showNewUserMenu();
        WNDU.showRegisterFlow();
    }

    function showSpellDetails(spell_code) {
        logEvent("spell_details", {code:spell_code});
        showErrorWnd({type:23,spell:spell_code});
    }

    function showBattleHistory(txt) {
        showErrorWnd({type:16,history:txt});
    }

    function showGesture(isLeft, possible_gestures, action_type, warlock_idx, last_gesture) {
        console.log("wnd_main.showGesture", isLeft, possible_gestures, action_type, warlock_idx, last_gesture);
        var close_current = false;
        if (!possible_gestures) {
            possible_gestures = isLeft ? gBattle.warlocks[0].plg : gBattle.warlocks[0].prg;
            close_current = true;
        }
        if (!action_type) {
            action_type = "normal";
        }
        if (!warlock_idx) {
            warlock_idx = 0;
        }

        if (action_type === "normal") {
            gBattle.currentHand = isLeft ? "L" : "R";
            gBattle.otherHand   = isLeft ? "R" : "L";
            gBattle.currentHandIdx = isLeft ? GC.WARLOCK_HAND_LEFT : GC.WARLOCK_HAND_RIGHT;
            gBattle.otherHandIdx = isLeft ? GC.WARLOCK_HAND_RIGHT : GC.WARLOCK_HAND_LEFT;
        }
        gERROR = {title: "Choose gesture for "+(isLeft ? "left" : "right") + " hand", is_left: isLeft, pga: possible_gestures.split(","),
            g:gBattle["ng" + gBattle.currentHand], at: action_type, widx: warlock_idx, lg: last_gesture};
        if (close_current) {
            closeChild();
        }

        WNDU.showGesture();
    }

    function getFullSpellbook() {
        return JSON.parse(core.getSpellBook());
    }

    function getSpellList(new_gesture, warlock_idx, hand_idx) {
        console.log("mainWindow.getSpellList", gBattle.currentHand, gBattle.currentHandIdx, new_gesture, warlock_idx, hand_idx);
        if (!warlock_idx) {
            warlock_idx = 0;
        }
        if (!hand_idx) {
            hand_idx = gBattle.currentHandIdx;
        }
        var is_player = warlock_idx === 0;

        if (is_player) {
            gBattle["ng" + gBattle.currentHand] = new_gesture;
        }
        var res = [], arr_cast_now = [], arr_cast_later = [];

        console.log("mainWindow.getSpellList", gBattle.L, gBattle.R, gBattle.ngL, gBattle.ngR);
        var arr = gBattle.warlocks[warlock_idx].spells, s, idx;
        var uncompleted_cnt = 0, i, Ln, charm_monster = 0, finished_high_lv_spell = 0, other_hand_finish_spell = 0, curr_hand_finish_two_hand_spell = 0;
        for (i = 0, Ln = arr.length; i < Ln; ++i) {
            s = arr[i];
            //console.log("mainWindow.getSpellList", i, JSON.stringify(s));
            s.choose = 0;
            s.row_type = 1;
            if (s.h !== hand_idx) {
                if ((new_gesture !== '') && (s.ng === new_gesture) && (s.t === 1) && (s.th === 1)) {
                    s.gp = '<font color="#10C9F5">'+s.g+'</font>';
                    s.cast_type = 1;
                    arr_cast_now.push(s);
                } else if ((s.t === 1) && (s.a > 0)) {
                    console.log("mainWindow.getSpellList", i, JSON.stringify(s));
                    ++other_hand_finish_spell;
                }
                continue;
            }
            if (s.sbl > playerSpellbookLevel) {
                if ((new_gesture !== '') && (s.ng === new_gesture) && (s.t === 1) && (s.a > 0)) {
                    ++finished_high_lv_spell;
                }
                continue;
            }

            if (new_gesture !== '') {
                if (s.ng === new_gesture) {
                    console.log("mainWindow.getSpellList iteration", i, new_gesture, JSON.stringify(s));
                    if (s.t === 1) {
                        s.gp = '<font color="#10C9F5">'+s.g+'</font>';
                        s.cast_type = 1;
                        arr_cast_now.push(s);
                        if (s.id === GUI.SPELL_CHARM_MONSTER) {
                            charm_monster = 1;
                        }
                        if (s.th === 1) {
                            ++curr_hand_finish_two_hand_spell;
                        }
                    } else {
                        idx = s.g.length - s.t + 1;
                        s.gp = '<font color="#10C9F5">'+s.g.substr(0, idx)+'</font><font color="#FEE2D6">'+s.g.substr(idx)+'</font>';
                        s.cast_type = 2;
                        arr_cast_later.push(s);
                        ++uncompleted_cnt;
                    }
                }
            } else {
                console.log("mainWindow.getSpellList iteration2", i, new_gesture, JSON.stringify(s));
                idx = s.g.length - s.t;
                s.gp = '<font color="#10C9F5">'+s.g.substr(0, idx)+'</font><font color="#FEE2D6">'+s.g.substr(idx)+'</font>';
                s.cast_type = 3;
                arr_cast_later.push(s);
                ++uncompleted_cnt;
            }
        }
        //arr_cast_now.push();

        if (is_player) {
            gBattle["completed_spell_" + gBattle.currentHand] = arr_cast_now.length > 0 ? 1 : 0;
        }
        if (new_gesture !== '') {
            gBattle.spellIdx = arr_cast_now.length;
            var def_or_none = is_player && gBattle.player_changed_mind;
            //var def_or_none = (gBattle.spellIdx > 0) || (is_player && (gBattle.player_changed_mind || ((playerSpellbookLevel < 5) && (finished_high_lv_spell > 0))));
            if (def_or_none) {
                gBattle.spellIdx = arr_cast_now.length + 1;
            } else if (arr_cast_now.length === 1) {
                arr_cast_now[0].choose = 1;
                gBattle.spellIdx = 1;
            } else if (arr_cast_now.length > 1) {
                var spi = 0;
                var max_spell_length = 0;
                for (var si = 0, sL = arr_cast_now.length; si < sL; ++si) {
                    if (arr_cast_now[si].gp.length >= max_spell_length) {
                        max_spell_length = arr_cast_now[si].gp.length;
                        spi = si;
                    }
                }
                arr_cast_now[spi].choose = 1;
                gBattle.spellIdx = spi + 1;
            }

            var def_spell = def_or_none ? {id:-1,gp:"?",n:"Default",choose:1,t:1,cast_type:1,row_type:1,is_charm_monster:charm_monster} : {gp:"None",n:"",choose:0,t:1,cast_type:0,row_type:1};
            res.push({gp:"?",n:(warlock_idx === 0 ? "Completed spells" : "Complete Opponent Spells"),choose:0,t:0,cast_type:100,row_type:2});
            res = res.concat(arr_cast_now);
            if (true || def_or_none || (arr_cast_now.length === 0) || ((curr_hand_finish_two_hand_spell > 0) && (other_hand_finish_spell > 0))) {
                res.push(def_spell);
            }
        } else {
            res.push({gp:"?",n:(warlock_idx === 0 ? warlockDictionary.getStringByCode("SpellbookName" + playerSpellbookLevel) : "Spells opponent may cast"),choose:0,t:0,cast_type:100,row_type:3});
        }
        if (uncompleted_cnt > 0) {
            if (new_gesture !== '') {
                res.push({gp:"?",n:(warlock_idx === 0 ? "Incomplete Spells" : "Incomplete Opponent Spells"),choose:0,t:0,cast_type:100,row_type:4});
            }
            res = res.concat(arr_cast_later);
        }
        if (new_gesture === '') {
            var h = GUI.getSpellBookLevelUpHint(true);
            if (h !== "") {
                res.push({gp:"?",n:h,row_type:5});
            }
        }

        return res;
    }

    function setGesture(gesture, spell, need_target) {
        var is_maladroit = (gBattle.warlocks[0].maladroit > 0) || (((gesture === "C") || (spell.th> 0)) && !gBattle.player_under_control);
        console.log("setGesture", gesture, JSON.stringify(spell), is_maladroit);
        if (!spell.n) {
            spell.n = "None";
        }

        gBattle.actions[gBattle.currentHand] = {g:gesture,s:spell};
        if (is_maladroit && (gBattle.actions[gBattle.otherHand].g !== gesture)) {
            gBattle["ng" + gBattle.otherHand] = gesture;
            gBattle.actions[gBattle.otherHand] = {g:gesture,s:{gp:"?",n:"Default",choose:1,t:1,cast_type:1,need_target:true}};
        }

        WNDU.arr_wnd_instance[WNDU.wnd_battle].setGesture(gesture, is_maladroit);
        if (need_target) {
            console.log("NEED_TARGET", gBattle.actions.P, gBattle.currentHandIdx);
            spell.permanency = (gBattle.actions.P === -1) || (gBattle.actions.P !== gBattle.currentHandIdx) ? 0 : 1;
            spell.delay = (gBattle.actions.D === -1) || (gBattle.actions.D !== gBattle.currentHandIdx) ? 0 : 1;
            WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(true, spell);
        } else {
            WNDU.arr_wnd_instance[WNDU.wnd_battle].currentSpell = {n:"",h:gBattle.currentHand === "L" ? 1 : 2,g:""};
            WNDU.arr_wnd_instance[WNDU.wnd_battle].battleChanged(true);
        }
        WNDU.processEscape();
    }

    function chooseTargetForSpell(isLeft) {
        gBattle.currentHand = isLeft ? "L" : "R";
        gBattle.otherHand   = isLeft ? "R" : "L";
        gBattle.currentHandIdx = isLeft ? GC.WARLOCK_HAND_LEFT : GC.WARLOCK_HAND_RIGHT;
        gBattle.otherHandIdx = isLeft ? GC.WARLOCK_HAND_RIGHT : GC.WARLOCK_HAND_LEFT;

        if (!gBattle.actions[gBattle.currentHand].g) {
            return;
        }

        WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(true, gBattle.actions[gBattle.currentHand].n);
        logEvent("Play_Target_View", {Mode:"spell"});
    }

    function battleWarlockPrepared() {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareHintWithCheck();
    }

    function chooseMonsterTarget(title) {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(false, {n:title});
        logEvent("Play_Target_View", {Mode:"monster"});
        //WNDU.processEscape();
    }

    function setSpellTarget(TargetName, lPermanent, lDelay, OperationType) {
        console.log("main.setSpellTarget", TargetName, lPermanent, lDelay, OperationType, gBattle.currentHand);
        if (OperationType === 1) {
            gBattle.actions[gBattle.currentHand].target = TargetName;
            if (lPermanent === 1) {
                gBattle.actions.P = gBattle.currentHandIdx;
            } else {
                lPermanent = 0;
            }
            if (lDelay === 1) {
                gBattle.actions.D = gBattle.currentHandIdx;
            } else {
                lDelay = 0;
            }

            logEvent("Play_Target_Clicked", {Mode:"spell",Target:TargetName,Spell:gBattle.actions[gBattle.currentHand].n,Permanent:lPermanent,Delay:lDelay});
        } else if (OperationType === 2) {
            console.log("before", gBattle.currentMonsterIdx, JSON.stringify(gBattle.actions.M[gBattle.currentMonsterIdx]));
            gBattle.actions.M[gBattle.currentMonsterIdx].target = TargetName;
            console.log("after", gBattle.currentMonsterIdx, JSON.stringify(gBattle.actions.M[gBattle.currentMonsterIdx]));
            logEvent("Play_Target_Clicked", {Mode:"monster",Target:TargetName,Monster:gBattle.actions.M[gBattle.currentMonsterIdx].name});
        }
    }

    function cleanPermanency() {
        gBattle.actions.P = -1;
    }

    function cleanDelay() {
        gBattle.actions.D = -1;
    }

    function chooseCharm(Action, H, G, data) {
        gERROR = {h:H,g:G,is_paralyze:Action === "paralized",title:warlockDictionary.getStringByCode("TitleAction_" + Action)};
        if (gERROR.is_paralyze) {
            gERROR.lgL = data.lgL;
            gERROR.lgR = data.lgR;
        }

        WNDU.showCharm();
    }

    function setCharm(H, G, no_need_escape) {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].setCharm(H, G);
        if (!no_need_escape) {
            WNDU.processEscape();
        }
    }

    function showOrders(arr) {
        gERROR = {title:warlockDictionary.getStringByCode("ReviewOrders"), data: arr};
        WNDU.showOrders();
    }

    function confirmOrders() {
        mdNoGesture.text = warlockDictionary.getStringByCode("ConfirmOrdersForTurn");
        mdNoGesture.dialogType = 1;
        mdNoGesture.visible = true;
    }

    function confirmLogout() {
        mdNoGesture.text = warlockDictionary.getStringByCode("ConfirmLogout");
        mdNoGesture.dialogType = 3;
        mdNoGesture.visible = true;
    }

    function showFeedbackWnd() {
        logEvent("Feedback_View", {});
        return showErrorWnd({id:-1,type:13,action:"feedback"});
    }

    function showRateUsWnd() {
        logEvent("Rating_View", {});
        return showErrorWnd({id:-1,type:14,action:"rate_us"});
    }

    function showSBLWnd() {
        return showErrorWnd({id:-1,type:22,sbl:playerSpellbookLevel});
    }

    function showShareWnd(game_level) {
        if (!game_level) {
            game_level = "vf";
        }
        core.getSharableLink(game_level);
        //return showErrorWnd({id:-1,type:18,action:"playe_with_friends"});
    }

    function showUserScoreWnd(data) {
        if (!data || (data.name === GUI.G_PROFILE.name)) {
            data = JSON.parse(JSON.stringify(GUI.G_PROFILE));
            data.isPlayer = true;
        } else {
            data.isPlayer = false;
        }

        data.type = 15;
        return showErrorWnd(data);
    }

    function confirmOrdersEx() {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].sendOrders();
        WNDU.closeChilds();
        processAfterClose();
    }

    function callInviteFriends() {
        console.log("start game WITH FRIEND");
        GUI.mainMenuAction("battle_with_friend");
    }

    function startGameWithBotEx() {
        GUI.startGameWithBotEx();
    }

    function startGameWithBot() {
        GUI.startGameWithBot();
    }

    function startTrainingGame() {
        GUI.startGameByCode('bot');
    }

    function startWarlockGame(WarlockName, FriendlyLevel) {
        GUI.startGameByCode('player', WarlockName, FriendlyLevel);
    }

    function startGameWithPlayer() {
        GUI.startGameWithPlayer();
    }

    function startGameWithPlayerEx() {
        GUI.startGameWithPlayerEx();
    }

    function processAfterClose() {
        if (afterCloseAction !== "") {
            var action_code = afterCloseAction;
            afterCloseAction = "";
            GUI.mainMenuAction(action_code);
        }
    }

    function processEscape() {
        console.log("main.qml:processEscape", afterCloseAction);
        WNDU.processEscape();
        if (!WNDU.isSomeWndOpenned()) {
            processAfterClose();
        }
    }

    function showBattleReadOnly(BattleID) {
        console.log("main.qml.showBattleReadOnly", BattleID);
        //GUI.mainMenuAction("show_read_only_battle", BattleID);
        var battle_str = core.battleReadOnly(BattleID);
        console.log("showBattleReadOnly", battle_str);
        if (!battle_str) {
            showErrorWnd({text:"Some problem with this battle, please contact viskgameua@gmail.com"});
            return;
        }

        gBattle = JSON.parse(battle_str);
        gBattle.read_only = true;
        WNDU.showBattle();
    }

    property int needTopList: 0;
    function getHallOfFame() {
        needTopList = 1;
        core.scanTopList(false, true);
        //WNDU.showTopWindow();
    }

    function showHallOfFameWindow() {
        WNDU.showTopWindow();
    }

    function showWndSpellbook(close_current, close_all) {
        gERROR = {sbl:playerSpellbookLevel,win_vs_bot:GUI.G_PROFILE.win_vs_bot,lelveup_hint:GUI.getSpellBookLevelUpHint(false)};
        WNDU.showSpellbook(close_current, close_all);
    }

    function storeWnd(wnd) {
        WNDU.storeWnd(wnd);
    }

    function closeChild(from_login) {
        if (from_login) {
            logEvent("login_finish");
        }
        WNDU.closeChild();
    }

    function linkActivated(link) {
        GUI.linkActivated(link)
    }

    function showToast(msg) {
        core.showNotification(msg);
    }

    function logEvent(event_name, params) {
        if (core.isAI) {
            return;
        }

        if (!(typeof params === 'object')) {
            params = {};
        }
        if (!params.engagement_time_msec) {
            params.engagement_time_msec  = 1;
        }
        if (!params.Mode) {
            params.Mode = ((GUI.G_PROFILE.finished_game_count < 3) || (GUI.G_PROFILE.elo <= 1500)) ? "beginner" : "ready";
        }
        if (!params.Spellbook_Level) {
            params.Spellbook_Level = playerSpellbookLevel;
        }
        if (!params.Finished_Trainings) {
            params.Finished_Trainings = GUI.G_PROFILE.win_vs_bot;
        }
        if (!params.Finished_Games) {
            params.Finished_Games = GUI.G_PROFILE.finished_game_count;
        }

        var param_str = "";
        for (const [key, value] of Object.entries(params)) {
            param_str += key + ";" + value + ";";
        }
        console.log("logEvent", event_name, JSON.stringify(params), param_str);
        core.logEvent(event_name, param_str);
        //analytics.logEvent(event_name, params);
    }

    function isMobile() {
        var currOs = Qt.platform.os, res = (currOs === "android") || (currOs === "ios") || (currOs === "blackberry") || (currOs === "winphone");
        console.log("isMobile", currOs, res);
        return res;
    }

    function calculateRatio() {
        var w, h, fw, fh;
        if (isMobile()) {
            w = Screen.width;// * Screen.devicePixelRatio;
            h = Screen.height;// * Screen.devicePixelRatio;
            fw = realScreenWidth;//Screen.desktopAvailableWidth;
            fh = realScreenHeight;//Screen.desktopAvailableHeight;
            //if ((Screen.desktopAvailableHeight < Screen.height) && (Screen.desktopAvailableHeight === mainWindow.height)) {
            //    h = Screen.desktopAvailableHeight + (Screen.height - Screen.desktopAvailableHeight) / 2;
            //    mainWindow.height = h;
            //}
        } else {
            w = mainWindow.width;
            h = mainWindow.height;
            fw = w;
            fh = h;
        }
        console.log("calculateRatio", "calculated", calculatedRatio, calculatedRatioFont);
        console.log("calculateRatio", "Px", Screen.devicePixelRatio, Screen.pixelDensity);
        console.log("calculateRatio", "Width", Screen.width, Screen.desktopAvailableWidth, mainWindow.width, realScreenWidth);
        console.log("calculateRatio", "Height", Screen.height, Screen.desktopAvailableHeight, mainWindow.height, realScreenHeight);

        if ((calculatedRatio === 0) || !isMobile()) {
            var dpi  = Screen.pixelDensity * Screen.devicePixelRatio;
            var wh   = Math.max(w, h);
            var ww   = Math.min(w, h);
            var fwh   = Math.max(fw, fh);
            var fww   = Math.min(fw, fh);
            var bh   = 1068;//800;
            var bw   = 600;
            var bdpi = 4.459 * 1.25;
            console.log(dpi, bdpi, ww, wh, bw, bh);
            ratioObject = Math.min(ww/bw, wh/bh);
            ratioFont = Math.max(1, Math.min((fwh*dpi)/(bdpi*bh), (fww*dpi/(bdpi*bw))));
        } else {
            ratioObject = calculatedRatio;
            ratioFont = calculatedRatioFont;
        }

        core.setUserProperties(Qt.platform.os, w + "x" + h, Qt.locale().name);
    }

    function creationFinished() {
        console.log("iWndContainer x y", iWndContainer.x, iWndContainer.y, iWndContainer.height, iWndContainer.width);
        calculateRatio();

        //googleAnalytic.setTID("YOUR-QA-CODE");
        //googleAnalytic.setScreenSize(Qt.screenWidth, Qt.screenHeight);
        //googleAnalytic.setOS(Qt.platform.os);

        logEvent("app_start", {ratio_o:ratioObject, ratio_f: ratioFont});
        //GUI.prepareNewGameBtn(1500);
    }

    Component.onCompleted: creationFinished()
}
