// source https://qt-project.org/forums/viewthread/26455

// This QML file is used to create a simple popup
// It will show an overlay on top of the parent and a small white area
// that is the dialog itself. For demo purposes no fancy stuff in the popup

import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: iRoot
    anchors.fill: parent
    z: 0

    property double height_koeff: iRoot.height / 800
    property var dict: Qt.dictionary
    property alias title_text: ltTitle.text
    property alias title_color: ltTitle.color
    property alias content_item: bwBody
    //property alias bg_visible: iTopBg.visible
    property alias with_controls: bwControl.visible
    property alias with_cancell: bbCancel.visible
    property alias with_apply: bbApply.visible
    property alias with_action1: bbAction1.visible
    property alias with_action2: bbAction2.visible
    property alias action1_text: bbAction1.text
    property alias action1_text_color: bbAction1.text_color
    property alias action2_text: bbAction2.text
    property alias action2_text_color: bbAction2.text_color
    property alias title_width: rTitle.width
    property alias body_width: bwBody.width

    signal cancel
    signal apply
    signal action1
    signal action2

    Rectangle {
        id: rOver
        anchors.fill: parent
        color: "black"
        opacity: 0.5
        z: 0
    }

    MouseArea {
        id: maOver
        anchors.fill: parent
        z: 0
    }

    /*Image {
        id: iTopBg
        anchors.fill: parent
        source: "qrc:/res/level/empty_paper.png"
        z: 0
    }*/

    Rectangle {
        id: rTitle
        anchors.top: parent.top
        anchors.topMargin: 0.05 * parent.height
        height: 0.10 * parent.height
        width: 0.60 * parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.left: parent.left
        //anchors.right: parent.right
        //height: 0.05 * parent.height
        color: "black"
        z: 10

        Rectangle {
            id: iCancel
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: parent.height
            color: "red"

            LargeText {
                id: ltClose
                anchors.fill: parent
                color: "white"
                text: "x"
            }

            MouseArea {
                id: maWndCancel
                anchors.fill: parent
                onClicked: {
                    iRoot.cancel();
                }
            }
        }

        LargeText {
            id: ltTitle
            anchors.fill: parent
            horizontalAlignment: Text.AlignLeft
            //anchors.top: parent.top
            //anchors.bottom: parent.bottom
            //anchors.right: parent.right
            //width: parent.width - parent.height
            text: "Base Window"
            color: "white"
        }
    }

    Rectangle {
        id: bwBody
        anchors.top: rTitle.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 0.80 * parent.height
        width: 0.60 * parent.width
        color: "black"
        z: 10
    }

    Rectangle {
        id: bwControl
        anchors.top: bwBody.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 0.10 * parent.height
        width: 0.60 * parent.width
        color: "black"
        z: 10
        visible: false

        BtnBig {
            id: bbCancel
            active: visible
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: 0.25 * parent.width
            text: dict.getStringByCode("Cancel")
            bg_color_active: "lightgray"
            border_color_active: "gray"
            onClicked: {
                iRoot.cancel();
            }
        }

        BtnBig {
            id: bbAction1
            active: visible
            visible: false
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: bbCancel.right
            width: 0.25 * parent.width
            text: "Action1"
            bg_color_active: "lightgray"
            border_color_active: "gray"
            onClicked: {
                iRoot.action1();
            }
        }

        BtnBig {
            id: bbAction2
            active: visible
            visible: false
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: bbAction1.right
            width: 0.25 * parent.width
            text: "Action2"
            bg_color_active: "lightgray"
            border_color_active: "gray"
            onClicked: {
                iRoot.action2();
            }
        }

        BtnBig {
            id: bbApply
            active: visible
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 0.25 * parent.width
            text: dict.getStringByCode("Apply")
            onClicked: {
                iRoot.apply();
            }
        }
    }

    function showWnd() {
        initFields();
        visible = true;
        //ttWait.start();
    }

    function hideWnd() {
        visible = false;
        //ttWait.stop();
        Qt.gameField.loadingStop();
    }

    onWith_controlsChanged: {
        console.log(with_controls, bwBody.height, iRoot.height);
        if (with_controls) {
            bwBody.height = 0.70 * iRoot.height;
        } else {
            bwBody.height = 0.80 * iRoot.height;
        }
    }

    function initFields() {
        console.log("BaseWindow.qml.initFields");
        if (with_controls) {
            //bwBody.height = 0.60 * iRoot.height;
        } else {
            //bwBody.height = 0.80 * iRoot.height;
        }
    }

    Component.onCompleted: {
        initFields();
        //Qt.gameField.storeWnd(dMainItem);
    }
}
