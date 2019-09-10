//import QtQuick 2.2
//import QtQuick.Window 2.1
//import QtQuick.Controls 1.1

import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
//import QtQuick.Controls 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import ua.sp.warloksduel 1.8
import ua.sp.warlockdictionary 1.0

import "qrc:/js/main_utils.js" as MUtils
import "qrc:/js/ai_utils.js" as AI
import "qrc:/js/user_profile_utils.js" as UU

import "qrc:/qml"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 507
    height: 900

    property real height_koeff: rMain.height / 800
    property bool exit_on_back_button: true
    property var  battles: MUtils.loadChallengesList(true)
    property var  top_player: MUtils.loadTopList(true)
    property var  spells: MUtils.default_spell_list
    property bool action_send_order: true

    menuBar: MenuBar {
        //visible: false
            Menu {
                id: mMain
                title: warlockDictionary.getStringByCode("Settings")

                MenuItem {
                    id: miAcc
                    text: warlockDictionary.getStringByCode("LoginSett")
                    onTriggered: {
                        getLoginFromUser();
                    }
                }

                MenuItem {
                    id: miProxy
                    text: warlockDictionary.getStringByCode("NetworkSett")
                    onTriggered: {
                        showProxySettings();
                    }
                }

                MenuItem {
                    id: miLang
                    text: warlockDictionary.getStringByCode("LangSett")
                    onTriggered: {
                        showLangSettings();
                    }
                }

                MenuSeparator {
                }


                MenuItem {
                    id: miExit
                    text: warlockDictionary.getStringByCode("Exit")
                    onTriggered: Qt.quit()
                }
            }

            Menu {
                id: mLoginsMenu
                title: warlockDictionary.getStringByCode("Accounts")

                MenuItem {
                    id: miLogin0
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(0);
                }

                MenuItem {
                    id: miLogin1
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(1);
                }

                MenuItem {
                    id: miLogin2
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(2);
                }

                MenuItem {
                    id: miLogin3
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(3);
                }

                MenuItem {
                    id: miLogin4
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(4);
                }

                MenuItem {
                    id: miLogin5
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(5);
                }

                MenuItem {
                    id: miLogin6
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(6);
                }

                MenuItem {
                    id: miLogin7
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(7);
                }

                MenuItem {
                    id: miLogin8
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(8);
                }

                MenuItem {
                    id: miLogin9
                    visible: false
                    text: ""
                    onTriggered: MUtils.autoLogin(9);
                }
            }
    }

    MessageDialog {
        id: mdNoGesture
        title: warlockDictionary.getStringByCode("AreYouSure")
        icon: StandardIcon.Warning
        text: warlockDictionary.getStringByCode("NoGestureForTurn")
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        //Component.onCompleted: visible = true
        onAccepted: {
            if (action_send_order) {
                MUtils.sendOrderEx();
            } else {
                Qt.quit();
            }
        }
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
                    action_send_order = false;
                    mdNoGesture.text = warlockDictionary.getStringByCode("ConfirmExit")
                    mdNoGesture.visible = true
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
                core.scanState();
                if (--rMain.timerCounter <= 0) {
                    tScanTimer.interval = core.isAI ? 10000 : 60000;
                }
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
                id: rsvReadyBattle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: rGesture.top


                /*Controls1.*/ScrollView {
                    id: svReadyBattle
                    anchors.fill: parent
                    clip: true

                    Flickable {
                        id: lvReadyBattle
                        //x: 0
                        //y: 0
                        anchors.fill: parent
                        contentWidth: rsvReadyBattle.width
                        contentHeight: 2000


                        Text {
                              id: tStepMsg
                              anchors.left: parent.left
                              anchors.right: parent.right
                              anchors.top: parent.top
                              anchors.topMargin: 8
                              ////font.pointSize: 13 * height_koeff
                              wrapMode: Text.WordWrap
                              textFormat: Text.RichText
                              //elide: Text.ElideMiddle
                              text: ""
                        }

                        Text {
                            id: tMonsterList
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: tStepMsg.bottom
                            anchors.topMargin: 8
                            text: ""
                            ////font.pointSize: 13 * height_koeff
                            textFormat: Text.RichText
                         }

                        Rectangle {
                            id: svWarlocks
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: tMonsterList.bottom
                            anchors.topMargin: 8
                            border.width: 1
                            border.color: "grey"
                            radius: 3
                        }

                        Rectangle {
                            id: svMonsterOrders
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: svWarlocks.bottom
                            anchors.topMargin: 8
                            //border.width: 1
                            //border.color: "red"
                            //radius: 3
                        }

                        Rectangle {
                            id: lvaoDelay
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: svMonsterOrders.bottom
                            anchors.topMargin: 8
                            height: cbDelay.height
                            visible: false
                            //border.width: 1
                            //border.color: "blue"
                            //radius: 3

                            Text {
                                id: tDelayLabel
                                anchors.left: parent.left
                                anchors.verticalCenter: cbDelay.verticalCenter
                                text: warlockDictionary.getStringByCode("DelaySpell")
                                ////font.pointSize: 13 * height_koeff
                            }

                            ComboBox {
                                id: cbDelay
                                anchors.left: tDelayLabel.right
                                anchors.leftMargin: 8
                                anchors.top: parent.top
                                width: 0.5 * parent.width
                                model: [warlockDictionary.getStringByCode("Neither"),
                                        warlockDictionary.getStringByCode("Left"),
                                        warlockDictionary.getStringByCode("Right"),
                                        warlockDictionary.getStringByCode("Two-handed")]
                            }
                        }

                        Rectangle {
                            id: lvaoPermanent
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: lvaoDelay.bottom
                            anchors.topMargin: 8
                            height: cbPermanent.height
                            visible: false

                            Text {
                                id: tPermanentLabel
                                anchors.left: parent.left
                                anchors.verticalCenter: cbPermanent.verticalCenter
                                text: warlockDictionary.getStringByCode("MakeSpellPermanent")
                                ////font.pointSize: 13 * height_koeff
                            }

                            ComboBox {
                                id: cbPermanent
                                anchors.left: tPermanentLabel.right
                                anchors.leftMargin: 8
                                anchors.top: parent.top
                                width: 0.5 * parent.width
                                model: [warlockDictionary.getStringByCode("Neither"),
                                        warlockDictionary.getStringByCode("Left"),
                                        warlockDictionary.getStringByCode("Right"),
                                        warlockDictionary.getStringByCode("Two-handed")]
                            }
                        }

                        Rectangle {
                            id: lvaoFire
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: lvaoPermanent.bottom
                            anchors.topMargin: 8
                            height: cbFire.height
                            visible: false

                            CheckBox {
                                id: cbFire
                                anchors.left: parent.left
                                anchors.leftMargin: 8
                                anchors.top: parent.top
                                text: ""
                                //////font.pointSize: 13
                            }
                        }

                        Rectangle {
                            id: lvaoCharmPerson
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: lvaoFire.bottom
                            anchors.topMargin: 8

                        }

                        Rectangle {
                            id: lvaoParalyze
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: lvaoCharmPerson.bottom
                            anchors.topMargin: 8
                        }
                    }
                }
            }

            Rectangle {
                id: rGesture
                height: rLHG.height * 2.1
                //y: rWarlocks.y + rWarlocks.height
                anchors.bottom: rSendOrder.top
                anchors.bottomMargin: 0.01 * parent.height
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
                        width: 0.05 * parent.width
                        text: "L"
                        //text: warlockDictionary.getStringByCode("LeftHand")
                        ////font.pointSize: 13 * height_koeff
                    }

                    ComboBox {
                        id: cbLHG
                        anchors.left: tLHGLabel.right
                        //anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        //width: 75
                        width: 0.25 * parent.width
                        //onCurrentIndexChanged: MUtils.getPossibleSpell(0)
                        onCurrentTextChanged: MUtils.getPossibleSpell(0)
                        model: [" "]
                    }

                    ComboBox {
                        id: cbLHS
                        anchors.left: cbLHG.right
                        //anchors.leftMargin: 0.01 * parent.width
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
                        //anchors.leftMargin: 0.01 * parent.width
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
                    anchors.top: rLHG.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: cbLHG.height

                    Text {
                        id: tRHGLabel
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.05 * parent.width
                        text: "R"
                        //text: warlockDictionary.getStringByCode("RightHand")
                        ////font.pointSize: 13 * height_koeff
                    }

                    ComboBox {
                        id: cbRHG
                        anchors.left: tRHGLabel.right
                        //anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.25 * parent.width
                        model: [" "]
                        //onCurrentIndexChanged: MUtils.getPossibleSpell(1)
                        onCurrentTextChanged: MUtils.getPossibleSpell(1)
                    }

                    ComboBox {
                        id: cbRHS
                        anchors.left: cbRHG.right
                        //anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.4 * parent.width
                        model: [" "]
                    }

                    ComboBox {
                        id: cbRHT
                        anchors.left: cbRHS.right
                        //anchors.leftMargin: 0.01 * parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        width: 0.3 * parent.width
                        model: [" "]
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

                    ////font.pointSize: 12 * height_koeff
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
                        ////font.pointSize: 12 * height_koeff
                        inputMethodHints: Qt.ImhNoPredictiveText + Qt.ImhNoAutoUppercase
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
                        ////font.pointSize: 16 * height_koeff
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
                    //anchors.fill: parent
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: btnAddDuel.top
                    model: battles

                    delegate:
                        Item {
                            id: lvdItemTop
                            height: rvChallengeState.height + rlvdItemTitle.height * 2 + lvdItemText.height + 0.01 * mainWindow.height * 3 + 0.02 * mainWindow.height
                            width: mainWindow.width

                            Rectangle {
                                id: lvdItem
                                //anchors.fill: parent
                                anchors.top: parent.top
                                anchors.topMargin: 0.01 * mainWindow.height
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 0.01 * mainWindow.height
                                anchors.left: parent.left
                                anchors.right: parent.right

                                border.width: 3
                                border.color: battles[index].level_color
                                radius: 7


                                Rectangle {
                                    id: rlvdItemTitle
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    height: lvdItemTitle.height + 0.01 * mainWindow.height
                                    color: battles[index].level_color

                                    Text {
                                        id: lvdItemTitle
                                        anchors.left: parent.left
                                        anchors.leftMargin: 0.05 * parent.width
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.right: parent.right
                                        //anchors.top: parent.top
                                        //anchors.topMargin: 0.01 * mainWindow.height
                                        //////font.pointSize: 13 * height_koeff
                                        wrapMode: Text.WordWrap
                                        font.bold: true
                                        text: battles[index].logins
                                    }
                                }

                                RowLayout {
                                    id: rvChallengeState
                                    anchors.left: parent.left
                                    anchors.leftMargin: 0.01 * parent.width
                                    anchors.right: parent.right
                                    anchors.rightMargin: 0.01 * parent.width
                                    anchors.top: rlvdItemTitle.bottom
                                    anchors.topMargin: 0.01 * mainWindow.height
                                    //visible: !battles[index].is_new_btn

                                    Text {
                                        id: lvdItemLevel
                                        text: battles[index].level
                                    }

                                    CheckBox {
                                        id: cbItemFast
                                        enabled: false
                                        checked: battles[index].fast === 1
                                        text: warlockDictionary.getStringByCode("Fast")
                                    }

                                    CheckBox {
                                        id: cbItemParaFC
                                        enabled: false
                                        checked: battles[index].parafc === 1
                                        text: warlockDictionary.getStringByCode("ParaFC")
                                    }

                                    CheckBox {
                                        id: cbItemMaladroit
                                        enabled: false
                                        checked: battles[index].maladroit === 1
                                        text: warlockDictionary.getStringByCode("Maladroit")
                                    }
                                }

                                Text {
                                    id: lvdItemText
                                    anchors.left: parent.left
                                    anchors.leftMargin: 0.01 * parent.width
                                    anchors.right: parent.right
                                    anchors.rightMargin: 0.01 * parent.width
                                    anchors.top: rvChallengeState.bottom
                                    anchors.topMargin: 0.01 * mainWindow.height
                                    //////font.pointSize: 13 * height_koeff
                                    wrapMode: Text.WordWrap
                                    // QString("{\"logins\":\"%1\",\"fast\":%2,\"level\":\"%3\",\"parafc\":%4,\"maladroit\":%5,\"desc\":\"%6\",\"battle_id\":%7}")
                                    text: battles[index].desc
                                        /*battles[index].level + " " +
                                     (battles[index].fast === 1 ? "Fast " : "") + (battles[index].parafc === 1? "ParaFC " : "") +
                                     (battles[index].maladroit === 1 ? "Maladroit " : "") + "\n" + battles[index].desc //*/
                                    //visible: !battles[index].is_new_btn
                                }

                                Rectangle {
                                    id: lvdItemAccept
                                    anchors.left: parent.left
                                    anchors.leftMargin: lvdItem.border.width
                                    anchors.right: parent.right
                                    anchors.rightMargin: lvdItem.border.width
                                    //anchors.top: lvdItemText.bottom
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: lvdItem.border.width
                                    height: rlvdItemTitle.height

                                    color: ma_accept.pressed ? "blue" : "lightblue"
                                    radius: 3
                                    border.width: 1
                                    border.color: "lightblue"

                                    Text {
                                        id: label_order_up
                                        text: warlockDictionary.getStringByCode("Accept")
                                        anchors.centerIn: parent
                                        //////font.pointSize: 13 * height_koeff
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
                                            var battle_id = battles[index].battle_id;
                                            console.log("accept battle", battle_id);
                                            MUtils.acceptChallenge('/accept/' + battle_id);
                                        }
                                    }
                                }
                         }
                    }

                    header: Rectangle {
                        id: lvdHeader
                        height: 0.1 * mainWindow.height
                        width: mainWindow.width
                        color: "lightgrey"

                        LargeText {
                            id: lvdHeaderText
                            anchors.fill: parent
                            text: warlockDictionary.getStringByCode("BattleList")
                            color: "white"
                        }
                    }
            }

            Rectangle {
                id: btnAddDuel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 0.06 * parent.height

                color: core.allowedAdd ? "lightblue" : "lightgrey"
                radius: 7
                border.width: 1
                border.color: core.allowedAdd ? "blue" : "grey"

                Text {
                    id: lbAddDuel
                    text: warlockDictionary.getStringByCode("AddChallenge")
                    anchors.centerIn: parent
                    //////font.pointSize: 13 * height_koeff
                }

                MouseArea {
                    id: maAddDuel
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        addDuel();
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

                    delegate: Rectangle {
                        id: lvsItem
                        height: 0.04 * mainWindow.height
                        width: mainWindow.width

                        LargeText {
                            id: lvsItemGestures
                            anchors.left: parent.left
                            anchors.top: parent.top
                            //anchors.topMargin: 0.01 * mainWindow.height
                            //anchors.bottom: parent.bottom

                            font.pixelSize: 20 * height_koeff
                            //wrapMode: Text.WordWrap
                            height: 0.035 * mainWindow.height
                            textFormat: Text.RichText
                            text: spells[index].g
                            width: 0.2 * mainWindow.width

                            horizontalAlignment: Text.AlignLeft
                        }

                        LargeText {
                            id: lvsItemHint
                            anchors.left: lvsItemGestures.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            /*anchors.left: lvsItemGestures.right
                            anchors.leftMargin: 0.01 * mainWindow.width
                            anchors.top: parent.top
                            anchors.topMargin: 0.01 * mainWindow.height*/
                            //////font.pointSize: 13 * height_koeff
                            //wrapMode: Text.WordWrap
                            text: spells[index].h
                            width: 0.2 * mainWindow.width

                            horizontalAlignment: Text.AlignLeft
                        }

                        LargeText {
                            id: lvsItemName
                            anchors.right: parent.right
                            anchors.rightMargin: 0.005 * mainWindow.width
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            /*anchors.left: lvsItemHint.right

                            anchors.right: parent.left
                            anchors.top: parent.top
                            anchors.topMargin: 0.01 * mainWindow.height*/
                            //////font.pointSize: 13 * height_koeff
                            wrapMode: Text.WordWrap
                            text: spells[index].n
                            width: 0.6 * mainWindow.width

                            horizontalAlignment: Text.AlignRight
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

                        LargeText {
                            id: lvsHeaderText
                            anchors.fill: parent
                            text: warlockDictionary.getStringByCode("SpellList")
                            color: "white"
                        }
                    }
            }
        }

        Rectangle {
            id: rTopList
            visible: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: tbMain.top

            ListView {
                    id: tvTopList
                    parent: rTopList
                    anchors.fill: parent
                    model: top_player

                    delegate: Rectangle {
                        id: lvtItem
                        height: 0.02 * mainWindow.height
                        width: mainWindow.width
                        color: UU.getTopPlayerColor(top_player[index], index)
                        border.width: UU.getTopPlayerBroderWidth(top_player[index], index)
                        border.color: UU.getTopPlayerBroderColor(top_player[index], index)

                        LargeText {
                            id: lvtItemText
                            anchors.fill: parent
                            //textFormat: Text.RichText
                            text: UU.getTopPlayerDesc(top_player[index])
                            fontSizeMode: Text.VerticalFit
                            horizontalAlignment: Text.AlignLeft
                        }

                        /*LargeText {
                            id: lvtItemName
                            anchors.left: parent.left
                            anchors.top: parent.top
                            //anchors.topMargin: 0.01 * mainWindow.height
                            //anchors.bottom: parent.bottom

                            //font.pixelSize: 20 * height_koeff
                            //wrapMode: Text.WordWrap
                            //height: 0.035 * mainWindow.height
                            //textFormat: Text.RichText
                            text: top_player[index].login
                            width: 0.2 * mainWindow.width

                            horizontalAlignment: Text.AlignLeft
                        }

                        LargeText {
                            id: lvtItemValue
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            text: UU.getTopPlayerDesc(top_player[index])
                            width: 0.8 * mainWindow.width

                            horizontalAlignment: Text.AlignLeft
                        }*/



                        MouseArea {
                            id: maTopListItem
                            anchors.fill: parent

                            onClicked: {
                                console.log("top click", top_player[index].login);
                                core.getWarlockInfo(top_player[index].login);
                                //MUtils.linkActivated("/show_spell_desc/" + spells[index].code);
                            }
                        }
                    }

                    header: Item {
                        id: ltrHeader
                        height: 0.05 * mainWindow.height
                        width: mainWindow.width

                        LargeText {
                            id: ltrHeaderText
                            anchors.fill: parent
                            text: warlockDictionary.getStringByCode("TopList")
                            color: "white"
                        }
                    }
            }
        }

        ToolBar {
            id: tbMain
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 0.1 * parent.height

            RowLayout {
                anchors.fill: parent

                ToolButton {
                    id: tbList
                    icon.source: "res/list.png"
                    onClicked: MUtils.showList()
                }

                ToolButton {
                    id: tbFight
                    icon.source: "res/fight.png"
                    onClicked: MUtils.showDuel()
                }

                ToolButton {
                    id: tbBook
                    icon.source: "res/spellbook.png"
                    onClicked: MUtils.showSpellBook()
                }

                ToolButton {
                    id: tbUser
                    icon.source: "res/user.png"
                    onClicked: showUserProfile()
                }

                ToolButton {
                    id: tbAdd
                    icon.source: "res/first-place.png"
                    onClicked: openRecords()
                }

                ToolButton {
                    id: tbRefresh
                    icon.source: "res/refresh.png"
                    onClicked: refreshData()
                    Layout.alignment: Qt.AlignRight
                }

                /*ToolButton {
                    id: tbSettings
                    icon.source: "res/settings.png"
                    onClicked: showMainMenu()
                    Layout.alignment: Qt.AlignRight
                }*/
            }
        }
    }

    property string tipTxt;
    property bool allowCloseTip: false;

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
        MUtils.showWindow("tip_message.qml");
    }

    function showErrorMessage() {
        console.log("Error: ", Qt.core.errorMsg)
        if (!core.isAI) {
            MUtils.showWindow("error_message.qml");
        }
    }

    function showFinishedBattle() {
        console.log("showFinishedBattle")
        MUtils.showWindow("finished_battle.qml");
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
        console.log("closeChild", exit_on_back_button, MUtils.wndTmp);
        if (MUtils.wndTmp) {
            MUtils.wndTmp.destroy();
            MUtils.wndTmp = null;
        }
        exit_on_back_button = true;
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

    property var warlockDictionary: WarlockDictionary

    function changeLanguage(new_lang, child_wnd) {
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
    }

    function creationFinished() {
        Qt.core = core;
        Qt.mainWindow = mainWindow;
        Qt.height_coeff = height_koeff;
        MUtils.dict = warlockDictionary;
        MUtils.cChatMessage = warlockDictionary.getStringByCode("ChatMessage");
        MUtils.cMosterFrom = warlockDictionary.getStringByCode("MonsterFrom") + " ";
        MUtils.default_spell_list = JSON.parse(core.defaultSpellListHtml);
        if (core.login === '') {
            showNewUserMenu();
        } else {
            core.scanState();
        }
        MUtils.cleanOrders();
        MUtils.showList();
        MUtils.showLoginMenu(core.accountMenu);

        //tipTxt = warlockDictionary.getStringByCode("JustRegistered");
        //showTipMessage(true);
    }

    Component.onCompleted: creationFinished()
}
