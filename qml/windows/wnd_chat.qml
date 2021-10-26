// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    //with_controls: true
    //with_apply: true
    //body_height_prc: 60

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        //color: "transparent"

        Text {
            id: ltTitle
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: parent.right
            //height: 70 * mainWindow.ratioObject
            font.pixelSize: 49 * mainWindow.ratioFont
            color: "#10C9F5"
            horizontalAlignment: Text.AlignLeft
            //fontSizeMode: Text.VerticalFit
            text: "Chat"
        }

        ScrollView {
            id: svError
            anchors.top: ltTitle.bottom
            anchors.topMargin: 20 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.bottom: ltiMsg.top
            contentWidth: -1

            Text {
                id: ltError
                y: 0
                x: 0
                width: dialogWindow.width - 0.11 * dialogWindow.height
                //height: svLog.height
                font.pixelSize: 28 * mainWindow.ratioFont
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
            anchors.bottomMargin: 0.02 * parent.height
            anchors.left: parent.left
            anchors.leftMargin: 20 * mainWindow.ratioObject
            anchors.right: iiCSend.left
            anchors.rightMargin: 20 * mainWindow.ratioObject
            height: 0.20 * dialogWindow.height
            //width: dialogWindow.width
            title: dict.getStringByCode("Say")
            title_color: "lightgray"
            //text_color: "black"
            transparent: false
            //placeholderText: "write your message there"
            regularExpression: /.{0,256}$/
        }

        IconInfo {
            id: iiCSend
            source: "qrc:/res/send_1.png"
            textVisible: false

            height: 64 * mainWindow.ratioObject
            width: 64 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 20 * mainWindow.ratioObject
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0.02 * parent.height
            active: true
            color: "transparent"
            //text_color: "#E7FFFF"
            //text: "Submit"

            onClicked: {
                mainWindow.storeBattleChatMsg(ltiMsg.text);
            }
        }

        z: 11
    }

    onCancel: {
        mainWindow.processEscape();
    }

    /*onApply: {
        mainWindow.storeBattleChatMsg(ltiMsg.text);
    }*/

    function showWnd() {
        initErrFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function initErrFields() {
        console.log("wnd_chat.initErrFields", JSON.stringify(mainWindow.gERROR));
        ltError.text = replaceAll(mainWindow.gERROR.text, "&quot;", '"');
        title_text = mainWindow.gERROR.title;
        if (mainWindow.gERROR.msg && (mainWindow.gERROR.msg.length > 0)) {
            ltiMsg.text = mainWindow.gERROR.msg;
        } else {
            ltiMsg.text = "";
        }

        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
        ltiMsg.setFontSize(0.20 * dialogWindow.height);
    }
}
