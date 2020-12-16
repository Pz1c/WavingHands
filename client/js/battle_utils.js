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
    w.banked_spell = w.player && (battle.fire !== "");
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

function prepareBattle(raw_battle) {
    battle.id = raw_battle.id;
    battle.warlocks = [];
    battle.elemental = {hp:0,type:"fire"};
    battle.monsters = {};
    battle.ngL = "-";
    battle.ngR = "-";
    // L left  obj
    // R Right obj
    // C Chat  text
    // D Delay int 0 - none, 1 left, 2 right
    // P Permanent int 0 - none, 1 left, 2 right
    // F Fire bool
    // M monsters arr of obj
    // CP - paralyze arr of obj
    // CC - paralyze arr of obj
    battle.actions = {L:{target:"Default"},R:{target:"Default"},C:"",D:0,P:0,F:false,M:[],CP:[],CC:[]};
    battle.paralyze = raw_battle.paralyze.split(";");
    battle.charm = raw_battle.charm.split(";");
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
    console.log("prepareOrder", JSON.stringify(battle.actions));
    var actions = battle.actions;
    var post_request = "say$";
    if (actions.C !== "") {
        post_request += actions.C.replace("$", "sign:dollar").replace("#", "sign:pos_number");
    }
    post_request += "#";
    // gesture
    post_request += "LH$"+actions.L.g+"#";
    post_request += "RH$"+actions.R.g+"#";
    post_request += "LHS$"+getSpellNameForOrder(actions.L)+"#";
    post_request += "RHS$"+getSpellNameForOrder(actions.R)+"#";
    post_request += "LHT$"+getSpellTargetForOrder(actions.L, battle.targetMap)+"#";
    post_request += "RHT$"+getSpellTargetForOrder(actions.R, battle.targetMap)+"#";

    var i, Ln, m_obj;
    for(i = 0, Ln = actions.M.length; i < Ln; ++i) {
        m_obj = actions.M[i];
        console.log("mt_label", JSON.stringify(m_obj));
        if (m_obj.target === m_obj.old_target) {
            continue;
        }

        post_request += m_obj.id + "$" + battle.targetMap[m_obj.target].replace(" ", "+") + "#";
    }

    for(i = 0, Ln = cpPersonObj.length; i < Ln; ++i) {
        console.log("pc_gesture_value", cpPersonObj[i].pc_gesture_value, cpPersonObj[i].pc_gesture_value.replace(">", "&gt;"))
        console.log("cpPersonObj", cpPersonObj[i])
        var pc_gv = cpPersonObj[i].pc_gesture_value;
        if (!pc_gv || pc_gv === '') {
            pc_gv = "-"
        }
        if (pc_gv === ">") {
            pc_gv = "&gt;"
        }

        post_request += "DIRECTHAND" + cpPersonObj[i].pc_target_id + "$" + cpPersonObj[i].pc_hand_value + "#"
        post_request += "DIRECTGESTURE" + cpPersonObj[i].pc_target_id + "$" + pc_gv + "#"
    }

    for(i = 0, Ln = pParalyzeObj.length; i < Ln; ++i) {
        post_request += "PARALYZE" + pParalyzeObj[i].p_target_id + "$" + pParalyzeObj[i].p_hand_value + "#"
    }

    if (lvaoDelay.visible) {
        post_request += "DELAY$"
        switch(cbDelay.currentIndex) {
            case 0:
                post_request += "#"
                break;
            case 1:
                post_request += "LH#"
                break;
            default:
                post_request += "RH#"
                break;
        }
    }

    if (lvaoPermanent.visible) {
        post_request += "PERM$"
        switch(cbPermanent.currentIndex) {
            case 0:
                post_request += "#"
                break;
            case 1:
                post_request += "LH#"
                break;
            default:
                post_request += "RH#"
                break;
        }
    }

    if (lvaoFire.visible && cbFire.checked) {
        post_request += "FIRE$1#"
    }
}
