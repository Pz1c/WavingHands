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

    signal cancel;

    property var dict: WarlockDictionary
    property var l_data: ({})
    property string code: "info_wnd"
    property string title_text: ""
    property string title_color: ""
    property alias content_item: bwBody
    property alias icon: iiIcon
    property alias title: tTitle.text
    property alias titleHeight: rTitle.height
    property alias titleVisible: rTitle.visible
    property alias gradient: rOver.gradient

	focus: true

    function onKeyPressed(event) {
        console.log("InfoWindow.KEY_PRESSED: " + event.key)
        if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
            console.log("InfoWindow.BACK_KEY_PRESSED: " + event.key)
            event.accepted = true;
            iRoot.cancel();
        }
    }

    Keys.onPressed: onKeyPressed

    Rectangle {
        id: rOver
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#210430" }
            GradientStop { position: 1.0; color: "#0654C0" }
        }

        Image {
            id: iBg
            source: "qrc:/res/stars_bg.png"
        }
        z: 0
    }

    MouseArea {
        id: maOver
        anchors.fill: parent
        z: 0
    }

    Item {
        id: rTitle
        anchors.top: parent.top
        anchors.topMargin: 24 * mainWindow.ratioObject
        anchors.left: parent.left
        anchors.right: parent.right
        height: 120 * mainWindow.ratioObject
        z: 10

        BtnWithIcon {
            id: bwiExit
            z: 15
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            width: 150 * mainWindow.ratioObject
            height: 50 * mainWindow.ratioObject

            transparent: true
            image: "qrc:/res/back_arrow.png"
            image_anchors.verticalCenter: bwiExit.verticalCenter
            image_anchors.left: bwiExit.left
            icon_width: 40 * mainWindow.ratioObject
            text_width: 50 * mainWindow.ratioObject
            text_color: "#FEE2D6"
            text: "Back"
            fontPixelSize: 28 * mainWindow.ratioFont

            onClicked: {
                iRoot.cancel();
            }
        }

        Text {
            id: tTitle
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: bwiExit.right
            anchors.right: iiIcon.left
            font.pixelSize: 41 * mainWindow.ratioFont
            color: "#FEE2D6"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: ""
            visible: false
        }

        IconInfo {
            id: iiIcon
            z: 15
            source: "qrc:/res/elemental_fire.png"
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 24 * mainWindow.ratioObject
            height: 50 * mainWindow.ratioObject
            width: 50 * mainWindow.ratioObject
            active: false
            animationEnabled: false
            borderOnPress: false
            visible: false
        }
    }

    Item {
        id: bwBody
        anchors.top: rTitle.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        z: 10
    }

    function fixHeight() {
        bwBody.height = iRoot.height
        /*if (with_controls) {
            bwBody.height = 0.60 * iRoot.height;
        } else {
            bwBody.height = 0.80 * iRoot.height;
        }*/
    }

    function showWnd() {
        initFields();
        visible = true;
        //ttWait.start();
    }

    function hideWnd() {
        visible = false;
        //ttWait.stop();
        //mainWindow.loadingStop();
    }

    function initFields() {
        console.log("InfoWindow.qml.initFields");
        fixHeight();
    }

    Component.onCompleted: {
        initFields();
        //mainWindow.storeWnd(dMainItem);
    }
}
