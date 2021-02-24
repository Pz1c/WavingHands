import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/components"

Item {
    id: rWarlock

    property var l_warlock: ({})
    property var l_IconInfoObj: ({})
    property var l_paralyzeActionData: ({})
    property var l_charmActionData: ({})
    property string l_control_icon: ""

    signal clicked(var data)
    signal doubleClicked(var data)

    LargeText {
       id: ltName
       text: l_warlock.name
       anchors.top: parent.top
       anchors.horizontalCenter: parent.horizontalCenter
       height: 0.1 * parent.height
       width: 0.3 * parent.width
       bg_visible: true
       bg_color: l_warlock.player ? "#210430" : "#544653"
       color: l_warlock.player ? "#A8F4F4" : "#FEE2D6"
       border_visible: false
       radius: 10
    }

    IconInfo {
        id: iiHP
        source: "qrc:/res/heart.png";
        text: l_warlock.hp
        height: 0.10 * parent.height
        width: height
        anchors.top: parent.top
        //anchors.topMargin: 0.01 * parent.height
        anchors.left: parent.left
        onClicked: {
            iconClick({action:"hp",value:text});
        }

        onDoubleClicked: {
            iconDoubleClick({action:"hp"});
        }
    }

    IconInfo {
        id: iiBanked
        source: "qrc:/res/banked_spell.png";
        text: ""
        height: 0.10 * parent.height
        width: height
        anchors.top: parent.top
        //anchors.topMargin: 0.01 * parent.height
        anchors.right: parent.right
        visible: l_warlock.player && (l_warlock.banked_spell !== "")
        checkbox: true
        checked: false
        l_data: ({action:"banked",value:l_warlock.banked_spell,checked:false})

        onClicked: {
            iconClick(l_data);
        }

        onDoubleClicked: {
            iconDoubleClick(l_data);
        }
    }


    ScrollView {
        id: svMonsters
        anchors.top: iiHP.bottom
        anchors.topMargin: 0.01 * parent.height
        anchors.left: parent.left
        width: 0.5 * parent.width
        height: 0.15 * parent.height
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        Item {
            id: iMonsters
            anchors.fill: parent
        }
    }

    ScrollView {
        id: svCharm
        anchors.top: iiHP.bottom
        anchors.topMargin: 0.01 * parent.height
        anchors.right: parent.right
        width: 0.5 * parent.width
        height: 0.15 * parent.height
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        Item {
            id: iCharm
            anchors.fill: parent
        }
    }

    ScrollView {
        id: svGestures
        anchors.bottom: rBottomLine.top
        anchors.left: parent.left
        anchors.right: iiLeft.left
        height: 0.73 * rWarlock.height
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

        ListView {
            id: lwGestures
            anchors.fill: parent
            orientation: ListView.Horizontal
            model: l_warlock.print_g
            delegate: Item {
                id: lwiGesture
                height: svGestures.height
                width: iiLeft.width

                IconInfo {
                    id: iiHL
                    source: "qrc:/res/"+lwGestures.model[index].l+".png"
                    iconVisible: lwGestures.model[index].lv
                    textVisible: false
                    height: 0.3 * rWarlock.height
                    width: parent.width
                    anchors.bottom: iiHR.top
                    anchors.bottomMargin: 0.01 * rWarlock.height
                    anchors.right: parent.right
                    active: false
                    gradient: Gradient {
                            GradientStop { position: 0.0; color: "#E7FFFF" }
                            GradientStop { position: 1.0; color: "#FEE2D6" }
                        }
                }

                IconInfo {
                    id: iiHR
                    source: "qrc:/res/"+lwGestures.model[index].r+".png"
                    iconVisible: lwGestures.model[index].rv
                    textVisible: false
                    height: 0.3 * rWarlock.height
                    width: parent.width
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0.05 * rWarlock.height
                    anchors.right: parent.right
                    active: false
                    gradient: Gradient {
                            GradientStop { position: 0.0; color: "#E7FFFF" }
                            GradientStop { position: 1.0; color: "#FEE2D6" }
                        }
                }
            }
        }
    }

    IconInfoR {
        id: iiLeft
        source: l_warlock.player ? "qrc:/res/stars_light.png" : "qrc:/res/g_=.png";
        text: ""
        height: 0.35 * parent.height
        width: 0.7 * height
        anchors.bottom: iiRight.top
        anchors.bottomMargin: 0.01 * parent.height
        anchors.right: parent.right
        visible: false

        onClicked: {
            if (l_warlock.player) {
                mainWindow.showGesture(true, l_warlock.plg);
            } else if (l_warlock.control_paralyze) {
                l_paralyzeActionData.hand = "LH";
                iconClick(l_paralyzeActionData);
            } else if (l_warlock.control_charmed) {
                l_charmActionData.hand = "LH";
                iconClick(l_charmActionData);
            }
        }

        onDoubleClicked: {
            if (l_warlock.player) {
                mainWindow.chooseTargetForSpell(true);
            }
        }
    }

    IconInfoR {
        id: iiRight
        source: "qrc:/res/stars_light.png";
        text: ""
        height: iiLeft.height
        width: iiLeft.width
        anchors.bottom: rBottomLine.top
        //anchors.bottomMargin: 0.02 * parent.height
        anchors.right: parent.right
        visible: false
        onClicked: {
            if (l_warlock.player) {
                mainWindow.showGesture(false, l_warlock.prg);
            } else if (l_warlock.control_paralyze) {
                l_paralyzeActionData.hand = "RH";
                iconClick(l_paralyzeActionData);
            } else if (l_warlock.control_charmed) {
                l_charmActionData.hand = "RH";
                iconClick(l_charmActionData);
            }
        }

        onDoubleClicked: {
            if (l_warlock.player) {
                mainWindow.chooseTargetForSpell(false);
            }
        }
    }

    Rectangle {
        id: rBottomLine
        height: 0.03 * parent.height
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        color: "#FEE2D6"
    }

    Component.onCompleted: finishWarlockCreation();

    function targetingOnOff(Enable, IsSpell) {
        console.log("Warlock.targetingOnOff", l_warlock.name, Enable);
        var opacity = Enable ? 0.3 : 1;
        var border_width = Enable ? 3 : 0;
        lwGestures.opacity = opacity;
        iiLeft.opacity = opacity;
        iiRight.opacity = opacity;
        //iiHP.active = Enable;
        iiHP.border.width = border_width;
        if (iiBanked.visible) {
            iiBanked.opacity = opacity;
        }
        var item, i, Ln;
        for (i = 0, Ln = iMonsters.children.length; i < Ln; ++i) {
            item = iMonsters.children[i];
            item.border.width = border_width;
            //item.active = Enable;
        }
        for (i = 0, Ln = iCharm.children.length; i < Ln; ++i) {
            item = iCharm.children[i];
            if (IsSpell && l_warlock.player && ((item.l_data.action === "permanency") || (item.l_data.action === "delay"))) {
                item.animate(Enable ? 0 : -1);
            } else {
                item.opacity = opacity;
                //item.active = !Enable;
            }
        }
    }

    function setGesture(Hand, GestureIcon) {
        console.log("Warlock.setGesture", Hand, GestureIcon);
        if (Hand === "L") {
            iiLeft.source = "qrc:/res/" + GestureIcon + ".png";
            iiLeft.color = "lightblue";
            if (!l_warlock.player) {
                iiRight.source = l_control_icon;
                iiRight.color = "transparent";
            }
        } else if (Hand === "R") {
            iiRight.source = "qrc:/res/" + GestureIcon + ".png";
            iiRight.color = "lightblue";
            if (!l_warlock.player) {
                iiLeft.source = l_control_icon;
                iiLeft.color = "transparent";
            }
        }
    }

    function iconClick(data) {
        console.log("iconClick", l_warlock.name, JSON.stringify(data));
        data.warlock_name = l_warlock.name;
        data.warlock_idx = l_warlock.warlock_idx;
        clicked(data);
    }

    function iconDoubleClick(data) {
        console.log("iconDoubleClick", l_warlock.name, JSON.stringify(data));
        data.warlock_name = l_warlock.name;
        doubleClicked(data);
    }

    function scrollGestures() {
        if (!svGestures.ScrollBar || !svGestures.ScrollBar.horizontal) {
            return;
        }

        console.log("Warlock.qml.finishWarlockCreation", svGestures.ScrollBar.horizontal.position, svGestures.ScrollBar.horizontal.stepSize);
        var cnt = 0;
        svGestures.ScrollBar.horizontal.stepSize = 0.1;
        while(svGestures.ScrollBar.horizontal.position < 1) {
            svGestures.ScrollBar.horizontal.increase();
            if (++cnt >= 10) {
                break;
            }
        }
        console.log("Warlock.qml.finishWarlockCreation", svGestures.ScrollBar.horizontal.position, cnt);
    }

    function prepareDynamic(type, arr, parent, code_value, start_x, incerment) {
        if (l_IconInfoObj.status === Component.Error || l_IconInfoObj.status !== Component.Ready) {
            console.log("prepareDynamic Error loading component:", l_IconInfoObj.errorString());
            return ;
        }
        var curr_x = start_x;
        var total_width = 0, is_checkbox;
        for(var i = 0, Ln = arr.length; i < Ln; ++i) {
            if (!arr[i]) {
                continue;
            }
            var arr_m = arr[i];
            is_checkbox = false;
            if (type === "s") {
                is_checkbox = (arr_m.action === "permanency") || (arr_m.action === "delay");
                if (arr_m.action === "paralized") {
                    l_paralyzeActionData = arr_m;
                }
                if (arr_m.action === "charmed") {
                    l_charmActionData = arr_m;
                }
            }

            var sprite = l_IconInfoObj.createObject(parent, {l_data: arr_m,x: curr_x, y: 0, height: parent.height, width: parent.height, text: arr_m[code_value], source: "qrc:/res/"+arr_m.icon+".png", checkbox: is_checkbox});
            if (sprite === null) {
                console.log("prepareDynamic Error creating object");
                continue;
            }
            console.log("prepareDynamic looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width, JSON.stringify(arr_m));
            sprite.clicked.connect(rWarlock.iconClick);
            sprite.doubleClicked.connect(rWarlock.iconDoubleClick);

            curr_x += incerment * sprite.width;
            total_width += sprite.width;
        }
    }

    function prepareMonsters() {
        prepareDynamic("m", l_warlock.monsters, iMonsters, "hp", 0, 1);
    }

    function prepareState() {
        prepareDynamic("s", l_warlock.statusIcons, iCharm, "value", iCharm.width - iCharm.height, -1);
    }

    function prepareHands() {
        var hands_visible = l_warlock.player || l_warlock.control_paralyze || l_warlock.control_charmed;
        iiLeft.visible = hands_visible;
        iiRight.visible = hands_visible;
        if (hands_visible && !l_warlock.player) {
           iiLeft.source = l_control_icon;
           iiRight.source = l_control_icon;
        }
    }

    //function setGesture:

    function finishWarlockCreation() {
        console.log("finishWarlockCreation", JSON.stringify(l_warlock));
        scrollGestures();
        prepareMonsters();
        prepareState();
        if (l_warlock.control_paralyze) {
            l_control_icon = "qrc:/res/paralized.png";
        } else if (l_warlock.control_charmed) {
            l_control_icon = "qrc:/res/charmed.png";
        }

        prepareHands();
        iiBanked.visible = l_warlock.player && (l_warlock.banked_spell !== "");
    }
}
