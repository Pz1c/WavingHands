var battle = {};
var cWarlockObject,cIconObject;
const G_WARLOCK_HEIGHT_COEFF = 0.45;

Qt.include("battle_gui_utils.js");

function prepareWarlock(w) {
    w.monsters = battle.monsters[w.name];
    w.print_g = preparePrintGestures(w.L, w.R);
    w.statusIcons = prepareStatusIcon(w);
    if (w.player) {
        w.banked_spell = battle.fire;
    }

    return w;
}

function parseTargets(targets_str) {
    battle.targetsList = [];
    battle.targetsMap = {};
    var strs = strToArr2D(targets_str, '#', ';', true);
    console.log("parseTargets", JSON.stringify(strs));
    var title;
    for(var i = 0, Ln = strs.length; i < Ln; ++i) {
        title = strs[i][1];
        if (title === " ") {
            title = "Default";
        }
        battle.targetsList.push(title);
        battle.targetsMap[title] = strs[i][0];
    }
}

function prepareBattle(raw_battle) {
    battle.warlocks = [];
    battle.elemental = {hp:0,type:"fire"};
    battle.monsters = [];

    var i, Ln;
    for(i = 0, Ln = raw_battle.monsters.length; i < Ln; ++i) {
        var m = raw_battle.monsters[i];
        if (!m.owner) {
            battle.elemental = m;
            battle.elemental.type = m.name.indexOf("Fire") !== -1 ? "fire" : "ice";
            continue;
        }
        if (m.name.indexOf("Goblin") !== -1) {
            m.icon = "goblin";
        } else if (m.name.indexOf("Ogre") !== -1) {
            m.icon = "ogre";
        } else if (m.name.indexOf("Troll") !== -1) {
            m.icon = "troll";
        } else if (m.name.indexOf("Giant") !== -1) {
            m.icon = "giant";
        }

        if (!battle.monsters[m.owner]) {
            battle.monsters[m.owner] = [];
        }
        battle.monsters[m.owner].push(m);
    }

    for (i = 0, Ln = raw_battle.warlocks.length; i < Ln; ++i) {
        var w = raw_battle.warlocks[i];
        if (!battle.monsters[w.name]) {
            battle.monsters[w.name] = [];
        }
        prepareWarlock(w);
        battle.warlocks.unshift(w);
    }

    parseTargets(raw_battle.targets);

    console.log("prepared battle", JSON.stringify(battle));
}

function prepareElemental() {
    if (battle.elemental.hp > 0) {
        iiElemental.text = battle.elemental.hp;
        iiElemental.source = "qrc:/res/elemental_"+battle.elemental.type+".png";
        iiElemental.visible = true;
    } else {
        iiElemental.visible = false;
    }
}

function prepareWarlocks() {
    console.log("iWarlocks");
    cleanChildren(iWarlocks);

    if (!cWarlockObject) {
        cWarlockObject = Qt.createComponent("qrc:///qml/game_components/Warlock.qml");
    }
    if (!cIconObject) {
        cIconObject = Qt.createComponent("qrc:///qml/components/IconInfo.qml");
    }
    if (cWarlockObject.status === Component.Error) {
        console.log("Error loading component: " + cWarlockObject.errorString());
        return ;
    }

    if (cWarlockObject.status === Component.Ready) {
        console.log("component ready");
        finishPrepareWarlockList();
    } else {
        console.log("connect to component ready signal");
        cWarlockObject.statusChanged.connect(finishPrepareWarlockList);
    }
}

function finishPrepareWarlockList() {
    console.log("finishPrepareWarlockList", battleWindow.height, battleWindow.width);
    if (cWarlockObject.status === Component.Error || cWarlockObject.status !== Component.Ready) {
        console.log("Error loading component:", cWarlockObject.errorString());
        return ;
    }
    var curr_y = 0;
    var total_height = 0;
    for(var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        if (!battle.warlocks[i] || !battle.warlocks[i].active) {
            continue;
        }
        var arr_m = battle.warlocks[i];
        var sprite = cWarlockObject.createObject(iWarlocks, {l_warlock: arr_m, l_IconInfoObj: cIconObject, x: 0, y: curr_y, height: G_WARLOCK_HEIGHT_COEFF * battleWindow.height, width: battleWindow.width});
        if (sprite === null) {
            console.log("Error creating object");
            continue;
        }
        console.log("looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
        console.log("svWarlocks.w: " + iWarlocks.width);
        console.log("svWarlocks.h: " + iWarlocks.height);
        curr_y += sprite.height;
        total_height += sprite.height;
    }
    iWarlocks.height = total_height;
}

function prepareGUI() {
    prepareElemental();
    prepareWarlocks()
}

function applyBattle(raw_battle) {
    prepareBattle(raw_battle);
    prepareGUI();
}
