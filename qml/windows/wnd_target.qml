// Target choosing: which warlock or monster a spell is cast at (spell mode), or which one a
// monster under the player's control attacks (monster mode). It took over from the targeting
// mode wnd_battle.qml used to switch itself into: the same warlocks with bigger tiles, no hands
// or gestures, and the permanency / bank toggles in the bottom bar.
//
// Opened by mainWindow.showTargetWnd(is_spell, spell) with mainWindow.gERROR = {is_spell, spell},
// where spell is the hand's spell object (spell mode) or {n: <monster popup title>} (monster
// mode). Done hands the choice to mainWindow.setSpellTarget() and the battle window's
// finishTargeting(); Back keeps the spell and leaves the order's target alone.

import QtQuick 2.15

import "qrc:/qml/components"
import "qrc:/js/small_gui_utils.js" as SGU

BaseWindow {
    id: targetItem

    with_controls: false
    body_width_prc: 100
    body_height_prc: 100
    title_height_prc: 0
    control_height_prc: 0
    bg_source: "qrc:/res/background_battle.png"

    property bool isSpell: true
    property var currentSpell: ({n:"",h:0,g:""})
    property int permanency: 0
    property int delay: 0
    // the clicked tile's data: {action:"hp",warlock_name} or the monster object, plus target_name
    property var target: ({target_name:""})
    property string targetName: ""
    property bool submitted: false
    // [{name, player, tiles:[{key, text, icon, info, data}]}]
    property var warlocks: []

    readonly property real tileSize: 108 * mainWindow.ratioObject
    readonly property real tileGap: 36 * mainWindow.ratioObject

    Item {
        id: targetWindow
        z: 11
        anchors.fill: content_item

        Flickable {
            id: fWarlocks
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: iPanel.top
            contentWidth: width
            contentHeight: cWarlocks.height
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            Column {
                id: cWarlocks
                width: fWarlocks.width

                Repeater {
                    id: rWarlocks
                    model: targetItem.warlocks

                    // one warlock: the name pill on top, the heart and monster tiles in centered
                    // rows below it, a line at the bottom. The warlocks share the screen equally
                    // as long as their rows fit, otherwise the list scrolls.
                    delegate: Item {
                        id: iSection

                        property var warlock: modelData
                        property int perRow: Math.max(1, Math.floor((width + targetItem.tileGap) / (targetItem.tileSize + targetItem.tileGap)))
                        property var rows: targetItem.chunkRows(warlock.tiles, perRow)
                        property real minHeight: ltName.height + rows.length * targetItem.tileSize + (rows.length + 1) * targetItem.tileGap + rLine.height

                        width: cWarlocks.width
                        height: Math.max(minHeight, fWarlocks.height / rWarlocks.count)

                        LargeText {
                            id: ltName
                            text: iSection.warlock.name
                            anchors.top: parent.top
                            anchors.horizontalCenter: parent.horizontalCenter
                            height: 36 * mainWindow.ratioObject
                            width: 246 * mainWindow.ratioObject
                            bg_visible: true
                            bg_color: iSection.warlock.player ? "#210430" : "#544653"
                            color: iSection.warlock.player ? "#A8F4F4" : "#FEE2D6"
                            border_visible: false
                            radius: 10
                        }

                        Item {
                            id: iTilesArea
                            anchors.top: ltName.bottom
                            anchors.bottom: rLine.top
                            anchors.left: parent.left
                            anchors.right: parent.right

                            Column {
                                id: cRows
                                anchors.centerIn: parent
                                spacing: targetItem.tileGap

                                Repeater {
                                    model: iSection.rows

                                    delegate: Row {
                                        id: rTiles

                                        property var rowTiles: modelData

                                        anchors.horizontalCenter: parent.horizontalCenter
                                        spacing: targetItem.tileGap

                                        Repeater {
                                            model: rTiles.rowTiles

                                            delegate: IconInfo {
                                                id: iiTile

                                                property var tile: modelData

                                                height: targetItem.tileSize
                                                width: targetItem.tileSize
                                                radius: 20
                                                source: tile.icon
                                                text: tile.text
                                                iconInfoSource: tile.info
                                                iconInfoVisible: tile.info !== ""
                                                l_data: tile.data
                                                // IconInfo would otherwise write border.width itself
                                                borderOnPress: false
                                                border.color: "#A8F4F4"
                                                border.width: (targetItem.targetName === tile.key ? 3 : 0) * mainWindow.ratioObject

                                                onClicked: {
                                                    targetItem.chooseTarget(l_data);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id: rLine
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 2 * mainWindow.ratioObject
                            color: "#FEE2D6"
                            visible: index < rWarlocks.count - 1
                        }
                    }
                }
            }
        }

        // the title, the special targets, the toggles and Done
        Item {
            id: iPanel
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 164 * mainWindow.ratioObject

            Rectangle {
                id: rPanelLine
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 2 * mainWindow.ratioObject
                color: "#FEE2D6"
            }

            Text {
                id: tTargetTitle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 12 * mainWindow.ratioObject
                anchors.right: parent.right
                anchors.rightMargin: 12 * mainWindow.ratioObject
                height: 56 * mainWindow.ratioObject
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 28 * mainWindow.ratioFont
                color: "#FEE2D6"
                textFormat: Text.StyledText
                elide: Text.ElideRight
                text: ""
            }

            Row {
                id: rTargetIcons
                anchors.top: tTargetTitle.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: 12 * mainWindow.ratioObject
                spacing: 24 * mainWindow.ratioObject

                IconInfo {
                    id: iiElemental
                    source: "qrc:/res/elemental_fire.png"
                    text: "3"
                    height: 72 * mainWindow.ratioObject
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 20
                    visible: false
                    borderOnPress: false
                    border.color: "#A8F4F4"
                    border.width: ((targetItem.targetName !== "") && l_data && (targetItem.targetName === l_data.name) ? 3 : 0) * mainWindow.ratioObject

                    onClicked: {
                        targetItem.chooseTarget(l_data);
                    }
                }

                IconInfo {
                    id: iiNobody
                    source: "qrc:/res/target_nobody.png"
                    text: ""
                    height: 72 * mainWindow.ratioObject
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 20
                    borderOnPress: false
                    border.color: "#A8F4F4"
                    border.width: (targetItem.targetName === "Nobody" ? 3 : 0) * mainWindow.ratioObject
                    l_data: ({action:"hp",warlock_name:"Nobody"})

                    onClicked: {
                        targetItem.chooseTarget(l_data);
                    }
                }

                // "whatever the server picks": only while the player is charmed or paralyzed
                IconInfo {
                    id: iiDefault
                    source: "qrc:/res/target_default.png"
                    text: ""
                    height: 72 * mainWindow.ratioObject
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 20
                    visible: false
                    borderOnPress: false
                    border.color: "#A8F4F4"
                    border.width: (targetItem.targetName === "Default" ? 3 : 0) * mainWindow.ratioObject
                    l_data: ({action:"hp",warlock_name:"Default"})

                    onClicked: {
                        targetItem.chooseTarget(l_data);
                    }
                }

                IconInfo {
                    id: iiPermanency
                    source: "qrc:/res/permanency.png"
                    text: ""
                    height: 72 * mainWindow.ratioObject
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 20
                    visible: false
                    checkbox: true
                    l_data: ({action:"permanency"})

                    onClicked: {
                        mainWindow.logEvent("Play_Spell_Click", {Click:"permanency"});
                        targetItem.permanency = checked ? 1 : 0;
                        targetItem.updateTitle();
                    }
                }

                IconInfo {
                    id: iiDelay
                    source: "qrc:/res/delay.png"
                    text: ""
                    height: 72 * mainWindow.ratioObject
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 20
                    visible: false
                    checkbox: true
                    l_data: ({action:"delay"})

                    onClicked: {
                        mainWindow.logEvent("Play_Spell_Click", {Click:"delay"});
                        targetItem.delay = checked ? 1 : 0;
                        targetItem.updateTitle();
                    }
                }
            }

            IconInfo {
                id: bbDone
                active: false
                source: "qrc:/res/send_"+(active ? "1" : "0")+".png"
                textVisible: true

                height: 64 * mainWindow.ratioObject
                iconHeight: 64 * mainWindow.ratioObject
                iconWidth: 64 * mainWindow.ratioObject
                textHeight: 64 * mainWindow.ratioObject
                textWidth: 80 * mainWindow.ratioObject
                width: (64 + 18 + 80) * mainWindow.ratioObject
                textAnchors.left: bbDone.left
                textAnchors.right: undefined
                iconAnchors.right: bbDone.right
                iconAnchors.centerIn: undefined
                anchors.verticalCenter: rTargetIcons.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 12 * mainWindow.ratioObject

                color: "transparent"
                text_color: active ? "#A8F4F4" : "#544653"
                text: "Done"

                onClicked: {
                    console.log("wnd_target.bbDone", JSON.stringify(mainWindow.gBattle.actions), JSON.stringify(target));
                    submitted = true;
                    mainWindow.setSpellTarget(target.target_name, permanency, delay, isSpell ? 1 : 2);
                    mainWindow.logEvent("Play_Target_Sumbit", {Target:target.target_name,Permanency:permanency,Delay:delay});
                    mainWindow.finishTargeting(target, currentSpell);
                    mainWindow.processEscape();
                }
            }
        }

        // how to bank / make permanent, shown for 1.5 s above the panel
        LargeText {
            id: ltHint
            anchors.bottom: iPanel.top
            anchors.bottomMargin: 12 * mainWindow.ratioObject
            anchors.left: parent.left
            anchors.leftMargin: 12 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 12 * mainWindow.ratioObject
            height: 72 * mainWindow.ratioObject
            visible: false
            text: ""
            color: "#210430"
            bg_color: "#FEE2D6"
            bg_visible: true
            bg_radius: 10
            border_visible: false
            wrapMode: Text.Wrap
            font.pixelSize: 21 * mainWindow.ratioFont

            PropertyAnimation {
                id: paHint
                running: false
                target: ltHint
                property: 'visible'
                to: false
                duration: 1500 // turns to false after 1.5s
            }

            onClicked: {
                visible = false;
                paHint.stop();
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function chunkRows(tiles, perRow) {
        var rows = [];
        for (var i = 0; i < tiles.length; i += perRow) {
            rows.push(tiles.slice(i, i + perRow));
        }
        return rows;
    }

    function isSummonSpell(spell) {
        if (!spell) {
            return false;
        }
        var g = spell.g ? spell.g : "";
        var n = spell.n ? spell.n : "";
        return (g.indexOf("SFW") !== -1) || (g.indexOf("cSWWS") !== -1) || (g.indexOf("cWSSW") !== -1) || (n.indexOf("Summon") === 0);
    }

    // "LH:Name" / "RH:Name" stands for the monster a summon with that hand would create this turn:
    // the player's is a target only when that hand's spell is a summon, an opponent's always is
    function isPlaceholderVisible(warlock, monster) {
        if (!warlock.player) {
            return true;
        }
        var hand = monster.name.indexOf("LH:") === 0 ? "L" : "R";
        var battle = mainWindow.gBattle;
        var spell = battle.actions && battle.actions[hand] ? battle.actions[hand].s : null;
        if (spell && (spell.n === "Default")) {
            // "keep what the server has": the server's spell for the hand
            spell = {n:battle["player_spell_" + hand]};
        }
        return isSummonSpell(spell);
    }

    function buildWarlocks() {
        var battle = mainWindow.gBattle;
        var res = [];
        if (!battle || !battle.warlocks) {
            return res;
        }
        for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
            var w = battle.warlocks[i];
            var monsters = w && battle.monsters && battle.monsters[w.name] ? battle.monsters[w.name] : [];
            // https://github.com/Pz1c/WavingHands/issues/259 (as the battle window)
            if (!w || (!battle.read_only && !w.active && (monsters.length === 0))) {
                continue;
            }
            var tiles = [{key:w.name,text:"" + w.hp,icon:"qrc:/res/heart_small.png",info:"",
                          data:{action:"hp",warlock_name:w.name,warlock_idx:i,value:w.hp}}];
            for (var j = 0, Lm = monsters.length; j < Lm; ++j) {
                var m = monsters[j];
                var placeholder = m.name.indexOf(":") !== -1;
                if (placeholder && !isPlaceholderVisible(w, m)) {
                    continue;
                }
                tiles.push({key:m.name,text:placeholder ? "?" : "" + m.hp,icon:"qrc:/res/" + m.icon + ".png",
                            info:m.enchantment_icon ? m.enchantment_icon : "",data:m});
            }
            res.push({name:w.name,player:w.player ? true : false,tiles:tiles});
        }
        return res;
    }

    function chooseTarget(data) {
        console.log("wnd_target.chooseTarget", JSON.stringify(data));
        if (data.action === "hp") {
            mainWindow.logEvent("Play_Warlock_Click", {Click:data.warlock_name});
        } else {
            mainWindow.logEvent("Play_Monster_Click", {Click:data.name});
        }
        data.target_name = data.action === "hp" ? data.warlock_name : data.name;
        target = data;
        targetName = data.target_name;
        bbDone.active = true;
        updateTitle();
    }

    function updateTitle() {
        tTargetTitle.text = SGU.getTargetTitle(isSpell, currentSpell.n ? currentSpell.n : "", targetName, permanency, delay);
    }

    function showShortHint(msg) {
        ltHint.text = msg;
        ltHint.visible = true;
        paHint.restart();
    }

    function showTargetHint() {
        var battle = mainWindow.gBattle;
        var txt = "";
        if (isSpell && currentSpell.n) {
            if (battle.player_has_bank) {
                txt = "To bank a " + currentSpell.n + ", click the bank icon and then choose a target";
            } else if (battle.player_has_permanency) {
                txt = "To make " + currentSpell.n + " permanent, click the permanency icon and then choose a target";
            }
        }
        if (txt !== "") {
            showShortHint(txt);
        } else {
            paHint.stop();
            ltHint.visible = false;
        }
    }

    function showWnd() {
        initTargetFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
        if (!submitted) {
            // Back: the spell stays chosen and the order's target is untouched, the battle
            // window still refreshes its Submit button and summon placeholders
            mainWindow.finishTargeting({target_name:""}, currentSpell);
        }
    }

    function initTargetFields() {
        console.log("wnd_target.initTargetFields", JSON.stringify(mainWindow.gERROR));
        var battle = mainWindow.gBattle;
        var data = mainWindow.gERROR ? mainWindow.gERROR : {};
        mainWindow.gERROR = {};
        isSpell = data.is_spell ? true : false;
        currentSpell = data.spell ? data.spell : {n:"",h:0,g:""};
        permanency = currentSpell.permanency ? 1 : 0;
        delay = currentSpell.delay ? 1 : 0;
        target = {target_name:""};
        targetName = "";
        submitted = false;
        bbDone.active = false;

        if (battle && battle.elemental && (battle.elemental.hp > 0)) {
            iiElemental.text = battle.elemental.hp;
            iiElemental.source = "qrc:/res/elemental_" + battle.elemental.type + ".png";
            iiElemental.l_data = battle.elemental;
            iiElemental.visible = true;
        } else {
            iiElemental.visible = false;
        }
        iiDefault.visible = battle && battle.player_under_control ? true : false;
        iiPermanency.visible = isSpell && battle && battle.player_has_permanency ? true : false;
        iiPermanency.setChecked(permanency === 1);
        iiDelay.visible = isSpell && battle && battle.player_has_bank ? true : false;
        iiDelay.setChecked(delay === 1);

        warlocks = buildWarlocks();
        fWarlocks.contentY = 0;
        updateTitle();
        showTargetHint();
    }

    Component.onCompleted: {
        mainWindow.storeWnd(targetItem);
        initTargetFields();
    }
}
