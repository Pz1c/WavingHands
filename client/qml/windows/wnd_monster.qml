// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: true
    apply_text: dict.getStringByCode("MonsterSetTarget")
    body_height_prc: 40

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item

        LargeText {
            id: ltStatus
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0.5 * parent.height
            color: "snow"
        }

        LargeText {
            id: ltOwnerTitle
            anchors.top: ltStatus.bottom
            anchors.left: parent.left
            height: 0.25 * parent.height
            width: 0.5 * parent.width
            color: "snow"
            text: dict.getStringByCode("MonsterOwnerTitle")
        }

        LargeText {
            id: ltOwnerValue
            anchors.top: ltStatus.bottom
            anchors.right: parent.right
            height: 0.25 * parent.height
            width: 0.5 * parent.width
            color: "snow"
            text: ""
        }

        LargeText {
            id: ltTargetTitle
            anchors.top: ltOwnerTitle.bottom
            anchors.left: parent.left
            height: 0.25 * parent.height
            width: 0.5 * parent.width
            color: "snow"
            text: dict.getStringByCode("MonsterTargetTitle")
        }

        LargeText {
            id: ltTargetValue
            anchors.top: ltOwnerValue.bottom
            anchors.right: parent.right
            height: 0.25 * parent.height
            width: 0.5 * parent.width
            color: "snow"
            text: ""
        }

        z: 11
    }

    onCancel: {
        mainWindow.processEscape();
    }

    onApply: {
        mainWindow.chooseMonsterTarget();
    }

    function showWnd() {
        initFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initMFields() {
        console.log("wnd_monster.initMFields", JSON.stringify(mainWindow.gERROR));

        var m = mainWindow.gERROR.data;
        title_text = m.name;
        ltStatus.text = m.status;
        ltOwnerValue.text = m.owner;
        ltTargetValue.text = m.target;

        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initMFields();
    }
}
