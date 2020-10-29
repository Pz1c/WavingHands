// source https://qt-project.org/forums/viewthread/26455
import QtQuick 2.15
import QtQuick.Controls 2.15 as C2

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: true
    title_text: dict.getStringByCode("EnterNewUser")

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        anchors.fill: content_item

        LargeText {
            id: tLogin
            anchors.top: parent.top
            anchors.topMargin: 0.02 * parent.height
            anchors.verticalCenter: parent.verticalCenter
            height: 0.2 * dialogWindow.height
            width: 0.6 * dialogWindow.width
            text: dict.getStringByCode("Login")
            color: "white"
        }

        C2.TextField {
            id: tiLogin
            height: tLogin.height
            width: 0.6 * dialogWindow.width
            anchors.top: tLogin.bottom
            anchors.topMargin: 0.02 * parent.height
            anchors.verticalCenter: parent.verticalCenter
            text: "Login example"
            inputMethodHints: Qt.ImhNoPredictiveText
            //validator: RegExpValidator { regExp: /^[a-zA-Z0-9_-]{2,10}$/ }
            validator: RegularExpressionValidator { regularExpression: /^[a-zA-Z0-9_-]{2,10}$/ }
            color: "white"
            background: Rectangle {
                radius: 7
                implicitWidth: 100
                implicitHeight: 24
                border.width: 1
                border.color: "white"
            }
        }

        LargeText {
            id: tEmail
            height: tLogin.height
            width: 0.6 * dialogWindow.width
            anchors.top: tiLogin.bottom
            anchors.topMargin: 0.02 * parent.height
            anchors.verticalCenter: parent.verticalCenter
            text: dict.getStringByCode("Email")
            color: "white"
        }


        C2.TextField {
            id: tiEmail
            height: tLogin.height
            width: 0.6 * dialogWindow.width
            anchors.top: tEmail.bottom
            anchors.topMargin: 0.02 * parent.height
            anchors.verticalCenter: parent.verticalCenter
            text: "email@example.com"
            inputMethodHints: Qt.ImhEmailCharactersOnly + Qt.ImhNoPredictiveText + Qt.ImhNoAutoUppercase
            //validator: RegExpValidator { regExp: /(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])/ }
            validator: RegularExpressionValidator { regularExpression: /(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])/ }
            color: "white"
            background: Rectangle {
                radius: 7
                implicitWidth: 100
                implicitHeight: 24
                border.width: 1
                border.color: "white"
            }
        }

        LargeText {
            id: tConfirmation
            height: tLogin.height
            width: 0.6 * dialogWindow.width
            anchors.top: tiEmail.bottom
            anchors.topMargin: 0.02 * parent.height
            anchors.verticalCenter: parent.verticalCenter
            text: dict.getStringByCode("Iam13")
            color: "white"
        }
    }
    onCancel: {
        Qt.gameField.processEscape();
    }

    onApply: {
        Qt.core.regNewUser(tiLogin.text, tiEmail.text);
    }
}
