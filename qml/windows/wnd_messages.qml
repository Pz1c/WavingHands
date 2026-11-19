// https://github.com/Pz1c/WavingHands/issues/320
// Messages between players, from the main menu and from the inbox banner of the game list.
// The inbox is QWarloksDuelCore::messages (sender, text, and Delete, which is deleteMsg: the
// site answers with the player page, so the list follows messagesChanged). The New Message
// form posts through sendMessageTo, which finds the recipient by name; tapping a sender's
// name fills its "To:" field. What a send came to is written beside the form's title.
// What the player wrote shows in the list too, under the message it answers, with the
// player's own name (plain, not a link): QWarloksDuelCore::sentMessages, kept on the device
// as the site has no outbox, so its Delete is deleteSentMsg. See buildRows.
// https://github.com/Pz1c/WavingHands/issues/374: a left drawer like the main menu (same
// width and brown background) over the dimmed game list. It is still a window of wnd_utils,
// so Back and the error windows opened above it work as before; a tap on the dimmed part
// closes it too.

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

Item {
    id: dMainItem
    anchors.fill: parent

    // main.qml's wrapper, so a language switch re-translates the cached window too
    property var dict: mainWindow.warlockDictionary
    property string code: "wnd_messages"
    // the parsed core.messages: [{from, id, text}]
    property var inbox: []
    // the list: the inbox and the player's own messages, see buildRows
    property var rows: []
    // what the last send came to
    property string status: ""
    property bool canSend: (tiTo.text.trim().length > 0) && (teSay.text.trim().length > 0)
    // sliding out: hidden once the drawer is off the screen
    property bool closing: false

    // the game list stays in sight beside the drawer, dimmed as under the main menu
    Rectangle {
        id: rDim
        anchors.fill: parent
        color: "#000000"
        opacity: rPanel.width > 0 ? 0.5 * (1 + rPanel.x / rPanel.width) : 0

        MouseArea {
            id: maDim
            anchors.fill: parent
            onClicked: dMainItem.close()
        }
    }

    Rectangle {
        id: rPanel
        width: Math.min(516 * mainWindow.ratioObject, parent.width)
        height: parent.height
        x: -width

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#544653" }
            GradientStop { position: 1.0; color: "#210430" }
        }

        Image {
            id: iBg
            anchors.fill: parent
            source: "qrc:/res/stars_bg.png"
        }

        // a tap between the controls stays in the drawer. No swipe to close: a DragHandler
        // here keeps the LargeText links of the list (Delete) from getting their press.
        MouseArea {
            id: maPanel
            anchors.fill: parent
        }

        Item {
            id: iHeader
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 188 * mainWindow.ratioObject

            Text {
                id: ltTitle
                anchors.bottom: rTitleLine.top
                anchors.bottomMargin: 12 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 42 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 42 * mainWindow.ratioObject
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#E7FFFF"
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                text: dict.getStringByCode("Messages") + (dMainItem.inbox.length > 0 ? " (" + dMainItem.inbox.length + ")" : "")
            }

            Rectangle {
                id: rTitleLine
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 2 * mainWindow.ratioObject
                color: "#E7FFFF"
            }
        }

        Item {
            id: iItem
            anchors.top: iHeader.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 42 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 42 * mainWindow.ratioObject

            Text {
                id: ltEmpty
                anchors.top: parent.top
                anchors.topMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 24 * mainWindow.ratioFont
                color: "#E7FFFF"
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
                visible: dMainItem.rows.length === 0
                text: dict.getStringByCode("NoIncomingMessages")
            }

            ListView {
                id: lvInbox
                anchors.top: parent.top
                anchors.topMargin: 12 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: iNewMessage.top
                anchors.bottomMargin: 12 * mainWindow.ratioObject
                clip: true
                model: dMainItem.rows
                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                // a sender's name is a link (tap to answer), the player's own is not; the
                // messages of one exchange stay close, the line comes under the last of them
                delegate: Item {
                    id: idRow
                    width: lvInbox.width
                    height: tRowFrom.height + tRowText.height + (modelData.tail ? 42 : 26) * mainWindow.ratioObject

                    Text {
                        id: tRowFrom
                        anchors.top: parent.top
                        anchors.topMargin: 12 * mainWindow.ratioObject
                        anchors.left: parent.left
                        width: Math.min(implicitWidth, parent.width - tRowColon.width)
                        font.pixelSize: 28 * mainWindow.ratioFont
                        font.underline: !modelData.mine
                        color: modelData.mine ? "#E7FFFF" : "#A8F4F4"
                        elide: Text.ElideRight
                        text: modelData.title

                        MouseArea {
                            id: maRowFrom
                            anchors.fill: parent
                            enabled: !modelData.mine
                            onClicked: dMainItem.replyTo(modelData.name)
                        }
                    }

                    Text {
                        id: tRowColon
                        anchors.left: tRowFrom.right
                        anchors.baseline: tRowFrom.baseline
                        font.pixelSize: tRowFrom.font.pixelSize
                        color: tRowFrom.color
                        text: ":"
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
                        visible: modelData.tail
                    }
                }
            }

            // New Message
            Item {
                id: iNewMessage
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 24 * mainWindow.ratioObject
                height: 310 * mainWindow.ratioObject

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
                    width: 0.8 * parent.width
                    height: 60 * mainWindow.ratioObject
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
                    width: 0.8 * parent.width
                    height: 90 * mainWindow.ratioObject
                    radius: 24 * mainWindow.ratioObject
                    color: "transparent"
                    border.color: "#E7FFFF"
                    border.width: 2 * mainWindow.ratioObject

                    TextEdit {
                        id: teSay
                        anchors.fill: parent
                        anchors.topMargin: 12 * mainWindow.ratioObject
                        anchors.bottomMargin: 12 * mainWindow.ratioObject
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

    NumberAnimation {
        id: aSlide
        target: rPanel
        property: "x"
        duration: 250
        easing.type: Easing.OutCubic

        onFinished: {
            if (dMainItem.closing) {
                dMainItem.closing = false;
                dMainItem.visible = false;
            }
        }
    }

    Connections {
        target: mainWindow.gameCore
        function onMessagesChanged() {
            dMainItem.refreshInbox();
        }
        function onSentMessagesChanged() {
            dMainItem.refreshInbox();
        }
        // only a send of this form: the profile window's form sends through the same core
        function onMessageSent(to) {
            if (!dMainItem.visible) {
                return;
            }
            teSay.text = "";
            dMainItem.status = dict.getStringByCode("MessageSent").replace("%1", to);
            dMainItem.showLastSent();
        }
    }

    function slideTo(x) {
        aSlide.stop();
        aSlide.to = x;
        aSlide.start();
    }

    // as Back does: wnd_utils takes the window off its stack and calls hideWnd
    function close() {
        if (!closing) {
            mainWindow.processEscape();
        }
    }

    function refreshInbox() {
        var arr = [], sent = [];
        try {
            arr = JSON.parse(mainWindow.gameCore.messages);
            sent = JSON.parse(mainWindow.gameCore.sentMessages);
        } catch (e) {
            console.log("wnd_messages.refreshInbox", e);
        }
        inbox = arr;
        rows = buildRows(arr, sent);
    }

    // The list: the inbox in the site's order with the player's own messages (oldest first)
    // put in. One written while the recipient's message was in the inbox (its "after" is
    // that message's text) is an answer and goes under it; any other goes above the
    // recipient's messages, which came later, or to the end when there are none. Rows of
    // one warlock in a run make an exchange: "tail" ends it, and an own message at its head
    // names the recipient, there being no message above to tell.
    function buildRows(msgs, sent) {
        var res = [], above = [], under = [], first = {}, rest = {}, rest_order = [];
        var me = mainWindow.gameCore.login, i, j, peer, at, list;
        for (i = 0; i < msgs.length; ++i) {
            peer = msgs[i].from.toLowerCase();
            if (first[peer] === undefined) {
                first[peer] = i;
            }
            above.push([]);
            under.push([]);
        }
        for (j = 0; j < sent.length; ++j) {
            peer = sent[j].to.toLowerCase();
            at = -1;
            for (i = 0; i < msgs.length; ++i) {
                if ((msgs[i].from.toLowerCase() === peer) && (msgs[i].text === sent[j].after)) {
                    at = i;
                }
            }
            if (at !== -1) {
                list = under[at];
            } else if (first[peer] !== undefined) {
                list = above[first[peer]];
            } else {
                // nothing of this warlock's in the inbox: one exchange for each of them
                if (rest[peer] === undefined) {
                    rest[peer] = [];
                    rest_order.push(peer);
                }
                list = rest[peer];
            }
            list.push({mine: true, peer: peer, name: sent[j].to, id: sent[j].ts, text: sent[j].text});
        }
        for (i = 0; i < msgs.length; ++i) {
            res = res.concat(above[i]);
            res.push({mine: false, peer: msgs[i].from.toLowerCase(), name: msgs[i].from, id: msgs[i].id, text: msgs[i].text});
            res = res.concat(under[i]);
        }
        for (i = 0; i < rest_order.length; ++i) {
            res = res.concat(rest[rest_order[i]]);
        }
        for (i = 0; i < res.length; ++i) {
            res[i].tail = (i === res.length - 1) || (res[i + 1].peer !== res[i].peer);
            if (!res[i].mine) {
                res[i].title = res[i].name;
            } else if ((i === 0) || (res[i - 1].peer !== res[i].peer)) {
                res[i].title = dict.getStringByCode("MessageFromTo").replace("%1", me).replace("%2", res[i].name);
            } else {
                res[i].title = me;
            }
        }
        return res;
    }

    // the message just sent may have landed below the visible part of the list
    function showLastSent() {
        var idx = -1, i;
        for (i = 0; i < rows.length; ++i) {
            if (rows[i].mine && ((idx === -1) || (rows[i].id > rows[idx].id))) {
                idx = i;
            }
        }
        if (idx !== -1) {
            lvInbox.positionViewAtIndex(idx, ListView.Contain);
        }
    }

    function replyTo(name) {
        console.log("wnd_messages.replyTo", name);
        tiTo.text = name;
        teSay.forceActiveFocus();
    }

    function deleteMessage(msg) {
        console.log("wnd_messages.deleteMessage", JSON.stringify(msg));
        if (msg.mine) {
            mainWindow.logEvent("Messages_Delete", {Source:"sent"});
            mainWindow.gameCore.deleteSentMsg("" + msg.id);
            return;
        }
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
        closing = false;
        visible = true;
        slideTo(0);
    }

    function hideWnd() {
        if (!visible || closing) {
            return;
        }
        closing = true;
        slideTo(-rPanel.width);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        refreshInbox();
        if (visible) {
            slideTo(0);
        }
    }
}
