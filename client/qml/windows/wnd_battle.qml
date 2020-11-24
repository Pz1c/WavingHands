// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"
import "qrc:/js/battle_utils.js" as BU

BaseWindow {
    id: dMainItem

    with_controls: true
    //with_apply: false
    body_width_prc: 100
    body_height_prc: 90
    title_height_prc: 5
    control_height_prc: 5

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        z: 11
        anchors.fill: content_item
        //color: "transparent"

        ScrollView {
            id: bfWarlocks
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: bfAll.top


        }

        Item {
            id: bfAll
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0.2 * parent.height

            LargeText {
               id: ltAll
               text: "All"
               anchors.top: parent.top
               anchors.verticalCenter: parent.verticalCenter
               height: 0.2 * parent.height
               width: 0.3 * parent.width
               bg_color: "#210430"
               color: "#A8F4F4"
            }

            IconInfo {
                id: iiElemental
                source: "qrc:/res/elemental_fire.png";
                text: "3"
                height: 0.15 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.01 * parent.height
                anchors.left: parent.left
            }

            IconInfo {
                id: iiChat
                source: "qrc:/res/banked_spell.png";
                text: ""
                height: 0.15 * parent.height
                width: height
                anchors.top: parent.top
                anchors.topMargin: 0.01 * parent.height
                anchors.right: parent.right
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
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
        BU.prepareBattle(mainWindow.gBattle);
    }

    Component.onCompleted: {
        initBattleFields();
    }
}
