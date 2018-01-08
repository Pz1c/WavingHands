//import QtQuick 2.2
//import QtQuick.Window 2.1
//import QtQuick.Controls 1.1

import QtQuick 2.1
import QtQuick.Controls 1.4 as Controls12
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.0

import ua.sp.warloksduel 1.8
import ua.sp.warlockdictionary 1.0

import "qrc:/main_utils.js" as MUtils

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 1420

    property real height_koeff: rMain.height / 800
    property bool exit_on_back_button: true
    property var  battles: MUtils.loadChallengesList(true)
    property var  spells: MUtils.default_spell_list

    MessageDialog {
        id: mdNoGesture
        title: warlockDictionary.getStringByCode("AreYouSure")
        icon: StandardIcon.Warning
        text: warlockDictionary.getStringByCode("NoGestureForTurn")
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        //Component.onCompleted: visible = true
        onAccepted: MUtils.sendOrderEx()
    }

    Rectangle {
        id: rMain
        anchors.fill: parent
        focus: true

        Keys.onPressed: {
            if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
                console.log("BACK_KEY_PRESSED: " + event.key)
                event.accepted = true;
                if (MUtils.wndTmp) {
                    closeChild();
                } else {
                    Qt.quit();
                }
            }
        }

        Timer {
            id: tScanTimer
            interval: 60000
            running: false
            repeat: true

            onTriggered: {
                console.log("start scanning");
                core.scanState()
            }
        }

        Rectangle {
            id: rReadyBattle
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: tbMain.top
            visible: false

            Rectangle {
                id: rTextLog
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 0.25 * parent.height
                border.width: 1
                border.color: "grey"
                radius: 3

                Controls12.ScrollView {
                    width: rTextLog.width
                    height: rTextLog.height

                    Text {
                          x: 0
                          y: 0
                          id: tStepMsg
                          width: mainWindow.width - 20
                          //height: 200
                          //anchors.fill: parent
                          text: ""
                          anchors.topMargin: 8
                          font.pointSize: 13 * height_koeff
                          wrapMode: Text.WordWrap
                          textFormat: Text.RichText
                          //elide: Text.ElideMiddle
                    }
                }
            }

            Rectangle {
                id: rMonsters
                height: (parent.height - tbMain.height) / 4
                anchors.top: rTextLog.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                border.width: 1
                border.color: "grey"
                radius: 3

                Controls12.ScrollView {
                    width: rMonsters.width
                    height: rMonsters.height

                    Text {
                          x: 0
                          y: 0
                          id: tMonsterList
                          width: mainWindow.width - 20
                          text: ""
                          anchors.topMargin: 8
                          font.pointSize: 13 * height_koeff
                          textFormat: Text.RichText
                     }
                }
            }

            Rectangle {
                id: rWarlocks
                height: 250
                anchors.top: rMonsters.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                border.width: 1
                border.color: "grey"
                radius: 3

                Controls12.ScrollView {
                    id: svrWarlocks
                    anchors.fill: parent
                    Rectangle {
                        id: svWarlocks
                        width: rWarlocks.width - 20
                    }
                }
            }

            Rectangle {
                id: rGesture
                height: rLHG.height * 2.1
                //y: rWarlocks.y + rWarlocks.height
                anchors.top: rWarlocks.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                border.width: 1
                border.color: "green"
                radius: 3

                Rectangle {
                    id: rLHG
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: cbLHG.height
                    //width: cbLHG.x + cbLHG.width

                    Text {
                        id: tLHGLabel
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.17 * parent.width
                        text: warlockDictionary.getStringByCode("LeftHand")
                        font.pointSize: 13 * height_koeff
                    }

                    ComboBox {
                        id: cbLHG
                        anchors.left: tLHGLabel.right
                        anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        //width: 75
                        width: 0.1 * parent.width
                        //onCurrentIndexChanged: MUtils.getPossibleSpell(0)
                        onCurrentTextChanged: MUtils.getPossibleSpell(0)
                        model: [" "]
                    }

                    ComboBox {
                        id: cbLHS
                        anchors.left: cbLHG.right
                        anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        //x: cbLHG.x + cbLHG.width + 3
                        //y: 0
                        //width: 130
                        width: 0.4 * parent.width
                        model: [" "]
                    }

                    ComboBox {
                        id: cbLHT
                        anchors.left: cbLHS.right
                        anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        //x: cbLHS.x + cbLHS.width + 3
                        //y: 0
                        //width: 130
                        width: 0.3 * parent.width
                        model: [" "]
                    }
                }

                Rectangle {
                    id: rRHG
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: cbRHG.height

                    Text {
                        id: tRHGLabel
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.17 * parent.width
                        text: warlockDictionary.getStringByCode("RightHand")
                        font.pointSize: 13 * height_koeff
                    }

                    ComboBox {
                        id: cbRHG
                        anchors.left: tRHGLabel.right
                        anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.1 * parent.width
                        model: [" "]
                        //onCurrentIndexChanged: MUtils.getPossibleSpell(1)
                        onCurrentTextChanged: MUtils.getPossibleSpell(1)
                    }

                    ComboBox {
                        id: cbRHS
                        anchors.left: cbRHG.right
                        anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.4 * parent.width
                        model: [" "]
                    }

                    ComboBox {
                        id: cbRHT
                        anchors.left: cbRHS.right
                        anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.3 * parent.width
                        model: [" "]
                    }
                }
            }

            Rectangle {
                id: rAdditionalOrders
                y: rGesture.y + rGesture.height
                height: 50
                width: parent.width
                border.width: 1
                border.color: "green"
                radius: 3

                Controls12.ScrollView {
                    id: svAdditionalOrders
                    width: rAdditionalOrders.width - 20

                    ListView {
                        id: lvaoAdditionalOrders

                        Rectangle {
                            id: lvaoDelay
                            height: cbDelay.height
                            visible: false
                            y: 0

                            Text {
                                id: tDelayLabel
                                x: 0
                                y: cbDelay.height / 2 - tDelayLabel.height / 2
                                text: warlockDictionary.getStringByCode("DelaySpell")
                                font.pointSize: 13 * height_koeff
                            }

                            ComboBox {
                                id: cbDelay
                                x: tDelayLabel.width + 3
                                y: 0
                                model: [warlockDictionary.getStringByCode("Neither"),
                                        warlockDictionary.getStringByCode("Left"),
                                        warlockDictionary.getStringByCode("Right"),
                                        warlockDictionary.getStringByCode("Two-handed")]
                                width: 100
                            }
                        }

                        Rectangle {
                            id: lvaoPermanent
                            height: cbPermanent.height
                            y: lvaoDelay.visible ? cbDelay.height + 2 : 0
                            visible: false

                            Text {
                                id: tPermanentLabel
                                x: 0
                                y: cbDelay.height / 2 - tDelayLabel.height / 2
                                text: warlockDictionary.getStringByCode("MakeSpellPermanent")
                                font.pointSize: 13 * height_koeff
                            }

                            ComboBox {
                                id: cbPermanent
                                x: tPermanentLabel.width + 3
                                y: 0
                                model: [warlockDictionary.getStringByCode("Neither"),
                                        warlockDictionary.getStringByCode("Left"),
                                        warlockDictionary.getStringByCode("Right"),
                                        warlockDictionary.getStringByCode("Two-handed")]
                                width: 100
                            }
                        }

                        Rectangle {
                            id: lvaoFire
                            height: tFireLabel.height
                            visible: false
                            y: (lvaoDelay.visible ? cbDelay.height + 2 : 0) + (lvaoPermanent.visible ? cbPermanent.height + 2 : 0)

                            Text {
                                id: tFireLabel
                                x: 0
                                y: 0//cbDelay.height / 2 - tDelayLabel.height / 2
                                font.pointSize: 13 * height_koeff

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: cbFire.checked = !cbFire.checked
                                }
                            }

                            CheckBox {
                                id: cbFire
                                x: tFireLabel.width + 3
                                y: tFireLabel.height / 2 - cbFire.height / 2
                                //font.pointSize: 13
                            }
                        }

                        Rectangle {
                            id: lvaoCharmPerson
                            y: (lvaoDelay.visible ? cbDelay.height + 2 : 0) + (lvaoPermanent.visible ? cbPermanent.height + 2 : 0) + (lvaoFire.visible ? lvaoFire.height + 2 : 0)

                        }

                        Rectangle {
                            id: lvaoParalyze
                            y: lvaoCharmPerson.y + lvaoCharmPerson.height + 2
                            x: 0

                        }
                    }
                }
            }

            Rectangle {
                id: rMonsterOrders
                height: 150
                width: parent.width
                y: rAdditionalOrders.y + rAdditionalOrders.height
                border.width: 1
                border.color: "green"
                radius: 3

                Controls12.ScrollView {
                    id: rsvMonsterOrders
                    width: rMonsterOrders.width - 20

                    Rectangle {
                        id: svMonsterOrders
                        width: rMonsterOrders.width - 20
                    }
                }
            }

            Rectangle {
                id: rSendOrder
                height: 0.1 * parent.height
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                //x: 0
                //y: tbMain.y - rSendOrder.height
                border.width: 1
                border.color: "grey"
                radius: 3

                Text {
                    id: tChatMessage
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    text: warlockDictionary.getStringByCode("ChatMessage")

                    font.pointSize: 12 * height_koeff
                }

                Rectangle {
                    id: rChatMsg
                    anchors.left: parent.left
                    anchors.right: btnDoit.left
                    anchors.top: tChatMessage.bottom
                    anchors.bottom: parent.bottom
                    radius: 7
                    border.width: 1
                    border.color: "black"

                    TextInput {
                        id: teChatMsg
                        width: parent.width - 6
                        height: parent.height - 2
                        anchors.centerIn: parent
                        text: ""
                        font.pointSize: 12 * height_koeff
                        inputMethodHints: Qt.ImhNoPredictiveText
                        //echoMode: TextInput.Password
                    }
                }

                Rectangle {
                    id: btnDoit
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: 0.25 * parent.width
                    height: parent.height
                    color: maDoit.pressed ? "green" : "lightgreen"
                    radius: 7
                    anchors.horizontalCenterOffset: 0
                    border.width: 3
                    border.color: "green"

                    Text {
                        id: labelDoit
                        anchors.centerIn: parent
                        text: warlockDictionary.getStringByCode("Submit")
                        font.pointSize: 16 * height_koeff
                    }

                    MouseArea {
                        id: maDoit
                        enabled: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        anchors.topMargin: 0
                        anchors.fill: parent
                        onClicked: {
                            MUtils.sendOrder()
                        }
                    }
                }
            }
        }

        Rectangle {
            id: rChallengeList
            visible: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: tbMain.top
            //width: parent.width
            //height: parent.height - tbMain.height

            ListView {
                    id: tvChallengeList
                    parent: rChallengeList
                    anchors.fill: parent
                    model: battles

                    delegate: Item {
                        id: lvdItem
                        height: lvdItemText.height + lvdItemAccept.height + 0.01 * mainWindow.height
                        width: mainWindow.width

                        Text {
                            id: lvdItemText
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.topMargin: 0.01 * mainWindow.height
                            font.pointSize: 13 * height_koeff
                            wrapMode: Text.WordWrap
                            // QString("{\"logins\":\"%1\",\"fast\":%2,\"level\":\"%3\",\"parafc\":%4,\"maladroit\":%5,\"desc\":\"%6\",\"battle_id\":%7}")
                            text: battles[index].logins + ".\n" + battles[index].level + " " +
                             (battles[index].fast === 1 ? "Fast " : "") + (battles[index].parafc === 1? "ParaFC " : "") +
                             (battles[index].maladroit === 1 ? "Maladroit " : "") + "\n" + battles[index].desc //*/
                        }

                        Rectangle {
                            id: lvdItemAccept
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: lvdItemText.bottom
                            //anchors.bottom: parent.bottom
                            height: 0.05 * mainWindow.height

                            color: ma_accept.pressed ? "blue" : "lightblue"
                            radius: 7
                            border.width: 0
                            border.color: "blue"

                            Text {
                                id: label_order_up
                                text: warlockDictionary.getStringByCode("Accept")
                                anchors.centerIn: parent
                                font.pointSize: 13 * height_koeff
                            }

                            MouseArea {
                                id: ma_accept
                                anchors.rightMargin: 0
                                anchors.bottomMargin: 0
                                anchors.leftMargin: 0
                                anchors.topMargin: 0
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var battle_id = battles[tvChallengeList.currentIndex].battle_id;
                                    console.log("accept battle", battle_id);
                                    //MUtils.acceptChallenge('/accept/' + battle_id);
                                }
                            }
                        }
                    }

                    header: Item {
                        id: lvdHeader
                        height: 0.05 * mainWindow.height
                        width: mainWindow.width

                        Text {
                            id: lvdHeaderText
                            anchors.centerIn: parent
                            font.pointSize: 15 * height_koeff
                            wrapMode: Text.WordWrap
                            text: warlockDictionary.getStringByCode("BattleList")
                        }
                    }
            }
        }

        Rectangle {
            id: rSpellList
            visible: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: tbMain.top

            ListView {
                    id: tvSpellList
                    parent: rSpellList
                    anchors.fill: parent
                    model: spells

                    delegate: Item {
                        id: lvsItem
                        height: lvsItemGestures.height
                        width: mainWindow.width

                        Text {
                            id: lvsItemGestures
                            anchors.left: parent.left
                            anchors.right: lvsItemHint.left
                            anchors.rightMargin: 0.01 * mainWindow.width
                            anchors.top: parent.top
                            anchors.topMargin: 0.01 * mainWindow.height
                            font.pointSize: 13 * height_koeff
                            wrapMode: Text.WordWrap
                            textFormat: Text.RichText
                            text: spells[index].g
                            width: 0.2 * mainWindow.width
                        }

                        Text {
                            id: lvsItemHint
                            anchors.left: lvsItemGestures.right
                            anchors.right: lvsItemName.left
                            anchors.rightMargin: 0.01 * mainWindow.width
                            anchors.top: parent.top
                            anchors.topMargin: 0.01 * mainWindow.height
                            font.pointSize: 13 * height_koeff
                            wrapMode: Text.WordWrap
                            text: spells[index].h
                            width: 0.2 * mainWindow.width
                        }

                        Text {
                            id: lvsItemName
                            anchors.left: lvsItemHint.right
                            anchors.right: parent.left
                            anchors.top: parent.top
                            anchors.topMargin: 0.01 * mainWindow.height
                            font.pointSize: 13 * height_koeff
                            wrapMode: Text.WordWrap
                            text: spells[index].n
                            width: 0.6 * mainWindow.width
                        }

                        MouseArea {
                            id: maSpell
                            anchors.fill: parent

                            onClicked: {
                                MUtils.linkActivated("/show_spell_desc/" + spells[index].code);
                            }
                        }
                    }

                    header: Item {
                        id: lvsHeader
                        height: 0.05 * mainWindow.height
                        width: mainWindow.width

                        Text {
                            id: lvsHeaderText
                            anchors.centerIn: parent
                            font.pointSize: 15 * height_koeff
                            wrapMode: Text.WordWrap
                            text: warlockDictionary.getStringByCode("SpellList")
                        }
                    }
            }

            /*Controls12.ScrollView {
                id: svSpellList
                width: rSpellList.width
                height: rSpellList.height

                Text {
                      x: 0
                      y: 0
                      id: tSpellList
                      width: mainWindow.width - 20
                      //height: 200
                      //anchors.fill: parent
                      text: ""
                      anchors.topMargin: 8
                      font.pointSize: 13 * height_koeff
                      wrapMode: Text.WordWrap
                      textFormat: Text.RichText
                      onLinkActivated: MUtils.linkActivated(link)
                }
            }*/

        }

        ToolBar {
            id: tbMain
            anchors.bottom: parent.bottom
            height: 0.1 * parent.height

            Controls12.ToolButton {
                id: tbList
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 0.05 * parent.height
                height: 0.8 * parent.height
                width: 0.8 * parent.height
                iconSource: "res/list.png"
                onClicked: MUtils.showList()
            }

            Controls12.ToolButton {
                id: tbFight
                anchors.top: parent.top
                anchors.left: tbList.right
                anchors.leftMargin: 0.1 * parent.height
                height: 0.8 * parent.height
                width: 0.8 * parent.height
                iconSource: "res/fight.png"
                onClicked: MUtils.showDuel()
            }

            Controls12.ToolButton {
                id: tbBook
                anchors.top: parent.top
                anchors.left: tbFight.right
                anchors.leftMargin: 0.1 * parent.height
                height: 0.8 * parent.height
                width: 0.8 * parent.height
                iconSource: "res/spellbook.png"
                onClicked: MUtils.showSpellBook()
            }

            Controls12.ToolButton {
                id: tbUser
                anchors.top: parent.top
                anchors.left: tbBook.right
                anchors.leftMargin: 0.1 * parent.height
                height: 0.8 * parent.height
                width: 0.8 * parent.height
                iconSource: "res/user.png"
                onClicked: showUserProfile()
            }

            Controls12.ToolButton {
                id: tbAdd
                anchors.top: parent.top
                anchors.left: tbUser.right
                anchors.leftMargin: 0.1 * parent.height
                height: 0.8 * parent.height
                width: 0.8 * parent.height
                iconSource: "res/add.png"
                onClicked: addDuel()
            }

            Controls12.ToolButton {
                id: tbRefresh
                anchors.top: parent.top
                anchors.left: tbAdd.right
                anchors.leftMargin: 0.1 * parent.height
                height: 0.8 * parent.height
                width: 0.8 * parent.height
                iconSource: "res/refresh.png"
                onClicked: refreshData()
            }

            Controls12.ToolButton {
                id: tbSettings
                anchors.top: parent.top
                anchors.left: tbRefresh.right
                anchors.leftMargin: 0.1 * parent.height
                height: 0.8 * parent.height
                width: 0.8 * parent.height
                iconSource: "res/settings.png"
                onClicked: showMainMenu()
            }
        }
    }

    function addDuel() {
        console.log("add duel");
        MUtils.showWindow("new_challenge.qml");
    }

    function refreshData() {
        console.log("user start scanning");
        core.scanState()
    }

    function showMainMenu() {
        MUtils.showWindow("menu_main.qml");
    }

    function showUserProfile() {
        MUtils.wndUP = Qt.createComponent("user_profile.qml");
        if (MUtils.wndUP.status === Component.Ready) {
            finishOpenUPWnd();
        } else {
            MUtils.wndUP.statusChanged.connect(finishOpenUPWnd);
        }
    }

    function finishOpenUPWnd() {
        if (MUtils.wndUP.status === Component.Ready) {
            MUtils.wndUP.createObject(mainWindow, {})
        } else if (MUtils.wndUP.status === Component.Error) {
            console.log("Error loading component:", MUtils.wndUP.errorString())
        }
    }

    function getLoginFromUser(child_wnd) {
        MUtils.showWindow("edit_login.qml", child_wnd);
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

    function showErrorMessage() {
        console.log("Error: ", Qt.core.errorMsg)
        MUtils.showWindow("error_message.qml");
    }

    function showFinishedBattle() {
        console.log("showFinishedBattle")
        MUtils.showWindow("finished_battle.qml");
    }

    function showReadyBattle() {
        console.log("showReadyBattle")
        MUtils.showReadyBattle();
    }

    function openBattleOnline(child_wnd) {
        if (child_wnd) {
            child_wnd.destroy();
        }

        Qt.openUrlExternally("https://games.ravenblack.net/warlocks?num=" + Qt.core.loadedBattleID + "&full=1")
    }

    function showNewUserMenu(child_wnd) {
        MUtils.showWindow("new_user_menu.qml", child_wnd);
    }

    function showSpellDetails(spell_code) {
        currSpell = spell_code;
        MUtils.showWindow("spell_details.qml");
    }

    function hideNewUserMenu() {
        console.log("hideNewUserMenu")
        MUtils.hideNewUserWnd()
    }

    function storeWnd(wnd) {
        MUtils.wndTmp = wnd;
        exit_on_back_button = false
    }

    function storeNewWnd(wnd) {
        MUtils.wndNew = wnd;
        exit_on_back_button = false
    }

    function closeChild() {
        if (MUtils.wndTmp) {
            MUtils.wndTmp.destroy()
            MUtils.wndTmp = null
        }
        exit_on_back_button = true
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

    WarlocksDuelCore {
        id: core
        onNeedLogin: getLoginFromUser()
        onErrorOccurred: showErrorMessage()
        onIsLoadingChanged: MUtils.isLoadingChanged()
        onFinishedBattleChanged: showFinishedBattle()
        onReadyBattleChanged: showReadyBattle()
        onRegisterNewUserChanged: hideNewUserMenu()
        onOrderSubmitedChanged: MUtils.cleanOrders()
        onTimerStateChanged: changeTimerState()
        onChallengeListChanged: MUtils.loadChallengesList(false)
        onSpellListHtmlChanged: MUtils.loadSpellList()
    }

    property var warlockDictionary: WarlockDictionary

    function changeLanguage(new_lang, child_wnd) {
        if (child_wnd) {
            child_wnd.destroy();
        }
        warlockDictionary.setCurrentLang(new_lang)
        setNewLanguage()
    }

    function setNewLanguage() {
        MUtils.default_spell_list = core.defaultSpellListHtml
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
        tCharMessage.text = warlockDictionary.getStringByCode("ChatMessage")
        labelDoit.text = warlockDictionary.getStringByCode("Submit")
        MUtils.cChatMessage = warlockDictionary.getStringByCode("ChatMessage")
        MUtils.cMosterFrom = warlockDictionary.getStringByCode("MonsterFrom") + " "
    }

    function creationFinished() {
        Qt.core = core
        Qt.mainWindow = mainWindow
        Qt.height_coeff = height_koeff;
        MUtils.dict = warlockDictionary
        MUtils.cChatMessage = warlockDictionary.getStringByCode("ChatMessage")
        MUtils.cMosterFrom = warlockDictionary.getStringByCode("MonsterFrom") + " "
        MUtils.default_spell_list = core.defaultSpellListHtml
        if (core.login === '') {
            showNewUserMenu()
        } else {
            core.scanState()
        }
        MUtils.cleanOrders()
        MUtils.showList()
    }

    Component.onCompleted: creationFinished()
}
