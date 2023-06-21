// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

import "qrc:/js/battle_gui_utils.js" as BGU
import "qrc:/js/spell_popup_gui_utils.js" as SPGU

InfoWindow {
    id: dMainItem
    titleVisible: false
    backgroundImage.visible: false
    gradient: Gradient {
                  /*GradientStop { position: 0.0;  color: "#210430" }
                  GradientStop { position: 0.65; color: "#1f1e4a" }
                  GradientStop { position: 1.0;  color: "#11c7f3" }*/
        GradientStop { position: 0.0;  color: "#210430" }
        GradientStop { position: 0.65; color: "#210430" }
        GradientStop { position: 1.0;  color: "#10C9F5" }
              }

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15


        Image {
            id: iAfterRegIcon
            anchors.top: parent.top
            //anchors.topMargin: 12 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 12 * mainWindow.ratioObject
            width: 148 * mainWindow.ratioObject
            height: 148 * mainWindow.ratioObject
            source: "qrc:/res/stab.png"
        }

        Text {
            id: tAfterRegTitle1
            anchors.top: iAfterRegIcon.bottom
            anchors.left: iAfterRegIcon.left
            anchors.topMargin: 48 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 12 * mainWindow.ratioObject
            wrapMode: Text.WordWrap
            font.pixelSize: 49 * mainWindow.ratioFont
            color: "#A8F4F4"
            text: "Ready to start?"

        }

        Text {
            id: tAfterRegTitle2
            anchors.top: tAfterRegTitle1.bottom
            anchors.left: tAfterRegTitle1.left
            anchors.topMargin: 48 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 12 * mainWindow.ratioObject
            wrapMode: Text.WordWrap
            font.pixelSize: 28 * mainWindow.ratioFont
            color: "#A8F4F4"
            text: "You will start with a small apprentice spellbook. After you've trained a bit, go ahead and play with other players."
        }

        Text {
            id: tAfterRegTitle3
            anchors.top: tAfterRegTitle2.bottom
            anchors.left: tAfterRegTitle2.left
            anchors.topMargin: 48 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 12 * mainWindow.ratioObject
            wrapMode: Text.WordWrap
            font.pixelSize: 28 * mainWindow.ratioFont
            color: "#A8F4F4"
            text: "This can be one of the best games you've ever played, but it takes 2 to 3 games to master, so take your time :)<br><br>Let's play!"
        }

        IconInfo {
            id: bbAfterRegStart
            active: true
            source: "qrc:/res/send_1.png"
            textVisible: true

            height: 80 * mainWindow.ratioObject
            iconHeight: 80 * mainWindow.ratioObject
            iconWidth: 80 * mainWindow.ratioObject
            textHeight: 80 * mainWindow.ratioObject
            textWidth: 80 * mainWindow.ratioObject
            width: (64 + 18 + 80) * mainWindow.ratioObject
            textAnchors.left: bbAfterRegStart.left
            textAnchors.right: undefined
            iconAnchors.right: bbAfterRegStart.right
            iconAnchors.centerIn: undefined
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 48 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 24 * mainWindow.ratioObject

            color: "transparent"
            text_color: "#A8F4F4"
            text: "Start"

            onClicked: {
                console.log("wnd_after_reg.start");
                mainWindow.afterRegAction();
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
        //saClick.stop();
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function initErrFields() {
        console.log("wnd_new_game.initFields.start", JSON.stringify(mainWindow.gERROR));
        mainWindow.gERROR = {};
        //console.log("wnd_new_game.initFields.end", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
