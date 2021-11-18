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
    //bodyAnchors.verticalCenter: undefined
    //bodyAnchors.horizontalCenter: undefined
    //bodyAnchors.right: content_parent.right
    //bodyAnchors.top: content_parent.top
    //bodyAnchors.bottom: content_parent.bottom


    bg_source: "qrc:/res/stars_bg.png"
    overRect.anchors.left: undefined
    overRect.width: 516 * mainWindow.ratioObject
    overRect.gradient: Gradient {
        GradientStop { position: 0.0; color: "#210430" }
        GradientStop { position: 1.0; color: "#0654C0" }
    }

    property string currGesture: ""
    property var arrGesture: [iiGC,iiGD,iiGF,iiGS,iiGP,iiGW,iiGSt,iiGN]
    property var arrGestureVal: ["C","D","F","S","P","W",">","-"]
    property var mapGesture: ({"C":iiGC,"D":iiGD,"F":iiGF,"S":iiGS,"P":iiGP,"W":iiGW,">":iiGSt,"-":iiGN})
    property var arrPossibleGesture: []
    //property var arrSpellFull: []
    property var arrSpell: []
    //property var arrSpellLater: []

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

        Rectangle {
            id: rpAlert
            anchors.top: parent.top
            anchors.topMargin: 60 * mainWindow.ratioObject
            anchors.left: iGlow.right
            anchors.leftMargin: 12 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 12 * mainWindow.ratioObject
            height: 0
            border.width: 1 * mainWindow.ratioObject
            border.color: "#10C9F5"
            color: "transparent"

            Text {
                id: rpAlertH1
                anchors.top: parent.top
                anchors.topMargin: 10 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 49 * ratioFont
                color: "#10C9F5"
                text: "Surrendering!"
            }

            Text {
                id: rpAlertH3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10 * mainWindow.ratioObject
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 28 * ratioFont
                color: "#10C9F5"
                text: "('P' with both hands)"
            }
        }

        ScrollView {
            id: svError
            anchors.top: rpAlert.bottom
            anchors.topMargin: 60 * mainWindow.ratioObject
            anchors.left: iGlow.right
            anchors.leftMargin: 12 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 12 * mainWindow.ratioObject
            anchors.bottom: iControls.top

            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded

            contentWidth: -1
            contentHeight: lvSpellList.model.length * 78 * mainWindow.ratioObject

            ListView {
                id: lvSpellList
                model: []
                anchors.fill: svError

                DelegateChooser {
                    id: chooser
                    role: "row_type"

                    DelegateChoice {
                        roleValue: "1"
                        delegate: Item {
                                id: idRoot
                                width: lvSpellList.width
                                height: 78 * mainWindow.ratioObject

                                Rectangle {
                                    id: rdSpellItem
                                    color: "transparent"
                                    radius: 30
                                    anchors.fill: parent
                                    //anchors.bottomMargin: 0.01 * dialogWindow.height
                                    //border.color: "#A8F4F4"
                                    //border.width: lvSpellList.model[index].choose === 1 ? (2 * mainWindow.ratioObject) : 0

                                    LargeText {
                                        id: rdbiGesture
                                        anchors.verticalCenter: rdSpellItem.verticalCenter
                                        anchors.left: rdSpellItem.left
                                        anchors.leftMargin: 12 * mainWindow.ratioObject
                                        width: 0.40 * parent.width
                                        height: 68 * mainWindow.ratioObject
                                        //anchors.rightMargin: 0.05 * parent.width
                                        color: "snow"
                                        horizontalAlignment: Text.AlignLeft
                                        text: lvSpellList.model[index].gp
                                        font.underline: lvSpellList.model[index].choose === 1
                                    }

                                    LargeText {
                                        id: rdbifTitle
                                        anchors.verticalCenter: rdSpellItem.verticalCenter
                                        anchors.right: rdSpellItem.right
                                        anchors.rightMargin: 12 * mainWindow.ratioObject
                                        height: 50 * mainWindow.ratioObject
                                        width: 0.55 * parent.width
                                        color: "snow"//lvSpellList.model[index].basic ? "snow" : "darkgrey"
                                        fontSizeMode: Text.VerticalFit
                                        horizontalAlignment: Text.AlignRight
                                        text: lvSpellList.model[index].n
                                    }

                                    MouseArea {
                                        id: maSpell
                                        anchors.fill: parent
                                        onClicked: {
                                            console.log("choose spell", mainWindow.gBattle.spellIdx, index, JSON.stringify(lvSpellList.model[index]), JSON.stringify(arrSpell[index]));
                                            if ((currGesture === "") && (lvSpellList.model[index].gp !== "?")) {
                                                mainWindow.showSpellDetails(lvSpellList.model[index].g);
                                            } else if ((arrSpell[index].cast_type === 1) && (arrSpell[index].choose !== 1)) {
                                                arrSpell[index].choose = 1;
                                                arrSpell[index].need_target = true;
                                                arrSpell[mainWindow.gBattle.spellIdx].choose = 0;
                                                mainWindow.gBattle.spellIdx = index;
                                                lvSpellList.model = arrSpell;
                                            } else if (arrSpell[index].cast_type === 2) {
                                                mainWindow.setGesture(currGesture, {gp:"?",n:"Default",choose:1,t:1,cast_type:1,need_target:true}, true);
                                            } else if (arrSpell[index].cast_type === 3) {
                                                setGesture(arrSpell[index].ng);
                                            }
                                        }

                                        onPressAndHold: {
                                            console.log("spell info", index, JSON.stringify(lvSpellList.model[index]));
                                            if(lvSpellList.model[index].gp !== "?") {
                                                mainWindow.showSpellDetails(lvSpellList.model[index].g);
                                            } else {

                                            }
                                        }
                                    }
                            }
                        }
                    }

                    DelegateChoice {
                        roleValue: "2"
                        delegate: Item {
                                id: idRoot2
                                width: lvSpellList.width
                                height: 78 * mainWindow.ratioObject

                                Text {
                                    id: idRoot2Title
                                    anchors.verticalCenter: idRoot2.verticalCenter
                                    anchors.left: parent.left//rdbifInfo.left
                                    anchors.leftMargin: 0.03 * parent.width
                                    height: 0.8 * idRoot2.height
                                    font.pixelSize: 28 * mainWindow.ratioFont
                                    color: "#0654C0"
                                    //fontSizeMode: Text.VerticalFit
                                    horizontalAlignment: Text.AlignRight
                                    text: lvSpellList.model[index].n
                                }
                            }
                        }

                    DelegateChoice {
                        roleValue: "3"
                        delegate: Item {
                                id: idRoot3
                                width: lvSpellList.width
                                height: 78 * mainWindow.ratioObject

                                Text {
                                    id: idRoot3Title
                                    anchors.verticalCenter: idRoot3.verticalCenter
                                    anchors.left: parent.left//rdbifInfo.left
                                    anchors.leftMargin: 0.03 * parent.width
                                    height: 0.8 * idRoot3.height
                                    font.pixelSize: 35 * mainWindow.ratioFont
                                    color: "#0654C0"
                                    //fontSizeMode: Text.VerticalFit
                                    horizontalAlignment: Text.AlignRight
                                    text: lvSpellList.model[index].n
                                }
                            }
                        }
                }

                delegate: chooser
            }
        }

        Rectangle {
            id: iControls
            anchors.bottom: parent.bottom
            anchors.left: iGlow.right
            anchors.right: parent.right
            height: 300 * mainWindow.ratioObject
            color: "#210430"

            Rectangle {
                id: rLine
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 12 * mainWindow.ratioObject
                color: "#551470"
            }

            Item {
                id: iGesture
                anchors.left: parent.left
                anchors.leftMargin: 30 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: rLine.bottom

                Gradient {
                    id: gActiveButton
                    GradientStop { position: 0.00; color: "#E7FFFF" }
                    GradientStop { position: 0.25; color: "#E7FFFF" }
                    GradientStop { position: 1.00; color: "#FEE2D6" }
                }

                IconInfo {
                    id: iiGC
                    source: "qrc:/res/g_c.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: parent.left
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    x: (12 + 0 * (60 + 24)) * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.bottomMargin: 12 * mainWindow.ratioObject
                    anchors.bottom: parent.verticalCenter
                    active: true
                    //color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture("C");
                    }
                }

                IconInfo {
                    id: iiGD
                    source: "qrc:/res/g_d.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: iiGC.right
                    //anchors.left: parent.left
                    //anchors.leftMargin: (12 + 60 + 24) * mainWindow.ratioObject
                    x: (12 + 1 * (60 + 24)) * mainWindow.ratioObject
                    anchors.bottom: parent.verticalCenter
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.bottomMargin: 12 * mainWindow.ratioObject
                    active: true
                    color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture("D");
                    }
                }

                IconInfo {
                    id: iiGF
                    source: "qrc:/res/g_f.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: iiGD.right
                    //anchors.left: parent.left
                    //anchors.leftMargin: (12 + 2 * (60 + 24)) * mainWindow.ratioObject
                    x: (12 + 2 * (60 + 24)) * mainWindow.ratioObject
                    anchors.bottom: parent.verticalCenter
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.bottomMargin: 12 * mainWindow.ratioObject
                    active: true
                    color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture("F");
                    }
                }

                IconInfo {
                    id: iiGS
                    source: "qrc:/res/g_s.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: iiGF.right
                    //anchors.left: parent.left
                    //anchors.leftMargin: (12 + 3 * (60 + 24)) * mainWindow.ratioObject
                    x: (12 + 3 * (60 + 24)) * mainWindow.ratioObject
                    anchors.bottom: parent.verticalCenter
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.bottomMargin: 12 * mainWindow.ratioObject
                    active: true
                    color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture("S");
                    }
                }

                IconInfo {
                    id: iiGP
                    source: "qrc:/res/g_p.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: parent.left
                    anchors.top: parent.verticalCenter
                    //anchors.leftMargin: (12 + 0 * (60 + 24)) * mainWindow.ratioObject
                    x: (12 + 0 * (60 + 24)) * mainWindow.ratioObject
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    active: true
                    color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture("P");
                    }
                }

                IconInfo {
                    id: iiGW
                    source: "qrc:/res/g_w.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: iiGP.right
                    //anchors.left: parent.left
                    //anchors.leftMargin: (12 + 1 * (60 + 24)) * mainWindow.ratioObject
                    x: (12 + 1 * (60 + 24)) * mainWindow.ratioObject
                    anchors.top: parent.verticalCenter
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    active: true
                    color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture("W");
                    }
                }

                IconInfo {
                    id: iiGSt
                    source: "qrc:/res/g__.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: iiGW.right
                    //anchors.left: parent.left
                    //anchors.leftMargin: (12 + 2 * (60 + 24)) * mainWindow.ratioObject
                    x: (12 + 2 * (60 + 24)) * mainWindow.ratioObject
                    anchors.top: parent.verticalCenter
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    active: true
                    color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture(">");
                    }
                }

                IconInfo {
                    id: iiGN
                    source: "qrc:/res/g_-.png"
                    textVisible: false
                    height: 78 * mainWindow.ratioObject
                    width: 60 * mainWindow.ratioObject
                    //anchors.left: iiGSt.right
                    //anchors.left: parent.left
                    //anchors.leftMargin: (12 + 3 * (60 + 24)) * mainWindow.ratioObject
                    x: (12 + 3 * (60 + 24)) * mainWindow.ratioObject
                    anchors.top: parent.verticalCenter
                    //anchors.leftMargin: 12 * mainWindow.ratioObject
                    //anchors.rightMargin: 12 * mainWindow.ratioObject
                    anchors.topMargin: 12 * mainWindow.ratioObject
                    active: true
                    color: "snow"
                    gradient: gActiveButton
                    iconHeight: 50 * mainWindow.ratioObject
                    iconWidth: 50 * mainWindow.ratioObject
                    radius: 20 * mainWindow.ratioObject

                    animationEnabled: false
                    onClicked: {
                        setGesture("-");
                    }
                }

                IconInfo {
                    id: iiSend
                    source: "qrc:/res/send_"+(active ? "1" : "0")+".png"
                    textVisible: false
                    height: 64 * mainWindow.ratioObject
                    width: 64 * mainWindow.ratioObject
                    anchors.right: parent.right
                    anchors.rightMargin: 62 * mainWindow.ratioObject
                    anchors.verticalCenter: parent.verticalCenter
                    active: false
                    color: "transparent"

                    onClicked: {
                        var spell = arrSpell[mainWindow.gBattle.spellIdx], s;
                        var need_targeting = (spell.n === "Stab") || (mainWindow.gBattle.spellIdx !== 0);
                        if (!need_targeting) {
                            for(var i = 0, Ln = arrSpell.length; i < Ln; ++i) {
                                s = arrSpell[i];
                                if ((s.ng === currGesture) && (s.t === 1) && (s.dt !== 0)) {
                                    need_targeting = true;
                                    break;
                                }
                            }
                        }

                        mainWindow.setGesture(currGesture, arrSpell[mainWindow.gBattle.spellIdx], need_targeting);
                    }

                    onDoubleClicked: {
                        mainWindow.setGesture(currGesture, arrSpell[mainWindow.gBattle.spellIdx], true);
                    }
                }

                /*IconInfo {
                    id: iiSendTarget
                    source: "qrc:/res/send_"+(active ? "1" : "0")+"_target.png"
                    textVisible: false
                    height: 0.4 * parent.height
                    width: 0.20 * parent.width
                    anchors.right: parent.right
                    anchors.leftMargin: 0.03 * parent.width
                    anchors.top: parent.verticalCenter
                    anchors.topMargin: 0.05 * parent.height
                    active: false
                    color: "transparent"

                    onClicked: {
                        mainWindow.setGesture(currGesture, arrSpell[mainWindow.gBattle.spellIdx], true);
                    }
                }*/

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
        currGesture = "";
        arrPossibleGesture = [];
        arrSpell = [];
        //arrSpellFull = [];
        initGFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function clearAll() {
        var set_enable = arrPossibleGesture.length > 0;
        for (var i = 0; i < 8; ++i) {
            arrGesture[i].height = 78 * mainWindow.ratioObject;
            arrGesture[i].width = 60 * mainWindow.ratioObject;
            arrGesture[i].iconHeight =  50 * mainWindow.ratioObject
            arrGesture[i].iconWidth = 50 * mainWindow.ratioObject
            if (arrGesture[i].checked) {
                arrGesture[i].checked = false;
                arrGesture[i].x += 6 * mainWindow.ratioObject;
            }
            //arrGesture[i].anchors.leftMargin = 12 * mainWindow.ratioObject;
            //arrGesture[i].anchors.rightMargin = 12 * mainWindow.ratioObject;
            if (set_enable && ((i < 6) || (arrPossibleGesture.length === 1))) {
                arrGesture[i].active = arrPossibleGesture.indexOf(arrGestureVal[i]) !== -1;
            } else {
                arrGesture[i].active = true;
            }

            if (arrGesture[i].active) {
                arrGesture[i].color = "snow";
                arrGesture[i].gradient = gActiveButton;
            } else {
                arrGesture[i].color = "#A8F4F4";
                arrGesture[i].gradient = undefined;
            }
        }
    }

    function setGesture(new_gesture) {
        clearAll()
        if (new_gesture === currGesture) {
            currGesture = "";
            new_gesture = "";
        }
        if (new_gesture !== "") {
            iiSend.active = true;
            //iiSendTarget.active = true;
            mapGesture[new_gesture].height = 90 * mainWindow.ratioObject;
            mapGesture[new_gesture].width = 72 * mainWindow.ratioObject;
            mapGesture[new_gesture].iconHeight =  70 * mainWindow.ratioObject;
            mapGesture[new_gesture].iconWidth = 70 * mainWindow.ratioObject
            mapGesture[new_gesture].x -= 6 * mainWindow.ratioObject;
            //mapGesture[new_gesture].anchors.leftMargin = 6 * mainWindow.ratioObject;
            //mapGesture[new_gesture].anchors.rightMargin = 6 * mainWindow.ratioObject;
            mapGesture[new_gesture].color = "lightblue";
            mapGesture[new_gesture].gradient = undefined;
            mapGesture[new_gesture].checked = true;
            currGesture = new_gesture;
        }
        if ((new_gesture === "P") && (mainWindow.gBattle["ng" + mainWindow.gBattle.otherHand] === "P")) {
            rpAlert.height = rpAlertH1.height + rpAlertH3.height + 30 * mainWindow.ratioObject;
            rpAlert.visible = true;
        } else {
            rpAlert.height = 0;
            rpAlert.visible = false;
        }

        if (new_gesture === "-") {
            mainWindow.gBattle.spellIdx = 0;
            arrSpell = [{gp:"?",n:"Default",t:1,choose:1,row_type:1}];
            //arrSpellLater = [];
        } else if (new_gesture === ">") {
            mainWindow.gBattle.spellIdx = 0;
            arrSpell = [{gp:">",n:"Stab",t:1,choose:1,row_type:1}];
            //arrSpellLater = [];
        } else {
            arrSpell = mainWindow.getSpellList(new_gesture);
        }/*if (new_gesture === "") {
            if (arrSpellFull.length === 0) {
                arrSpellFull = mainWindow.getSpellList("");
                lvSpellList.model = arrSpellFull;
            }
            //svError.contentHeight = iSpellBook.height;
            iCast.visible = false;
            iSpellBook.visible = true;
            return;
        } else {
            var tmp_arr = mainWindow.getSpellList(new_gesture);
            arrSpell = tmp_arr[0];
            arrSpellLater = tmp_arr[1];
        }*/
        //lvCast.model = arrSpell;
        //lvCastNext.model = arrSpellLater;
        //svError.contentHeight = iCast.height;
        //iSpellBook.visible = false;
        //iCast.visible = true;
        console.log("FINAL ARRAY", JSON.stringify(arrSpell));
        lvSpellList.model = arrSpell;
    }

    function initGFields() {
        console.log("wnd_gesture.initGFields", JSON.stringify(mainWindow.gERROR));
        iiSend.active = false;
        //iiSendTarget.active = false;
        //title_text = mainWindow.gERROR.title;
        arrPossibleGesture = mainWindow.gERROR.pga;
        setGesture(arrPossibleGesture.length === 1 ? arrPossibleGesture[0] : mainWindow.gERROR.g);
        mainWindow.gERROR = {};
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initGFields();
    }
}
