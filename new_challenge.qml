// source https://qt-project.org/forums/viewthread/26455

// This QML file is used to create a simple popup
// It will show an overlay on top of the parent and a small white area
// that is the dialog itself. For demo purposes no fancy stuff in the popup

import QtQuick 2.0
import QtQuick.Controls 2.0
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
        width: 0.8 * dMainItem.width
        height: rDescription.y + rDescription.height + cbFriendly.height * 2 + 10
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

        Text {
            id: dialogTitle
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            text: dict.getStringByCode("EnterChallenge")
            //font.pointSize: 16 * height_koeff
        }

        CheckBox {
            id: cbParalyzeFC
            checked: true
            text: qsTr("ParalyzeFC")
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.top: dialogTitle.bottom
            anchors.topMargin: 0.01 * parent.width

        }

        CheckBox {
            id: cbMaladroit
            checked: true
            text: qsTr("Maladroit")
            anchors.left: cbParalyzeFC.right
            anchors.leftMargin: 0.01 * parent.width
            anchors.top: dialogTitle.bottom
            anchors.topMargin: 0.01 * parent.width
        }

        Text {
            id: tCountLabel
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.verticalCenter: cbCount.verticalCenter
            text: dict.getStringByCode("WarlocksCount")
            //font.pointSize: 13 * height_koeff
        }

        ComboBox {
            id: cbCount
            anchors.right: parent.right
            anchors.rightMargin: 0.01 * parent.width
            anchors.top: cbMaladroit.bottom
            anchors.topMargin: 0.01 * parent.width
            width: parent.width - (tCountLabel.x + Math.max(tCountLabel.width, tFriendly.width)) - 10
            model: [2, 3, 4, 5, 6]
            currentIndex: 0
        }

        Text {
            id: tFriendly
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.verticalCenter: cbFriendly.verticalCenter
            text: dict.getStringByCode("FriendlyLevel")
            //font.pointSize: 13 * height_koeff
        }

        ComboBox {
            id: cbFriendly
            anchors.right: parent.right
            anchors.rightMargin: 0.01 * parent.width
            anchors.top: cbCount.bottom
            anchors.topMargin: 0.01 * parent.width
            width: cbCount.width
            model: [dict.getStringByCode("Ladder"),
                    dict.getStringByCode("Friendly"),
                    dict.getStringByCode("VFriendly")]
            currentIndex: 1
        }

        Text {
            id: tDescription
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.top: cbFriendly.bottom
            anchors.topMargin: 0.01 * parent.width
            text: dict.getStringByCode("Description")
            //font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rDescription
            height: cbFriendly.height * 4 - 3
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.right: parent.right
            anchors.rightMargin: 0.01 * parent.width
            anchors.top: tDescription.bottom
            anchors.topMargin: 0.01 * parent.width
            radius: 7
            border.width: 1
            border.color: "black"

            TextEdit {
                id: tiDescription
                anchors.centerIn: parent
                text: ""
                //font.pointSize: 12 * height_koeff
                inputMethodHints: Qt.ImhNoPredictiveText
                //echoMode: TextInput.Password
            }
        }

        Rectangle {
                id: btnDoit
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 2 * cbFriendly.height
                width: 0.5 * parent.width
                color: maDoit.pressed ? "green" : "lightgreen"
                radius: 7
                border.width: 3
                border.color: "green"

                Text {
                    id: labelDoit
                    text: dict.getStringByCode("OK")
                    anchors.centerIn: parent
                    //font.pointSize: 16 * height_koeff
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
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: btnDoit.width
                height: btnDoit.height
                color: maCancel.pressed ? "grey" : "lightgrey"
                radius: 7
                border.width: 3
                border.color: "grey"

                Text {
                    id: labelSkip
                    text: dict.getStringByCode("Cancel")
                    anchors.centerIn: parent
                    //font.pointSize: 16 * height_koeff
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
