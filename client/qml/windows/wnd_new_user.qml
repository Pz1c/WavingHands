// source https://qt-project.org/forums/viewthread/26455
import QtQuick 2.15
import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: true
    title_text: dict.getStringByCode("EnterNewUser")
    body_height_prc: 40
    with_action1: true
    action1_text: dict.getStringByCode("Login")

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        anchors.fill: content_item
        color: "snow"
        z: 11

        LabeledTextInput {
            id: ltiEmail
            anchors.top: dialogWindow.top
            anchors.topMargin: 0.02 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            height: 0.38 * dialogWindow.height
            width: dialogWindow.width
            title: dict.getStringByCode("Email")
            placeholderText: "example@mail.com"
            regularExpression: /(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])/
        }

        LabeledTextInput {
            id: ltiLogin
            anchors.top: ltiEmail.bottom
            anchors.topMargin: 0.02 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            height: 0.38 * dialogWindow.height
            width: dialogWindow.width
            title: dict.getStringByCode("Login")
            placeholderText: "Merlin"
            regularExpression: /^[a-zA-Z0-9_-]{2,10}$/
        }

        LargeText {
            id: tConfirmation
            height: 0.18 * parent.height
            width: dialogWindow.width
            anchors.top: ltiLogin.bottom
            anchors.topMargin: 0.02 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            text: dict.getStringByCode("Iam13")
            color: "black"
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    onApply: {
        var err_cnt = 0;
        if ((ltiLogin.text.length < 2) || (ltiLogin.text.length > 10)) {
            ltiLogin.border_color = "red";
            ++err_cnt;
        } else {
            ltiLogin.border_color = "black";
        }

        if ((ltiEmail.text.indexOf("@") === -1) || (ltiEmail.text.indexOf(".") === -1)|| (ltiEmail.text.length < 6)) {
            ltiEmail.border_color = "red";
            ++err_cnt;
        } else {
            ltiEmail.border_color = "black";
        }

        if (err_cnt === 0) {
            Qt.core.regNewUser(ltiLogin.text, ltiEmail.text);
        }
    }

    onAction1: {
        console.log("wnd_new_user.onAction1");
        mainWindow.getLoginFromUser(true);
    }

    Component.onCompleted: {
        console.log("wnd_new_user.onCompleted", dialogWindow.z, content_item.z);
    }
}
