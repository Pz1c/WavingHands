// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

BaseWindow {
    id: dMainItem

    with_controls: false
    with_apply: true
    body_height_prc: 70
    control_height_prc: 0

    property string currGesture: ""
    property var arrGesture: [iiGC,iiGD,iiGF,iiGS,iiGP,iiGW,iiGSt,iiGN]
    property var mapGesture: ({"C":iiGC,"D":iiGD,"F":iiGF,"S":iiGS,"P":iiGP,"W":iiGW,">":iiGSt,"-":iiGN})

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
            anchors.bottom: iGesture.top


            ListView {
                id: lvSpellList
                model: []
                anchors.fill: parent

                delegate: Item {
                        id: idRoot
                        width: lvSpellList.width
                        height: 0.1 * dialogWindow.height
                        Rectangle {
                        id: rdSpellItem
                        color: "#544653"
                        radius: 30
                        anchors.fill: parent
                        anchors.topMargin: 0.01 * dialogWindow.height
                        anchors.bottomMargin: 0.01 * dialogWindow.height

                        LargeText {
                            id: rdbiGesture
                            anchors.top: rdSpellItem.top
                            anchors.bottom: rdSpellItem.bottom
                            anchors.left: rdSpellItem.left
                            anchors.leftMargin: 0.03 * parent.width
                            width: 0.40 * parent.width
                            //anchors.rightMargin: 0.05 * parent.width
                            color: "#FEE2D6"
                            horizontalAlignment: Text.AlignLeft
                            text: lvSpellList.model[index].g
                        }

                        LargeText {
                            id: rdbifTitle
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.left: rdbiGesture.right
                            anchors.right: rdbifInfo.left
                            color: "#FEE2D6"
                            fontSizeMode: Text.VerticalFit
                            horizontalAlignment: Text.AlignRight
                            text: lvSpellList.model[index].t
                        }

                        Image {
                            id: rdbifInfo
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            width: 0.5 * height
                            source: "qrc:/res/g_=.png";
                            z: 13

                            MouseArea {
                                id: maSpellInfo
                                anchors.fill: parent
                                onClicked: {
                                    console.log("spell info", index, JSON.stringify(lvSpellList.model[index]));
                                    mainWindow.showSpellDetails(lvSpellList.model[index].cg);
                                }
                            }
                        }

                        MouseArea {
                            id: maSpell
                            anchors.fill: parent
                            onClicked: {
                                console.log("choose spell", index, JSON.stringify(lvSpellList.model[index]));
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: iGesture
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 0.3 * parent.height

            IconInfo {
                id: iiGC
                source: "qrc:/res/g_c.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: parent.left
                anchors.leftMargin: 0.03 * parent.width
                anchors.bottom: parent.verticalCenter
                anchors.bottomMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture("C");
                }
            }

            IconInfo {
                id: iiGD
                source: "qrc:/res/g_d.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: iiGC.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.bottom: parent.verticalCenter
                anchors.bottomMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture("D");
                }
            }

            IconInfo {
                id: iiGF
                source: "qrc:/res/g_f.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: iiGD.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.bottom: parent.verticalCenter
                anchors.bottomMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture("F");
                }
            }

            IconInfo {
                id: iiGS
                source: "qrc:/res/g_s.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: iiGF.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.bottom: parent.verticalCenter
                anchors.bottomMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture("S");
                }
            }

            IconInfo {
                id: iiGP
                source: "qrc:/res/g_p.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: parent.left
                anchors.leftMargin: 0.03 * parent.width
                anchors.top: parent.verticalCenter
                anchors.topMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture("P");
                }
            }

            IconInfo {
                id: iiGW
                source: "qrc:/res/g_w.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: iiGP.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.top: parent.verticalCenter
                anchors.topMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture("W");
                }
            }

            IconInfo {
                id: iiGSt
                source: "qrc:/res/g__.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: iiGW.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.top: parent.verticalCenter
                anchors.topMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture(">");
                }
            }

            IconInfo {
                id: iiGN
                source: "qrc:/res/g_-.png"
                textVisible: false
                height: 0.3 * parent.height
                width: 0.15 * parent.width
                anchors.left: iiGSt.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.top: parent.verticalCenter
                anchors.topMargin: 0.05 * parent.height
                active: true
                color: "snow"
                onClicked: {
                    setGesture("-");
                }
            }

            IconInfo {
                id: iiSend
                source: "qrc:/res/send_0.png"
                textVisible: false
                height: 0.20 * parent.width
                width: 0.20 * parent.width
                anchors.right: parent.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.verticalCenter: parent.verticalCenter
                active: true
                color: "transparent"
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
        initGFields()();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function clearAll() {
        for (var i = 0; i < 8; ++i) {
            arrGesture[i].height = 0.3 * iGesture.height;
            arrGesture[i].color = "snow";
        }
    }

    function setGesture(new_gesture) {
        clearAll()
        mapGesture[new_gesture].height = 0.4 * iGesture.height;
        mapGesture[new_gesture].color = "lightblue";
        currGesture = new_gesture;
        if (new_gesture === "-") {
            lvSpellList.model = [];
        } else if (new_gesture === ">") {
            lvSpellList.model = [{g:">",t:"Stab",cg:">"}];
        } else {
            lvSpellList.model = mainWindow.getSpellList(new_gesture);
        }
    }

    function initGFields() {
        console.log("wnd_gesture.initGFields", JSON.stringify(mainWindow.gERROR));
        //ltError.text = mainWindow.gERROR.text;
        title_text = mainWindow.gERROR.title;
        setGesture(mainWindow.gERROR.g);
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initGFields();
    }
}
