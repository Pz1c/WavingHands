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
        //contentWidth: iMonsters.width
        //contentHeight: iMonsters.height

        ListView {
            id: iMonsters
            orientation: ListView.Horizontal
            model: []
            delegate: IconInfo {
                id: iiDM
                l_data: iMonsters.model[index]
                height: 78 * l_ratio
                width: 78 * l_ratio
                text: iMonsters.model[index].text
                source: "qrc:/res/"+iMonsters.model[index].icon+".png"
                iconInfoSource: iMonsters.model[index].enchantment_icon
                iconInfoVisible: iMonsters.model[index].enchantment_icon !== ""
                checkbox: false//iMonsters.model[index].is_checkbox
                radius: 20
                onClicked: {
                    rWarlock.iconClick(l_data);
                }
                onDoubleClicked: {
                    rWarlock.iconDoubleClick(l_data);
                }
            }
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
        //contentWidth: iCharm.width
        //contentHeight: iCharm.height

        ListView {
            id: iCharm
            orientation: ListView.Horizontal
            layoutDirection: Qt.RightToLeft
            model: []
            delegate: IconInfo {
                id: iiDC
                l_data: iCharm.model[index]
                height: 78 * l_ratio
                width: 78 * l_ratio
                text: iCharm.model[index].text
                source: "qrc:/res/"+iCharm.model[index].icon+".png"
                checkbox: iCharm.model[index].is_checkbox
                radius: 20
                onClicked: {
                    rWarlock.iconClick(l_data);
                }
                onDoubleClicked: {
                    rWarlock.iconDoubleClick(l_data);
                }
            }
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

    IconInfo {
        id: iiLeft
        //source: l_warlock.player ? "qrc:/res/stars_light.png" : "qrc:/res/g_=.png";
        source: "qrc:/res/stars_light.png"
        text: ""
        height: 120 * l_ratio
        width: 90 * l_ratio
        iconHeight: (l_warlock.player || l_warlock.control_paralyze || l_warlock.control_charmed ? 84 : 43) * l_ratio
        iconWidth: (l_warlock.player || l_warlock.control_paralyze || l_warlock.control_charmed ? 84 : 43) * l_ratio
        anchors.bottom: iiRight.top
        anchors.bottomMargin: 18 * l_ratio
        anchors.right: parent.right
        visible: l_warlock.player || (l_warlock.turn_num > 1)
        radius: 20
        border.color: "#E7FFFF"
        border.width: (l_warlock.player ? 3 : 0) * l_ratio
        active: true
        bg_color: "transparent"

        onClicked: {
            if (l_warlock.player) {
                mainWindow.showGesture(true, l_warlock.plg, "normal", l_warlock.warlock_idx);
                mainWindow.logEvent("Play_Left_Click");
            } else if (l_warlock.control_paralyze) {
                //mainWindow.showGesture(true, l_warlock.plg, "paralyze", l_warlock.warlock_idx);
                l_paralyzeActionData.hand = "LH";
                l_paralyzeActionData.plg = l_warlock.plg;
                l_paralyzeActionData.warlock_idx = l_warlock.warlock_idx;
                iconClick(l_paralyzeActionData);
                mainWindow.logEvent("Play_FFF_Choose", {Hand:"Left"});
            } else if (l_warlock.control_charmed) {
                //mainWindow.showGesture(true, l_warlock.plg, "charm", l_warlock.warlock_idx);
                l_charmActionData.hand = "LH";
                l_charmActionData.plg = l_warlock.plg;
                l_charmActionData.warlock_idx = l_warlock.warlock_idx;
                iconClick(l_charmActionData);
                mainWindow.logEvent("Play_PSDF_Choose", {Hand:"Left"});
            } else {
                mainWindow.showGesture(true, l_warlock.plg, "enemy", l_warlock.warlock_idx);
                mainWindow.logEvent("Play_Check_Enemy_Spells", {Hand:"Left"});
            }
        }

        onDoubleClicked: {
            if (l_warlock.player) {
                mainWindow.chooseTargetForSpell(true);
            }
        }
    }

    IconInfo {
        id: iiRight
        source: "qrc:/res/stars_light.png";
        text: ""
        height: iiLeft.height
        width: iiLeft.width
        iconHeight: (l_warlock.player || l_warlock.control_paralyze || l_warlock.control_charmed ? 84 : 43) * l_ratio
        iconWidth: (l_warlock.player || l_warlock.control_paralyze || l_warlock.control_charmed ? 84 : 43) * l_ratio
        anchors.bottom: rBottomLine.top
        anchors.bottomMargin: 16 * l_ratio
        anchors.right: parent.right
        visible: l_warlock.player || (l_warlock.turn_num > 1)
        radius: 20
        border.color: "#E7FFFF"
        border.width: (l_warlock.player ? 3 : 0) * l_ratio
        active: true
        bg_color: "transparent"

        onClicked: {
            if (l_warlock.player) {
                mainWindow.showGesture(false, l_warlock.prg, "normal", l_warlock.warlock_idx);
                mainWindow.logEvent("Play_Right_Click");
            } else if (l_warlock.control_paralyze) {
                //mainWindow.showGesture(false, l_warlock.prg, "paralyze", l_warlock.warlock_idx);
                l_paralyzeActionData.hand = "RH";
                l_paralyzeActionData.plg = l_warlock.plg;
                //l_paralyzeActionData.warlock_idx = l_warlock.warlock_idx;
                iconClick(l_paralyzeActionData);
                mainWindow.logEvent("Play_FFF_Choose", {Hand:"Right"});
            } else if (l_warlock.control_charmed) {
                //mainWindow.showGesture(false, l_warlock.prg, "charm", l_warlock.warlock_idx);
                l_charmActionData.hand = "RH";
                l_charmActionData.plg = l_warlock.plg;
                //l_charmActionData.warlock_idx = l_warlock.warlock_idx;
                iconClick(l_charmActionData);
                mainWindow.logEvent("Play_PSDF_Choose", {Hand:"Right"});
            } else {
                mainWindow.showGesture(false, l_warlock.prg, "enemy", l_warlock.warlock_idx);
                mainWindow.logEvent("Play_Check_Enemy_Spells", {Hand:"Right"});
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

    function showHideSummonIcon(spell) {
        if (!l_warlock.player || !spell) {
            return;
        }
        console.log("showHideSummonIcon", JSON.stringify(spell));
        for (var i = 0, Ln = iMonsters.count; i < Ln; ++i) {
            var item = iMonsters.itemAtIndex(i);
            var name = item.l_data && item.l_data.name ? item.l_data.name : "";
            if (name.indexOf(":") === -1) {
                continue;
            }
            var action, spell_g = spell.g ? spell.g : "";

            if ((spell_g.indexOf("SFW") !== -1) || (spell_g.indexOf("cSWWS") !== -1) || (spell_g.indexOf("cWSSW") !== -1)) {
                action = 1;
            } else {
                action = 2;
            }
            if (!(((name.indexOf("LH:") !== -1) && (spell.h === 1)) || ((name.indexOf("RH:") !== -1) && (spell.h === 2)))) {
                action = 0;
            }
            if (action === 1) {
                item.visible = true;
                item.width = iMonsters.height;
            } else if (action === 2) {
                item.visible = false;
                item.width = 0;
            }
        }
    }

    function hintOnOff(Restore) {
        iiLeft.visible = Restore;
        iiRight.visible = Restore;
    }

    function targetingOnOff(Enable, IsSpell, Permanency, Delay) {
        console.log("Warlock.targetingOnOff", l_warlock.name, Enable, IsSpell, Permanency, Delay);
        var opacity = Enable ? 0.3 : 1;
        var border_width = Enable ? 3 : 0;
        lwGestures.opacity = opacity;
        iiLeft.opacity = opacity;
        iiRight.opacity = opacity;
        iiLeft.active = !Enable;
        iiRight.active = !Enable;
        rBottomLine.visible = l_warlock.player || !Enable;
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
        for (i = 0, Ln = iCharm.count; i < Ln; ++i) {
            item = iCharm.itemAtIndex(i);

            var has_action = item.hasOwnProperty('l_data') && item.l_data.hasOwnProperty('action');
            var is_perm = has_action && (item.l_data.action === "permanency");
            var is_del = has_action && (item.l_data.action === "delay");
            var perm_or_del =  is_perm || is_del;
            var perm_or_del_checked = perm_or_del && item.l_data.hasOwnProperty('checked') && (item.l_data.checked || Enable);
            console.log(JSON.stringify(item.l_data), perm_or_del_checked);

            if (IsSpell && l_warlock.player && perm_or_del_checked) {
                if (Enable) {
                    item.animate(1);
                }
                if (item.checkbox && ((is_perm && !Permanency) || (is_del && !Delay))) {
                    if ((is_perm && Permanency) || (is_del && Delay)) {
                        item.setChecked(true);
                    } else if ((is_perm && !Permanency) || (is_del && !Delay)) {
                        item.setChecked(false);
                    }
                }
                item.active = Enable;// || perm_or_del_checked;

            } else {
                item.opacity = opacity;
                //item.active = !Enable;
            }
        }
    }

    function highlightAction(a) {
        console.log("Warlock.highlightAction", a.object_type, a.object);
        if (a.object_type === "warlock") {
            switch(a.object) {
            case "hp":
                iiHP.color = a.color ? a.color : iiHP.bg_color_checked;
                break;
            case "gestures":
                lwGestures.model = a.data;
                scrollGestures();
                break;
            }
        } else {
            // monster
        }
    }

    function highlightActionRestore(a) {
        console.log("Warlock.highlightActionRestore", a.object_type, a.object);
        if (a.object_type === "warlock") {
            switch(a.object) {
            case "hp":
                iiHP.color = iiHP.bg_color;
                break;
            case "gestures":
                lwGestures.model = l_warlock.print_g;
                scrollGestures();
                break;
            }
        } else {
            // monster
        }
    }

    function setGesture(Hand, GestureIcon, Gesture, Inside) {
        console.log("Warlock.setGesture", Hand, GestureIcon, l_warlock.lgR, l_warlock.lgL, Gesture, l_warlock.control_paralyze, Inside);
        if (Hand === "L") {
            iiLeft.source = "qrc:/res/" + GestureIcon + ".png";
            iiLeft.color = "lightblue";
            if (!l_warlock.player) {
                iiRight.source = l_control_icon;
                iiRight.color = "transparent";
            } else if (!Inside) {
                if (l_warlock.control_paralyze) {
                    l_paralyzeActionData.hand = "RH";
                    l_paralyzeActionData.plg = l_warlock.prg;
                    l_paralyzeActionData.warlock_idx = l_warlock.warlock_idx;
                    l_paralyzeActionData.skip_show_gesture = true;
                    iconClick(l_paralyzeActionData);
                    var par_g = SG.getNextParalyzedGesture(l_warlock.lgR, mainWindow.gBattle.is_fc);
                    mainWindow.setCharm("RH", par_g, true);
                    setGesture("R", SG.getIconByGesture(par_g, true), par_g, true);
                } else if (l_warlock.control_charmed) {
                    l_charmActionData.hand = "LH";
                    l_charmActionData.plg = l_warlock.plg;
                    l_charmActionData.warlock_idx = l_warlock.warlock_idx;
                    l_charmActionData.skip_show_gesture = true;
                    iconClick(l_charmActionData);
                    mainWindow.setCharm("LH", Gesture, true);
                }
            }
        } else if (Hand === "R") {
            iiRight.source = "qrc:/res/" + GestureIcon + ".png";
            iiRight.color = "lightblue";
            if (!l_warlock.player) {
                iiLeft.source = l_control_icon;
                iiLeft.color = "transparent";
            } else if (!Inside) {
                if (l_warlock.control_paralyze) {
                    l_paralyzeActionData.hand = "LH";
                    l_paralyzeActionData.plg = l_warlock.plg;
                    l_paralyzeActionData.warlock_idx = l_warlock.warlock_idx;
                    l_paralyzeActionData.skip_show_gesture = true;
                    iconClick(l_paralyzeActionData);
                    var par_gr = SG.getNextParalyzedGesture(l_warlock.lgL, mainWindow.gBattle.is_fc);
                    mainWindow.setCharm("LH", par_gr, true);
                    setGesture("L", SG.getIconByGesture(par_gr, true), par_gr, true);
                } else if (l_warlock.control_charmed) {
                    l_charmActionData.hand = "RH";
                    l_charmActionData.plg = l_warlock.prg;
                    l_charmActionData.warlock_idx = l_warlock.warlock_idx;
                    l_charmActionData.skip_show_gesture = true;
                    iconClick(l_charmActionData);
                    mainWindow.setCharm("RH", Gesture, true);
                }
            }
        }
    }

    function iconClick(data) {
        console.log("iconClick", l_warlock.name, JSON.stringify(data));
        data.warlock_name = l_warlock.name;
        data.warlock_idx = l_warlock.warlock_idx;
        if (((data.action === "charmed") || (data.action === "paralized")) && !data.hand) {
            iiLeft.animate(1);
            iiRight.animate(1);
        } else {
            clicked(data);
        }
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
        var total_width = 0;
        for(var i = 0, Ln = arr.length; i < Ln; ++i) {
            if (!arr[i]) {
                continue;
            }
            var arr_m = arr[i];
            arr_m.is_checkbox = false;
            arr_m.text = arr_m[code_value];
            if (type === "s") {
                arr_m.is_checkbox = (arr_m.action === "permanency") || (arr_m.action === "delay");
                if (arr_m.is_checkbox) {
                    arr_m.checked = false;
                }

                if (arr_m.action === "paralized") {
                    l_paralyzeActionData = arr_m;
                }
                if (arr_m.action === "charmed") {
                    l_charmActionData = arr_m;
                }
            }

            /*var sprite = l_IconInfoObj.createObject(parent, {l_data: arr_m,x: curr_x, y: 0, height: parent.height, width: parent.height,
                                                        text: arr_m[code_value],
                                                        source: "qrc:/res/"+arr_m.icon+".png", checkbox: arr_m.is_checkbox, radius: 20});
            if (sprite === null) {
                console.log("prepareDynamic Error creating object");
                continue;
            }
            console.log("prepareDynamic looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width, JSON.stringify(arr_m));
            sprite.clicked.connect(rWarlock.iconClick);
            sprite.doubleClicked.connect(rWarlock.iconDoubleClick);

            curr_x += incerment * (sprite.width + 18 * l_ratio);
            total_width += sprite.width + 18 * l_ratio;
            if (l_warlock.player && (type === "m") && (arr_m.name.indexOf(":") !== -1)) {
                sprite.width = 0;
                sprite.visible = false;
            }*/
        }
        parent.model = arr;

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
        //iiLeft.visible = lh_visible;
        //iiRight.visible = rh_visible;
        if (l_warlock.player) {
            var arr_lg = l_warlock.plg.split(",");
            if (arr_lg.length === 1) {
                l_warlock.pgL = arr_lg[0];
            }
            var arr_rg = l_warlock.prg.split(",");
            if (arr_rg.length === 1) {
                l_warlock.pgR = arr_rg[0];
            }

            if (l_warlock.pgL) {
                setGesture("L", SG.getIconByGesture(l_warlock.pgL, true));
                mainWindow.gBattle.actions["L"].g = l_warlock.pgL;
            }

            if (l_warlock.pgR) {
                setGesture("R", SG.getIconByGesture(l_warlock.pgR, true));
                mainWindow.gBattle.actions["R"].g = l_warlock.pgR;
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
        } else {
            l_control_icon = "qrc:/res/stars_light.png";
        }

        prepareHands();

        if (!l_warlock.player && (l_warlock.fh > 0)) {
            var lh = l_warlock.fh === 1 ? "L" : "R";
            var ng = (l_warlock.charmed > 0) ? "-" : SG.getNextParalyzedGesture(l_warlock["lg"+lh], mainWindow.gBattle.is_fc);
            setGesture(lh, SG.getIconByGesture(ng, true), ng);
        }

        iiBanked.visible = (l_warlock.banked_spell !== "");
        //iiBanked.enabled = iiBanked.visible && l_warlock.player;
    }
}
