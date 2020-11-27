// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: true
    body_height_prc: 60

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        //color: "transparent"

        ScrollView {
            id: svError
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: ltiMsg.top

            Text {
                id: ltError
                y: 0
                x: 0
                width: dialogWindow.width - 0.11 * dialogWindow.height
                //height: svLog.height
                font.pixelSize: 0.05 * dialogWindow.height
                wrapMode: Text.WordWrap
                color: "snow"
                textFormat: Text.RichText
                /*
                onLinkActivated: {
                    mainWindow.linkActivated(link);
                    mainWindow.processEscape();
                }*/
            }
        }

        LabeledTextInput {
            id: ltiMsg
            anchors.bottom: dialogWindow.bottom
            anchors.topMargin: 0.02 * parent.height
            height: 0.20 * dialogWindow.height
            width: dialogWindow.width
            title: dict.getStringByCode("Say")
            title_color: "lightgray"
            text_color: "black"
            transparent: false
            //placeholderText: "write your message there"
            regularExpression: /.{0,256}$/
        }

        z: 11
    }

    onCancel: {
        mainWindow.processEscape();
    }

    onApply: {
        mainWindow.storeBattleChatMsg(ltiMsg.text);
    }

    function showWnd() {
        initFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initErrFields() {
        console.log("wnd_chat.initErrFields", JSON.stringify(mainWindow.gERROR));
        ltError.text = mainWindow.gERROR.text;
        title_text = mainWindow.gERROR.title;
        if (mainWindow.gERROR.msg && (mainWindow.gERROR.msg.length > 0)) {
            ltiMsg.text = mainWindow.gERROR.msg;
        }
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
