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

            Item {
                id: iIcon
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: 0.6 * parent.width
                height: 0.6 * parent.width

                Image {
                    id: iiIcon
                    anchors.centerIn: parent
                    source: "qrc:/res/spellbook.png"
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
                id: ltTitle
                anchors.top: iIcon.bottom
                anchors.topMargin: 5 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 70 * mainWindow.ratioObject
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#0654C0"
                horizontalAlignment: Text.AlignHCenter
                text: "Unlocked a new spellbook"
            }

            Text {
                id: ltShortDesc
                anchors.top: ltTitle.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 50 * mainWindow.ratioObject
                color: "#10C9F5"
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 63 * mainWindow.ratioFont
                //fontSizeMode: Text.VerticalFit
                wrapMode: Text.WordWrap
                text: dict.getStringByCode("SpellbookName" + mainWindow.playerSpellbookLevel)
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
                text: "View Spellbook"//dict.getStringByCode("MonsterSetTarget")
                border.width: 0
                radius: 30
                visible: true
                z: 30
                font.pixelSize: 49 * mainWindow.ratioFont
                gradient: gDef

                width: 366 * ratioObject
                height: 96 * ratioObject
                anchors.bottom: bbSkipAction.top
                anchors.bottomMargin: 50 * ratioObject
                anchors.horizontalCenter: parent.horizontalCenter


                onClicked: {
                    console.log("wnd_popup.mainAction");
                    mainWindow.showWndSpellbook(1, 0);
                }
            }

            BtnBig {
                id: bbSkipAction
                text_color: "#A8F4F4"
                text: "Skip"
                transparent: true
                font.underline: true
                border.width: 0
                radius: 30
                visible: false
                gradient: gDef

                width: 366 * ratioObject
                height: 60 * ratioObject
                font.pixelSize: 28 * mainWindow.ratioFont
                fontSizeMode: Text.VerticalFit

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    console.log("skip feedback action");
                    mainWindow.processEscape();

                }
            }

            Text {
                id: bbBtn3
                color: "#A8F4F4"
                text: "Skip"
                font.underline: true
                visible: false
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 24 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.leftMargin: 24 * mainWindow.ratioObject

                MouseArea {
                    id: maBtn3
                    anchors.fill: parent
                    onClicked: {
                        console.log("btn3 feedback action");
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
        saClick.stop();
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    function initErrFields() {
        console.log("wnd_spell.initFields", JSON.stringify(mainWindow.gERROR));        
        saClick.start();
        mainWindow.gERROR = {};
        console.log("wnd_spell.initFields", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
