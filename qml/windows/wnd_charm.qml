// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"
import "qrc:/js/small_gui_utils.js" as SG

BaseWindow {
    id: dMainItem

    with_controls: false
    with_apply: true
    body_height_prc: 70
    control_height_prc: 0

    property bool isParalyze: false
    property bool isFC: false
    property var arrHands: [{h:"LH",choose:1},{h:"RH",choose:0}]
    property int currHand: 0
    property string currGesture: "-"
    property var arrGesture: [iiGC,iiGD,iiGF,iiGS,iiGP,iiGW,iiGSt,iiGN]
    property var arrGestureVal: ["C","D","F","S","P","W",">","-"]
    property var mapGesture: ({"C":iiGC,"D":iiGD,"F":iiGF,"S":iiGS,"P":iiGP,"W":iiGW,">":iiGSt,"-":iiGN})
    property var arrPossibleGesture: []
    property var lastGesture: ({"LH":"","RH":""})

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
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            ListView {
                id: lvHandList
                model: arrHands
                anchors.fill: parent

                delegate: Item {
                        id: idRoot
                        width: lvHandList.width
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
                        border.width: lvHandList.model[index].choose === 1 ? 3 : 0

                        LargeText {
                            id: rdbiGesture
                            anchors.centerIn: rdSpellItem
                            color: "snow"
                            horizontalAlignment: Text.AlignHCenter
                            text: dict.getStringByCode(lvHandList.model[index].h)
                        }

                        MouseArea {
                            id: maSpell
                            anchors.fill: parent
                            onClicked: {
                                console.log("choose hand", index, JSON.stringify(lvHandList.model[index]), JSON.stringify(arrHands[index]));
                                setHand(lvHandList.model[index].h);
                                /*arrHands[currHand].choose = 0;
                                arrHands[index].choose = 1;
                                currHand = index;
                                lvHandList.model = arrHands;*/
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
            height: 0.3 * dialogWindow.height

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
                animationEnabled: false
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
                animationEnabled: false
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
                animationEnabled: false
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
                animationEnabled: false
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
                animationEnabled: false
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
                animationEnabled: false
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
                animationEnabled: false
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
                animationEnabled: false
                onClicked: {
                    setGesture("-");
                }
            }

            IconInfo {
                id: iiSend
                source: "qrc:/res/send_"+(active ? "1" : "0")+".png"
                textVisible: false
                height: 0.20 * parent.width
                width: 0.20 * parent.width
                anchors.right: parent.right
                anchors.leftMargin: 0.03 * parent.width
                anchors.verticalCenter: parent.verticalCenter
                active: false
                color: "transparent"

                onClicked: {
                    console.log("set hand");
                    mainWindow.setCharm(arrHands[currHand].h, currGesture);
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
        initChFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function clearAll() {
        var set_enable = !isParalyze;
        for (var i = 0; i < 8; ++i) {
            arrGesture[i].height = 0.3 * iGesture.height;
            arrGesture[i].active = set_enable;
            if (arrGesture[i].active) {
                arrGesture[i].color = "snow";
            } else {
                arrGesture[i].color = "darksalmon";
            }
        }
    }

    function setHand(new_hand, force) {
        if (!new_hand || (new_hand === "")) {
            return;
        }
        if ((arrHands[currHand].h === new_hand) && !force) {
            return;
        }
        arrHands[currHand].choose = 0;
        currHand = new_hand === "LH" ? 0 : 1;
        arrHands[currHand].choose = 1;
        lvHandList.model = arrHands;

        if (isParalyze) {
            var ng = SG.getNextParalyzedGesture(lastGesture[new_hand], isFC);
            console.log("wnd_charm.setHand", ng, new_hand, JSON.stringify(arrHands[currHand]), JSON.stringify(paraMap), JSON.stringify(lastGesture))
            setGesture(ng);
        }
    }

    function setGesture(new_gesture) {
        clearAll()
        if (new_gesture !== "") {
            iiSend.active = true;
            mapGesture[new_gesture].height = 0.12 * dialogWindow.height;
            mapGesture[new_gesture].color = "lightblue";
            currGesture = new_gesture;
        }
    }

    function initChFields() {
        console.log("wnd_charm.initChFields", JSON.stringify(mainWindow.gERROR));
        title_text = mainWindow.gERROR.title;
        arrPossibleGesture = [];
        isParalyze = mainWindow.gERROR.is_paralyze;
        isFC = mainWindow.gBattle.is_fc;
        if (isParalyze) {
            lastGesture["LH"] = mainWindow.gERROR.lgL;
            lastGesture["RH"] = mainWindow.gERROR.lgR;
        }
        setHand(mainWindow.gERROR.h, true);
        if (!isParalyze) {
            setGesture(mainWindow.gERROR.g);
        }
        iiSend.active = true;
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initChFields();
    }
}
