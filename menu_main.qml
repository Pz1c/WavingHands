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
                id: btnUser
                x: parent.width / 2 - btnUser.width / 2
                y: 10
                width: 95 * parent.width / 100
                height: parent.height / 5
                color: maUser.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelUser
                    text: dict.getStringByCode("LoginSett")
                    anchors.centerIn: parent
                    //font.pointSize: 16 * height_koeff
                    font.pixelSize: (labelUser.paintedWidth > 0.97 * parent.width) ? (0.97 * parent.width / labelUser.paintedWidth) * 16 * height_koeff : 16 * height_koeff
                }

                MouseArea {
                    id: maUser
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.getLoginFromUser(dMainItem);
                    }
                }
        }

        Rectangle {
                id: btnLanguage
                x: parent.width / 2 - btnLanguage.width / 2
                y: btnUser.y + btnUser.height + parent.height / 5 - 10
                width: 95 * parent.width / 100
                height: parent.height / 5
                color: maLanguage.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelLanguage
                    text: dict.getStringByCode("LangSett")
                    anchors.centerIn: parent
                    //font.pointSize: 16 * height_koeff
                    font.pixelSize: (labelLanguage.paintedWidth > 0.97 * parent.width) ? (0.97 * parent.width / labelLanguage.paintedWidth) * 16 * height_koeff : 16 * height_koeff
                }

                MouseArea {
                    id: maLanguage
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.showLangSettings(dMainItem)
                    }
                }
        }


        Rectangle {
                id: btnNetwork
                x: parent.width / 2 - btnNetwork.width / 2
                y: btnLanguage.y + btnLanguage.height + parent.height / 5 - 10
                width: 95 * parent.width / 100
                height: parent.height / 5
                color: maNetwork.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelNetwork
                    text: dict.getStringByCode("NetworkSett")
                    anchors.centerIn: parent
                    //font.pointSize: 16 * height_koeff
                    font.pixelSize: (labelNetwork.paintedWidth > 0.97 * parent.width) ? (0.97 * parent.width / labelNetwork.paintedWidth) * 16 * height_koeff : 16 * height_koeff
                }

                MouseArea {
                    id: maNetwork
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    anchors.fill: parent
                    onClicked: {
                        Qt.mainWindow.showProxySettings(dMainItem)
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
