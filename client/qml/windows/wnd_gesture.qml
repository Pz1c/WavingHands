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
    property var arrGestureVal: ["C","D","F","S","P","W",">","-"]
    property var mapGesture: ({"C":iiGC,"D":iiGD,"F":iiGF,"S":iiGS,"P":iiGP,"W":iiGW,">":iiGSt,"-":iiGN})
    property var arrPossibleGesture: []
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
            anchors.bottom: iGesture.top
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
                        border.width: lvSpellList.model[index].choose === 1 ? 3 : 0

                        LargeText {
                            id: rdbiGesture
                            anchors.top: rdSpellItem.top
                            anchors.bottom: rdSpellItem.bottom
                            anchors.left: rdSpellItem.left
                            anchors.leftMargin: 0.03 * parent.width
                            width: 0.40 * parent.width
                            //anchors.rightMargin: 0.05 * parent.width
                            color: "snow"
                            horizontalAlignment: Text.AlignLeft
                            text: lvSpellList.model[index].gp
                        }

                        LargeText {
                            id: rdbifTitle
                            anchors.verticalCenter: rdSpellItem.verticalCenter
                            anchors.left: rdbiGesture.right
                            anchors.leftMargin: 0.01 * parent.width
                            anchors.right: rdbifInfo.left
                            anchors.rightMargin: 0.01 * parent.width
                            height: 0.8 * rdSpellItem.height
                            color: "snow"
                            fontSizeMode: Text.VerticalFit
                            horizontalAlignment: Text.AlignRight
                            text: lvSpellList.model[index].n
                        }

                        IconInfo {
                            id: rdbifInfo
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 0.03 * parent.width
                            height: 0.8 * parent.height
                            width: 0.1 * parent.width
                            source: "qrc:/res/g_=.png";
                            color: "snow"
                            z: 13

                            onClicked: {
                                console.log("spell info", index, JSON.stringify(lvSpellList.model[index]));
                                if(lvSpellList.model[index].gp !== "?") {
                                    mainWindow.showSpellDetails(lvSpellList.model[index].g);
                                } else {

                                }
                            }
                        }

                        MouseArea {
                            id: maSpell
                            anchors.fill: parent
                            onClicked: {
                                console.log("choose spell", mainWindow.gBattle.spellIdx, index, JSON.stringify(lvSpellList.model[index]), JSON.stringify(arrSpell[index]));
                                if ((arrSpell[index].cast_type === 1) && (arrSpell[index].choose !== 1)) {
                                    arrSpell[index].choose = 1;
                                    arrSpell[mainWindow.gBattle.spellIdx].choose = 0;
                                    mainWindow.gBattle.spellIdx = index;
                                    lvSpellList.model = arrSpell;
                                } else if (arrSpell[index].cast_type === 2) {
                                    mainWindow.setGesture(currGesture, {gp:"?",n:"Default",choose:1,t:1,cast_type:1}, true);
                                } else if (arrSpell[index].cast_type === 3) {
                                    setGesture(arrSpell[index].ng);
                                }
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
                    mainWindow.setGesture(currGesture, arrSpell[mainWindow.gBattle.spellIdx]);
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

    function clearAll() {
        var set_enable = arrPossibleGesture.length > 0;
        for (var i = 0; i < 8; ++i) {
            arrGesture[i].height = 0.3 * iGesture.height;
            if (set_enable && (i < 6)) {
                arrGesture[i].active = arrPossibleGesture.indexOf(arrGestureVal[i]) !== -1;
            } else {
                arrGesture[i].active = true;
            }

            if (arrGesture[i].active) {
                arrGesture[i].color = "snow";
            } else {
                arrGesture[i].color = "darksalmon";
            }
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
        if (new_gesture === "-") {
            arrSpell = [{gp:"?",n:"Default",t:1,choose:1}];
        } else if (new_gesture === ">") {
            arrSpell = [{gp:">",n:"Stab",t:1,choose:1}];
        } else {
            arrSpell = mainWindow.getSpellList(new_gesture);
        }
        lvSpellList.model = arrSpell;
    }

    function initGFields() {
        console.log("wnd_gesture.initGFields", JSON.stringify(mainWindow.gERROR));
        iiSend.active = false;
        title_text = mainWindow.gERROR.title;
        arrPossibleGesture = mainWindow.gERROR.pga;
        setGesture(mainWindow.gERROR.g);
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initGFields();
    }
}
