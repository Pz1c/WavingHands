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
    property int l_damageSeq: 0
    // The turn news starts at the life points from the previous turn and applies
    // every hit and cure as its cause row appears.
    property int l_hpDisplay: l_warlock.turn_news_hp_start !== undefined ? l_warlock.turn_news_hp_start : l_warlock.hp
    property bool l_dead: (l_warlock.turn_news_dead !== undefined)
                          ? l_warlock.turn_news_dead
                          : ((parseInt(l_warlock.hp, 10) <= 0) &&
                             ((l_warlock.status || "").toLowerCase().indexOf("surrender") === -1))

    signal clicked(var data)
    signal doubleClicked(var data)

    ListModel {
        id: lmDamageNumbers
    }

    // Red damage and green healing numbers are created here and float away from
    // the right side of the target icon.
    Repeater {
        model: lmDamageNumbers
        delegate: Text {
            id: tDamageNumber
            property int numberId: model.number_id
            property real startY: model.start_y
            x: model.start_x
            y: startY
            text: model.text
            color: model.color
            font.pixelSize: 32 * mainWindow.ratioFont
            font.bold: true
            style: Text.Outline
            styleColor: "#210430"
            opacity: 1
            scale: 0.8
            z: 2000

            ParallelAnimation {
                running: true
                NumberAnimation {
                    target: tDamageNumber
                    property: "y"
                    to: tDamageNumber.startY - 40 * mainWindow.ratioObject
                    duration: 900
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    target: tDamageNumber
                    property: "opacity"
                    to: 0
                    duration: 900
                    easing.type: Easing.InQuad
                }
                NumberAnimation {
                    target: tDamageNumber
                    property: "scale"
                    from: 0.8
                    to: 1.15
                    duration: 220
                    easing.type: Easing.OutBack
                }
            }

            Timer {
                interval: 950
                running: true
                onTriggered: rWarlock.removeDamageNumber(tDamageNumber.numberId)
            }
        }
    }

    function findTurnNewsMonster(name) {
        var roster = l_warlock.turn_news_monsters;
        if (roster) {
            for (var i = 0, Ln = roster.length; i < Ln; ++i) {
                if (roster[i] && (roster[i].name === name)) {
                    return roster[i];
                }
            }
        }
        var monsters = l_warlock.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === name) {
                return monsters[j];
            }
        }
        return null;
    }

    function changeHealth(amount, object_type, object_name) {
        var delta = parseInt(amount, 10);
        if (!delta) {
            return;
        }
        if (object_type === "monster") {
            var monster = findTurnNewsMonster(object_name);
            if (monster) {
                var shown = (monster.turn_news_hp_display !== undefined)
                        ? monster.turn_news_hp_display : parseInt(monster.text, 10);
                if (isNaN(shown)) {
                    shown = parseInt(monster.hp, 10);
                }
                if (isNaN(shown)) {
                    shown = 0;
                }
                monster.turn_news_hp_display = Math.max(0, shown + delta);
            }
            var mi = findMonsterItem(object_name);
            if (mi && mi.l_data && (mi.l_data.turn_news_hp_display !== undefined)) {
                mi.l_hpOverride = mi.l_data.turn_news_hp_display;
            }
            return;
        }
        l_hpDisplay = Math.max(0, l_hpDisplay + delta);
    }

    function registerSummonedMonsterAction(monster) {
        var b = mainWindow.gBattle;
        if (!b || !b.actions || !b.actions.M) {
            return;
        }
        for (var i = 0, Ln = b.actions.M.length; i < Ln; ++i) {
            if (b.actions.M[i] && (b.actions.M[i].name === monster.name)) {
                monster.action_idx = i;
                return;
            }
        }
        var id = (b.targetsMap && b.targetsMap[monster.name]) ? b.targetsMap[monster.name] : "";
        if (!id) {
            return;
        }
        var target = b.enemy_name || "Nobody";
        monster.id = id;
        monster.target = target;
        monster.old_target = target;
        monster.under_control = l_warlock.player ? true : false;
        monster.action = "m";
        monster.action_idx = b.actions.M.length;
        b.actions.M.push(monster);
    }

    function isPlaceholderMonster(m) {
        return m && ((m.name.indexOf("LH:") === 0) || (m.name.indexOf("RH:") === 0));
    }

    function showFinalPlaceholders() {
        if (l_warlock.read_only || !l_warlock.turn_news_monsters) {
            return;
        }
        var roster = l_warlock.turn_news_monsters.slice();
        var monsters = l_warlock.monsters || [];
        for (var i = 0, Ln = monsters.length; i < Ln; ++i) {
            if (!isPlaceholderMonster(monsters[i])) {
                continue;
            }
            var found = false;
            for (var j = 0, LnJ = roster.length; j < LnJ; ++j) {
                if (roster[j] && (roster[j].name === monsters[i].name)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                roster.push(monsters[i]);
            }
        }
        l_warlock.turn_news_monsters = roster;
        refreshTurnNewsMonsters();
    }

    function showHandsAfterNews() {
        iiLeft.visible = !l_dead && (isHandsVisible === 1);
        iiRight.visible = !l_dead && (isHandsVisible === 1);
    }

    function showSummonedMonster(name, icon, hp) {
        var roster = l_warlock.turn_news_monsters;
        if (!roster) {
            return;
        }
        for (var i = 0, Ln = roster.length; i < Ln; ++i) {
            if (roster[i] && (roster[i].name === name)) {
                return;
            }
        }
        var monster = null;
        var monsters = l_warlock.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === name) {
                monster = monsters[j];
                break;
            }
        }
        if (!monster) {
            // It may have been summoned and destroyed before the turn ended,
            // so it is not in the final roster.
            var shown = parseInt(hp, 10);
            if (isNaN(shown) || (shown < 0)) {
                shown = 0;
            }
            monster = {name:name, owner:l_warlock.name, hp:shown,
                       icon:(icon && (icon !== "")) ? icon : "summon",
                       enchantment_icon:"", status:"", action:"m",
                       allow_choose_target:false, is_elemental:false,
                       turn_news_hp_start:shown, turn_news_hp_display:shown,
                       turn_news_dead:false, text:shown};
        }
        if (monster.turn_news_hp_display === undefined) {
            monster.turn_news_hp_display = (monster.turn_news_hp_start !== undefined)
                    ? monster.turn_news_hp_start : monster.hp;
        }
        monster.turn_news_dead = false;
        monster.text = monster.turn_news_hp_display;
        roster.push(monster);
        // Assign a fresh array: a plain JS array does not notify the ListView
        // when it is mutated in place, so the new icon would not appear.
        roster = roster.slice();
        l_warlock.turn_news_monsters = roster;
        registerSummonedMonsterAction(monster);
        refreshTurnNewsMonsters();
    }

    function showMonsterDeath(name) {
        var monster = findTurnNewsMonster(name);
        if (monster) {
            monster.turn_news_dead = true;
            monster.turn_news_hp_display = 0;
        }
        var b = mainWindow.gBattle;
        if (b && b.actions && b.actions.M) {
            for (var i = 0, Ln = b.actions.M.length; i < Ln; ++i) {
                if (b.actions.M[i] && (b.actions.M[i].name === name)) {
                    b.actions.M[i].turn_news_dead = true;
                }
            }
        }
        var mi = findMonsterItem(name);
        if (mi) {
            if (mi.restoreNews) {
                mi.restoreNews();
            }
            mi.l_deadOverride = true;
            mi.l_hpOverride = 0;
        }
    }

    function showDeath(object_type, object_name) {
        if (object_type === "monster") {
            showMonsterDeath(object_name);
            return;
        }
        paHPHighlight.stop();
        iiHP.scale = 1;
        iiHP.color = iiHP.bg_color;
        l_dead = true;
        l_hpDisplay = 0;
    }

    function showDamage(amount, object_type, object_name) {
        var damage = parseInt(amount, 10);
        if (!(damage > 0)) {
            return;
        }
        changeHealth(-damage, object_type, object_name);
        showFloatingNumber("-" + damage, "#FF3B30", object_type, object_name);
    }

    function showHeal(amount, object_type, object_name) {
        var heal = parseInt(amount, 10);
        if (!(heal > 0)) {
            return;
        }
        changeHealth(heal, object_type, object_name);
        showFloatingNumber("+" + heal, "#4CD964", object_type, object_name);
    }

    function showFloatingNumber(text, color, object_type, object_name) {
        var pos = getDamagePosition(object_type, object_name);
        if (!pos) {
            return;
        }
        var seq = l_damageSeq++;
        // Separate simultaneous hits on the same target by a few pixels, always
        // further to the right so the number never covers the icon.
        var jitter = (seq % 5) * 12 * mainWindow.ratioFont;
        lmDamageNumbers.append({number_id: seq, text: text, color: color,
                                start_x: pos.x + jitter, start_y: pos.y - 18 * mainWindow.ratioFont});
    }

    // Start the number at the right edge of the target icon, so it reads as
    // coming off the card instead of covering the icon itself.
    function getDamagePosition(object_type, object_name) {
        if (object_type === "monster") {
            var mi = findMonsterItem(object_name);
            if (!mi) {
                return null;
            }
            var mp = mapFromItem(mi, mi.width + 6 * l_ratio, mi.height * 0.55);
            return {x: mp.x, y: mp.y};
        }
        var hp = mapFromItem(iiHP, iiHP.width + 14 * l_ratio, iiHP.height * 0.65);
        return {x: hp.x, y: hp.y};
    }

    function removeDamageNumber(number_id) {
        for (var i = 0; i < lmDamageNumbers.count; ++i) {
            if (lmDamageNumbers.get(i).number_id === number_id) {
                lmDamageNumbers.remove(i);
                return;
            }
        }
    }

    function clearDamageNumbers() {
        lmDamageNumbers.clear();
        l_damageSeq = 0;
    }

    function finalWarlockDead() {
        return (parseInt(l_warlock.hp, 10) <= 0) &&
                ((l_warlock.status || "").toLowerCase().indexOf("surrender") === -1);
    }

    function resetHealthDisplay() {
        l_hpDisplay = l_warlock.hp;
        l_dead = finalWarlockDead();
        // The temporary roster only exists while the turn news plays. Put the
        // real current monsters back, including removing any RIP placeholders.
        for (var k = 0, LnK = iMonsters.count; k < LnK; ++k) {
            var item = iMonsters.itemAtIndex(k);
            if (item) {
                item.l_deadOverride = false;
                item.l_hpOverride = -1;
            }
        }
        l_warlock.turn_news_monsters = undefined;
        // The temporary turn-news tiles belong to the old step too.
        tempNewsMonsters = [];
        tempMonsterKey = "[]";
        tempStatusKey = "[]";
        iCharm.model = l_warlock.statusIcons || [];
        var monsters = l_warlock.monsters || [];
        for (var i = 0, Ln = monsters.length; i < Ln; ++i) {
            monsters[i].turn_news_dead = false;
            monsters[i].turn_news_hp_display = monsters[i].hp;
            monsters[i].text = monsters[i].hp;
        }
        prepareMonsters();
    }

    // The auto-play can be closed between two highlight frames; make sure the
    // little attack marker does not outlive the turn news.
    function restoreAllNews() {
        for (var i = 0, Ln = iMonsters.count; i < Ln; ++i) {
            var item = iMonsters.itemAtIndex(i);
            if (item && item.restoreNews) {
                item.restoreNews();
            }
        }
    }

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

    // The player's wizard icon in his game color; the life-points number keeps
    // its original bottom-right corner spot.
    IconInfo {
        id: iiHP
        source: l_dead ? "qrc:/res/RIP2.png"
                       : "qrc:/res/wizard_" + (l_warlock.color ? l_warlock.color : "grey") + ".png";
        text: l_hpDisplay
        textVisible: false
        height: 78 * l_ratio
        width: height
        anchors.top: parent.top
        anchors.topMargin: 12 * l_ratio
        anchors.left: parent.left
        anchors.leftMargin: 6 * l_ratio

        // number: original corner geometry (like the built-in btnText)
        Text {
            text: l_hpDisplay
            visible: !l_dead
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 1 * l_ratio
            anchors.bottomMargin: 1 * l_ratio
            height: 0.3 * parent.height
            width: 0.3 * parent.width
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 30 * mainWindow.ratioFont
            font.weight: Font.Bold
            color: "white"
        }

        onClicked: {
            iconClick({action:"hp",value:text});
        }

        onDoubleClicked: {
            iconDoubleClick({action:"hp"});
        }
    }

    // the turn news lights the HP card up when a spell or an attack gets to this warlock
    // https://github.com/Pz1c/WavingHands/issues/306
    ParallelAnimation {
        id: paHPHighlight

        ColorAnimation {
            id: caHPHighlight
            target: iiHP
            property: "color"
            duration: 350
        }

        SequentialAnimation {
            NumberAnimation { target: iiHP; property: "scale"; to: 1.15; duration: 150; easing.type: Easing.OutQuad }
            NumberAnimation { target: iiHP; property: "scale"; to: 1; duration: 200; easing.type: Easing.InOutQuad }
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
                property int l_hpOverride: -1
                property bool l_deadOverride: false
                property int l_hpDisplay: (l_hpOverride >= 0) ? l_hpOverride
                                           : (iMonsters.model[index].turn_news_hp_display !== undefined
                                              ? iMonsters.model[index].turn_news_hp_display
                                              : iMonsters.model[index].text)
                property bool l_dead: l_deadOverride || (iMonsters.model[index].turn_news_dead === true)
                l_data: iMonsters.model[index]
                height: 78 * l_ratio
                width: 78 * l_ratio
                text: l_hpDisplay
                textVisible: !l_dead
                source: l_dead ? "qrc:/res/RIP2.png"
                               : "qrc:/res/"+iMonsters.model[index].icon+".png"
                iconInfoSource: iMonsters.model[index].enchantment_icon
                iconInfoVisible: !l_dead && (iMonsters.model[index].enchantment_icon !== "")
                checkbox: false//iMonsters.model[index].is_checkbox
                radius: 20

                // the turn news lights the monster up when it attacks or when a spell or an attack gets to it
                // https://github.com/Pz1c/WavingHands/issues/227
                ParallelAnimation {
                    id: paMonsterHighlight

                    ColorAnimation {
                        id: caMonsterHighlight
                        target: iiDM
                        property: "color"
                        duration: 350
                    }

                    SequentialAnimation {
                        NumberAnimation { target: iiDM; property: "scale"; to: 1.15; duration: 150; easing.type: Easing.OutQuad }
                        NumberAnimation { target: iiDM; property: "scale"; to: 1; duration: 200; easing.type: Easing.InOutQuad }
                    }
                }

                function highlightNews(news_color, news_icon) {
                    caMonsterHighlight.to = news_color;
                    paMonsterHighlight.restart();
                    if (news_icon) {
                        iMonsterNewsIcon.source = "qrc:/res/" + news_icon + ".png";
                        iMonsterNewsIcon.visible = true;
                    }
                }

                function restoreNews() {
                    paMonsterHighlight.stop();
                    scale = 1;
                    color = bg_color;
                    iMonsterNewsIcon.visible = false;
                }

                // A small attack marker tells which monster made the attack.
                Image {
                    id: iMonsterNewsIcon
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 3 * l_ratio
                    anchors.bottomMargin: 3 * l_ratio
                    width: 28 * l_ratio
                    height: width
                    visible: false
                    z: 10
                }

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

                // the turn news lights a status up when the spell that gives it lands or does its job
                // https://github.com/Pz1c/WavingHands/issues/227
                ParallelAnimation {
                    id: paStatusHighlight

                    ColorAnimation {
                        id: caStatusHighlight
                        target: iiDC
                        property: "color"
                        duration: 350
                    }

                    SequentialAnimation {
                        NumberAnimation { target: iiDC; property: "scale"; to: 1.15; duration: 150; easing.type: Easing.OutQuad }
                        NumberAnimation { target: iiDC; property: "scale"; to: 1; duration: 200; easing.type: Easing.InOutQuad }
                    }
                }

                function highlightNews(news_color) {
                    caStatusHighlight.to = news_color;
                    paStatusHighlight.restart();
                }

                function restoreNews() {
                    paStatusHighlight.stop();
                    scale = 1;
                    color = checked ? bg_color_checked : bg_color;
                }

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

                    // the turn news lights the spell's letters up one by one, each one pops
                    SequentialAnimation on scale {
                        running: lwGestures.model[index].lp === true
                        NumberAnimation { to: 1.15; duration: 100; easing.type: Easing.OutQuad }
                        NumberAnimation { to: 1; duration: 150; easing.type: Easing.InOutQuad }
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

                    SequentialAnimation on scale {
                        running: lwGestures.model[index].rp === true
                        NumberAnimation { to: 1.15; duration: 100; easing.type: Easing.OutQuad }
                        NumberAnimation { to: 1; duration: 150; easing.type: Easing.InOutQuad }
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
            var item = iMonsters.itemAtIndex(i) as IconInfo;
            if (!item) {
                // Delegate not created (scrolled out of view): nothing to show or hide.
                continue;
            }
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
        if (Restore) {
            resetHealthDisplay();
        }
        iiLeft.visible = Restore && !l_dead && (isHandsVisible === 1);
        iiRight.visible = Restore && !l_dead && (isHandsVisible === 1);
        if (Restore) {
            setTempIcons([], []);
        }
    }

    // https://github.com/Pz1c/WavingHands/issues/227
    // Temporary icons of the turn news: a status the warlock had during the turn but not at its end
    // (a one-turn shield, a countered enchantment) and monsters that are gone (died, summon absorbed).
    // They sit in the real lists while the news plays: statuses first (at the right edge), monsters last.
    // The lists are only rebuilt when the set changes, so a step keeps the tiles of the step before.
    property string tempStatusKey: "[]"
    property string tempMonsterKey: "[]"
    // Monsters that are only on the board during the news (gone or absorbed). They are
    // overlaid on the reconstructed turn-news roster, never on the final one.
    property var tempNewsMonsters: []

    function setTempIcons(statuses, monsters) {
        var sk = JSON.stringify(statuses), mk = JSON.stringify(monsters);
        if (sk !== tempStatusKey) {
            tempStatusKey = sk;
            iCharm.model = statuses.length > 0 ? statuses.concat(l_warlock.statusIcons) : l_warlock.statusIcons;
        }
        if (mk !== tempMonsterKey) {
            tempMonsterKey = mk;
            tempNewsMonsters = monsters;
            refreshTurnNewsMonsters();
        }
    }

    function targetingOnOff(Enable, IsSpell, Permanency, Delay) {
        console.log("Warlock.targetingOnOff", l_warlock.name, Enable, IsSpell, Permanency, Delay);
        var opacity = Enable ? 0.3 : 1;
        var border_width = Enable ? 3 : 0;
        /*lwGestures.opacity = opacity;
        iiLeft.opacity = opacity;
        iiRight.opacity = opacity;
        iiLeft.active = !Enable;
        iiRight.active = !Enable;
        rBottomLine.visible = l_warlock.player || !Enable;
        if (iiBanked.visible) {
            iiBanked.opacity = opacity;
        }*/
        lwGestures.visible = !Enable;
        iiLeft.visible = !Enable;
        iiRight.visible = !Enable;
        iiLeft.active = !Enable;
        iiLeft.visible = !Enable;
        iiRight.active = !Enable;
        iiRight.visible = !Enable;
        rBottomLine.visible = l_warlock.player || !Enable;
        iiBanked.visible = (l_warlock.banked_spell !== "") && !Enable;
        if (Enable) {
           iiHP.animate(1);
        }

        var item, i, Ln;

        for (i = 0, Ln = iCharm.count; i < Ln; ++i) {
            item = iCharm.itemAtIndex(i) as IconInfo;
            if (!item) {
                // Delegate not created (scrolled out of view): it gets its state when built.
                continue;
            }

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
            }
            item.visible = !Enable || perm_or_del;
        }
    }

    function highlightAction(a) {
        console.log("Warlock.highlightAction", a.object_type, a.object);
        if (a.object_type === "warlock") {
            switch(a.object) {
            case "hp":
                caHPHighlight.to = a.color ? a.color : iiHP.bg_color_checked;
                paHPHighlight.restart();
                break;
            case "gestures":
                lwGestures.model = a.data;
                scrollGestures();
                break;
            }
        } else if (a.object_type === "monster") {
            var mi = findMonsterItem(a.object);
            if (mi) {
                mi.highlightNews(a.color ? a.color : "#FEE2D6", a.news_icon);
            }
        } else if (a.object_type === "status") {
            var si = findStatusItem(a.object);
            if (si) {
                si.highlightNews(a.color ? a.color : "#10C9F5");
            }
        }
    }

    function highlightActionRestore(a) {
        console.log("Warlock.highlightActionRestore", a.object_type, a.object);
        if (a.object_type === "warlock") {
            switch(a.object) {
            case "hp":
                paHPHighlight.stop();
                iiHP.scale = 1;
                iiHP.color = iiHP.bg_color;
                break;
            case "gestures":
                lwGestures.model = l_warlock.print_g;
                scrollGestures();
                break;
            }
        } else if (a.object_type === "monster") {
            var mr = findMonsterItem(a.object);
            if (mr) {
                mr.restoreNews();
            }
        } else if (a.object_type === "status") {
            var sr = findStatusItem(a.object);
            if (sr) {
                sr.restoreNews();
            }
        }
    }

    // the IconInfo delegate of the monster with that name, null when there is none
    function findMonsterItem(name) {
        return findListItem(iMonsters, function(d) { return d.name === name; });
    }

    // the IconInfo delegate of the status icon with that code (shield, maladroit...), null when there is none
    // https://github.com/Pz1c/WavingHands/issues/227
    function findStatusItem(code) {
        return findListItem(iCharm, function(d) { return d.action === code; });
    }

    // a delegate of the list by its model data; a tile scrolled out of view is brought in first so it exists
    function findListItem(list, match) {
        var i, Ln, item;
        for (i = 0, Ln = list.count; i < Ln; ++i) {
            item = list.itemAtIndex(i);
            if (item && item.l_data && match(item.l_data)) {
                return item;
            }
        }
        for (i = 0, Ln = list.count; i < Ln; ++i) {
            if (list.model[i] && match(list.model[i])) {
                list.positionViewAtIndex(i, ListView.Contain);
                list.forceLayout();
                item = list.itemAtIndex(i);
                return item ? item : null;
            }
        }
        return null;
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
            if (type === "m") {
                if (arr_m.turn_news_hp_display === undefined) {
                    arr_m.turn_news_hp_display = (arr_m.turn_news_hp_start !== undefined)
                            ? arr_m.turn_news_hp_start : arr_m[code_value];
                }
                arr_m.text = arr_m.turn_news_hp_display;
            } else {
                arr_m.text = arr_m[code_value];
            }
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

        }
        parent.model = arr;

    }

    // Single source for the monster tiles while the turn news plays: the dynamic
    // turn-news roster (previous turn + summons/deaths) plus the temporary tiles
    // that only exist in history. 17b6b71's temp icons and the newer dynamic roster
    // must not overwrite each other's ListView model.
    function refreshTurnNewsMonsters() {
        var base = (l_warlock.turn_news_monsters !== undefined)
                ? l_warlock.turn_news_monsters : l_warlock.monsters;
        var shown = (base || []).slice();
        var temps = tempNewsMonsters || [];
        for (var i = 0, Ln = temps.length; i < Ln; ++i) {
            var temp = temps[i];
            if (!temp) {
                continue;
            }
            var found = false;
            for (var j = 0, LnJ = shown.length; j < LnJ; ++j) {
                if (shown[j] && (shown[j].name === temp.name)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                shown.push(temp);
            }
        }
        prepareDynamic("m", shown, iMonsters, "hp", 0, 1);
    }

    function prepareMonsters() {
        refreshTurnNewsMonsters();
    }

    function prepareState() {
        prepareDynamic("s", l_warlock.statusIcons, iCharm, "value", svCharm.width - svCharm.height, -1);
    }

    property int isHandsVisible: 0

    function prepareHands() {
        var hands_visible = (l_warlock.player || l_warlock.control_paralyze || l_warlock.control_charmed);
        var lh_visible = hands_visible || (l_warlock.paralyzed_hand === "LH");
        var rh_visible = hands_visible || (l_warlock.paralyzed_hand === "RH");
        var ng;
        isHandsVisible = !l_warlock.read_only && !(!l_warlock.player && (mainWindow.gBattle.turn_num === 1)) ? 1 : 0;
        iiLeft.visible = isHandsVisible === 1;
        iiRight.visible = isHandsVisible === 1;
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
        mainWindow.battleWarlockPrepared(l_warlock.name);
        //iiBanked.enabled = iiBanked.visible && l_warlock.player;
    }
}
