// source https://qt-project.org/forums/viewthread/26455

// With auto matching on (https://github.com/Pz1c/WavingHands/issues/216) the Smart Match
// button gives way to a box that says so and opens the setting; off, Smart Match first
// offers to turn it on (wnd_auto_match_popup.qml).

import QtQuick 2.15

import "qrc:/qml/components"

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
            visible: !mainWindow.gameCore.keepMeOn

            onClicked: {
                console.log("start game btn 1");
                mainWindow.processEscape();
                if (mainWindow.gameCore.keepMeOn) {
                    mainWindow.startGameWithPlayerEx();
                } else {
                    mainWindow.showAutoMatchWnd();
                }
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
            // takes Smart Match's place while that is hidden
            anchors.top: bbAm.visible ? bbAm.bottom : iStab.bottom
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

        // "Auto start matches ON": opens the setting
        Rectangle {
            id: rAutoMatch
            color: "#0654C0"
            visible: mainWindow.gameCore.keepMeOn

            anchors.left: parent.left
            anchors.leftMargin: 30 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 30 * mainWindow.ratioObject
            anchors.bottom: parent.bottom
            // low enough to clear the Invite a friend link on a 16:9 screen
            anchors.bottomMargin: 30 * mainWindow.ratioObject
            height: 102 * mainWindow.ratioObject

            Text {
                id: tAutoMatchTitle
                anchors.left: parent.left
                anchors.leftMargin: 18 * mainWindow.ratioObject
                anchors.right: tAutoMatchArrow.left
                anchors.rightMargin: 12 * mainWindow.ratioObject
                anchors.top: parent.top
                anchors.topMargin: 14 * mainWindow.ratioObject
                font.pixelSize: 34 * mainWindow.ratioFont
                color: "#FEE2D6"
                elide: Text.ElideRight
                text: warlockDictionary.getStringByCode("AutoMatchBoxTitle")
            }

            Text {
                id: tAutoMatchDesc
                anchors.left: tAutoMatchTitle.left
                anchors.right: tAutoMatchTitle.right
                anchors.top: tAutoMatchTitle.bottom
                anchors.topMargin: 4 * mainWindow.ratioObject
                font.pixelSize: 24 * mainWindow.ratioFont
                color: "#FEE2D6"
                elide: Text.ElideRight
                text: warlockDictionary.getStringByCode("AutoMatchBoxDesc")
            }

            Text {
                id: tAutoMatchArrow
                anchors.right: parent.right
                anchors.rightMargin: 18 * mainWindow.ratioObject
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 40 * mainWindow.ratioFont
                color: "#FEE2D6"
                text: ">"
            }

            MouseArea {
                id: maAutoMatch
                anchors.fill: parent
                onClicked: {
                    console.log("wnd_new_game.autoMatchSettings");
                    mainWindow.logEvent("AutoMatch_Settings_Click", {});
                    mainWindow.processEscape();
                    mainWindow.showSettingsWnd();
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
