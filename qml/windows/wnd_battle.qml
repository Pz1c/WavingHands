// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"
import "qrc:/js/battle_utils.js" as BU
import "qrc:/js/ai_utils.js" as AI

BaseWindow {
    id: battleItem

    with_controls: false
    //with_apply: false
    body_width_prc: 100
    body_height_prc: 95
    title_height_prc: 5
    control_height_prc: 0
    with_action1: true
    action1_text: "Chat"
    bg_source: "qrc:/res/background_battle.png"

    property int operationMode: 0
    // 0 - normal state
    // 1 - spell targeting
    property int permanency: 0
    property int delay: 0

    // This rectangle is the actual popup
    Item {
        id: battleWindow
        z: 11
        anchors.fill: content_item
        //color: "transparent"

        ScrollView {
            id: bfWarlocks
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: bfAll.top

            Item {
                id: iWarlocks
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }

        Item {
            id: bfAll
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0.1 * parent.height

            LargeText {
               id: ltAll
               text: "All"
               anchors.top: parent.top
               anchors.horizontalCenter: parent.horizontalCenter
               height: 0.5 * parent.height
               width: 0.3 * parent.width
               bg_visible: true
               bg_color: "#544653"
               color: "#FEE2D6"
               border_visible: false
               radius: 10
            }

            BtnBig {
                id: bbSendOrders
                text_color: "#ABF4F4"
                text: warlockDictionary.getStringByCode("SendOrders")
                bg_color_active: "#551470"
                border_color_active: "#551470"
                radius: 30
                visible: false

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#FEE2D6" }
                    GradientStop { position: 0.5; color: "#551470" }
                    GradientStop { position: 1.0; color: "#FEE2D6" }
                }

                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                height: 0.6 * parent.height
                width: 0.5 * parent.width

                onClicked: {
                    console.log("wnd_battle.initBattleFields", JSON.stringify(mainWindow.gBattle.actions));
                    mainWindow.showOrders(BU.getOrdersForReview(dict));
                }
            }

            IconInfo {
                id: iiElemental
                source: "qrc:/res/elemental_fire.png";
                text: "3"
                height: 0.9 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.05 * parent.height
                anchors.left: parent.left

                onClicked: {
                    iconClick(iiElemental.l_data)
                }

                onDoubleClicked: {
                    iconDoubleClick(iiElemental.l_data)
                }
            }

            IconInfo {
                id: iiChat
                source: "qrc:/res/chat.png";
                text: ""
                text_color: "red"
                height: 0.9 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.05 * parent.height
                anchors.right: parent.right

                onClicked: {
                    mainWindow.showBattleChat();
                }
            }

            IconInfo {
                id: iiNobody
                source: "qrc:/res/target_nobody.png";
                text: ""
                height: 0.9 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.05 * parent.height
                anchors.right: iiDefault.left
                visible: false

                onClicked: {
                    if (operationMode >= 1) {
                        iconClick({action:"hp",warlock_name:"Nobody"});
                    }
                }
            }

            IconInfo {
                id: iiDefault
                source: "qrc:/res/send_1.png";
                text: ""
                height: 0.9 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.05 * parent.height
                anchors.right: parent.right
                visible: false

                onClicked: {
                    if (operationMode >= 1) {
                        iconClick({action:"hp",warlock_name:"Default"});
                    }
                }
            }

            LargeText {
                id: ltHint
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 0.6 * parent.width
                visible: false
                text: "test hint 01"
                color: "snow"
                bg_color: "blue"
                bg_visible: true
                wrapMode: Text.Wrap

                onClicked: {
                    visible = false;
                }
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function iconClick(data) {
        console.log("wnd_battle.iconClick", JSON.stringify(data));
        if (operationMode >= 1) {
            if ((operationMode === 1) && (data.action === "permanency")) {
                permanency = !permanency ? 1 : 0;
            } else if ((operationMode === 1) && (data.action === "delay")) {
                delay = !delay ? 1 : 0;
            } else if ((data.action === "hp") || (data.action === "m")) {
                setTargetingOnOff(false, true);
                var target_name = data.action === "hp" ? data.warlock_name : data.name;
                mainWindow.setSpellTarget(target_name, permanency, delay, operationMode);
                operationMode = 0;
                if ((data.action === "m") && BU.checkIsMonsterCharmed(data)) {
                    //open target window for charmed monster
                    data.under_control = true;
                    iconDoubleClick(data);
                }
            } else {
                mainWindow.showErrorWnd({type:0,text:"Please choose Warlock or Monster as Spell target",title:"Wrong target"});
            }
        } else /*if (data.action === "m") {
            data.target = mainWindow.gBattle.actions.M[data.action_idx].target;
            mainWindow.gBattle.currentMonsterIdx = data.action_idx;
            mainWindow.showErrorWnd({type:4,text:"",title:"",data:data});
        } else */if (data.action === "banked") {
            mainWindow.gBattle.actions.F = data.checked ? 1 : 0;
        } else if (data.active) {
            console.log("DO some action", JSON.stringify(data));
            BU.battle.currentCharm = BU.getCharmDataByAction(data);
            mainWindow.chooseCharm(data.action, BU.battle.currentCharm.h, BU.battle.currentCharm.g, data);
        } else {
            iconDoubleClick(data);
        }
    }

    function iconDoubleClick(data) {
        console.log("wnd_battle.iconDoubleClick", JSON.stringify(data));
        var msg_text, msg_title, spell_code, msg_type = 1, target_name;

        switch(data.action) {
        case "hp":
            msg_title = "Warlock's hit point"
            msg_text = data.value + " damage to death";
            break;
        case "m":
            msg_title = "Monster"
            msg_text = data.name;
            if (data.owner !== "Nobody") {
                msg_text += " (owner by "+data.owner+")";
                msg_type = 4;
                console.log("get target", JSON.stringify(mainWindow.gBattle.actions), mainWindow.gBattle.actions.M[data.action_idx].target);
                data.target = mainWindow.gBattle.actions.M[data.action_idx].target;
                mainWindow.gBattle.currentMonsterIdx = data.action_idx;
                console.log("set target", data.target);
            }
            if (data.status) {
                msg_text += " " + data.status;
            }
            if (data.target) {
                msg_text += " attack " + data.target;
            }
            break;
        case "banked":
            msg_title = "Banked spell";
            msg_text = data.value + "<br>To get delayed spell cast Delay Effect DWSSSP<br>" + dict.getStringByCode("DWSSSP_desc");
            break;
        default: // spell
            spell_code = BU.icon_status_spell[data.action];
            if (spell_code) {
                msg_title = dict.getStringByCode(spell_code);
                msg_text  = dict.getStringByCode(spell_code + "_desc");
            } else {
                msg_title = "Charm";
                msg_text  = data.action;
            }
            break;
        }
        mainWindow.showErrorWnd({type:msg_type,text:msg_text,title:msg_title,data:data});
    }

    function setTargetingOnOff(Enable, IsSpell) {
        if (!Enable) {
            battleChanged();
        } else {
            bbSendOrders.visible = false;
        }

        for (var i = 0, Ln = iWarlocks.children.length; i < Ln; ++i) {
            iWarlocks.children[i].targetingOnOff(Enable, IsSpell);
        }
        //ltAll.text = Enable ? "Nobody" : "All";
        //ltAll.border_visible = Enable;
        iiNobody.visible = Enable;
        iiDefault.visible = Enable;
        iiElemental.border.width = Enable ? 3 : 0;
        iiChat.visible = !Enable;
        //iiChat.opacity = Enable ? 0.3 : 1;
    }

    function battleChanged() {
        bbSendOrders.visible = !(!mainWindow.gBattle.actions["L"].g || !mainWindow.gBattle.actions["R"].g);
    }

    function setGesture(gesture, is_maladroit) {
        iWarlocks.children[0].setGesture(mainWindow.gBattle.currentHand, 'g_' + BU.getIconByGesture(gesture));
        if (is_maladroit) {
            iWarlocks.children[0].setGesture(mainWindow.gBattle.otherHand, 'g_' + BU.getIconByGesture(gesture));
        }

        battleChanged();
    }

    function setCharm(hand, gesture) {
        if (BU.battle.currentCharm.warlock_idx > 0) {
            console.log(BU.battle.currentCharm.warlock_idx, "try to find", JSON.stringify(BU.battle.currentCharm));
            var ww = iWarlocks.children[BU.battle.currentCharm.warlock_idx];
            if (ww && ww.l_warlock) {
                ww.setGesture(hand.substr(0, 1), "g_" + BU.getIconByGesture(gesture));
            } else {
                console.log(BU.battle.currentCharm.warlock_idx, "now found warlock");
            }
        }

        BU.setCharm(hand, gesture);
    }

    function prepareToTargeting(is_spell) {
        setTargetingOnOff(true, is_spell);
        operationMode = is_spell ? 1 : 2;
        permanency = 0;
        delay = 0;
    }

    function sendOrders() {
        BU.prepareOrder();
    }

    function showWnd() {
        initBattleFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initBattleFields() {
        console.log("wnd_battle.initBattleFields", x, y, width, height, parent.x, parent.y, parent.width, parent.height);
        console.log("wnd_battle.initBattleFields", JSON.stringify(mainWindow.gBattle));
        if (!mainWindow.gBattle || !mainWindow.gBattle.id) {
            return;
        }
        title_text = "Battle #" + mainWindow.gBattle.id;
        BU.applyBattle();
        battleChanged();
        AI.processBattle(mainWindow.gBattle, mainWindow.gameCore.isAI);
        if (mainWindow.gameCore.isAI) {
            sendOrders();
        }
    }

    Component.onCompleted: {
        mainWindow.storeWnd(battleItem);
        initBattleFields();
    }
}
