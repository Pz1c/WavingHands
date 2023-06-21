// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

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
    overRect.width: 528 * mainWindow.ratioObject
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

        Gradient {
            id: gGesture
            GradientStop { position: 0.0;  color: "#E7FFFF" }
            GradientStop { position: 0.25; color: "#E7FFFF" }
            GradientStop { position: 1.0;  color: "#FEE2D6" }
        }

        //ScrollView {
        Item {
            id: svError
            anchors.top: parent.top
            anchors.topMargin: 60 * mainWindow.ratioObject
            anchors.left: iGlow.right
            //anchors.leftMargin: 12 * mainWindow.ratioObject
            anchors.right: parent.right
            //anchors.rightMargin: 12 * mainWindow.ratioObject
            anchors.bottom: iiSend.top

            //ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            //ScrollBar.vertical.policy: ScrollBar.AsNeeded
            //contentHeight: iSpellBook.height

            Item {
                id: iSpellBook
                anchors.top:  parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                //height: lvOrderList.height + ltSpellbook.height

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

                Rectangle {
                    id: rLine
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: ltSpellbook.bottom
                    height: 0//2 * mainWindow.ratioObject
                    color: "#FEE2D6"
                }

                ListView {
                    id: lvOrderList
                    model: []
                    anchors.top: rLine.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    //height: lvOrderList.model.length * 500 * mainWindow.ratioObject

                    contentWidth: -1
                    contentHeight: lvOrderList.model.length * 300 * mainWindow.ratioObject
                    flickableDirection: Flickable.VerticalFlick

                    delegate: chooser

                    DelegateChooser {
                        id: chooser
                        role: "row_type"

                        DelegateChoice {
                            roleValue: "1"
                            delegate: Item {
                                id: idHRoot
                                width: lvOrderList.width
                                height:  80 * mainWindow.ratioObject

                                IconInfo {
                                    id: rdbiHIcon
                                    source: "qrc:/res/lightning_notice.png"
                                    visible: true
                                    anchors.left: idHRoot.left
                                    anchors.leftMargin: 24 * mainWindow.ratioObject
                                    anchors.verticalCenter: idHRoot.verticalCenter
                                    height: 48 * mainWindow.ratioObject
                                    width: height
                                    text: ""
                                    textVisible: false
                                    color: "transparent"
                                }

                                Text {
                                    id: rdbiHGesture
                                    color: "#E7FFFF"
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                    text: lvOrderList.model[index].v
                                    font.pixelSize: 21 * mainWindow.ratioFont
                                    wrapMode: Text.WordWrap
                                    anchors.left: rdbiHIcon.right
                                    anchors.leftMargin: 24 * mainWindow.ratioObject
                                    anchors.right: parent.right
                                    anchors.rightMargin: 24 * mainWindow.ratioObject
                                    anchors.top: parent.top
                                    anchors.topMargin: 5 * mainWindow.ratioObject
                                    //height: 92 * mainWindow.ratioObject
                                }

                                Rectangle {
                                    id: rItemHLineB
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.bottom: idHRoot.bottom
                                    height: 2 * mainWindow.ratioObject
                                    color: "#FEE2D6"
                                    visible: index === lvOrderList.model.length - 1
                                }
                            }
                        }

                        DelegateChoice {
                            roleValue: "0"
                            delegate: Item {
                            id: idRoot
                            width: lvOrderList.width
                            height: Math.max(rdbiGesture.contentHeight + 12 * mainWindow.ratioObject, 102 * mainWindow.ratioObject)

                                IconInfo {
                                    id: rdbiIcon
                                    source: lvOrderList.model[index].icon_visible ? "qrc:/res/" + lvOrderList.model[index].icon + ".png" : ""
                                    visible: lvOrderList.model[index].icon_visible
                                    anchors.left: idRoot.left
                                    anchors.leftMargin: 24 * mainWindow.ratioObject
                                    anchors.verticalCenter: idRoot.verticalCenter
                                    height: 78 * mainWindow.ratioObject
                                    width: lvOrderList.model[index].icon_width * mainWindow.ratioObject
                                    text: lvOrderList.model[index].icon_text
                                    textVisible: lvOrderList.model[index].icon_text !== ""
                                    color: lvOrderList.model[index].type === "PP" ? "transparent" : "#551470"
                                    gradient: ((lvOrderList.model[index].type === "LH") || (lvOrderList.model[index].type === "RH")) ? gGesture : undefined
                                }

                                Text {
                                    id: rdbiGesture
                                    color: "#FEE2D6"
                                    horizontalAlignment: Text.AlignLeft
                                    text: lvOrderList.model[index].v
                                    font.pixelSize: 28 * mainWindow.ratioFont
                                    wrapMode: Text.WordWrap
                                    anchors.left: idRoot.left
                                    anchors.leftMargin: 126 * mainWindow.ratioObject
                                    anchors.right: rdbiAction.left
                                    anchors.rightMargin: 24 * mainWindow.ratioObject
                                    anchors.top: parent.top
                                    anchors.topMargin: 5 * mainWindow.ratioObject
                                    height: 92 * mainWindow.ratioObject
                                }

                                /*ScrollView {
                                    id: rdbiSVGesture
                                    anchors.left: idRoot.left
                                    anchors.leftMargin: 126 * mainWindow.ratioObject
                                    anchors.right: rdbiAction.left
                                    anchors.rightMargin: 24 * mainWindow.ratioObject
                                    anchors.verticalCenter: parent.verticalCenter
                                    height: 92 * mainWindow.ratioObject
                                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                                    ScrollBar.vertical.policy: ScrollBar.AsNeeded
                                    contentWidth: -1

                                    Text {
                                        id: rdbiGesture
                                        color: "#FEE2D6"
                                        horizontalAlignment: Text.AlignLeft
                                        text: lvOrderList.model[index].v
                                        font.pixelSize: 28 * mainWindow.ratioFont
                                        wrapMode: Text.WordWrap
                                        anchors.fill: parent
                                    }
                                }*/

                                LargeText {
                                    id: rdbiAction
                                    anchors.right: idRoot.right
                                    anchors.rightMargin: 24 * mainWindow.ratioObject
                                    anchors.verticalCenter: idRoot.verticalCenter
                                    color: "#FEE2D6"
                                    text: ">"
                                    font.pixelSize: 28 * mainWindow.ratioFont
                                    width: 48 * mainWindow.ratioObject
                                    height: 64 * mainWindow.ratioObject
                                }

                                MouseArea {
                                    id: maSpell
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("click on order", index, JSON.stringify(lvOrderList.model[index]));
                                        var item = lvOrderList.model[index];
                                        mainWindow.logEvent("Play_Review_Click", {RowType: item.type});
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
                                        case "H":
                                            break;
                                        default:
                                            mainWindow.closeChild();
                                        }
                                    }
                                }

                                Rectangle {
                                    id: rItemLineT
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.top: idRoot.top
                                    height: 2 * mainWindow.ratioObject
                                    color: "#FEE2D6"
                                    visible: true
                                }

                                Rectangle {
                                    id: rItemLineB
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.bottom: idRoot.bottom
                                    height: 2 * mainWindow.ratioObject
                                    color: "#FEE2D6"
                                    visible: index === lvOrderList.model.length - 1
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
                        mainWindow.logEvent("Play_Review_Final_Submit");
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
        var is_surrender = mainWindow.gERROR.data && (mainWindow.gERROR.data.length > 0) && mainWindow.gERROR.data[0] && mainWindow.gERROR.data[0].type &&
                (mainWindow.gERROR.data[0].type === "PP");
        iiSend.width = (!is_surrender ? (64 + 30 + 80) : (64 + 30 + 120)) * mainWindow.ratioObject;
        iiSend.textWidth = (!is_surrender ? 80 : 120)* mainWindow.ratioObject;
        iiSend.text = is_surrender ? "SURRENDER!" : "Submit";
        iiSend.animate(1);
        mainWindow.gERROR = {};
        mainWindow.logEvent("Play_Submit_Clicked", {Warning: is_surrender ? "surrendering" : "none"});

    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initGFields();
    }
}
