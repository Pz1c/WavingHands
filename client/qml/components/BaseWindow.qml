// source https://qt-project.org/forums/viewthread/26455

// This QML file is used to create a simple popup
// It will show an overlay on top of the parent and a small white area
// that is the dialog itself. For demo purposes no fancy stuff in the popup

import QtQuick 2.12
import QtQuick.Controls 2.12
import ua.sp.warlockdictionary 1.0

Item {
    id: iRoot
    anchors.fill: parent
    z: 0

    property double height_koeff: iRoot.height / 800
    property var dict: WarlockDictionary
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
    property alias body_height: bwBody.height
    property int body_width_prc: 95
    property int body_height_prc: 60
    property int title_height_prc: 10
    property int control_height_prc: 10


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

    Item {
        id: rTitle
        anchors.bottom: bwBody.top
        //anchors.topMargin: 0.05 * parent.height
        height: title_height_prc / 100 * parent.height
        width: bwBody.width
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.left: parent.left
        //anchors.right: parent.right
        //height: 0.05 * parent.height
        //color: "snow"
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
            horizontalAlignment: Text.AlignLeft
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: iCancel.left
            //width: parent.width - parent.height
            text: "Base Window"
            color: "darkgrey"
        }
    }

    Item {
        id: bwBody
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: body_height_prc / 100 * iRoot.height
        width: body_width_prc / 100 * iRoot.width
        //color: "snow"
        z: 10
    }

    Image {
        id: iTopBg
        anchors.top: rTitle.top
        anchors.bottom: bwControl.bottom
        anchors.left: bwControl.left
        anchors.right: bwControl.right
        source: "qrc:/res/background.png"
        z: 9
    }

    Item {
        id: bwControl
        anchors.top: bwBody.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: control_height_prc / 100 * parent.height
        width: bwBody.width
        //color: "snow"
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
            //bg_color_active: "lightgray"
            //border_color_active: "gray"
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
            //bg_color_active: "lightgray"
            //border_color_active: "gray"
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
        //Qt.gameField.loadingStop();
    }

    function fixHeight() {
        bwBody.height = body_height_prc / 100 * iRoot.height
        /*if (with_controls) {
            bwBody.height = 0.60 * iRoot.height;
        } else {
            bwBody.height = 0.80 * iRoot.height;
        }*/
    }

    onWith_controlsChanged: {
        console.log("onWith_controlsChanged", with_controls, bwBody.height, iRoot.height);
        fixHeight();
    }

    function initFields() {
        console.log("BaseWindow.qml.initFields", bwBody.height, iRoot.height);
        fixHeight();
    }

    Component.onCompleted: {
        initFields();
        //Qt.gameField.storeWnd(dMainItem);
    }
}
