import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qml/components"
import "qrc:/js/small_gui_utils.js" as SG

Item {
    id: rWarlock

    property var l_warlock: ({})
    property var l_IconInfoObj: ({})
    property var l_paralyzeActionData: ({})
    property var l_charmActionData: ({})
    property string l_control_icon: ""
    property real l_ratio: 1

    signal clicked(var data)
    signal doubleClicked(var data)

    LargeText {
       id: ltName
       text: l_warlock.name
       anchors.top: parent.top
       anchors.horizontalCenter: parent.horizontalCenter
       height: 36 * l_ratio
       width: 246 * l_ratio
       bg_visible: true
       bg_color: l_warlock.player ? "#210430" : "#544653"
       color: l_warlock.player ? "#A8F4F4" : "#FEE2D6"
       border_visible: false
       radius: 10
    }

    IconInfo {
        id: iiHP
        source: "qrc:/res/heart_small.png";
        text: l_warlock.hp
        height: 78 * l_ratio
        width: height
        anchors.top: parent.top
        anchors.topMargin: 12 * l_ratio
        anchors.left: parent.left
        anchors.leftMargin: 6 * l_ratio
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
        height: 78 * l_ratio
        width: height
        anchors.top: parent.top
        anchors.topMargin: 12 * l_ratio
        anchors.right: parent.right
        anchors.rightMargin: 6 * l_ratio
        visible: l_warlock.banked_spell !== ""
        checkbox: l_warlock.player
        checked: false
        l_data: ({action:"banked",value:l_warlock.banked_spell,checked:false})

        onClicked: {
            if (l_warlock.player) {
                l_data.action = "banked";
            } else {
                l_data.action = "banked_info";
            }

            iconClick(l_data);
        }

        onDoubleClicked: {
            iconDoubleClick(l_data);
        }
    }


    ScrollView {
        id: svMonsters
        anchors.top: iiHP.bottom
        anchors.topMargin: 12 * l_ratio
        anchors.left: parent.left
        width: 0.5 * parent.width
        height: 78 * l_ratio
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        contentWidth: iMonsters.width
        contentHeight: iMonsters.height

        Item {
            id: iMonsters
            //anchors.left: parent.left
            //anchors.top: parent.top
            //anchors.fill: parent
            height: 78 * l_ratio
            x:0
            y:0
        }
    }

    ScrollView {
        id: svCharm
        anchors.top: iiHP.bottom
        anchors.topMargin: 12 * l_ratio
        anchors.right: parent.right
        //anchors.left: svMonsters.right
        width: 0.5 * parent.width
        height: 78 * l_ratio
        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        contentWidth: iCharm.width
        contentHeight: iCharm.height

        Item {
            id: iCharm
            //anchors.right: parent.right
            //anchors.top: parent.top
            //anchors.fill: parent
            height: 78 * l_ratio
            //x:0
            y:0
        }
    }

    ScrollView {
        id: svGestures
        anchors.bottom: rBottomLine.top
        anchors.bottomMargin: 28 * l_ratio
        anchors.left: parent.left
        anchors.right: iiLeft.left
        anchors.rightMargin: 48 * l_ratio
        height: 234 * l_ratio
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
                width: 90 * l_ratio

                IconInfo {
                    id: iiHL
                    source: "qrc:/res/"+lwGestures.model[index].l+".png"
                    iconVisible: lwGestures.model[index].lv
                    textVisible: false
                    height: 108 * l_ratio
                    width: 78 * l_ratio
                    iconHeight: 64 * l_ratio
                    iconWidth: 64 * l_ratio
                    anchors.bottom: iiHR.top
                    anchors.bottomMargin: 18 * l_ratio
                    anchors.left: parent.left
                    active: false
                    radius: 20
                    gradient: Gradient {
                            GradientStop { position: 0.0;  color: lwGestures.model[index].la ? "#E7FFFF" : "#756F79" }
                            GradientStop { position: lwGestures.model[index].la ? 0.25 : 0.15; color: lwGestures.model[index].la ? "#E7FFFF" : "#544653" }
                            GradientStop { position: 1.0;  color: lwGestures.model[index].la ? "#FEE2D6" : "#544653" }
                        }
                }

                IconInfo {
                    id: iiHR
                    source: "qrc:/res/"+lwGestures.model[index].r+".png"
                    iconVisible: lwGestures.model[index].rv
                    textVisible: false
                    height: 108 * l_ratio
                    width: 78 * l_ratio
                    iconHeight: 64 * l_ratio
                    iconWidth: 64 * l_ratio
                    anchors.bottom: parent.bottom
                    //anchors.bottomMargin: 0.05 * rWarlock.height
                    anchors.left: parent.left
                    active: false
                    radius: 20
                    gradient: Gradient {
                        GradientStop { position: 0.0;  color: lwGestures.model[index].ra ? "#E7FFFF" : "#756F79" }
                        GradientStop { position: lwGestures.model[index].ra ? 0.25 : 0.15; color: lwGestures.model[index].ra ? "#E7FFFF" : "#544653" }
                        GradientStop { position: 1.0;  color: lwGestures.model[index].ra ? "#FEE2D6" : "#544653" }
                    }
                }
            }
        }
    }

    IconInfoR {
        id: iiLeft
        source: l_warlock.player ? "qrc:/res/stars_light.png" : "qrc:/res/g_=.png";
        text: ""
        height: 120 * l_ratio
        width: 90 * l_ratio
        iconHeight: 84 * l_ratio
        iconWidth: 84 * l_ratio
        anchors.bottom: iiRight.top
        anchors.bottomMargin: 18 * l_ratio
        anchors.right: parent.right
        visible: false
        radius: 20
        active: true

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
        iconHeight: 84 * l_ratio
        iconWidth: 84 * l_ratio
        anchors.bottom: rBottomLine.top
        anchors.bottomMargin: 16 * l_ratio
        anchors.right: parent.right
        visible: false
        radius: 20
        active: true

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
        height: 2 * l_ratio
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
        iiLeft.active = !Enable;
        iiRight.active = !Enable;
        //iiHP.active = Enable;
        //iiHP.border.width = border_width;
        if (iiBanked.visible) {
            iiBanked.opacity = opacity;
        }
        var item, i, Ln;
        /*for (i = 0, Ln = iMonsters.children.length; i < Ln; ++i) {
            item = iMonsters.children[i];
            item.border.width = border_width;
            //item.active = Enable;
        }*/
        for (i = 0, Ln = iCharm.children.length; i < Ln; ++i) {
            item = iCharm.children[i];
            var perm_or_del = ((item.l_data.action === "permanency") || (item.l_data.action === "delay"));
            if (IsSpell && l_warlock.player && perm_or_del) {
                item.animate(Enable ? 1 : -1);
                item.active = Enable || (perm_or_del && item.l_data.checked);
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

            var sprite = l_IconInfoObj.createObject(parent, {l_data: arr_m,x: curr_x, y: 0, height: parent.height, width: parent.height, text: arr_m[code_value],
                                                        source: "qrc:/res/"+arr_m.icon+".png", checkbox: is_checkbox, radius: 20});
            if (sprite === null) {
                console.log("prepareDynamic Error creating object");
                continue;
            }
            console.log("prepareDynamic looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width, JSON.stringify(arr_m));
            sprite.clicked.connect(rWarlock.iconClick);
            sprite.doubleClicked.connect(rWarlock.iconDoubleClick);

            curr_x += incerment * (sprite.width + 18 * l_ratio);
            total_width += sprite.width + 18 * l_ratio;
        }
        parent.width = total_width;
        //parent.height = total_height;
        //parent.parent.contentWidth = total_width;
        //console.log("prepareDynamic", total_width, parent.width, parent.parent.contentWidth, parent.parent.width);
    }

    function prepareMonsters() {
        prepareDynamic("m", l_warlock.monsters, iMonsters, "hp", 0, 1);
    }

    function prepareState() {
        prepareDynamic("s", l_warlock.statusIcons, iCharm, "value", svCharm.width - svCharm.height, -1);
    }

    function prepareHands() {
        var hands_visible = l_warlock.player || l_warlock.control_paralyze || l_warlock.control_charmed;
        var lh_visible = hands_visible || (l_warlock.paralyzed_hand === "LH");
        var rh_visible = hands_visible || (l_warlock.paralyzed_hand === "RH");
        var ng;
        iiLeft.visible = lh_visible;
        iiRight.visible = rh_visible;
        if (l_warlock.player) {
            if (l_warlock.pgL) {
                setGesture("L", SG.getIconByGesture(l_warlock.pgL, true));
            }
            if (l_warlock.pgR) {
                setGesture("R", SG.getIconByGesture(l_warlock.pgR, true));
            }
        } else {
            if (hands_visible) {
               iiLeft.source = l_control_icon;
               iiRight.source = l_control_icon;
            } else if (lh_visible) {
                ng = SG.getNextParalyzedGesture(l_warlock.lgL, mainWindow.gBattle.is_fc);
                iiLeft.source = SG.getFullIconPathByGesture(ng);
                iiLeft.color = "lightblue";
            } else if (rh_visible) {
                ng = SG.getNextParalyzedGesture(l_warlock.lgR, mainWindow.gBattle.is_fc);
                iiRight.source = SG.getFullIconPathByGesture(ng);
                iiRight.color = "lightblue";
            }
        }
    }

    //function setGesture:
    function setMonsterWidth() {
        var m_w = iMonsters.width;
        var c_w = iCharm.width;
        var t_w = rWarlock.width;
        var t_w_h = 0.5 * t_w;
        console.log("setMonsterWidth", l_warlock.name, m_w, c_w, t_w, t_w_h);
        if ((m_w < t_w_h) && (c_w < t_w_h)) {
            console.log("setMonsterWidth", l_warlock.name, "point1");
            return;
        }
        if ((m_w + c_w <= t_w) || ((m_w < t_w_h))) {
            svMonsters.width = m_w;
            svCharm.width = t_w - m_w;
            console.log("setMonsterWidth", l_warlock.name, "point2");
            return;
        }
        if (c_w < t_w_h) {
            svMonsters.width = t_w - c_w;
            svCharm.width = c_w;
            console.log("setMonsterWidth", l_warlock.name, "point3");
            return;
        }
    }

    function finishWarlockCreation() {
        console.log("finishWarlockCreation", JSON.stringify(l_warlock));
        scrollGestures();
        prepareMonsters();
        prepareState();
        setMonsterWidth();

        if (l_warlock.control_paralyze) {
            l_control_icon = "qrc:/res/paralized.png";
        } else if (l_warlock.control_charmed) {
            l_control_icon = "qrc:/res/charmed.png";
        }

        prepareHands();

        if (!l_warlock.player && (l_warlock.fh > 0)) {
            var lh = l_warlock.fh === 1 ? "L" : "R";
            var ng = (l_warlock.charmed > 0) ? "-" : SG.getNextParalyzedGesture(l_warlock["lg"+lh], mainWindow.gBattle.is_fc);
            setGesture(lh, SG.getIconByGesture(ng, true));
        }

        iiBanked.visible = (l_warlock.banked_spell !== "");
        //iiBanked.enabled = iiBanked.visible && l_warlock.player;
    }
}
