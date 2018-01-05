// source https://qt-project.org/forums/viewthread/26455

// This QML file is used to create a simple popup
// It will show an overlay on top of the parent and a small white area
// that is the dialog itself. For demo purposes no fancy stuff in the popup

import QtQuick 2.0
import QtQuick.Controls 1.1
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
        width: 3 * dMainItem.width / 4
        height: 240
        radius: 10
        anchors.topMargin: 0
        y: 200
        anchors.horizontalCenter: parent.horizontalCenter

        focus: true

        Keys.onPressed: {
            if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
                console.log("BACK_KEY_PRESSED: " + event.key)
                event.accepted = true;
                clickCancel()
            }
        }

        Text {
            id: dialogTitle
            x: 19
            y: 3
            anchors.top: parent.top
            text: dict.getStringByCode("EnterChallenge")
            anchors.topMargin: 8
            font.pointSize: 12 * height_koeff
        }

        Text {
            id: tParalyzeFCLabel
            x: 3
            y: dialogTitle.y + dialogTitle.height + 7
            text: qsTr("ParalyzeFC")
            font.pointSize: 13 * height_koeff

            MouseArea {
                anchors.fill: parent
                onClicked: cbParalyzeFC.checked = !cbParalyzeFC.checked
            }
        }

        CheckBox {
            id: cbParalyzeFC
            checked: true
            x: tParalyzeFCLabel.x + tParalyzeFCLabel.width + 3
            y: tParalyzeFCLabel.y + tParalyzeFCLabel.height / 2 - cbParalyzeFC.height / 2
        }

        Text {
            id: tMaladroitLabel
            x: cbParalyzeFC.x + cbParalyzeFC.width + 10
            y: tParalyzeFCLabel.y
            text: qsTr("Maladroit")
            font.pointSize: 13 * height_koeff

            MouseArea {
                anchors.fill: parent
                onClicked: cbMaladroit.checked = !cbMaladroit.checked
            }
        }

        CheckBox {
            id: cbMaladroit
            checked: true
            x: tMaladroitLabel.x + tMaladroitLabel.width + 3
            y: tMaladroitLabel.y + tMaladroitLabel.height / 2 - cbMaladroit.height / 2
        }

        Text {
            id: tCountLabel
            x: 3
            y: tMaladroitLabel.y + tMaladroitLabel.height + 7
            text: dict.getStringByCode("WarlocksCount")
            font.pointSize: 13 * height_koeff
        }

        ComboBox {
            id: cbCount
            x: tCountLabel.x + tCountLabel.width + 3
            y: tCountLabel.y + tCountLabel.height / 2 - cbCount.height / 2
            model: [2, 3, 4, 5, 6]
        }

        Text {
            id: tFriendly
            x: 3
            y: tCountLabel.y + tCountLabel.height + 7
            text: dict.getStringByCode("FriendlyLevel")
            font.pointSize: 13 * height_koeff
        }

        ComboBox {
            id: cbFriendly
            x: tFriendly.x + tFriendly.width + 3
            y: tFriendly.y + tFriendly.height / 2 - cbFriendly.height / 2
            width: 160
            model: [dict.getStringByCode("Ladder"),
                    dict.getStringByCode("Friendly"),
                    dict.getStringByCode("VFriendly")]
            currentIndex: 1
        }

        Text {
            id: tDescription
            x: 10
            y: tFriendly.y + tFriendly.height + 3
            text: dict.getStringByCode("Description")
            anchors.topMargin: 8
            font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rDescription
            width: dialogWindow.width - 20
            height: dialogTitle.height * 4 - 3
            x: 10
            y: tDescription.y + tDescription.height
            radius: 7
            border.width: 1
            border.color: "black"

            TextEdit {
                id: tiDescription
                width: parent.width - 6
                height: parent.height - 2
                x: 3
                y: 1
                anchors.centerIn: parent
                text: ""
                font.pointSize: 12 * height_koeff
                inputMethodHints: Qt.ImhNoPredictiveText
                //echoMode: TextInput.Password
            }
        }

        Rectangle {
                id: btnDoit
                x: 0
                y: parent.height - btnDoit.height
                width: parent.width / 2
                height: 33
                color: maDoit.pressed ? "green" : "lightgreen"
                radius: 7
                border.width: 3
                border.color: "green"

                Text {
                    id: labelDoit
                    x: 38
                    y: 18
                    text: dict.getStringByCode("OK")
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
                    onClicked: {
                        Qt.core.createNewChallenge(1 == 1, 1 == 0, cbParalyzeFC.checked, cbMaladroit.checked,
                                                   cbCount.currentText, cbFriendly.currentIndex, tiDescription.text)
                        Qt.mainWindow.closeChild()
                    }
                }
        }


        Rectangle {
                id: btnSkip
                x: parent.width / 2
                y: parent.height - btnDoit.height
                width: btnDoit.width
                height: btnDoit.height
                color: maCancel.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelSkip
                    x: 38
                    y: 18
                    text: dict.getStringByCode("Cancel")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 16 * height_koeff
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
