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


        Image {
            id: iSword
            anchors.top: parent.top
            anchors.topMargin: 24 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            width: 96 * mainWindow.ratioObject
            height: 78 * mainWindow.ratioObject
            source: "qrc:/res/sword.png"
        }

        Text {
            id: tSword
            anchors.verticalCenter: iSword.verticalCenter
            anchors.left: iSword.right
            anchors.leftMargin: 12 * mainWindow.ratioObject
            font.pixelSize: 42 * mainWindow.ratioFont
            color: "#10C9F5"
            text: warlockDictionary.getStringByCode("NewGamePractice")
        }

        BtnBig {
            id: bbTb
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("NewVFGameBtn1")
            bg_color_active: "#551470"
            border_color_active: "#551470"
            radius: 30

            gradient: Gradient {
                GradientStop { position: 0.0 ; color: "#905B93" }
                GradientStop { position: 0.75; color: "#551470" }
                GradientStop { position: 1.0 ; color: "#551470" }
            }

            width: 258 * mainWindow.ratioObject
            height: 78 * mainWindow.ratioObject
            anchors.top: iSword.bottom
            anchors.topMargin: 42 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 30 * mainWindow.ratioObject

            font.pixelSize: 42 * mainWindow.ratioFont

            onClicked: {
                console.log("start game btn 1");
                mainWindow.processEscape();
                mainWindow.startGameWithBotEx();
            }
        }

        BtnBig {
            id: bbMo
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("NewVFGameBtn2")
            transparent: true
            font.underline: true
            border.width: 0
            visible: true

            width: 0.5 * parent.width
            height: 60 * mainWindow.ratioObject
            font.pixelSize: 28 * mainWindow.ratioFont
            fontSizeMode: Text.VerticalFit

            anchors.top: bbTb.bottom
            anchors.topMargin: 42 * mainWindow.ratioObject
            anchors.horizontalCenter: bbTb.horizontalCenter

            onClicked: {
                mainWindow.processEscape();
                mainWindow.showSearchWarlockWnd("vf");
//                mainWindow.startTrainingGame();
            }
        }


        Image {
            id: iStab
            anchors.top: iSword.bottom
            anchors.topMargin: 372 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            width: 96 * mainWindow.ratioObject
            height: 78 * mainWindow.ratioObject
            source: "qrc:/res/stab.png"
        }

        Text {
            id: tStab
            anchors.verticalCenter: iStab.verticalCenter
            anchors.left: iStab.right
            anchors.leftMargin: 12 * mainWindow.ratioObject
            font.pixelSize: 42 * mainWindow.ratioFont
            color: "#10C9F5"
            text: warlockDictionary.getStringByCode("NewGameDuel")
        }

        BtnBig {
            id: bbAm
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("NewFGameBtn1")
            bg_color_active: "#551470"
            border_color_active: "#551470"
            radius: 30

            gradient: Gradient {
                GradientStop { position: 0.0 ; color: "#905B93" }
                GradientStop { position: 0.75; color: "#551470" }
                GradientStop { position: 1.0 ; color: "#551470" }
            }

            width: 258 * mainWindow.ratioObject
            height: 78 * mainWindow.ratioObject
            anchors.top: iStab.bottom
            anchors.topMargin: 42 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 30 * mainWindow.ratioObject

            font.pixelSize: 42 * mainWindow.ratioFont

            onClicked: {
                console.log("start game btn 1");
                mainWindow.processEscape();
                mainWindow.startGameWithPlayerEx();
            }
        }

        BtnBig {
            id: bbIf
            text_color: "#ABF4F4"
            text: warlockDictionary.getStringByCode("NewFGameBtn2")
            bg_color_active: "#551470"
            border_color_active: "#551470"
            radius: 30

            gradient: Gradient {
                GradientStop { position: 0.0 ; color: "#905B93" }
                GradientStop { position: 0.75; color: "#551470" }
                GradientStop { position: 1.0 ; color: "#551470" }
            }

            width: 258 * mainWindow.ratioObject
            height: 78 * mainWindow.ratioObject
            anchors.top: bbAm.bottom
            anchors.topMargin: 42 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 30 * mainWindow.ratioObject

            font.pixelSize: 42 * mainWindow.ratioFont

            onClicked: {
                console.log("start game btn 1");
                mainWindow.processEscape();
                mainWindow.showSearchWarlockWnd("f");
            }
        }

        BtnBig {
            id: bbFbN
            text_color: "#A8F4F4"
            text: warlockDictionary.getStringByCode("NewFGameBtn3")
            transparent: true
            font.underline: true
            border.width: 0
            visible: true

            width: 0.5 * parent.width
            height: 60 * mainWindow.ratioObject
            font.pixelSize: 28 * mainWindow.ratioFont
            fontSizeMode: Text.VerticalFit

            anchors.top: bbIf.bottom
            anchors.topMargin: 42 * mainWindow.ratioObject
            anchors.horizontalCenter: bbTb.horizontalCenter

            onClicked: {
                mainWindow.processEscape();
                mainWindow.callInviteFriends();
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
