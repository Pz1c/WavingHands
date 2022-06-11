// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"
import "qrc:/js/battle_utils.js" as BU
import "qrc:/js/battle_wnd_utils.js" as BWU
import "qrc:/js/ai_utils.js" as AI
import "qrc:/js/small_gui_utils.js" as SGU

BaseWindow {
    id: battleItem

    with_controls: false
    //with_apply: false
    body_width_prc: 100
    body_height_prc: 100
    title_height_prc: 0
    control_height_prc: 0
    with_action1: false
    action1_text: "Chat"
    bg_source: "qrc:/res/background_battle.png"

    property int operationMode: 0
    // 0 - normal state
    // 1 - spell targeting
    property var currentSpell: ({n:"",h:0,g:""})
    property int permanency: 0
    property int delay: 0
    property var target: ({target_name:""})

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
            contentWidth: -1
            contentHeight: iWarlocks.height

            Rectangle {
                id: rTutOverlay
                anchors.fill: parent
                color: "black"
                opacity: 0.5
                visible: ltTutorial.visible
                z:1000

                MouseArea {
                    id: maTutOver
                    anchors.fill: parent
                }
            }

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
            height: 120 * mainWindow.ratioObject

            LargeText {
               id: ltAll
               text: "All"
               anchors.top: parent.top
               anchors.horizontalCenter: parent.horizontalCenter
               height: 36 * mainWindow.ratioObject
               width: 246 * mainWindow.ratioObject
               bg_visible: true
               bg_color: "#544653"
               bg_radius: 10
               color: "#FEE2D6"
               border_visible: false
               visible: false
            }

            IconInfo {
                id: iiSpellbook
                source: "qrc:/res/spellbook.png";
                text: ""
                text_color: "red"
                height: 78 * mainWindow.ratioObject
                width: height
                iconHeight: 48 * mainWindow.ratioObject
                iconWidth: 48 * mainWindow.ratioObject
                anchors.verticalCenter: parent.verticalCenter
                //anchors.top: parent.top
                //anchors.topMargin: 3 * mainWindow.ratioObject
                anchors.right: iiScroll.left
                anchors.rightMargin: (32 + 32 + 78) * mainWindow.ratioObject
                radius: 10
                borderOnPress: false
                color: buttonPressed ? "#A8F4F4" : "transparent"

                onClicked: {
                    mainWindow.showWndSpellbook();
                    mainWindow.logEvent("Play_Spellbook_Click");
                }
            }

            IconInfo {
                id: iiChat
                source: "qrc:/res/chat.png";
                text: ""
                text_color: "red"
                height: 78 * mainWindow.ratioObject
                width: height
                iconHeight: 48 * mainWindow.ratioObject
                iconWidth: 48 * mainWindow.ratioObject
                anchors.verticalCenter: parent.verticalCenter
                //anchors.top: parent.top
                //anchors.topMargin: 10 * mainWindow.ratioObject
                anchors.right: iiScroll.left
                anchors.rightMargin: 32 * mainWindow.ratioObject
                radius: 10
                borderOnPress: false
                color: buttonPressed ? "#A8F4F4" : "transparent"
                visible: false

                onClicked: {
                    source = "qrc:/res/chat.png";
                    mainWindow.showBattleChat(mainWindow.gBattle.battle_chat);
                    mainWindow.logEvent("Play_Chat_Click");
                }
            }

            IconInfo {
                id: iiScroll
                source: "qrc:/res/scroll.png";
                text: ""
                text_color: "red"
                height: 78 * mainWindow.ratioObject
                width: height
                iconHeight: 48 * mainWindow.ratioObject
                iconWidth: 48 * mainWindow.ratioObject
                anchors.verticalCenter: parent.verticalCenter
                //anchors.top: parent.top
                //anchors.topMargin: 0 * mainWindow.ratioObject
                anchors.right: bbSendOrders.left
                anchors.rightMargin: 14 * mainWindow.ratioObject
                radius: 10
                borderOnPress: false
                color: buttonPressed ? "#A8F4F4" : "transparent"

                onClicked: {
                    mainWindow.showBattleHistory(mainWindow.gBattle.battle_hist);
                    mainWindow.logEvent("Play_History_Click");
                }
            }

            IconInfo {
                id: bbSendOrders
                active: false
                source: "qrc:/res/send_"+(active ? "1" : "0")+".png"
                textVisible: true

                height: 64 * mainWindow.ratioObject
                iconHeight: 64 * mainWindow.ratioObject
                iconWidth: 64 * mainWindow.ratioObject
                textHeight: 64 * mainWindow.ratioObject
                textWidth: 80 * mainWindow.ratioObject
                width: (64 + 18 + 80) * mainWindow.ratioObject
                textAnchors.left: bbSendOrders.left
                textAnchors.right: undefined
                iconAnchors.right: bbSendOrders.right
                iconAnchors.centerIn: undefined
                anchors.bottom: rTT.top
                //anchors.verticalCenter: parent.verticalCenter
                //anchors.top: parent.top
                //anchors.topMargin: 5 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 12 * mainWindow.ratioObject

                color: "transparent"
                text_color: active ? "#A8F4F4" : "#544653"
                text: "Submit"

                onClicked: {
                    console.log("wnd_battle.initBattleFields", JSON.stringify(mainWindow.gBattle.actions));
                    mainWindow.showOrders(BU.getOrdersForReview(dict));
                    mainWindow.logEvent("Play_Submit_Clicked");
                }
            }

            IconInfo {
                id: bbChooseTarget
                active: false
                source: "qrc:/res/send_"+(active ? "1" : "0")+".png"
                textVisible: true
                visible: false

                height: 64 * mainWindow.ratioObject
                iconHeight: 64 * mainWindow.ratioObject
                iconWidth: 64 * mainWindow.ratioObject
                textHeight: 64 * mainWindow.ratioObject
                textWidth: 80 * mainWindow.ratioObject
                width: (64 + 18 + 80) * mainWindow.ratioObject
                textAnchors.left: bbChooseTarget.left
                textAnchors.right: undefined
                iconAnchors.right: bbChooseTarget.right
                iconAnchors.centerIn: undefined
                anchors.bottom: rTT.top
                anchors.bottomMargin: 6 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 12 * mainWindow.ratioObject

                color: "transparent"
                text_color: active ? "#A8F4F4" : "#544653"
                text: "Done"

                onClicked: {
                    console.log("wnd_battle.initBattleFields", JSON.stringify(mainWindow.gBattle.actions));
                    setTargetingOnOff(false, true);
                    mainWindow.setSpellTarget(target.target_name, permanency, delay, operationMode);
                    operationMode = 0;
                    mainWindow.logEvent("Play_Target_Sumbit", {Target:target.target_name,Permanency:permanency,Delay:delay});
                    showHideSummonIcon();
                }
            }

            IconInfo {
                id: iiElemental
                source: "qrc:/res/elemental_fire.png";
                text: "3"
                height: 78 * mainWindow.ratioObject
                width: height
                anchors.bottom: rTT.top
                anchors.bottomMargin: 6 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 6 * mainWindow.ratioObject
                radius: 10

                onClicked: {
                    iconClick(iiElemental.l_data)
                }

                onDoubleClicked: {
                    iconDoubleClick(iiElemental.l_data)
                }
            }

            Rectangle {
                id: rTT
                color: "#544653"
                anchors.bottom: parent.bottom//iiNobody.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 36 * mainWindow.ratioObject
                visible: false

                Text {
                    id: tTargetTitle
                    anchors.left: parent.left
                    anchors.leftMargin: 6 * mainWindow.ratioObject
                    //anchors.bottom: parent.bottom
                    //anchors.bottomMargin: 6 * mainWindow.ratioObject
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 28 * mainWindow.ratioFont
                    color: "#FEE2D6"
                    text: ""
                    textFormat: Text.RichText
                }
            }

            IconInfo {
                id: iiNobody
                source: "qrc:/res/target_nobody.png";
                text: ""
                height: 78 * mainWindow.ratioObject
                iconHeight: 60 * mainWindow.ratioObject
                iconWidth: 60 * mainWindow.ratioObject
                width: height
                //anchors.verticalCenter: parent.verticalCenter
                anchors.bottom: rTT.top
                anchors.bottomMargin: 6 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: ((iiElemental.visible ? 84: 0) + 6) * mainWindow.ratioObject
                visible: false
                radius: 10
                l_data: ({action:"hp",warlock_name:"Nobody"})

                onClicked: {
                    if (operationMode >= 1) {
                        iconClick(l_data);
                    }
                }
            }

            IconInfo {
                id: iiDefault
                source: "qrc:/res/target_default.png";
                text: ""
                height: 78 * mainWindow.ratioObject
                iconHeight: 60 * mainWindow.ratioObject
                iconWidth: 60 * mainWindow.ratioObject
                width: height
                anchors.bottom: rTT.top
                anchors.bottomMargin: 6 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: ((iiElemental.visible ? 84 : 0) + 54) * mainWindow.ratioObject
                visible: false
                radius: 10
                l_data: ({action:"hp",warlock_name:"Default"})

                onClicked: {
                    if (operationMode >= 1) {
                        iconClick(l_data);
                    }
                }
            }

            LargeText {
                id: ltHint
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: iiElemental.right
                anchors.right: iiDefault.left
                visible: false
                text: "test hint 01"
                color: "#210430"
                bg_color: "#FEE2D6"
                bg_visible: true
                bg_radius: 10
                border_visible: false
                wrapMode: Text.Wrap
                font.pixelSize: 21 * mainWindow.ratioFont

                PropertyAnimation {
                    id: paHint
                    running: false
                    target: ltHint
                    property: 'visible'
                    to: false
                    duration: 1500 // turns to false after 1.5s
                }

                onClicked: {
                    visible = false;
                    paHint.stop();
                }
            }

            Rectangle {
                id: ltTutorial
                anchors.top: parent.top
                anchors.topMargin: 6 * mainWindow.ratioObject
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 18 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 24 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 24 * mainWindow.ratioObject
                z:2000
                visible: false
                color: "#FEE2D6"
                radius: 10
                property var tutorialData: ([])
                property int tutorialDataIdx: 0
                property alias text: ltTTT.text

                LargeText {
                    id: ltTTTPrev
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 18 * mainWindow.ratioObject
                    width: 30 * mainWindow.ratioObject
                    height: 0.80 * parent.height
                    fontSizeMode: Text.VerticalFit
                    text: "<"
                    color: "#210430"
                    visible: ltTutorial.tutorialDataIdx > 0
                    font.pixelSize: 21 * mainWindow.ratioFont
                    onClicked: {
                        showTutorialData(-1);
                    }
                }

                LargeText {
                    id: ltTTT
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: ltTTTPrev.right
                    anchors.leftMargin: 6 * mainWindow.ratioObject
                    anchors.right: ltTTTNext.left
                    anchors.rightMargin: 6 * mainWindow.ratioObject
                    font.pixelSize: 21 * mainWindow.ratioFont
                    text: "test hint 01"
                    color: "#210430"
                    wrapMode: Text.Wrap

                    onClicked: {
                        showTutorialData(1);
                    }
                }

                LargeText {
                    id: ltTTTNext
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 18 * mainWindow.ratioObject
                    width: 30 * mainWindow.ratioObject
                    height: ltTutorial.tutorialDataIdx < ltTutorial.tutorialData.length - 1 ? 0.80 * parent.height : 0.50 * parent.height
                    fontSizeMode: Text.VerticalFit
                    font.underline: !(ltTutorial.tutorialDataIdx < ltTutorial.tutorialData.length - 1)
                    text: ltTutorial.tutorialDataIdx < ltTutorial.tutorialData.length - 1 ? ">" : "got it!"
                    color: "#210430"
                    font.pixelSize: 21 * mainWindow.ratioFont
                    onClicked: {
                        ltTTT.clicked();
                    }
                }
            }
        }
    }

    function showTutorialData(diff, skip_restore) {
        BWU.showTutorialData(diff, skip_restore);
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function iconClick(data) {
        console.log("wnd_battle.iconClick", JSON.stringify(data));
        if (data.action === "hp") {
            mainWindow.logEvent("Play_Warlock_Click", {Click:data.warlock_name});
        } else if (data.action === "m") {
            mainWindow.logEvent("Play_Monster_Click", {Click:data.name});
        } else {
            mainWindow.logEvent("Play_Spell_Click", {Click:data.action});
        }

        if (operationMode >= 1) {
            if ((operationMode === 1) && (data.action === "permanency")) {
                permanency = !permanency ? 1 : 0;
            } else if ((operationMode === 1) && (data.action === "delay")) {
                delay = !delay ? 1 : 0;
            } else if ((data.action === "hp") || (data.action === "m")) {
                target = data;
                bbChooseTarget.active = true;
                //setTargetingOnOff(false, true);
                target.target_name = data.action === "hp" ? data.warlock_name : data.name;
                tTargetTitle.text = getTargetTitle(operationMode === 1, currentSpell.n);
                //tTargetTitle.visible = true;
                //mainWindow.setSpellTarget(target_name, permanency, delay, operationMode);
                //operationMode = 0;
                if ((data.action === "m") && BU.checkIsMonsterCharmed(data)) {
                    //open target window for charmed monster
                    data.under_control = true;
                    //iconDoubleClick(data);
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
            if (data.checked) {
                showShortHint("Banked "+data.value+" will be fired");
            } else {
                showShortHint("Do not fire banked spell: " + data.value);
            }
        } else if (data.action === "banked_info") {
            showShortHint(data.warlock_name + " banked a " + data.value);
        } else if (delay && (data.action === "delay")) {
            mainWindow.cleanDelay();
        } else if (permanency && (data.action === "permanency")) {
            mainWindow.cleanPermanency()
        } else if (data.active_action) {
            console.log("DO some action", JSON.stringify(data));
            BU.battle.currentCharm = BU.getCharmDataByAction(data);
            //mainWindow.showGesture(data.action, BU.battle.currentCharm.h, BU.battle.currentCharm.g, data);
            if (!data.skip_show_gesture) {
                var is_left = data.hand === "LH";
                mainWindow.showGesture(is_left, data.plg, data.action, data.warlock_idx, is_left ? data.lgL : data.lgR);
            }
        } else {
            iconDoubleClick(data);
        }
    }

    function iconDoubleClick(data) {
        console.log("wnd_battle.iconDoubleClick", JSON.stringify(data));
        var msg_text, msg_title, spell_code, msg_type = 1, target_name;

        switch(data.action) {
        case "hp":
            msg_type = 4;
            msg_title = "Warlock's hit point"
            msg_text = data.value + " damage to death";
            break;
        case "m":
            msg_type = 4;
            msg_title = "Monster"
            msg_text = data.name;
            if (data.owner !== "Nobody") {
                msg_text += " (owner by "+data.owner+")";
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
            msg_type = 7
            spell_code = BU.getIconStatusSpell(data.action);
            if (spell_code) {
                msg_title = dict.getStringByCode(spell_code);
                msg_text  = dict.getStringByCode(spell_code + "_desc");
            } else {
                spell_code = "PSDF";
                msg_title  = "Charm";
                msg_text   = data.action;
            }
            break;
        }
        mainWindow.showErrorWnd({type:msg_type,text:msg_text,title:msg_title,data:data,spell:spell_code});
    }

    function showShortHint(msg) {
        ltHint.text = msg;
        ltHint.visible = true;
        paHint.start();
    }

    function getTargetTitle(IsSpell, Title) {
        return SGU.getTargetTitle(IsSpell, Title, target.target_name, permanency, delay);
    }

    function setTargetingOnOff(Enable, IsSpell, Title) {
        if (!Enable) {
            battleChanged();
            ltHint.visible = false;
        } else {
            //bbSendOrders.active = false;
            if (Title) {
                var txt = "";//"Select target for " + Title;
                if (IsSpell) {
                    if (mainWindow.gBattle.player_has_bank) {
                        txt = "To bank a "+Title+", click the the bank icon, and then choose the spell's target";
                    } else if (mainWindow.gBattle.player_has_permanent) {
                        txt = "To do a "+Title+" permanent, click the the permanency icon, and then choose the spell's target";
                    } else if (Title !== "Counter Spell") {
                        //txt += " spell";
                    }
                }

                //
                if (txt !== "") {
                    showShortHint(txt);
                }
            }
        }

        for (var i = 0, Ln = iWarlocks.children.length; i < Ln; ++i) {
            iWarlocks.children[i].targetingOnOff(Enable, IsSpell);
        }
        //ltAll.text = Enable ? "Nobody" : "All";
        //ltAll.border_visible = Enable;
        iiNobody.visible = Enable;
        if (Enable) {
            tTargetTitle.text = getTargetTitle(IsSpell, Title);
        }
        rTT.visible = Enable;// && (target.target_name !== "");
        iiDefault.visible = Enable && mainWindow.gBattle.player_under_control;
        //iiElemental.border.width = Enable ? 3 : 0;
        bbSendOrders.visible = !Enable;
        bbChooseTarget.active = false;
        bbChooseTarget.visible = Enable;
        if (!mainWindow.gBattle.with_bot) {
            iiChat.visible = !Enable;
        }
        iiScroll.visible = !Enable;
        iiSpellbook.visible = !Enable;
        //iiChat.opacity = Enable ? 0.3 : 1;
    }

    function showHideSummonIcon() {
        for (var i = 0, Ln = iWarlocks.children.length; i < Ln; ++i) {
            iWarlocks.children[i].showHideSummonIcon(currentSpell);
        }
    }

    function battleChanged(FromGesture) {
        var old_state = bbSendOrders.active;
        var new_state = !(!mainWindow.gBattle.actions["L"].g || !mainWindow.gBattle.actions["R"].g);
        if (old_state != new_state) {
            bbSendOrders.active = new_state;
            if (new_state) {
                bbSendOrders.animate(1);
            }
        }
        if (FromGesture) {
            //
            showHideSummonIcon();
        }
    }

    function setGesture(gesture, is_maladroit) {
        iWarlocks.children[0].setGesture(mainWindow.gBattle.currentHand, 'g_' + BU.getIconByGesture(gesture), gesture);
        if (is_maladroit || (gesture === "C")) {
            iWarlocks.children[0].setGesture(mainWindow.gBattle.otherHand, 'g_' + BU.getIconByGesture(gesture), gesture);
        }
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

    function prepareToTargeting(is_spell, spell) {
        currentSpell = spell;
        target = {target_name:""};
        operationMode = is_spell ? 1 : 2;
        setTargetingOnOff(true, is_spell, currentSpell.n);
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
        if (!mainWindow.gBattle.with_bot) {
            iiChat.visible = true;
            iiSpellbook.anchors.rightMargin = (32 + 78 + 32) * mainWindow.ratioObject;
        } else {
            iiChat.visible = false;
            iiSpellbook.anchors.rightMargin = 32 * mainWindow.ratioObject;
        }
        if (mainWindow.gameCore.isAI) {
            sendOrders();
            mainWindow.processEscape();
        }
    }

    Component.onCompleted: {
        console.log("battle window", x, y);
        mainWindow.storeWnd(battleItem);
        initBattleFields();
        //iiElemental.visible = true;
    }
}
