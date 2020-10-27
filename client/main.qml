//import QtQuick 2.2
//import QtQuick.Window 2.1
//import QtQuick.Controls 1.1

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import ua.sp.warloksduel 1.8
import ua.sp.warlockdictionary 1.0

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

    property var warlockDictionary: WarlockDictionary
    property real height_koeff: 1//rMain.height / 800
    property bool exit_on_back_button: true
    property var  battles: MUtils.loadChallengesList(true)
    property var  top_player: MUtils.loadTopList(true)
    property var  spells: MUtils.default_spell_list
    property bool action_send_order: true
    property string tipTxt;
    property bool allowCloseTip: false;

    Keys.onPressed: {
        console.log("loader.KEY_PRESSED: " + event.key)
        if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
            console.log("loader.BACK_KEY_PRESSED: " + event.key)
            event.accepted = true;
            WNDU.processEscape();
        }
    }

    AnalyticItem {
        id: analytics
        userId: //take uuid
            core.uuid
    }

    WarlocksDuelCore {
        id: core
        onNeedLogin: getLoginFromUser()
        onErrorOccurred: showErrorMessage()
        onIsLoadingChanged: MUtils.isLoadingChanged()
        onFinishedBattleChanged: showFinishedBattle()
        onReadyBattleChanged: {
            console.log("onReadyBattleChanged");
            showReadyBattle();
        }
        onRegisterNewUserChanged: {
                Qt.core.createNewChallenge(1, 0, 1, 1, 2, 2, "First battle, TRANING BOT ONLY");
                hideNewUserMenu();
                tipTxt = warlockDictionary.getStringByCode("JustRegistered");
                showTipMessage(true);
            }
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

            //mLoginsMenu.removeItem(mLoginsMenu.itemAt(0))
            //mLoginsMenu.addItem("test1");
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("‹")
                //onClicked: stack.pop()
            }
            Label {
                text: "Title"
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                text: qsTr("⋮")
                onClicked: dMenu.open()
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

    function addDuel() {
        console.log("add duel");
        if (core.allowedAdd) {
            MUtils.showWindow("new_challenge.qml");
        } else {
            tipTxt = warlockDictionary.getStringByCode("WaitTrainingBattle");
            showTipMessage(true);
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
        MUtils.showWindow("menu_main.qml");
    }

    function showUserProfile(other) {
        console.log("showUserProfile", other);
        Qt.show_info_self = other === true ? false : true;
        //MUtils.showWindow("user_profile.qml");
        MUtils.wndUP = Qt.createComponent("qrc:///qml/user_profile.qml");
        if (MUtils.wndUP.status === Component.Ready) {
            console.log("showUserProfile", MUtils.wndUP.status, "READY");
            finishOpenUPWnd();
        } else {
            console.log("showUserProfile", MUtils.wndUP.status, "?", MUtils.wndUP.errorString());
            MUtils.wndUP.statusChanged.connect(finishOpenUPWnd);
        }//*/
    }

    function finishOpenUPWnd() {
        console.log("finishOpenUPWnd", MUtils.wndUP.status, "READY");
        if (MUtils.wndUP.status === Component.Ready) {
            MUtils.wndUP.createObject(mainWindow, {})
            console.log("finishOpenUPWnd", "createObject");
        } else if (MUtils.wndUP.status === Component.Error) {
            console.log("Error loading component:", MUtils.wndUP.errorString())
        }
    }

    function getLoginFromUser(child_wnd, real_login) {
        if (real_login) {
            MUtils.showWindow("edit_login.qml", child_wnd);
        } else {
            showNewUserMenu(child_wnd);
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
        if (MUtils.wndUP) {
            MUtils.wndUP.destroy();
            MUtils.wndUP = null;
        }
    }

    function showTipMessage(allowClose) {
        console.log("Tip: ", tipTxt, allowClose);
        allowCloseTip = allowClose ? true : false;
        WNDU.showErrorWnd({text:tipTxt,type:1});
        //MUtils.showWindow("tip_message.qml");
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

    function openBattleOnline(child_wnd) {
        if (child_wnd) {
            child_wnd.destroy();
        }
        //getOnlineUrl

        Qt.openUrlExternally(core.getOnlineUrl());
    }

    function showNewUserMenu(child_wnd) {
        //MUtils.showWindow("new_user_menu.qml", child_wnd);
    }

    function showSpellDetails(spell_code) {
        currSpell = spell_code;
        WNDU.showErrorWnd({text:getSpellDescription(),type:1});
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

    property string currSpell: ""

    function getSpellDescription() {
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

    function creationFinished() {
        Qt.core = core;
        Qt.mainWindow = mainWindow;
        Qt.height_coeff = height_koeff;
        MUtils.dict = warlockDictionary;
        MUtils.cChatMessage = warlockDictionary.getStringByCode("ChatMessage");
        MUtils.cMosterFrom = warlockDictionary.getStringByCode("MonsterFrom") + " ";
        MUtils.default_spell_list = JSON.parse(core.defaultSpellListHtml);
        showNewUserMenu();
        //dMenu.open();
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
    }

    Component.onCompleted: creationFinished()
}
