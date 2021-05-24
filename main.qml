//import QtQuick 2.2
//import QtQuick.Window 2.1
//import QtQuick.Controls 1.1

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
    width: 507
    height: 900

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
        edge: Qt.RightEdge

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

            onClicked: {
                console.log("try refresh state");
                dMenu.close();
                core.scanState(0);
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
                console.log("cbLoginAs.onAccepted", currentIndex, currentValue, core.login);
                if ((currentIndex > 0) && (currentValue !== core.login)) {
                    dMenu.close();
                    core.autoLogin(currentIndex - 1)
                }
            }
        }
    }

    Rectangle {
        id: iBG
        anchors.fill: parent
        z: -1
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

    Rectangle {
            id: tbTop
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0.05 * parent.height
            color: "#210430"

            ToolButton {
                id: tbProfile
                //text: qsTr("‹")
                height: tbTop.height
                width: tbTop.height
                anchors.left: parent.left

                onClicked: {
                    WNDU.showSpellbook();
                }
                background: Image {
                    anchors.fill: parent
                    source: "res/spellbook.png"
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
                console.log("start game btn 1");
                GUI.startGame(0);
            }
        }

        BtnBig {
            id: bbNewBotGame
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("NewGameWithPlayer")
            transparent: true
            border.width: 0
            visible: core.allowedAdd || true

            width: 0.5 * parent.width
            height: 0.05 * parent.height
            anchors.top: bbNewGame.bottom
            anchors.topMargin: 0.01 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                console.log("start game btn 2");
                GUI.startGame(1);
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
                id: iMainScrollBody
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
                                anchors.right: rdbifIcon.left
                                //width: 0.90 * parent.width - parent.height
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
                                anchors.right: parent.right
                                anchors.rightMargin: 0.03 * parent.width
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
                                anchors.rightMargin: 0.03 * parent.width
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
            startRegisterFlowIndex = 4;
            WNDU.showRegisterFlow();
        } else {
            showNewUserMenu();
        }
    }

    function showTipMessage(TipTxt, allowClose) {
        console.log("Tip: ", TipTxt, allowClose);
        allowCloseTip = allowClose ? true : false;
        showErrorWnd({text:TipTxt,type:1});
    }

    function showErrorWnd(data) {
        WNDU.showErrorWnd(data);
    }

    function showErrorMessage() {
        console.log("Error: ", core.errorMsg)
        if (!core.isAI) {
            showErrorWnd({text:core.errorMsg});
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
        logEvent("showFinishedBattle", {battle_id:core.loadedBattleID});
        showErrorWnd({text:core.finishedBattle,type:2,title:"Battle #" + bit});
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
        var short_desc_code = spell_code + "_short_desc";
        var short_desc = warlockDictionary.getStringByCode(spell_code + "_short_desc");
        var msg_txt = "";
        if (short_desc !== short_desc_code) {
            msg_txt = short_desc + "<br>Details:<br>";
        }
        msg_txt += warlockDictionary.getStringByCode(spell_code + "_desc");
        showErrorWnd({text:msg_txt,type:1,title:warlockDictionary.getStringByCode(spell_code)});
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

    function getSpellList(new_gesture) {
        if (new_gesture === "FULL_LIST") {
            return getFullSpellbook();
        }

        console.log("mainWindow.getSpellList", gBattle.currentHand, gBattle.currentHandIdx, new_gesture);
        gBattle["ng" + gBattle.currentHand] = new_gesture;
        var res = [], arr_cast_now = [{gp:"?",n:"Default",choose:1,t:1,cast_type:1}], arr_cast_later = [], arr_cast_other = [];
        console.log("mainWindow.getSpellList", gBattle.L, gBattle.R, gBattle.ngL, gBattle.ngR);
        //var str = core.getSpellList(gBattle.L + gBattle.ngL, gBattle.R + gBattle.ngR, 0);
        //console.log("mainWindow.getSpellList", str);
        var arr = gBattle.warlocks[0].spells, s, idx;
        for (var i = 0, Ln = arr.length; i < Ln; ++i) {
            s = arr[i];
            s.choose = 0;
            if (s.h !== gBattle.currentHandIdx) {
                continue;
            }

            if (new_gesture !== '') {
                if (s.t === 1) {
                    if (s.ng === new_gesture) {
                        s.gp = '<font color="#A8F4F4">'+s.g+'</font>';
                        s.cast_type = 1;
                        arr_cast_now.push(s);
                    } /*else {
                        idx = s.g.length - s.t + 1;
                        s.gp = '<font color="#A8F4F4">'+s.g.substr(0, idx)+'</font><font color="#E7FFFF">'+s.g.substr(idx)+'</font>';
                        s.cast_type = 2;
                        arr_cast_later.push(s);
                    }*/
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
                    s.gp = '<font color="#A8F4F4">'+s.g.substr(0, idx)+'</font><font color="#E7FFFF">'+s.g.substr(idx)+'</font>';
                    s.cast_type = 3;
                    arr_cast_other.push(s);
                //}
            }
        }
        //arr_cast_now.push();
        gBattle.spellIdx = 0;
        return arr_cast_now.concat(arr_cast_later).concat(arr_cast_other);
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
            WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(true);
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

        WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(true);
    }

    function chooseMonsterTarget() {
        WNDU.arr_wnd_instance[WNDU.wnd_battle].prepareToTargeting(false);
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

    function creationFinished() {

        logEvent("gameFieldReady");
        //GUI.prepareNewGameBtn(1500);
    }

    Component.onCompleted: creationFinished()
}
