// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: false
    body_height_prc: 60
    control_height_prc: 5
    title_text: dict.getStringByCode("SpellbookTitle")

    property var arrSpell: []

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 11

        ScrollView {
            id: svError
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            ListView {
                id: lvSpellList
                model: []
                anchors.fill: parent

                delegate: Item {
                        id: idRoot
                        width: lvSpellList.width
                        height: 0.08 * dialogWindow.height
                        Rectangle {
                        id: rdSpellItem
                        color: "transparent"
                        radius: 30
                        anchors.centerIn: parent
                        height: 0.95 * parent.height
                        width: 0.95 * parent.width
                        anchors.bottomMargin: 0.01 * dialogWindow.height
                        border.color: "#A8F4F4"
                        border.width: 0

                        LargeText {
                            id: rdbiGesture
                            anchors.top: rdSpellItem.top
                            anchors.bottom: rdSpellItem.bottom
                            anchors.left: rdSpellItem.left
                            anchors.leftMargin: 0.03 * parent.width
                            width: 0.40 * parent.width
                            //anchors.rightMargin: 0.05 * parent.width
                            color: lvSpellList.model[index].basic ? "snow" : "darkgrey"
                            horizontalAlignment: Text.AlignLeft
                            text: lvSpellList.model[index].g
                        }

                        LargeText {
                            id: rdbifTitle
                            anchors.verticalCenter: rdSpellItem.verticalCenter
                            anchors.left: rdbiGesture.right
                            anchors.leftMargin: 0.01 * parent.width
                            anchors.right: parent.right//rdbifInfo.left
                            anchors.rightMargin: 0.03 * parent.width
                            height: 0.8 * rdSpellItem.height
                            color: "snow"
                            fontSizeMode: Text.VerticalFit
                            horizontalAlignment: Text.AlignRight
                            text: lvSpellList.model[index].n
                        }

                        MouseArea {
                            id: maSpell
                            anchors.fill: parent
                            onClicked: {
                                console.log("choose spell", mainWindow.gBattle.spellIdx, index, JSON.stringify(lvSpellList.model[index]), JSON.stringify(arrSpell[index]));
                                mainWindow.showSpellDetails(lvSpellList.model[index].g);
                            }
                        }
                    }
                }
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    onApply: {
        //mainWindow.storeBattleChatMsg(ltiMsg.text);
    }

    function showWnd() {
        initGFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initGFields() {
        console.log("wnd_spellbook.initGFields", arrSpell.length);
        if (arrSpell.length === 0) {
            arrSpell = mainWindow.getSpellList("FULL_LIST");
        }
        lvSpellList.model = arrSpell;
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initGFields();
    }
}
