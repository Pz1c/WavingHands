// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

InfoWindow {
    id: dMainItem

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15

        Item {
            id: iItem
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 24 * mainWindow.ratioObject
            anchors.bottom: parent.bottom
            z: 17

            Text {
                id: ltTitle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 70 * mainWindow.ratioObject
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#10C9F5"
                horizontalAlignment: Text.AlignLeft
                text: "Challenge to a live match?"
            }

            Text {
                id: ltShortDesc
                anchors.top: ltTitle.bottom
                anchors.topMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 50 * mainWindow.ratioObject
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont
                //fontSizeMode: Text.VerticalFit
                wrapMode: Text.WordWrap
                text: "In a live match each player has upto 30 minutes to play a whole game.<br>After time runs out you ha ve 45 seconds per turn"
            }

            ScrollView {
                id: svMain
                anchors.top: ltShortDesc.bottom
                anchors.topMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                contentWidth: -1

                Text {
                    id: ltError
                    width: iItem.width * 0.99
                    wrapMode: Text.Wrap
                    textFormat: Text.RichText
                    font.pixelSize: 21 * mainWindow.ratioFont
                    color: "#FEE2D6"
                    horizontalAlignment: Text.AlignJustify
                    text: "Note: On each turn, the time counter continues counting once you enter the game,
                           or when 1 minute passes from the time when you get notified of a new turn (the erlier of the two)"
                    //onLinkActivated: function (link) { errorLinkActivated(link) }
                }
            }

            Gradient {
                id: gDef
                GradientStop { position: 0.0 ; color: "#905B93" }
                GradientStop { position: 0.75; color: "#551470" }
                GradientStop { position: 1.0 ; color: "#551470" }
            }

            BtnBig {
                id: bbAction
                text_color: "#ABF4F4"
                text: "Live Match"
                border.width: 0
                radius: 30
                z: 30
                font.pixelSize: 49 * mainWindow.ratioFont
                gradient: gDef

                width: 366 * ratioObject
                height: 96 * ratioObject
                anchors.bottom: bbBtn3.top
                anchors.bottomMargin: 48 * ratioObject
                anchors.horizontalCenter: parent.horizontalCenter


                onClicked: {
                    console.log("start online match", JSON.stringify(l_data));
                    mainWindow.processEscape();
                    mainWindow.startWarlockGame(l_data.warlock, l_data.level, true);
                }
            }

            Text {
                id: bbBtn3
                color: "#A8F4F4"
                text: "Regular Match"
                font.underline: true
                visible: true
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 24 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                MouseArea {
                    id: maBtn3
                    anchors.fill: parent
                    onClicked: {
                        console.log("start regular match");
                        mainWindow.processEscape();
                        mainWindow.startWarlockGame(l_data.warlock, l_data.level);
                    }
                }
            }
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
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function errorLinkActivated(link) {
        console.log("wnd_spell.onLinkActivated", link, l_data.type);
    }

    function initErrFields() {
        console.log("wnd_spell.initFields", JSON.stringify(mainWindow.gERROR));
        if (!mainWindow.gERROR || (!mainWindow.gERROR.spell && !mainWindow.gERROR.data && !(mainWindow.gERROR.type >= 7))) {
        return;
        }

        l_data = mainWindow.gERROR;

        mainWindow.gERROR = {};
        console.log("wnd_spell.initFields", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
