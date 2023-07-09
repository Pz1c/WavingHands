// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

import "qrc:/js/user_profile_utils.js" as UPU

InfoWindow {
    id: dMainItem

    property var warlock_data: ({})

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
                text: "Warlock stats"
            }

            Text {
                id: ltTitle
                anchors.top: ltGesture.bottom
                anchors.topMargin: 5 * mainWindow.ratioObject
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
                text: "online"
                font.italic: true
                visible: false
            }

            Rectangle {
                id: rDesc
                anchors.top: ltTitle.bottom
                anchors.topMargin: 3 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#FEE2D6"
                width: 330 * mainWindow.ratioObject
                height: 3 * mainWindow.ratioObject
            }

            Item {
                id: rStats
                anchors.top: rDesc.bottom
                anchors.topMargin: 12 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter
                width: 330 * mainWindow.ratioObject
                height: ltLast.height + 10 * mainWindow.ratioObject

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
                    text: "Score: "
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
                    text: "Played: "
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

                    text: "Won: "
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
                    text: "Lost: "
                }
            }

            BtnBig {
                id: bbAction
                text_color: "#ABF4F4"
                text: "Challenge"
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
                anchors.bottom: bbBtn2.top
                anchors.bottomMargin: 50 * ratioObject
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


            Text {
                id: bbBtn2
                color: "#A8F4F4"
                text: "Practice together"
                font.underline: true
                visible: false
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 24 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                MouseArea {
                    id: maBtn2
                    anchors.fill: parent
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
        warlock_data = mainWindow.gERROR;
        mainWindow.gERROR = {};
        console.log("wnd_warlock_popup.initFields.start", JSON.stringify(warlock_data));

        ltTitle.text = warlock_data.name;

        ltLast.text = UPU.getLastActivityText(warlock_data.last_activity);
        ltScore.text = "Score: " + warlock_data.elo;
        ltWon.text = "Won: " + warlock_data.won;
        ltLost.text = "Lost: " + (warlock_data.played - warlock_data.won);
        ltPlayed.text = "Played: " + warlock_data.played;
        bbBtn2.visible = !warlock_data.isPlayer;
        bbAction.text = warlock_data.isPlayer ? "Hall of Fame" : "Challenge";
        ltTitleOnline.visible = warlock_data.online;
        saClick.start();

        //console.log("wnd_warlock_popup.initFields.end", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
