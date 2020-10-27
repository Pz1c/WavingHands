// source https://qt-project.org/forums/viewthread/26455
import QtQuick 2.12
import QtQuick.Controls 2.12

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: false

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        anchors.centerIn: content_item
        height: 0.8 * parent.height
        width: 0.95 * parent.width
        //focus: true

        color: "black"
        opacity: 0.8

        Text {
            id: dialogTitle
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: dict.getStringByCode("EnterNewUser")
            anchors.topMargin: 8
            //font.pointSize: 12 * height_koeff
        }

        Text {
            id: tLogin
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.verticalCenter: rLogin.verticalCenter
            height: dialogTitle.height

            text: dict.getStringByCode("Login")
            //font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rLogin
            height: dialogTitle.height
            width: parent.width - (tLogin.x + Math.max(tLogin.width, tPass.width, tEmail.width)) - 10
            anchors.right: parent.right
            anchors.rightMargin: 0.01 * parent.width
            anchors.top: dialogTitle.bottom
            anchors.topMargin: 2

            radius: 7
            border.width: 1
            border.color: "black"

            TextInput {
                id: tiLogin
                width: parent.width - 6
                height: parent.height - 2
                anchors.centerIn: parent
                text: ""
                //font.pointSize: 12 * height_koeff
                inputMethodHints: Qt.ImhNoPredictiveText + Qt.ImhNoAutoUppercase
                //echoMode: TextInput.Password
            }
        }

        Text {
            id: tPass
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.verticalCenter: rPass.verticalCenter
            height: dialogTitle.height

            text: dict.getStringByCode("Password")
            //font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rPass
            height: dialogTitle.height
            width: rLogin.width
            anchors.right: parent.right
            anchors.rightMargin: 0.01 * parent.width
            anchors.top: rLogin.bottom
            anchors.topMargin: 2
            radius: 7
            border.width: 1
            border.color: "black"

            TextInput {
                id: tiPass
                width: parent.width - 6
                height: parent.height - 2
                anchors.centerIn: parent
                text: ""
                //font.pointSize: 12 * height_koeff
                inputMethodHints: Qt.ImhNoPredictiveText + Qt.ImhNoAutoUppercase
                echoMode: TextInput.Password
            }
        }

        Text {
            id: tEmail
            height: dialogTitle.height
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            anchors.verticalCenter: rEmail.verticalCenter

            text: dict.getStringByCode("Email")
            //font.pointSize: 12 * height_koeff
        }

        Rectangle {
            id: rEmail
            height: dialogTitle.height
            width: rLogin.width
            anchors.right: parent.right
            anchors.rightMargin: 0.01 * parent.width
            anchors.top: rPass.bottom
            anchors.topMargin: 2
            radius: 7
            border.width: 1
            border.color: "black"

            TextInput {
                id: tiEmail
                width: parent.width - 6
                height: parent.height - 2
                anchors.centerIn: parent
                text: ""
                //font.pointSize: 12 * height_koeff
                inputMethodHints: Qt.ImhEmailCharactersOnly + Qt.ImhNoPredictiveText + Qt.ImhNoAutoUppercase
                //echoMode: TextInput.Password
            }
        }

        CheckBox {
            checked: true
            enabled: false
            id: tConfirmation
            anchors.top: rEmail.bottom
            anchors.topMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 0.01 * parent.width
            text: dict.getStringByCode("Iam13")
        }

        Rectangle {
                id: btnDoit
                height: rEmail.height * 2
                width: 0.5 * parent.width
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                //anchors.rightMargin: 0.01 * parent.width
                color: maDoit.pressed ? "green" : "lightgreen"
                radius: 7
                border.width: 3
                border.color: "green"

                Text {
                    id: labelDoit
                    anchors.centerIn: parent
                    text: dict.getStringByCode("Register")
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
                height: btnDoit.height
                width: 0.5 * parent.width
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                //anchors.leftMargin: 0.01 * parent.width
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
        Qt.mainWindow.hideNewUserMenu()
    }

    function initFields() {
        Qt.mainWindow.storeNewWnd(dMainItem)
    }

    Component.onCompleted: initFields()
}
