// https://github.com/Pz1c/WavingHands/issues/149
// Shown once the player has made a PvP game by hand while "Keep me On" is off: the game
// now waits for an opponent, and the player may let the app keep one open from now on
// (type 202 in wnd_utils.js, raised by QWarloksDuelCore::finishCreateChallenge).

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
                text: dict.getStringByCode("KeepMeOnTitle")
            }

            Text {
                id: ltShortDesc
                anchors.top: ltTitle.bottom
                anchors.topMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("KeepMeOnShortDesc")
            }

            Text {
                id: ltDesc
                anchors.top: ltShortDesc.bottom
                anchors.topMargin: 48 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("KeepMeOnDesc")
            }

            BtnBig {
                id: bbAction
                text_color: "#ABF4F4"
                text: dict.getStringByCode("KeepMeOnBtn")
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
                anchors.bottom: bbSkipAction.top
                anchors.bottomMargin: 30 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    console.log("wnd_keep_me_on_popup.keepMeOn");
                    mainWindow.logEvent("KeepMeOn_Click", {});
                    mainWindow.gameCore.setKeepMeOn(true);
                    mainWindow.processEscape();
                }
            }

            Text {
                id: bbSkipAction
                color: "#A8F4F4"
                text: dict.getStringByCode("Skip")
                font.underline: true
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 28 * mainWindow.ratioFont

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 120 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                MouseArea {
                    id: maSkip
                    anchors.fill: parent
                    onClicked: {
                        console.log("wnd_keep_me_on_popup.skip");
                        mainWindow.logEvent("KeepMeOn_Skip", {});
                        mainWindow.processEscape();
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
        console.log("wnd_keep_me_on_popup.initFields", JSON.stringify(mainWindow.gERROR));
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
