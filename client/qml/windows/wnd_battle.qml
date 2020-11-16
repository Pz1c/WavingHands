// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    //with_apply: false
    body_height_prc: 60

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        //color: "transparent"

        ScrollView {
            id: svError
            anchors.fill: parent


        }

        z: 11
    }

    onCancel: {
        Qt.gameField.processEscape();
    }

    function showWnd() {
        initFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initBattleFields() {
        console.log("wnd_battle.initBattleFields", JSON.stringify(Qt.battle));
        if (!Qt.battle || !Qt.battle.id) {
            return;
        }

    }

    Component.onCompleted: {
        initBattleFields();
    }
}
