// source https://qt-project.org/forums/viewthread/26455
import QtQuick 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: true
    title_text: dict.getStringByCode("EnterLoginPass")
    body_height_prc: 40

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        anchors.fill: content_item
        color: "black"
        z: 11

        LabeledTextInput {
            id: ltiLogin
            anchors.top: dialogWindow.top
            anchors.topMargin: 0.02 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            height: 0.40 * dialogWindow.height
            width: dialogWindow.width
            title_color: "white"
            title: dict.getStringByCode("Login")
            text_color: "white"
            placeholderText: "Merlin"
            regularExpression: /^[a-zA-Z0-9_-]{2,10}$/
        }

        LabeledTextInput {
            id: ltiPass
            anchors.top: ltiLogin.bottom
            anchors.topMargin: 0.02 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            height: 0.40 * dialogWindow.height
            width: dialogWindow.width
            title_color: "white"
            title: dict.getStringByCode("Password")
            text_color: "white"
            isPassword: true
            placeholderText: ""
            regularExpression: /^.{4,10}$/
        }
    }

    onCancel: {
        Qt.gameField.processEscape();
    }

    onApply: {
        var err_cnt = 0;
        if ((ltiLogin.text.length < 2) || (ltiLogin.text.length > 10)) {
            ltiLogin.border_color = "red";
            ++err_cnt;
        } else {
            ltiLogin.border_color = "white";
        }

        if ((ltiPass.text.length < 4) || (ltiPass.text.length > 10)) {
            ltiPass.border_color = "red";
            ++err_cnt;
        } else {
            ltiPass.border_color = "white";
        }

        if (err_cnt === 0) {
            Qt.core.setLogin(ltiLogin.text, ltiPass.text);
        }
    }

    onAction1: {
        console.log("wnd_new_user.onAction1");
    }

    Component.onCompleted: {
        console.log("wnd_new_user.onCompleted", dialogWindow.z, content_item.z);
    }
}
