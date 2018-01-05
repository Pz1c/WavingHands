// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.0
import QtQuick.Controls 1.1

import "qrc:/user_profile_utils.js" as UPUtils

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
        //anchors.fill: parent
        width: 2 * dMainItem.width / 3
        height: 2 * dMainItem.height / 3
        radius: 10
        //anchors.topMargin: 0
        anchors.centerIn: parent

        focus: true

        Keys.onPressed: {
            if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
                console.log("BACK_KEY_PRESSED: " + event.key)
                event.accepted = true;
                clickCancel()
            }
        }

        ScrollView {
            width: dialogWindow.width
            height: dialogWindow.height - btnDoit.height

            Text {
                  x: 5
                  y: 0
                  id: tPlayerInfo
                  width: dialogWindow.width - 30
                  //height: 200
                  //anchors.fill: parent
                  text: ""
                  anchors.topMargin: 8
                  font.pointSize: 13 * height_koeff
                  wrapMode: Text.WordWrap
                  textFormat: Text.RichText
                  onLinkActivated: UPUtils.linkActivated(link)
            }
        }

        Rectangle {
                id: btnDoit
                x: parent.width / 2 - btnDoit.width / 2
                y: parent.height - btnDoit.height
                width: parent.width / 2
                height: parent.height / 5 > 50 ? 50 : parent.height / 5
                color: maDoit.pressed ? "green" : "lightgreen"
                radius: 7
                border.width: 3
                border.color: "green"

                Text {
                    id: labelDoit
                    x: 38
                    y: 18
                    text: dict.getStringByCode("Close")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 16 * height_koeff
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
        Qt.mainWindow.closeChild();
    }

    function initFields() {
        console.log("initFields")
        tPlayerInfo.text = Qt.core.playerInfo;
        Qt.mainWindow.storeWnd(dMainItem)
    }

    Component.onCompleted: initFields()
}
