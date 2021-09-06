// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: false
    with_apply: true
    body_height_prc: 100
    title_height_prc: 0
    control_height_prc: 0
    body_width_prc: 100
    overOpacity: 0

    bg_source: "qrc:/res/stars_bg.png"
    overRect.anchors.left: undefined
    overRect.width: 516 * mainWindow.ratioObject
    overRect.gradient: Gradient {
        GradientStop { position: 0.0; color: "#210430" }
        GradientStop { position: 1.0; color: "#0654C0" }
    }

    // This rectangle is the actual popup
    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 11

        Image {
            id: iGlow
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 84 * mainWindow.ratioObject
            source: "qrc:/res/glow.png"
            fillMode: Image.Stretch
            opacity: 0.5
        }

        ScrollView {
            id: svError
            anchors.top: parent.top
            anchors.topMargin: 60 * mainWindow.ratioObject
            anchors.left: iGlow.right
            anchors.leftMargin: 12 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 12 * mainWindow.ratioObject
            anchors.bottom: parent.bottom

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            Item {
                id: iSpellBook
                anchors.top:  parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: (60 + lvOrderList.model.length * 102) * mainWindow.ratioObject

                visible: true

                LargeText {
                    id: ltSpellbook
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 13 * mainWindow.ratioObject
                    fontSizeMode: 14 * mainWindow.ratioFont
                    height: 60 * mainWindow.ratioObject
                    width: parent.width
                    color: "#2DA0A5"
                    horizontalAlignment: Text.AlignLeft
                    text: "Review your orders:"
                }

                ListView {
                    id: lvOrderList
                    model: []
                    anchors.top: ltSpellbook.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: lvOrderList.model.length * 102 * mainWindow.ratioObject

                    contentWidth: -1

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
                                border.color: lvOrderList.model[index].c ? lvOrderList.model[index].c : "snow"
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

                IconInfo {
                    id: iiSend
                    source: "qrc:/res/send_1.png"
                    textVisible: true

                    height: 64 * mainWindow.ratioObject
                    iconHeight: 64 * mainWindow.ratioObject
                    iconWidth: 64 * mainWindow.ratioObject
                    textHeight: 64 * mainWindow.ratioObject
                    textWidth: 80 * mainWindow.ratioObject
                    width: (64 + 30 + 80) * mainWindow.ratioObject
                    textAnchors.left: iiSend.left
                    textAnchors.right: undefined
                    iconAnchors.right: iiSend.right
                    iconAnchors.centerIn: undefined
                    anchors.right: parent.right
                    anchors.rightMargin: 48 * mainWindow.ratioObject
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 30 * mainWindow.ratioObject
                    active: true
                    color: "transparent"
                    text_color: "#E7FFFF"
                    text: "Submit"

                    onClicked: {
                        mainWindow.confirmOrdersEx();
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
