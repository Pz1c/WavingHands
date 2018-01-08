// source https://qt-project.org/forums/viewthread/26455

// This QML file is used to create a simple popup
// It will show an overlay on top of the parent and a small white area
// that is the dialog itself. For demo purposes no fancy stuff in the popup

import QtQuick 2.0
import QtQuick.Controls 1.2
// Use an item as container to group both the overlay and the dialog
// I do this because the overlay itself has an opacity set, and otherwise
// also the dialog would have been semi-transparent.
// I use an Item instead of an Rectangle. Always use an 'Item' if it does not
// display stuff itself, this is better performance wise.
Item {
    id: dMainItem
    anchors.fill: parent
    property double height_koeff: dMainItem.height / 800
    property var dict: Qt.mainWindow.warlockDictionary

    // Add a simple animation to fade in the popup
    // let the opacity go from 0 to 1 in 400ms
    PropertyAnimation {
        target: dMainItem
        property: "opacity"
        duration: 400
        from: 0
        to: 1
        easing.type: Easing.InOutQuad
        running: true
    }

    // This rectange is the a overlay to partially show the parent through it
    // and clicking outside of the 'dialog' popup will do 'nothing'
    Rectangle {
        anchors.fill: parent
        id: overlay
        color: "#000000"
        opacity: 0.6
        // add a mouse area so that clicks outside
        // the dialog window will not do anything
        MouseArea {
            anchors.fill: parent
        }
    }

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        width: 0.98 * parent.width
        height: 0.8 * parent.height
        radius: 10
        anchors.verticalCenterOffset: -30
        anchors.horizontalCenterOffset: 0
        anchors.centerIn: parent
        border.width: 3
        border.color: "grey"

        focus: true

        Keys.onPressed: {
            if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
                console.log("BACK_KEY_PRESSED: " + event.key)
                event.accepted = true;
                clickCancel()
            }
        }

        Text {
            x: 10
            id: tErrMsg
            anchors.top: parent.top
            text: ""
            anchors.topMargin: 8
            width: svText.width - 20
            //font.pointSize: 16 * height_koeff
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
        }

        ScrollView {
          id: svText
          x: 0
          y: 0
          width: parent.width
          height: parent.height - btnDoit.height
          contentItem: tErrMsg
        }

        Rectangle {
            id: btnDoit
            y: parent.height - btnDoit.height
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width / 2
            height: parent.height / 11
            color: "lightgreen"
            radius: 7
            anchors.horizontalCenterOffset: 0
            border.width: 3
            border.color: "green"

            Text {
                id: labelDoit
                x: 38
                y: 18
                text: dict.getStringByCode("OK")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                //font.pointSize: 16 * height_koeff
            }

            MouseArea {
                id: maDoit
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.fill: parent
                onClicked: clickCancel()
            }
        }
    }

    function clickCancel() {
        dMainItem.destroy()
    }

    function initFields() {
        tErrMsg.text = Qt.mainWindow.getSpellDescription();
    }

    Component.onCompleted: initFields()
}
