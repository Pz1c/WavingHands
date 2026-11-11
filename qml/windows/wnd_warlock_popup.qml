// source https://qt-project.org/forums/viewthread/26455

// The profile of a warlock: the player's own (stats, Hall of Fame) or another warlock's.
// https://github.com/Pz1c/WavingHands/issues/121: another warlock's profile also lists the
// warlock's games (QWarloksDuelCore::profileBattles, a tap opens the game) and has a
// "Write a Message" form, sent through sendMessageTo like the form of wnd_messages.qml.

import QtQuick 2.15

import "qrc:/qml/components"

import "qrc:/js/user_profile_utils.js" as UPU

InfoWindow {
    id: dMainItem

    property var warlock_data: ({})
    // the games of another warlock: [{id, s, d, r, o, w}]
    property var battles: []
    // the message form is open
    property bool writing: false
    // a message of this form is on its way
    property bool sending: false
    // what the last send came to
    property string status: ""
    property bool canSend: !sending && (teSay.text.trim().length > 0)

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15

        Flickable {
            id: fBody
            anchors.top: parent.top
            //anchors.topMargin: 24 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 24 * mainWindow.ratioObject
            anchors.bottom: rSay.visible ? rSay.top : parent.bottom
            z: 17
            clip: true
            contentWidth: width
            contentHeight: iItem.height
            boundsBehavior: Flickable.StopAtBounds

            Item {
                id: iItem
                width: fBody.width
                // the player's own profile keeps its button at the bottom of the window
                height: warlock_data.isPlayer
                        ? Math.max(fBody.height, cStats.height + bbAction.height + (36 + 108) * mainWindow.ratioObject)
                        : Math.max(fBody.height, cOther.y + cOther.height + 24 * mainWindow.ratioObject)
                // where the action button of another warlock's profile sits
                property real actionTop: cStats.height + 36 * mainWindow.ratioObject

                Column {
                    id: cStats
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Item {
                        id: iIcon
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: (warlock_data.isPlayer ? 0.6 : 0.36) * parent.width
                        height: width

                        Image {
                            id: iiIcon
                            anchors.centerIn: parent
                            source: "qrc:/res/trophy_3d.png"
                            height: 0.8 * parent.height
                            width: height

                            SequentialAnimation on height {
                                id: saClick
                                loops: Animation.Infinite
                                running: false

                                PropertyAnimation {
                                    from: 0.8 * iIcon.height
                                    to: iIcon.height
                                    duration: 3000
                                }

                                PropertyAnimation {
                                    from: iIcon.height
                                    to: 0.8 * iIcon.height
                                    duration: 3000
                                }
                            }
                        }
                    }

                    Item {
                        id: iGesture
                        width: parent.width
                        height: ltGesture.height + 5 * mainWindow.ratioObject

                        Text {
                            id: ltGesture
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.right: parent.right
                            //height: 70 * mainWindow.ratioObject
                            font.pixelSize: 42 * mainWindow.ratioFont
                            color: "#0654C0"
                            horizontalAlignment: Text.AlignHCenter
                            text: dict.getStringByCode("WarlockStats")
                        }
                    }

                    Item {
                        id: iTitle
                        width: parent.width
                        height: ltTitle.height + 5 * mainWindow.ratioObject

                        Text {
                            id: ltTitle
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            //anchors.right: parent.right
                            //height: 70 * mainWindow.ratioObject
                            font.pixelSize: 42 * mainWindow.ratioFont
                            color: "#10C9F5"
                            horizontalAlignment: Text.AlignHCenter
                            text: ""
                        }

                        Text {
                            id: ltTitleOnline
                            anchors.left: ltTitle.right
                            anchors.leftMargin: 5 * mainWindow.ratioObject
                            anchors.bottom: ltTitle.bottom
                            anchors.bottomMargin: 4 * mainWindow.ratioObject
                            font.pixelSize: 28 * mainWindow.ratioFont
                            color: "#FEE2D6"
                            horizontalAlignment: Text.AlignHCenter
                            text: dict.getStringByCode("OnlineLabel")
                            font.italic: true
                            visible: false
                        }
                    }

                    Item {
                        id: iDesc
                        width: parent.width
                        height: (3 + 3) * mainWindow.ratioObject

                        Rectangle {
                            id: rDesc
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#FEE2D6"
                            width: 330 * mainWindow.ratioObject
                            height: 3 * mainWindow.ratioObject
                        }
                    }

                    Item {
                        id: iStats
                        width: parent.width
                        height: rStats.height + 12 * mainWindow.ratioObject

                        Item {
                            id: rStats
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 330 * mainWindow.ratioObject
                            height: ltLast.height + ltScore.height + ltPlayed.height + 10 * mainWindow.ratioObject

                            Text {
                                id: ltLast
                                anchors.top: rStats.top
                                //anchors.topMargin: 5 * mainWindow.ratioObject
                                anchors.horizontalCenter: parent.horizontalCenter
                                //height: 70 * mainWindow.ratioObject
                                font.pixelSize: 28 * mainWindow.ratioFont
                                color: "#FEE2D6"
                                horizontalAlignment: Text.AlignHCenter
                                text: ""
                            }

                            Text {
                                id: ltScore
                                anchors.top: ltLast.bottom
                                anchors.topMargin: 5 * mainWindow.ratioObject
                                anchors.left: parent.left
                                //height: 70 * mainWindow.ratioObject
                                font.pixelSize: 28 * mainWindow.ratioFont
                                color: "#FEE2D6"
                                horizontalAlignment: Text.AlignLeft
                                text: dict.getStringByCode("StatScore")
                            }

                            Text {
                                id: ltPlayed
                                anchors.top: ltScore.bottom
                                anchors.topMargin: 5 * mainWindow.ratioObject
                                anchors.left: parent.left
                                //height: 70 * mainWindow.ratioObject
                                font.pixelSize: 28 * mainWindow.ratioFont
                                color: "#FEE2D6"
                                horizontalAlignment: Text.AlignLeft
                                text: dict.getStringByCode("StatPlayed")
                            }

                            Text {
                                id: ltWon
                                anchors.top: ltLast.bottom
                                anchors.topMargin: 5 * mainWindow.ratioObject
                                anchors.right: parent.right
                                //height: 70 * mainWindow.ratioObject
                                font.pixelSize: 28 * mainWindow.ratioFont
                                color: "#FEE2D6"
                                horizontalAlignment: Text.AlignRight

                                text: dict.getStringByCode("StatWon")
                            }

                            Text {
                                id: ltLost
                                anchors.top: ltWon.bottom
                                anchors.topMargin: 5 * mainWindow.ratioObject
                                anchors.right: parent.right
                                //height: 70 * mainWindow.ratioObject
                                font.pixelSize: 28 * mainWindow.ratioFont
                                color: "#FEE2D6"
                                horizontalAlignment: Text.AlignRight
                                text: dict.getStringByCode("StatLost")
                            }
                        }
                    }
                }

                BtnBig {
                    id: bbAction
                    text_color: "#ABF4F4"
                    text: dict.getStringByCode("Challenge")
                    border.width: 0
                    radius: 30
                    visible: true
                    z: 30
                    font.pixelSize: 49 * mainWindow.ratioFont
                    gradient: Gradient {
                        GradientStop { position: 0.0 ; color: "#905B93" }
                        GradientStop { position: 0.75; color: "#551470" }
                        GradientStop { position: 1.0 ; color: "#551470" }
                    }

                    width: 366 * ratioObject
                    height: 96 * ratioObject
                    y: warlock_data.isPlayer ? iItem.height - height - 108 * ratioObject : iItem.actionTop
                    anchors.horizontalCenter: parent.horizontalCenter


                    onClicked: {
                        console.log("chooseMonsterTarget", JSON.stringify(l_data));
                        mainWindow.processEscape();
                        if (warlock_data.isPlayer) {
                            mainWindow.showHallOfFameWindow();
                        } else if (warlock_data.online && warlock_data.mobile) {
                            mainWindow.showOnlineGameStart(warlock_data.name, 1);
                        } else {
                            mainWindow.startWarlockGame(warlock_data.name, 1);
                        }
                    }
                }

                // another warlock: the links under Challenge and the warlock's games
                Column {
                    id: cOther
                    y: iItem.actionTop + bbAction.height + 24 * mainWindow.ratioObject
                    anchors.left: parent.left
                    anchors.right: parent.right
                    visible: !warlock_data.isPlayer

                    Item {
                        id: iBtn2
                        width: parent.width
                        height: 54 * mainWindow.ratioObject

                        Text {
                            id: bbBtn2
                            anchors.centerIn: parent
                            color: "#A8F4F4"
                            text: dict.getStringByCode("PracticeTogether")
                            font.underline: true
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 28 * mainWindow.ratioFont

                            MouseArea {
                                id: maBtn2
                                anchors.fill: parent
                                anchors.margins: -9 * mainWindow.ratioObject
                                onClicked: {
                                    console.log("btn2 practice action");
                                    mainWindow.processEscape();
                                    if (warlock_data.online && warlock_data.mobile) {
                                        mainWindow.showOnlineGameStart(warlock_data.name, 2);
                                    } else {
                                        mainWindow.startWarlockGame(warlock_data.name, 2);
                                    }
                                }
                            }
                        }
                    }

                    Item {
                        id: iWrite
                        width: parent.width
                        height: 54 * mainWindow.ratioObject

                        Text {
                            id: ltWrite
                            anchors.centerIn: parent
                            color: "#A8F4F4"
                            text: dict.getStringByCode("WriteMessage")
                            font.underline: true
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 28 * mainWindow.ratioFont

                            MouseArea {
                                id: maWrite
                                anchors.fill: parent
                                anchors.margins: -9 * mainWindow.ratioObject
                                onClicked: dMainItem.writeMessage()
                            }
                        }
                    }

                    Item {
                        id: iGames
                        width: parent.width
                        height: 72 * mainWindow.ratioObject
                        visible: dMainItem.battles.length > 0

                        Text {
                            id: ltGames
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 6 * mainWindow.ratioObject
                            anchors.left: parent.left
                            anchors.right: parent.right
                            font.pixelSize: 28 * mainWindow.ratioFont
                            color: "#E7FFFF"
                            horizontalAlignment: Text.AlignLeft
                            elide: Text.ElideRight
                            text: warlock_data.name ? dict.getStringByCode("ProfileGames").replace("%1", warlock_data.name) : ""
                        }
                    }

                    // the rows of the game list of main.qml
                    Repeater {
                        id: rBattles
                        model: dMainItem.battles

                        delegate: Item {
                            id: idRow
                            width: cOther.width
                            height: 96 * mainWindow.ratioObject

                            Rectangle {
                                id: rRowBg
                                color: "#544653"
                                opacity: 0.39
                                radius: 30
                                anchors.fill: parent
                                anchors.bottomMargin: 12 * mainWindow.ratioObject
                            }

                            Text {
                                id: tRowText
                                anchors.verticalCenter: rRowBg.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 36 * mainWindow.ratioObject
                                anchors.right: iRowWon.visible ? iRowWon.left : (rRowReady.visible ? rRowReady.left : tRowLink.left)
                                anchors.rightMargin: 12 * mainWindow.ratioObject
                                font.pixelSize: 28 * mainWindow.ratioFont
                                color: modelData.s === 2 ? "#10C9F5" : "#FEE2D6"
                                horizontalAlignment: Text.AlignLeft
                                elide: Text.ElideRight
                                text: modelData.d
                            }

                            // the player's own turn in this game
                            Rectangle {
                                id: rRowReady
                                width: 24 * mainWindow.ratioObject
                                height: 24 * mainWindow.ratioObject
                                anchors.verticalCenter: rRowBg.verticalCenter
                                anchors.right: tRowLink.left
                                anchors.rightMargin: 18 * mainWindow.ratioObject
                                visible: modelData.r === 1
                                radius: width / 2
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#9DFFD3" }
                                    GradientStop { position: 1.0; color: "#9CDFFF" }
                                }
                            }

                            Image {
                                id: iRowWon
                                source: "qrc:/res/stars.png"
                                width: 24 * mainWindow.ratioObject
                                height: 24 * mainWindow.ratioObject
                                anchors.verticalCenter: rRowBg.verticalCenter
                                anchors.right: tRowLink.left
                                anchors.rightMargin: 18 * mainWindow.ratioObject
                                visible: modelData.w === 1
                            }

                            Text {
                                id: tRowLink
                                anchors.verticalCenter: rRowBg.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 18 * mainWindow.ratioObject
                                font.pixelSize: 28 * mainWindow.ratioFont
                                color: "#FEE2D6"
                                text: ">"
                            }

                            MouseArea {
                                id: maRow
                                anchors.fill: rRowBg
                                onClicked: dMainItem.openBattle(modelData)
                            }
                        }
                    }
                }
            }
        }

        // Write a Message
        Rectangle {
            id: rSay
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 214 * mainWindow.ratioObject
            z: 18
            visible: dMainItem.writing && !warlock_data.isPlayer

            gradient: Gradient {
                GradientStop { position: 0.0; color: "#210430" }
                GradientStop { position: 1.0; color: "#544653" }
            }

            // a tap between the controls stays in the form
            MouseArea {
                id: maSay
                anchors.fill: parent
            }

            Item {
                id: iSay
                anchors.fill: parent
                anchors.leftMargin: 42 * mainWindow.ratioObject
                anchors.rightMargin: 42 * mainWindow.ratioObject

                Text {
                    id: ltSay
                    anchors.top: parent.top
                    anchors.topMargin: 24 * mainWindow.ratioObject
                    anchors.left: parent.left
                    height: 36 * mainWindow.ratioObject
                    font.pixelSize: 26 * mainWindow.ratioFont
                    color: "#E7FFFF"
                    verticalAlignment: Text.AlignVCenter
                    text: dict.getStringByCode("MessageSay")
                }

                Text {
                    id: ltStatus
                    anchors.left: ltSay.right
                    anchors.leftMargin: 18 * mainWindow.ratioObject
                    anchors.right: parent.right
                    anchors.verticalCenter: ltSay.verticalCenter
                    font.pixelSize: 24 * mainWindow.ratioFont
                    font.italic: true
                    color: "#10C9F5"
                    horizontalAlignment: Text.AlignRight
                    elide: Text.ElideRight
                    text: dMainItem.status
                }

                Rectangle {
                    id: rSayText
                    anchors.top: ltSay.bottom
                    anchors.topMargin: 4 * mainWindow.ratioObject
                    anchors.left: parent.left
                    width: 0.8 * parent.width
                    height: 108 * mainWindow.ratioObject
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

                        onTextChanged: {
                            if (text.length > 0) {
                                dMainItem.status = "";
                            }
                        }

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
                    anchors.left: rSayText.right
                    anchors.leftMargin: 12 * mainWindow.ratioObject
                    anchors.right: parent.right
                    anchors.verticalCenter: rSayText.verticalCenter
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

    Connections {
        target: mainWindow.gameCore
        // only a send of this form: the Messages window sends through the same core
        function onMessageSent(to) {
            if (!dMainItem.sending) {
                return;
            }
            dMainItem.sending = false;
            teSay.text = "";
            dMainItem.status = dict.getStringByCode("MessageSent").replace("%1", to);
        }
        // the send failed: the error window is up, the text stays to be sent again
        function onErrorOccurred() {
            dMainItem.sending = false;
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function showWnd() {
        initErrFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
        saClick.stop();
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function writeMessage() {
        console.log("wnd_warlock_popup.writeMessage", warlock_data.name);
        writing = true;
        teSay.forceActiveFocus();
    }

    function sendMessage() {
        if (!canSend) {
            return;
        }
        var txt = teSay.text.trim();
        console.log("wnd_warlock_popup.sendMessage", warlock_data.name, txt.length);
        mainWindow.logEvent("Messages_Send", {Source:"profile"});
        status = "";
        sending = true;
        mainWindow.gameCore.sendMessageTo(warlock_data.name, txt);
    }

    function openBattle(b) {
        console.log("wnd_warlock_popup.openBattle", JSON.stringify(b));
        mainWindow.openProfileBattle(b);
    }

    function initErrFields() {
        warlock_data = mainWindow.gERROR;
        mainWindow.gERROR = {};
        console.log("wnd_warlock_popup.initFields.start", JSON.stringify(warlock_data));

        ltTitle.text = warlock_data.name;
        ltLast.text = UPU.getLastActivityText(warlock_data.last_activity);
        ltScore.text = dict.getStringByCode("StatScore") + warlock_data.elo;
        ltWon.text = dict.getStringByCode("StatWon") + warlock_data.won;
        ltLost.text = dict.getStringByCode("StatLost") + (warlock_data.played - warlock_data.won);
        ltPlayed.text = dict.getStringByCode("StatPlayed") + warlock_data.played;
        bbAction.text = dict.getStringByCode(warlock_data.isPlayer ? "TopList" : "Challenge");
        ltTitleOnline.visible = warlock_data.online;
        battles = (!warlock_data.isPlayer && warlock_data.battles) ? warlock_data.battles : [];
        writing = false;
        sending = false;
        status = "";
        teSay.text = "";
        fBody.contentY = 0;
        saClick.start();

        //console.log("wnd_warlock_popup.initFields.end", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
