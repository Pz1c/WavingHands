var battle = {};
var cWarlockObject,cIconObject;
const G_WARLOCK_HEIGHT_COEFF = 0.45;

Qt.include("battle_gui_utils.js");

function prepareWarlock(w) {
    w.monsters = battle.monsters[w.name];
    w.print_g = preparePrintGestures(w.L, w.R);
    w.id = battle.targetsMap[w.name];
    w.control_paralyze = battle.paralyze.indexOf(w.id) !== -1;
    w.control_charmed  = battle.charm.indexOf(w.id) !== -1;
    w.statusIcons = prepareStatusIcon(w);
    w.banked_spell = w.player && (battle.fire !== "") ? battle.fire : "";
    if (w.player) {
        battle.L = w.L;
        battle.R = w.R;
    }

    return w;
}

function parseTargets(targets_str) {
    battle.targetsList = [];
    battle.targetsMap = {};
    var strs = strToArr2D(targets_str, '#', ';', true);
    var m_owner;
    console.log("parseTargets", JSON.stringify(strs));
    var title, obj_id;
    for(var i = 0, Ln = strs.length; i < Ln; ++i) {
        obj_id = strs[i][0];
        title = strs[i][1];
        if (title === " ") {
            title = "Default";
        }
        battle.targetsList.push(title);
        battle.targetsMap[title] = obj_id;
        battle.targetsMap[obj_id] = title;

        if ((title.indexOf("LH:") === 0) || (title.indexOf("RH:") === 0)) {
            m_owner = title.substr(3);
            if (!battle.monsters[m_owner]) {
                battle.monsters[m_owner] = [];
            }
            battle.monsters[m_owner].push({name:title,status:"",hp:title.substr(0, 1),owner:m_owner,icon:getMonsterIconByName(title),action:"m",action_idx:battle.actions.M.length});
            battle.actions.M.push({id:obj_id,target:"",old_target:""});
        }
    }
}

function setParaActions(paralyze, charm) {
    battle.paralyze = paralyze !== "" ? paralyze.split(";") : [];
    battle.charm = charm !== "" ? charm.split(";") : [];
    var i, Ln;
    for(i = 0, Ln = battle.paralyze.length; i < Ln; ++i) {
        if (!battle.paralyze[i]) {
            continue;
        }

        battle.actions.CP[battle.paralyze[i]] = "LH";
    }
    for(i = 0, Ln = battle.charm.length; i < Ln; ++i) {
        if (!battle.charm[i]) {
            continue;
        }
        battle.actions.CC[battle.charm[i]] = {h:"LH",g:"-"};
    }
}

function prepareBattle(raw_battle) {
    battle.id = raw_battle.id;
    battle.fire = raw_battle.fire;
    battle.warlocks = [];
    battle.elemental = {hp:0,type:"fire"};
    battle.monsters = {};
    battle.ngL = "-";
    battle.ngR = "-";
    // L left  obj
    // R Right obj
    // C Chat  text
    // D Delay int 0 - none, 1 left, 2 right, -1 unavailable
    // P Permanent int 0 - none, 1 left, 2 right, -1 unavailable
    // F Fire int 0, 1, -1
    // M monsters arr of obj
    // CP - paralyze arr of obj
    // CC - paralyze arr of obj
    battle.actions = {L:{target:"Default"},R:{target:"Default"},C:"",D:-1,P:-1,F:-1,M:[],CP:{},CC:{}};
    setParaActions(raw_battle.paralyze, raw_battle.charm);
    parseTargets(raw_battle.targets);

    var i, Ln;
    for (i = 0, Ln = raw_battle.monsters.length; i < Ln; ++i) {
        var m = raw_battle.monsters[i];
        if (!m.owner) {
            battle.elemental = m;
            battle.elemental.type = m.name.indexOf("Fire") !== -1 ? "fire" : "ice";
            battle.elemental.action = "m";
            continue;
        }
        m.icon = getMonsterIconByName(m.name);
        m.action = "m";
        m.action_idx = battle.actions.M.length;

        if (!battle.monsters[m.owner]) {
            battle.monsters[m.owner] = [];
        }

        battle.monsters[m.owner].push(m);
        battle.actions.M.push({id:battle.targetsMap[m.name],target:m.target,old_target:m.target});
    }

    for (i = 0, Ln = raw_battle.warlocks.length; i < Ln; ++i) {
        var w = raw_battle.warlocks[i];
        if (!battle.monsters[w.name]) {
            battle.monsters[w.name] = [];
        }
        prepareWarlock(w);
        battle.warlocks.unshift(w);
    }
    //battle.chat = raw_battle.chat;
    delete battle.monsters;
    console.log("prepared battle", JSON.stringify(battle));
}

function prepareElemental() {
    if (battle.elemental.hp > 0) {
        iiElemental.text = battle.elemental.hp;
        iiElemental.source = "qrc:/res/elemental_"+battle.elemental.type+".png";
        iiElemental.l_data = battle.elemental;
        iiElemental.visible = true;
    } else {
        iiElemental.visible = false;
    }
}

function prepareWarlocks() {
    console.log("iWarlocks");
    cleanChildren(iWarlocks);

    if (!cIconObject) {
        cIconObject = Qt.createComponent("qrc:///qml/components/IconInfo.qml");
    }
    if (!cWarlockObject) {
        cWarlockObject = Qt.createComponent("qrc:///qml/game_components/Warlock.qml");
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
        sprite.clicked.connect(iconClick);
        sprite.doubleClicked.connect(iconDoubleClick);
        console.log("looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
        console.log("svWarlocks.w: " + iWarlocks.width);
        console.log("svWarlocks.h: " + iWarlocks.height);
        curr_y += sprite.height;
        total_height += sprite.height;
    }
    iWarlocks.height = total_height;
}

function prepareChat() {
    var with_msg = battle.chat > 0;
    console.log("prepareChat", with_msg, battle.chat);
    iiChat.text = with_msg ? battle.chat : "";
}

function prepareGUI() {
    prepareElemental();
    prepareWarlocks();
    prepareChat();
}

function applyBattle() {
    var raw_battle = mainWindow.gBattle;
    prepareBattle(raw_battle);
    prepareGUI();
    mainWindow.gBattle = battle;
}

function prepareOrder() {
    console.log("prepareOrder", "before", JSON.stringify(battle.actions));
    var actions = battle.actions;
    var post_request = "say$";
    if (actions.C !== "") {
        post_request += actions.C.replace("$", "sign:dollar").replace("#", "sign:pos_number");
    }
    post_request += "#";
    console.log("prepareOrder", "point1", post_request);
    // gesture
    post_request += "LH$"+actions.L.g+"#";
    post_request += "RH$"+actions.R.g+"#";
    console.log("prepareOrder", "point2", post_request);
    post_request += "LHS$"+getSpellNameForOrder(actions.L)+"#";
    post_request += "RHS$"+getSpellNameForOrder(actions.R)+"#";
    console.log("prepareOrder", "point3", post_request);
    post_request += "LHT$"+getSpellTargetForOrder(actions.L, battle.targetsMap)+"#";
    post_request += "RHT$"+getSpellTargetForOrder(actions.R, battle.targetsMap)+"#";
    console.log("prepareOrder", "point4", post_request);

    var i, Ln, m_obj, pc_gv;
    for(i = 0, Ln = actions.M.length; i < Ln; ++i) {
        m_obj = actions.M[i];
        console.log("mt_label", JSON.stringify(m_obj));
        if (m_obj.target === m_obj.old_target) {
            continue;
        }

        post_request += m_obj.id + "$" + battle.targetsMap[m_obj.target].replace(" ", "+") + "#";
    }
    console.log("prepareOrder", "point5", post_request);
    for(i = 0, Ln = battle.paralyze.length; i < Ln; ++i) {
        post_request += "PARALYZE" + battle.paralyze[i] + "$" + actions.CP[battle.paralyze[i]] + "#";
    }
    console.log("prepareOrder", "point6", post_request);
    for(i = 0, Ln = battle.charm.length; i < Ln; ++i) {
        pc_gv = actions.CP[battle.charm[i]].g;
        if (!pc_gv || pc_gv === '') {
            pc_gv = "-"
        }
        if (pc_gv === ">") {
            pc_gv = "&gt;"
        }
        post_request += "DIRECTHAND" + battle.charm[i] + "$" + actions.CP[battle.charm[i]].h + "#";
        post_request += "DIRECTGESTURE" + battle.charm[i] + "$" + pc_gv + "#";
    }
    console.log("prepareOrder", "point7", post_request);

    if (actions.D !== -1) {
        post_request += "DELAY$" + getHandByIdx(actions.D);
    }
    console.log("prepareOrder", "point8", post_request);
    if (actions.P !== -1) {
        post_request += "PERM$" + getHandByIdx(actions.P);
    }
    console.log("prepareOrder", "point9", post_request);
    if (actions.F === 1) {
        post_request += "FIRE$1#"
    }
    console.log("prepareOrder", "after", post_request);

    mainWindow.gameCore.sendOrders(post_request);
    mainWindow.processEscape();
}
