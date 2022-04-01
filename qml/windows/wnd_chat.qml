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
            anchors.bottom: iAction.top
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

        Item {
            id: iAction
            anchors.bottom: iText.top
            anchors.bottomMargin: 16 * mainWindow.ratioObject
            anchors.left: parent.left
            //anchors.leftMargin: 3 * mainWindow.ratioObject
            anchors.right: parent.right
            //anchors.rightMargin: 3 * mainWindow.ratioObject
            height: tTitle.height + 67 * mainWindow.ratioObject

            Text {
                id: tTitle
                anchors.bottom: rTextEdit.top
                anchors.topMargin: 16 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 16 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 16 * mainWindow.ratioObject
                //height: 0.20 * dialogWindow.height
                font.pixelSize: 41 * mainWindow.ratioFont
                //width: dialogWindow.width
                text: dict.getStringByCode("Say")
                color: "#E7FFFF"
            }

            Rectangle {
                id: rTextEdit
                height: 64 * mainWindow.ratioObject
                anchors.bottom: parent.bottom
                //anchors.bottomMargin: 3 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 16 * mainWindow.ratioObject
                anchors.right: iiCSend.left
                anchors.rightMargin: 32 * mainWindow.ratioObject
                radius: 10
                color: "transparent"
                border.color: "#E7FFFF"
                border.width: 2 * mainWindow.ratioObject
                z: 10

                //TextField {
                TextInput {
                    id: tiTextEdit
                    anchors.centerIn: parent
                    width: 0.98 * parent.width
                    height: 0.98 * parent.height
                    inputMethodHints: Qt.ImhNoPredictiveText
                    color: "#E7FFFF"
                    font.pixelSize: 48 * mainWindow.ratioFont
                    z: 15
                }
            }


            IconInfo {
                id: iiCSend
                source: "qrc:/res/send_1.png"
                textVisible: false

                height: 64 * mainWindow.ratioObject
                width: 64 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 16 * mainWindow.ratioObject
                anchors.bottom: parent.bottom
                //anchors.bottomMargin: 3 * mainWindow.ratioObject
                active: true
                color: "transparent"

                onClicked: {
                    setNewMsgText(tiTextEdit.text);
                    //mainWindow.storeBattleChatMsg(tiTextEdit.text);
                }
            }
        }

        Item {
            id: iText
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 16 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 16 * mainWindow.ratioObject
            height: tMsgShow.text !== "" ? tMsgShow.contentHeight : 0

            Text {
                id: tMsgShow
                anchors.fill: parent
                font.pixelSize: 28 * mainWindow.ratioFont
                //width: dialogWindow.width
                //text: dict.getStringByCode("Say")
                color: "#E7FFFF"
                onLinkActivated: {
                    setNewMsgText("");
                }
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

    function setNewMsgText(txt) {
        if (txt !== "") {
            tMsgShow.text = '<p><font color="#10C9F5" size=+1>Message:</font></p><p><i>"' +
                txt + '"</i>&nbsp;&nbsp;&nbsp;<a href="/delete_msg"><font color="#10C9F5">Delete</font></a></p>';
        } else {
            tMsgShow.text = "";
        }
        mainWindow.storeBattleChatMsg(txt);
    }

    function initErrFields() {
        console.log("wnd_chat.initErrFields", JSON.stringify(mainWindow.gERROR));
        ltError.text = replaceAll(mainWindow.gERROR.text, "&quot;", '"');
        title_text = mainWindow.gERROR.title;
        setNewMsgText(mainWindow.gERROR.msg);
        tiTextEdit.text = "";

        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
        //ltiMsg.setFontSize(0.20 * dialogWindow.height);
    }
}
