import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import ua.sp.warloksduel 2.0
import ua.sp.warlockdictionary 1.0

import "qrc:/js/game_constant.js" as GC
import "qrc:/js/gui_utils.js" as GUI
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
    width: 506//600
    height: 900//1068
    flags: Qt.FramelessWindowHint|Qt.Window

    property real ratioObject: 1
    property real ratioFont: 1

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
            case 0: return Qt.quit();
            case 1: return confirmOrdersEx();
            case 2: return joinBattleDialogResult(true);
            }
        }

        onRejected: {
            if (dialogType == 2) {
                joinBattleDialogResult(false);
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
    property alias mainContainer: iWndContainer
    property alias mainHeader: tbTop
    property var gERROR: ({})
    property var gBattle: ({})

    WarlocksDuelCore {
        id: core
        onNeedLogin: getLoginFromUser()
        onErrorOccurred: showErrorMessage()
        onIsLoadingChanged: GUI.isLoadingChanged()
        onBattleListChanged: GUI.newBattleList()
        onFinishedBattleChanged: showFinishedBattle()
        onReadyBattleChanged: showReadyBattle();
        onRegisterNewUserChanged: GUI.newUserRegistered(core)
        //onOrderSubmitedChanged: MUtils.cleanOrders(core)
        onTimerStateChanged: changeTimerState()
        onChallengeListChanged: GUI.loadChallengeList()
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
        /*onAccountMenuChanged: {
            console.log("onAccountMenuChanged");
            GUI.prepareLoginMenu(core.accountMenu);
        }*/
        onPlayerInfoChanged: GUI.userProfileChanged()
        onLoginChanged: closeChild()

        Component.onCompleted: {
            console.log("Core.completed");
            if (core.login === '') {
                tbTop.visible = false;
                rBody.visible = false;
                showNewUserMenu();
            } else {
                core.scanState();
                GUI.prepareLoginMenu(core.accountMenu);
            }
        }
    }

    /*AnalyticItem {
        id: analytics
        userId: //take uuid
            core.uuid
    }*/

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
    property alias keyListener: iWndContainer

    color: "#551470"

    Item {
        id: iWndContainer
        anchors.fill: parent
        z: 500
        focus: true

        Keys.onBackPressed: WNDU.processEscape();
        Keys.onEscapePressed: WNDU.processEscape();
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
        width: 0.66 * mainWindow.width
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
        }

        BtnBig {
            id: bbMenuRefresh
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("Refresh")
            transparent: true
            border.width: 0
            height: 0.1 * parent.height
            anchors.top: parent.top
            anchors.topMargin: 0.01 * parent.height
            anchors.left: parent.left
            anchors.right: parent.right
            visible: false

            onClicked: {
                console.log("try refresh state");
                dMenu.close();
                core.scanState(0);
            }
        }

        BtnBig {
            id: bbSpellbook
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("Spellbook")
            transparent: true
            border.width: 0
            height: 0.1 * parent.height
            anchors.top: parent.top
            anchors.topMargin: 0.01 * parent.height
            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                console.log("try refresh state");
                dMenu.close();
                WNDU.showSpellbook();
            }
        }

        BtnBig {
            id: bbMenuLogin
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("Login")
            transparent: true
            border.width: 0
            height: 0.1 * parent.height
            anchors.bottom: cbLoginAs.top
            anchors.bottomMargin: 0.01 * parent.height
            anchors.left: parent.left
            anchors.right: parent.right

            onClicked: {
                console.log("try login");
                dMenu.close();
                getLoginFromUser(true);
            }
        }

        ComboBox {
            id: cbLoginAs
            height: 0.1 * parent.height
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0.01 * parent.height
            anchors.left: parent.left
            anchors.right: parent.right
            model: []

            onCurrentIndexChanged: {
                var curr_login = core.login;
                console.log("cbLoginAs.onAccepted", currentIndex, currentValue, cbLoginAs.model[currentIndex], curr_login);
                if ((currentIndex > 0) && (cbLoginAs.model[currentIndex] !== curr_login)) {
                    dMenu.close();
                    core.autoLogin(currentIndex - 1);
                }
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

            ToolButton {
                id: tbMenu
                anchors.left: parent.left
                anchors.leftMargin: 18 * ratioObject
                anchors.top: parent.top
                anchors.topMargin: 18 * ratioObject
                height: 48 * ratioObject
                width: 48 * ratioObject

                onClicked: dMenu.open()
                background: Image {
                    anchors.fill: parent
                    source: "res/menu.png"
                }
            }

            LargeText {
                id: lPlayerTitle
                text: "Hi, " + core.login
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.VerticalFit
                color: "#FEE2D6"
                font.pixelSize: 28 * ratioFont

                anchors.left: tbMenu.right
                anchors.right: tbProfile.left
                height: 48 * ratioObject
                anchors.verticalCenter: parent.verticalCenter
            }

            ToolButton {
                id: tbProfile
                anchors.right: parent.right
                anchors.rightMargin: 18 * ratioObject
                anchors.top: parent.top
                anchors.topMargin: 18 * ratioObject
                height: 48 * ratioObject
                width: 48 * ratioObject

                onClicked: {
                    WNDU.showSpellbook();
                }
                background: Image {
                    anchors.fill: parent
                    source: "res/spellbook.png"
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

        BtnBig {
            id: bbNewGame
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("TrainingGame")
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
                GUI.startGame(0);
            }
        }

        BtnBig {
            id: bbNewBotGame
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("NewGameWithPlayer")
            transparent: true
            font.underline: true
            border.width: 0
            visible: core.allowedAdd || true

            width: 0.5 * parent.width
            height: 60 * ratioObject
            font.pixelSize: 28 * ratioFont
            fontSizeMode: Text.VerticalFit

            anchors.top: bbNewGame.bottom
            anchors.topMargin: 24 * ratioObject
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                console.log("start game btn 2");
                GUI.startGame(1);
            }
        }

        ScrollView {
            id: iBattleList
            anchors.top: bbNewBotGame.bottom
            anchors.topMargin: 130 * ratioObject
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 24 * ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 10 * ratioObject
            visible: core.allowedAdd || true
            contentHeight: iMainScrollBody.height

            Item {
                id: iMainScrollBody
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: ltActiveBattle.height + lvActiveBattle.height + ltFinishedBattle.height + lvFinishedBattle.height

                LargeText {
                    id: ltActiveBattle
                    anchors.top: parent.top
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
                    anchors.top: parent.top
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
                            Rectangle {
                                id: rdBattleItem
                                color: "#544653"
                                radius: 30
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 12 * ratioObject

                                LargeText {
                                    id: rdbiText
                                    anchors.verticalCenter: rdBattleItem.verticalCenter
                                    height: 48 * ratioObject
                                    font.pixelSize: 28 * ratioFont
                                    anchors.left: rdBattleItem.left
                                    anchors.leftMargin: 36 * ratioObject
                                    anchors.right: rdbifIcon.left
                                    anchors.rightMargin: 5 * ratioObject
                                    color: "#FEE2D6"
                                    horizontalAlignment: Text.AlignLeft

                                    text: lvActiveBattle.model[index].d
                                }

                                Rectangle {
                                    id: rdbifIcon
                                    width: 24 * ratioObject
                                    height: 24 * ratioObject
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: rdbiLink.right
                                    anchors.rightMargin: 36 * ratioObject
                                    visible: lvActiveBattle.model[index].s === 1
                                    radius: width / 2
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: "#9DFFD3" }
                                        GradientStop { position: 1.0; color: "#9CDFFF" }
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
                                        core.getBattle(lvActiveBattle.model[index].id, lvActiveBattle.model[index].s);
                                    }
                                }
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
                            Rectangle {
                            id: rdfBattleItem
                            radius: 30
                            color: "#544653"
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
                                source: lvFinishedBattle.model[index].d.indexOf("Won vs. ") === -1 ? "res/RIP2.png" : "res/stars.png"
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
                                }
                            }
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

    function showErrorMessage() {
        var err_msg = core.errorMsg;
        console.log("Error: ", err_msg);
        if (!core.isAI) {
            if (err_msg.indexOf("{") === -1) {
                showErrorWnd({text:err_msg});
            } else {
                showErrorWnd(JSON.parse(err_msg));
            }
        }
    }

    function showBattleChat(battle_id, new_msg) {
        console.log("showBattleChat");
        showErrorWnd({type:3,text:core.finishedBattle, title: "Battle #" + gBattle.id + " chat", msg: gBattle.actions.C});
    }

    function storeBattleChatMsg(msg) {
        gBattle.actions.C = msg;
        processEscape();
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
        //WNDU.showNewUserMenu();
        WNDU.showRegisterFlow();
    }

    function showSpellDetails(spell_code) {
        logEvent("spell_details", {code:spell_code});
        //var short_desc_code = spell_code + "_short_desc";
        //var short_desc = warlockDictionary.getStringByCode(spell_code + "_short_desc");
        //var msg_txt = "";
        //if (short_desc !== short_desc_code) {
        //    msg_txt = short_desc + "<br>Details:<br>";
        //}
        //msg_txt += warlockDictionary.getStringByCode(spell_code + "_desc");

        showErrorWnd({type:7,spell:spell_code});
    }

    function showGesture(isLeft, possible_gestures) {
        var close_current = false;
        if (!possible_gestures) {
            possible_gestures = isLeft ? gBattle.warlocks[0].plg : gBattle.warlocks[0].prg;
            close_current = true;
        }

        gBattle.currentHand = isLeft ? "L" : "R";
        gBattle.otherHand   = isLeft ? "R" : "L";
        gBattle.currentHandIdx = isLeft ? GC.WARLOCK_HAND_LEFT : GC.WARLOCK_HAND_RIGHT;
        gBattle.otherHandIdx = isLeft ? GC.WARLOCK_HAND_RIGHT : GC.WARLOCK_HAND_LEFT;
        gERROR = {title: "Choose gesture for "+(isLeft ? "left" : "right") + " hand", is_left: isLeft, pga: possible_gestures.split(","), g:gBattle["ng" + gBattle.currentHand]};
        if (close_current) {
            closeChild();
        }

        WNDU.showGesture();
    }

    function getFullSpellbook() {
        return JSON.parse(core.getSpellBook());
    }

    function getSpellListOld(new_gesture) {
        if (new_gesture === "FULL_LIST") {
            return getFullSpellbook();
        }

        console.log("mainWindow.getSpellList", gBattle.currentHand, gBattle.currentHandIdx, new_gesture);
        gBattle["ng" + gBattle.currentHand] = new_gesture;
        var res = [], arr_cast_now = [], arr_cast_later = [], arr_cast_other = [];
        console.log("mainWindow.getSpellList", gBattle.L, gBattle.R, gBattle.ngL, gBattle.ngR);
        //var str = core.getSpellList(gBattle.L + gBattle.ngL, gBattle.R + gBattle.ngR, 0);
        //console.log("mainWindow.getSpellList", str);
        var arr = gBattle.warlocks[0].spells, s, idx;
        /*if (new_gesture !== '') {
            console.log("mainWindow.getSpellList", JSON.stringify(arr));
        }*/

        for (var i = 0, Ln = arr.length; i < Ln; ++i) {
            s = arr[i];
            s.choose = 0;
            if (s.h !== gBattle.currentHandIdx) {
                continue;
            }

            if (new_gesture !== '') {
                if (s.ng === new_gesture) {
                    console.log("mainWindow.getSpellList iteration", i, new_gesture, JSON.stringify(s));
                    if (s.t === 1) {
                        s.gp = '<font color="#10C9F5">'+s.g+'</font>';
                        s.cast_type = 1;
                        arr_cast_now.push(s);
                    } else {
                        idx = s.g.length - s.t + 1;
                        s.gp = '<font color="#10C9F5">'+s.g.substr(0, idx)+'</font><font color="#FEE2D6">'+s.g.substr(idx)+'</font>';
                        s.cast_type = 2;
                        arr_cast_later.push(s);
                    }
                }
            } else {
                /*if (s.ng === new_gesture) {
                    if (s.t === 1) {
                        s.gp = '<font color="#A8F4F4">'+s.g+'</font>';
                        s.cast_type = 1;
                        arr_cast_now.push(s);
                    } else {
                        idx = s.g.length - s.t + 1;
                        s.gp = '<font color="#A8F4F4">'+s.g.substr(0, idx)+'</font><font color="#E7FFFF">'+s.g.substr(idx)+'</font>';
                        s.cast_type = 2;
                        arr_cast_later.push(s);
                    }
                } else {*/
                    idx = s.g.length - s.t;
                    s.gp = '<font color="#10C9F5">'+s.g.substr(0, idx)+'</font><font color="#FEE2D6">'+s.g.substr(idx)+'</font>';
                    s.cast_type = 3;
                    arr_cast_other.push(s);
                //}
            }
        }
        //arr_cast_now.push();

        if (new_gesture !== '') {
            gBattle.spellIdx = arr_cast_now.length;
            var def_spell = ((gBattle.spellIdx > 0) || gBattle.player_changed_mind) ? {gp:"?",n:"Default",choose:1,t:1,cast_type:1} : {gp:"None",n:"",choose:0,t:1,cast_type:0};
            return [arr_cast_now.concat([def_spell]), arr_cast_later];
        } else {
            return arr_cast_other;
        }
    }

    function getSpellList(new_gesture) {
        console.log("mainWindow.getSpellList", gBattle.currentHand, gBattle.currentHandIdx, new_gesture);
        gBattle["ng" + gBattle.currentHand] = new_gesture;
        var res = [], arr_cast_now = [], arr_cast_later = [];//[[],[],[],[],[],[],[],[],[]];

        console.log("mainWindow.getSpellList", gBattle.L, gBattle.R, gBattle.ngL, gBattle.ngR);
        //var str = core.getSpellList(gBattle.L + gBattle.ngL, gBattle.R + gBattle.ngR, 0);
        //console.log("mainWindow.getSpellList", str);
        var arr = gBattle.warlocks[0].spells, s, idx;
        /*if (new_gesture !== '') {
            console.log("mainWindow.getSpellList", JSON.stringify(arr));
        }*/
        var uncompleted_cnt = 0, i, Ln;
        for (i = 0, Ln = arr.length; i < Ln; ++i) {
            s = arr[i];
            s.choose = 0;
            s.row_type = 1;
            if (s.h !== gBattle.currentHandIdx) {
                continue;
            }

            if (new_gesture !== '') {
                if (s.ng === new_gesture) {
                    console.log("mainWindow.getSpellList iteration", i, new_gesture, JSON.stringify(s));
                    if (s.t === 1) {
                        s.gp = '<font color="#10C9F5">'+s.g+'</font>';
                        s.cast_type = 1;
                        arr_cast_now.push(s);
                    } else {
                        idx = s.g.length - s.t + 1;
                        s.gp = '<font color="#10C9F5">'+s.g.substr(0, idx)+'</font><font color="#FEE2D6">'+s.g.substr(idx)+'</font>';
                        s.cast_type = 2;
                        arr_cast_later/*[s.sg]*/.push(s);
                        ++uncompleted_cnt;
                    }
                }
            } else {
                console.log("mainWindow.getSpellList iteration2", i, new_gesture, JSON.stringify(s));
                idx = s.g.length - s.t;
                s.gp = '<font color="#10C9F5">'+s.g.substr(0, idx)+'</font><font color="#FEE2D6">'+s.g.substr(idx)+'</font>';
                s.cast_type = 3;
                arr_cast_later/*[s.sg]*/.push(s);
                ++uncompleted_cnt;
            }
        }
        //arr_cast_now.push();

        gBattle["completed_spell_" + gBattle.currentHand] = arr_cast_now.length > 0 ? 1 : 0;
        if (new_gesture !== '') {
            gBattle.spellIdx = arr_cast_now.length;
            var def_or_none = ((gBattle.spellIdx > 0) || gBattle.player_changed_mind);
            if (def_or_none) {
                gBattle.spellIdx = 1 + arr_cast_now.length;
            }
            var def_spell = def_or_none ? {gp:"?",n:"Default",choose:1,t:1,cast_type:1,row_type:1} : {gp:"None",n:"",choose:0,t:1,cast_type:0,row_type:1};
            res.push({gp:"?",n:"Completed spells",choose:0,t:0,cast_type:100,row_type:2});
            res = res.concat(arr_cast_now);
            res.push(def_spell);
        } else {
            res.push({gp:"?",n:"Spellbook",choose:0,t:0,cast_type:100,row_type:3});
        }
        if (uncompleted_cnt > 0) {
            if (new_gesture !== '') {
                res.push({gp:"?",n:"Uncompleted spells",choose:0,t:0,cast_type:100,row_type:2});
            }
            res = res.concat(arr_cast_later);
        }

        return res;
    }

    function setGesture(gesture, spell, need_target) {
        var is_maladroit = (gBattle.warlocks[0].maladroit > 0) || ((gesture === "C") && !(gBattle.warlocks[0].amnesia > 0));
        console.log("setGesture", gesture, JSON.stringify(spell), is_maladroit);
        gBattle.actions[gBattle.currentHand] = {g:gesture,s:spell};
        if (is_maladroit && (gBattle.actions[gBattle.otherHand].g !== gesture)) {
            gBattle["ng" + gBattle.otherHand] = gesture;
            gBattle.actions[gBattle.otherHand] = {g:gesture,s:{gp:"?",n:"Default",choose:1,t:1,cast_type:1,need_target:true}};
        }

        WNDU.arr_wnd_instance[WNDU.wnd_battle].setGesture(gesture, is_maladroit);
        if (need_target) {
            WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(true, spell.n);
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
    }

    function chooseMonsterTarget(title) {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(false, title);
        WNDU.processEscape();
    }

    function setSpellTarget(TargetName, Permanent, Delay, OperationType) {
        console.log("main.setSpellTarget", TargetName, Permanent, Delay, OperationType, gBattle.currentHand);
        if (OperationType === 1) {
            gBattle.actions[gBattle.currentHand].target = TargetName;
            if (Permanent === 1) {
                gBattle.actions.P = gBattle.currentHandIdx;
            }
            if (Delay === 1) {
                gBattle.actions.D = gBattle.currentHandIdx;
            }
        } else if (OperationType === 2) {
            console.log("before", gBattle.currentMonsterIdx, JSON.stringify(gBattle.actions.M[gBattle.currentMonsterIdx]));
            gBattle.actions.M[gBattle.currentMonsterIdx].target = TargetName;
            console.log("after", gBattle.currentMonsterIdx, JSON.stringify(gBattle.actions.M[gBattle.currentMonsterIdx]));
        }
    }

    function chooseCharm(Action, H, G, data) {
        gERROR = {h:H,g:G,is_paralyze:Action === "paralized",title:warlockDictionary.getStringByCode("TitleAction_" + Action)};
        if (gERROR.is_paralyze) {
            gERROR.lgL = data.lgL;
            gERROR.lgR = data.lgR;
        }

        WNDU.showCharm();
    }

    function setCharm(H, G) {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].setCharm(H, G);
        WNDU.processEscape();
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

    function confirmOrdersEx() {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].sendOrders();
        WNDU.closeChilds();
    }

    function processEscape() {
        WNDU.processEscape();
    }

    function storeWnd(wnd) {
        WNDU.storeWnd(wnd);
    }

    function closeChild() {
        WNDU.closeChild();
    }

    function linkActivated(link) {
        GUI.linkActivated(link)
    }

    function changeTimerState() {
        timerCounter = 0;
        if (core.timerState) {
            if (!tScanTimer.running) {
                tScanTimer.start();
            }
        } else {
            if (tScanTimer.running) {
                tScanTimer.stop();
            }
        }
    }

    /*function changeGesture(Gesture, Left) {
        MUtils.changeGesture(Gesture, Left);
    }*/

    function logEvent(event_name, params) {
        if (core.isAI) {
            return;
        }

        if (!params) {
            params = {};
        }

        console.log("logEvent", event_name, JSON.stringify(params));
        //analytics.logEvent(event_name, params);
    }

    function calculateRatio() {
        var w, h, currOs = Qt.platform.os;
        if ((currOs === "android") || (currOs === "ios") || (currOs === "blackberry") || (currOs === "winphone")) {
            w = Screen.width;// .desktopAvailableWidth;
            h = Screen.height;// .desktopAvailableHeight;
        } else {
            w = mainWindow.width;
            h = mainWindow.height;
        }
        var dpi = Screen.pixelDensity;
        var wh = Math.max(w, h);
        var ww = Math.min(w, h);
        var bh = 1068;//800;
        var bw = 600;
        var bdip = 3.74;
        console.log(dpi, bdip, ww, wh, bw, bh);
        ratioObject = Math.min(ww/bw, wh/bh);
        ratioFont = Math.max(1, Math.min((wh*bdip)/(dpi*bh), (ww*bdip/(dpi*bw))));
    }

    function creationFinished() {
        console.log("iWndContainer x y", iWndContainer.x, iWndContainer.y);
        calculateRatio();
        logEvent("gameFieldReady", {ratio_o:ratioObject, ratio_f: ratioFont});
        //GUI.prepareNewGameBtn(1500);
    }

    Component.onCompleted: creationFinished()
}
