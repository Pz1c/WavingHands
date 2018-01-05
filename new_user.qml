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
        width: 2 * dMainItem.width / 3
        height: (dialogTitle.height + 5) * 6
        radius: 10
        anchors.topMargin: 0
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
            x: 19
            y: 3
            anchors.top: parent.top
            text: dict.getStringByCode("EnterNewUser")
            anchors.topMargin: 8
            font.pointSize: 12 * height_koeff
        }

        Text {
            id: tLogin
            x: 10
            y: dialogTitle.y + 3 + dialogTitle.height
            text: dict.getStringByCode("Login")
            anchors.topMargin: 8
            font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rLogin
            width: dialogWindow.width - 20 - tLogin.width
            height: dialogTitle.height
            x: tLogin.x + tLogin.width + 3
            y: tLogin.y
            radius: 7
            border.width: 1
            border.color: "black"

            TextInput {
                id: tiLogin
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

        Text {
            id: tPass
            x: 10
            y: tLogin.y + 3 + tLogin.height
            text: dict.getStringByCode("Password")
            anchors.topMargin: 8
            font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rPass
            width: dialogWindow.width - 20 - tPass.width
            height: dialogTitle.height
            x: tPass.x + tPass.width + 3
            y: tPass.y
            radius: 7
            border.width: 1
            border.color: "black"

            TextInput {
                id: tiPass
                width: parent.width - 6
                height: parent.height - 2
                x: 3
                y: 1
                anchors.centerIn: parent
                text: ""
                font.pointSize: 12 * height_koeff
                inputMethodHints: Qt.ImhNoPredictiveText
                echoMode: TextInput.Password
            }
        }

        Text {
            id: tEmail
            x: 10
            y: tPass.y + 3 + tPass.height
            text: dict.getStringByCode("Email")
            anchors.topMargin: 8
            font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rEmail
            width: dialogWindow.width - 20 - tEmail.width
            height: dialogTitle.height
            x: tEmail.x + tEmail.width + 3
            y: tEmail.y
            radius: 7
            border.width: 1
            border.color: "black"

            TextInput {
                id: tiEmail
                width: parent.width - 6
                height: parent.height - 2
                x: 3
                y: 1
                anchors.centerIn: parent
                text: ""
                font.pointSize: 12 * height_koeff
                inputMethodHints: Qt.ImhEmailCharactersOnly
                //echoMode: TextInput.Password
            }
        }

        CheckBox {
            checked: true
            enabled: false
            id: tConfirmation
            x: 10
            y: tEmail.y + 3 + tEmail.height
            text: dict.getStringByCode("Iam13")
            anchors.topMargin: 8
            //font.pointSize: 12 * height_koeff
        }

        Rectangle {
                id: btnDoit
                x: 0
                y: parent.height - btnDoit.height
                width: parent.width / 2
                height: parent.height / 5
                color: maDoit.pressed ? "green" : "lightgreen"
                radius: 7
                border.width: 3
                border.color: "green"

                Text {
                    id: labelDoit
                    x: 38
                    y: 18
                    text: dict.getStringByCode("Register")
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
                        if (tiLogin.text == '' || tiLogin.text == "Please, type login") {
                            tiLogin.text = "Please, type login"
                            return
                        }

                        if (tiPass.text == '') {
                            return
                        }

                        if (tiEmail.text == '' || tiEmail.text == "please@type.email") {
                            tiEmail.text = "please@type.email"
                            return
                        }

                        Qt.mainWindow.storeWnd(dMainItem)
                        Qt.core.regNewUser(tiLogin.text, tiPass.text, tiEmail.text)
                    }
                }
        }


        Rectangle {
                id: btnSkip
                x: parent.width / 2
                y: parent.height - btnDoit.height
                width: parent.width / 2
                height: parent.height / 5
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
        Qt.mainWindow.hideNewUserMenu()
    }

    function initFields() {
        Qt.mainWindow.storeNewWnd(dMainItem)
    }

    Component.onCompleted: initFields()
}
