// source https://qt-project.org/forums/viewthread/26455

// This QML file is used to create a simple popup
// It will show an overlay on top of the parent and a small white area
// that is the dialog itself. For demo purposes no fancy stuff in the popup

import QtQuick 2.0
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
        width: 0.9 * parent.width
        height: 0.9 * dMainItem.height
        radius: 10
        anchors.centerIn: parent

        focus: true

        Keys.onPressed: {
            if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
                console.log("BACK_KEY_PRESSED: " + event.key)
                event.accepted = true;
                clickCancel()
            }
        }

        Rectangle {
                id: btnUser
                anchors.top: parent.top
                anchors.topMargin: 0.1 * parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                width: 0.8 * parent.width
                height: 0.25 * parent.height
                color: maUser.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelUser
                    text: dict.getStringByCode("LoginIn")
                    anchors.centerIn: parent
                    wrapMode: Text.WordWrap
                    font.pixelSize: 26 * height_koeff
                }

                MouseArea {
                    id: maUser
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.getLoginFromUser(dMainItem, true);
                    }
                }
        }


        Rectangle {
                id: btnNetwork
                anchors.centerIn: parent
                width: btnUser.width
                height: btnUser.height
                color: maNetwork.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelNetwork
                    text: dict.getStringByCode("Register")
                    anchors.centerIn: parent
                    wrapMode: Text.WordWrap
                    font.pixelSize: 26 * height_koeff
                }

                MouseArea {
                    id: maNetwork
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.showRegisterUser(dMainItem)
                    }
                }
        }


        Rectangle {
                id: btnSkip
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0.1 * parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                width: btnUser.width
                height: btnUser.height
                color: maCancel.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelSkip
                    text: dict.getStringByCode("Cancel")
                    anchors.centerIn: parent
                    wrapMode: Text.WordWrap
                    font.pixelSize: 26 * height_koeff
                }

                MouseArea {
                    id: maCancel
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
        console.log("cancel");
        Qt.mainWindow.closeChild()
    }

    function initFields() {
        Qt.mainWindow.storeWnd(dMainItem)
    }

    Component.onCompleted: initFields()
}
