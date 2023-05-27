// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

import "qrc:/js/battle_gui_utils.js" as BGU
import "qrc:/js/spell_popup_gui_utils.js" as SPGU

InfoWindow {
    id: dMainItem

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15

        Item {
            id: iItem
            anchors.top: parent.top
            //anchors.topMargin: 24 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 24 * mainWindow.ratioObject
            anchors.bottom: parent.bottom
            z: 17

            Item {
                id: iIcon
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: 0.6 * parent.width
                height: 0.6 * parent.width

                Image {
                    id: iiIcon
                    anchors.centerIn: parent
                    source: "qrc:/res/stars_purple.png"
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


            Text {
                id: ltGesture
                anchors.top: iIcon.bottom
                anchors.topMargin: 5 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 70 * mainWindow.ratioObject
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#0654C0"
                horizontalAlignment: Text.AlignHCenter
                text: ""
            }

            Text {
                id: ltTitle
                anchors.top: ltGesture.bottom
                anchors.topMargin: 5 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 70 * mainWindow.ratioObject
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#10C9F5"
                horizontalAlignment: Text.AlignHCenter
                text: ""
            }

            Text {
                id: ltShortDesc
                anchors.top: ltTitle.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 50 * mainWindow.ratioObject
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 28 * mainWindow.ratioFont
                //fontSizeMode: Text.VerticalFit
                wrapMode: Text.WordWrap
                text: ""
            }

            Rectangle {
                id: rDesc
                anchors.top: ltShortDesc.bottom
                anchors.topMargin: 28 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#FEE2D6"
                width: 330 * mainWindow.ratioObject
                height: 3 * mainWindow.ratioObject
            }

            ScrollView {
                id: svMain
                anchors.top: rDesc.bottom
                anchors.topMargin: 30 * mainWindow.ratioObject
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

                    //onLinkActivated: function (link) { errorLinkActivated(link) }
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
        saClick.stop();
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function initErrFields() {
        console.log("wnd_spell_popup.initFields.start", JSON.stringify(mainWindow.gERROR));
        switch(mainWindow.gERROR.type) {
        case 9:
            SPGU.prepareDataTypeFinishedGame(mainWindow.gERROR, dict);
            break;
        default:
            SPGU.prepareDataTypeSpell(mainWindow.gERROR.spell, dict);
            break;
        }

        saClick.start();
        mainWindow.gERROR = {};
        console.log("wnd_spell_popup.initFields.end", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
