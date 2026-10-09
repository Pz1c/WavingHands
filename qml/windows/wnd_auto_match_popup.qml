// https://github.com/Pz1c/WavingHands/issues/216
// Shown by Smart Match while auto matching is off (type 203 in wnd_utils.js): "Auto join"
// turns the setting on, and the app joins or creates games from then on; "Join only once"
// makes this one match the old way, with no Keep me On offer on top of it.

import QtQuick 2.15

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
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#10C9F5"
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("AutoMatchTitle")
            }

            Text {
                id: ltDesc
                anchors.top: ltTitle.bottom
                anchors.topMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("AutoMatchDesc")
            }

            BtnBig {
                id: bbAction
                text_color: "#ABF4F4"
                text: dict.getStringByCode("AutoMatchBtn")
                border.width: 0
                radius: 30
                z: 30
                font.pixelSize: 49 * mainWindow.ratioFont
                gradient: Gradient {
                    GradientStop { position: 0.0 ; color: "#905B93" }
                    GradientStop { position: 0.75; color: "#551470" }
                    GradientStop { position: 1.0 ; color: "#551470" }
                }

                width: 366 * mainWindow.ratioObject
                height: 96 * mainWindow.ratioObject
                anchors.bottom: bbOnceAction.top
                anchors.bottomMargin: 30 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    console.log("wnd_auto_match_popup.autoJoin");
                    mainWindow.logEvent("AutoMatch_Click", {});
                    mainWindow.processEscape();
                    // the scan this starts joins or creates the first game
                    mainWindow.gameCore.setKeepMeOn(true);
                }
            }

            Text {
                id: bbOnceAction
                color: "#A8F4F4"
                text: dict.getStringByCode("AutoMatchOnce")
                font.underline: true
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 28 * mainWindow.ratioFont

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 120 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                MouseArea {
                    id: maOnce
                    anchors.fill: parent
                    onClicked: {
                        console.log("wnd_auto_match_popup.once");
                        mainWindow.logEvent("AutoMatch_Once", {});
                        mainWindow.processEscape();
                        mainWindow.startGameWithPlayerEx(true);
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

    function initErrFields() {
        console.log("wnd_auto_match_popup.initFields", JSON.stringify(mainWindow.gERROR));
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
