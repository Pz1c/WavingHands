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
        anchors.fill: content_item
        //color: "transparent"

        ScrollView {
            id: svWarlocks
            anchors.fill: parent


        }



        z: 11
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function showWnd() {
        initFields();
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
        title_text = "Battle #"+mainWindow.gBattle.id;
    }

    Component.onCompleted: {
        initBattleFields();
    }
}
