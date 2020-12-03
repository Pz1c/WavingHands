// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"
import "qrc:/js/battle_utils.js" as BU

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

            IconInfo {
                id: iiElemental
                source: "qrc:/res/elemental_fire.png";
                text: "3"
                height: 0.9 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.05 * parent.height
                anchors.left: parent.left
            }

            IconInfo {
                id: iiChat
                source: "qrc:/res/chat.png";
                text: ""
                height: 0.9 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.05 * parent.height
                anchors.right: parent.right

                onClicked: {
                    mainWindow.showBattleChat();
                }
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function iconClick(data) {
        console.log("wnd_battle.iconClick", JSON.stringify(data));
    }

    function iconDoubleClick(data) {
        console.log("wnd_battle.iconDoubleClick", JSON.stringify(data));
    }

    function setTargetingOnOff(Enable) {
        for (var i = 0, Ln = iWarlocks.children.length; i < Ln; ++i) {
            iWarlocks.children[i].targetingOnOff(Enable);
        }
        iiElemental.border.width = Enable ? 3 : 0;
        iiChat.active = !Enable;
        iiChat.opacity = Enable ? 0.3 : 1;
    }

    function prepareToTargeting(gesture) {
        iWarlocks.children[0].setGesture(mainWindow.gBattle.currentHand, 'g_' + BU.getIconByGesture(gesture));
        setTargetingOnOff(true);
    }

    function showWnd() {
        initBattleFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initBattleFields() {
        console.log("wnd_battle.initBattleFields", JSON.stringify(mainWindow.gBattle));
        if (!mainWindow.gBattle || !mainWindow.gBattle.id) {
            return;
        }
        title_text = "Battle #" + mainWindow.gBattle.id;
        BU.applyBattle();
    }

    Component.onCompleted: {
        mainWindow.storeWnd(battleItem);
        initBattleFields();
    }
}
