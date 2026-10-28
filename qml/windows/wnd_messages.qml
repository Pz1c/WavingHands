// https://github.com/Pz1c/WavingHands/issues/320
// Messages between players, from the main menu and from the inbox banner of the game list.
// The inbox is QWarloksDuelCore::messages (sender, text, and Delete, which is deleteMsg: the
// site answers with the player page, so the list follows messagesChanged). The New Message
// form posts through sendMessageTo, which finds the recipient by name; tapping a sender's
// name fills its "To:" field. What a send came to is written beside the form's title.

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    // the parsed core.messages: [{from, id, text}]
    property var inbox: []
    // what the last send came to
    property string status: ""
    property bool canSend: (tiTo.text.trim().length > 0) && (teSay.text.trim().length > 0)

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15

        Item {
            id: iItem
            anchors.fill: parent
            anchors.leftMargin: 24 * mainWindow.ratioObject
            anchors.rightMargin: 24 * mainWindow.ratioObject
            z: 17

            Text {
                id: ltTitle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#E7FFFF"
                horizontalAlignment: Text.AlignLeft
                text: dict.getStringByCode("Messages") + (dMainItem.inbox.length > 0 ? " (" + dMainItem.inbox.length + ")" : "")
            }

            Rectangle {
                id: rTitleLine
                anchors.top: ltTitle.bottom
                anchors.topMargin: 8 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                height: 2 * mainWindow.ratioObject
                color: "#E7FFFF"
            }

            Text {
                id: ltEmpty
                anchors.top: rTitleLine.bottom
                anchors.topMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 28 * mainWindow.ratioFont
                color: "#E7FFFF"
                horizontalAlignment: Text.AlignLeft
                visible: dMainItem.inbox.length === 0
                text: dict.getStringByCode("NoIncomingMessages")
            }

            ListView {
                id: lvInbox
                anchors.top: rTitleLine.bottom
                anchors.topMargin: 12 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: iNewMessage.top
                anchors.bottomMargin: 12 * mainWindow.ratioObject
                clip: true
                model: dMainItem.inbox
                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                delegate: Item {
                    id: idRow
                    width: lvInbox.width
                    height: tRowFrom.height + tRowText.height + 42 * mainWindow.ratioObject

                    Text {
                        id: tRowFrom
                        anchors.top: parent.top
                        anchors.topMargin: 12 * mainWindow.ratioObject
                        anchors.left: parent.left
                        font.pixelSize: 28 * mainWindow.ratioFont
                        font.underline: true
                        color: "#A8F4F4"
                        text: modelData.from + ":"

                        MouseArea {
                            id: maRowFrom
                            anchors.fill: parent
                            onClicked: dMainItem.replyTo(modelData.from)
                        }
                    }

                    Text {
                        id: tRowText
                        anchors.top: tRowFrom.bottom
                        anchors.topMargin: 6 * mainWindow.ratioObject
                        anchors.left: parent.left
                        anchors.right: ltRowDelete.left
                        anchors.rightMargin: 12 * mainWindow.ratioObject
                        font.pixelSize: 24 * mainWindow.ratioFont
                        color: "#FEE2D6"
                        wrapMode: Text.WordWrap
                        text: "\"" + modelData.text + "\""
                    }

                    LargeText {
                        id: ltRowDelete
                        anchors.right: parent.right
                        anchors.verticalCenter: tRowText.verticalCenter
                        height: 40 * mainWindow.ratioObject
                        width: 90 * mainWindow.ratioObject
                        font.pixelSize: 24 * mainWindow.ratioFont
                        font.underline: true
                        fontSizeMode: Text.VerticalFit
                        horizontalAlignment: Text.AlignRight
                        color: "#A8F4F4"
                        text: dict.getStringByCode("MessageDelete")

                        onClicked: dMainItem.deleteMessage(modelData)
                    }

                    Rectangle {
                        id: rRowLine
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 1
                        color: "#E7FFFF"
                        opacity: 0.3
                    }
                }
            }

            // New Message
            Item {
                id: iNewMessage
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 12 * mainWindow.ratioObject
                height: 344 * mainWindow.ratioObject

                Text {
                    id: ltNewTitle
                    anchors.top: parent.top
                    anchors.left: parent.left
                    height: 48 * mainWindow.ratioObject
                    font.pixelSize: 36 * mainWindow.ratioFont
                    color: "#E7FFFF"
                    verticalAlignment: Text.AlignVCenter
                    text: dict.getStringByCode("NewMessage")
                }

                Text {
                    id: ltStatus
                    anchors.left: ltNewTitle.right
                    anchors.leftMargin: 18 * mainWindow.ratioObject
                    anchors.right: parent.right
                    anchors.verticalCenter: ltNewTitle.verticalCenter
                    font.pixelSize: 24 * mainWindow.ratioFont
                    font.italic: true
                    color: "#10C9F5"
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: dMainItem.status
                }

                Rectangle {
                    id: rNewLine
                    anchors.top: ltNewTitle.bottom
                    anchors.topMargin: 6 * mainWindow.ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 2 * mainWindow.ratioObject
                    color: "#E7FFFF"
                }

                Text {
                    id: ltTo
                    anchors.top: rNewLine.bottom
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    anchors.left: parent.left
                    height: 36 * mainWindow.ratioObject
                    font.pixelSize: 26 * mainWindow.ratioFont
                    color: "#E7FFFF"
                    verticalAlignment: Text.AlignVCenter
                    text: dict.getStringByCode("MessageTo")
                }

                Rectangle {
                    id: rTo
                    anchors.top: ltTo.bottom
                    anchors.topMargin: 4 * mainWindow.ratioObject
                    anchors.left: parent.left
                    width: 0.78 * parent.width
                    height: 64 * mainWindow.ratioObject
                    radius: height / 2
                    color: "transparent"
                    border.color: "#E7FFFF"
                    border.width: 2 * mainWindow.ratioObject

                    TextInput {
                        id: tiTo
                        anchors.fill: parent
                        anchors.leftMargin: 24 * mainWindow.ratioObject
                        anchors.rightMargin: 24 * mainWindow.ratioObject
                        verticalAlignment: TextInput.AlignVCenter
                        font.pixelSize: 26 * mainWindow.ratioFont
                        color: "#FEE2D6"
                        clip: true
                        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
                        validator: RegularExpressionValidator { regularExpression: /^[a-zA-Z0-9_-]{0,32}$/ }

                        onTextEdited: dMainItem.status = ""

                        Text {
                            id: tToHint
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            font: tiTo.font
                            color: "#9A96B4"
                            elide: Text.ElideRight
                            visible: tiTo.text.length === 0
                            text: dict.getStringByCode("MessageToHint")
                        }
                    }
                }

                Text {
                    id: ltSay
                    anchors.top: rTo.bottom
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    anchors.left: parent.left
                    height: 36 * mainWindow.ratioObject
                    font.pixelSize: 26 * mainWindow.ratioFont
                    color: "#E7FFFF"
                    verticalAlignment: Text.AlignVCenter
                    text: dict.getStringByCode("MessageSay")
                }

                Rectangle {
                    id: rSay
                    anchors.top: ltSay.bottom
                    anchors.topMargin: 4 * mainWindow.ratioObject
                    anchors.left: parent.left
                    width: 0.78 * parent.width
                    height: 120 * mainWindow.ratioObject
                    radius: 24 * mainWindow.ratioObject
                    color: "transparent"
                    border.color: "#E7FFFF"
                    border.width: 2 * mainWindow.ratioObject

                    TextEdit {
                        id: teSay
                        anchors.fill: parent
                        anchors.topMargin: 14 * mainWindow.ratioObject
                        anchors.bottomMargin: 14 * mainWindow.ratioObject
                        anchors.leftMargin: 24 * mainWindow.ratioObject
                        anchors.rightMargin: 24 * mainWindow.ratioObject
                        font.pixelSize: 26 * mainWindow.ratioFont
                        color: "#FEE2D6"
                        wrapMode: TextEdit.Wrap
                        clip: true

                        Text {
                            id: tSayHint
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            font: teSay.font
                            color: "#9A96B4"
                            wrapMode: Text.WordWrap
                            visible: teSay.text.length === 0
                            text: dict.getStringByCode("MessageSayHint")
                        }
                    }
                }

                Image {
                    id: iSend
                    anchors.left: rSay.right
                    anchors.leftMargin: 12 * mainWindow.ratioObject
                    anchors.right: parent.right
                    anchors.verticalCenter: rSay.verticalCenter
                    height: 72 * mainWindow.ratioObject
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/res/send_1.png"
                    opacity: dMainItem.canSend ? 1 : 0.45

                    MouseArea {
                        id: maSend
                        anchors.fill: parent
                        onClicked: dMainItem.sendMessage()
                    }
                }
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    Connections {
        target: mainWindow.gameCore
        function onMessagesChanged() {
            dMainItem.refreshInbox();
        }
        // only a send of this form: the profile window's form sends through the same core
        function onMessageSent(to) {
            if (!dMainItem.visible) {
                return;
            }
            teSay.text = "";
            dMainItem.status = dict.getStringByCode("MessageSent").replace("%1", to);
        }
    }

    function refreshInbox() {
        var arr = [];
        try {
            arr = JSON.parse(mainWindow.gameCore.messages);
        } catch (e) {
            console.log("wnd_messages.refreshInbox", e);
        }
        inbox = arr;
    }

    function replyTo(name) {
        console.log("wnd_messages.replyTo", name);
        tiTo.text = name;
        teSay.forceActiveFocus();
    }

    function deleteMessage(msg) {
        console.log("wnd_messages.deleteMessage", JSON.stringify(msg));
        mainWindow.logEvent("Messages_Delete", {});
        mainWindow.gameCore.deleteMsg("" + msg.id);
    }

    function sendMessage() {
        if (!canSend) {
            return;
        }
        var to = tiTo.text.trim(), txt = teSay.text.trim();
        console.log("wnd_messages.sendMessage", to, txt.length);
        mainWindow.logEvent("Messages_Send", {});
        status = "";
        mainWindow.gameCore.sendMessageTo(to, txt);
    }

    function showWnd() {
        status = "";
        refreshInbox();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        refreshInbox();
    }
}
