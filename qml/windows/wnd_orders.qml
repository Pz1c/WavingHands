// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: true
    with_apply: true
    body_height_prc: 60
    control_height_prc: 10

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 11

        ScrollView {
            id: svError
            anchors.fill: parent
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            ListView {
                id: lvOrderList
                model: []
                anchors.fill: parent

                delegate: Item {
                        id: idRoot
                        width: lvOrderList.width
                        height: 0.10 * dialogWindow.height

                        Rectangle {
                            id: rdSpellItem
                            color: "transparent"
                            radius: 30
                            anchors.centerIn: parent
                            height: 0.95 * parent.height
                            width: 0.95 * parent.width
                            anchors.bottomMargin: 0.01 * dialogWindow.height
                            border.color: lvOrderList.model[index].c
                            border.width: 1

                            LargeText {
                                id: rdbiGesture
                                anchors.centerIn: rdSpellItem
                                color: "snow"
                                horizontalAlignment: Text.AlignLeft
                                text: lvOrderList.model[index].v
                                width: 0.9 * parent.width
                                height: 0.8 * parent.height
                            }

                            MouseArea {
                                id: maSpell
                                anchors.fill: parent
                                onClicked: {
                                    console.log("click on order", index, JSON.stringify(lvOrderList.model[index]));
                                    var item = lvOrderList.model[index];
                                    switch(item.type) {
                                    case "LH":
                                        mainWindow.showGesture(true);
                                        break;
                                    case "LH":
                                        mainWindow.showGesture(false);
                                        break;
                                    case "M":
                                        mainWindow.gBattle.currentMonsterIdx = item.action_idx;
                                        mainWindow.showErrorWnd({type:4,text:"",title:"Monster",data:mainWindow.gBattle.actions.M[item.action_idx],close_current:1});
                                        break;
                                    default:
                                        mainWindow.closeChild();
                                    }
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
        mainWindow.confirmOrdersEx();
    }

    function showWnd() {
        initGFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function initGFields() {
        console.log("wnd_gesture.initGFields", JSON.stringify(mainWindow.gERROR));
        title_text = mainWindow.gERROR.title;
        lvOrderList.model = mainWindow.gERROR.data;
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initGFields();
    }
}
