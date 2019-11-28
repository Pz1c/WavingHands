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
        //y: 200
        width: 2 * dMainItem.width / 3
        height: dMainItem.height - 200
        radius: 10
        anchors.topMargin: 0
        //anchors.verticalCenterOffset: 0
        //anchors.horizontalCenterOffset: -21
        //anchors.top: parent.top
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
                id: btnUA
                x: parent.width / 2 - btnUA.width / 2
                y: 10
                width: 2 * parent.width / 3
                height: parent.height / 5
                color: maUA.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelUA
                    x: 38
                    y: 18
                    text: dict.getStringByCode("LangUA")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    //font.pointSize: 16 * height_koeff
                }

                MouseArea {
                    id: maUA
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.changeLanguage("ua", dMainItem)
                    }
                }
        }

        Rectangle {
                id: btnEN
                x: parent.width / 2 - btnEN.width / 2
                y: btnUA.y + btnUA.height + parent.height / 5 - 10
                width: 2 * parent.width / 3
                height: parent.height / 5
                color: maEN.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelEN
                    x: 38
                    y: 18
                    text: dict.getStringByCode("LangEN")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    //font.pointSize: 16 * height_koeff
                }

                MouseArea {
                    id: maEN
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.changeLanguage("en", dMainItem)
                    }
                }
        }


        Rectangle {
                id: btnRU
                x: parent.width / 2 - btnRU.width / 2
                y: btnEN.y + btnEN.height + parent.height / 5 - 10
                width: 2 * parent.width / 3
                height: parent.height / 5
                color: maRU.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelRU
                    x: 38
                    y: 18
                    text: dict.getStringByCode("LangRU")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    //font.pointSize: 16 * height_koeff
                }

                MouseArea {
                    id: maRU
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.changeLanguage("ru", dMainItem)
                    }
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
