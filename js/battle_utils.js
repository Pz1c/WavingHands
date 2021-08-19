var battle = {};
var cWarlockObject,cIconObject;
const G_WARLOCK_HEIGHT = 474;
Qt.include("battle_gui_utils.js");


function prepareWarlock(w) {
    w.monsters = battle.monsters[w.name];
    w.print_g = preparePrintGestures(w.L, w.R, w.smcL, w.smcR);
    if (!w.id) {
        w.id = battle.targetsMap[w.name];
    }
    w.paralyzed_hand = battle.paralyzed_hand[w.id];
    if (!w.paralyzed_hand) {
        w.paralyzed_hand = "";
    }
    if ((w.fh > 0) && !w.paralyzed_hand) {
        w.paralyzed_hand = w.fh === 1 ? "LH" : "RH";
    }

    w.control_paralyze = battle.paralyze.indexOf(w.id) !== -1;
    w.control_charmed  = battle.charm.indexOf(w.id) !== -1;
    w.statusIcons = prepareStatusIcon(w);
    w.banked_spell = w.player && (battle.fire !== "") ? battle.fire : "";
    if (w.player) {
        battle.L = w.L;
        battle.R = w.R;
        battle.player_gesture_L = w.pgL;
        battle.player_gesture_R = w.pgR;
        battle.player_spell_L = w.psL;
        battle.player_spell_R = w.psR;
        battle.player_target_L = w.ptL;
        battle.player_target_R = w.ptR;
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
    for (var i = 0, Ln = strs.length; i < Ln; ++i) {
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
            battle.monsters[m_owner].push({name:title,status:"",hp:title.substr(0, 1),owner:m_owner,icon:getMonsterIconByName(title),action:"m",action_idx:battle.actions.M.length,under_control:true});
            battle.actions.M.push({id:obj_id,target:"",old_target:"",under_control:true,owner:m_owner,status:""});
        }
    }
}

function setParaActions(paralyze, charm) {
    battle.paralyze = [];
    battle.charm = [];
    var i, Ln;
    if (paralyze !== "") {
        var tp = paralyze.split(";");
        for(i = 0, Ln = tp.length; i < Ln; ++i) {
            if (!tp[i]) {
                continue;
            }
            battle.paralyze.push(tp[i]);
            battle.actions.CP[tp[i]] = {h:"LH",g:"-",t:"P"};
        }
    }
    if (charm !== "") {
        var tc = charm.split(";");
        for(i = 0, Ln = tc.length; i < Ln; ++i) {
            if (!tc[i]) {
                continue;
            }
            battle.charm.push(tc[i]);
            battle.actions.CC[tc[i]] = {h:"LH",g:"-",t:"C"};
        }
    }
}

function parseParalyzedHands(arr) {
    battle.paralyzed_hand = {};
    var item;
    for (var i = 0, Ln = arr.length; i < Ln; ++i) {
        item = arr[i];
        battle.paralyzed_hand[item[0]] = item[1];
    }
}

function prepareBattle(raw_battle) {
    battle = {id:raw_battle.id,fire:raw_battle.fire,chat:raw_battle.chat,is_fdf:raw_battle.is_fdf,is_fc:raw_battle.is_fc,warlocks:[],elemental:{hp:0,type:"fire"},
        monsters:{},ngL:"",ngR:"",turn_num: raw_battle.turn_num,hint: raw_battle.hint};
    // L left  obj
    // R Right obj
    // C Chat  text
    // D Delay int 0 - none, 1 left, 2 right, -1 unavailable
    // P Permanent int 0 - none, 1 left, 2 right, -1 unavailable
    // F Fire int 0, 1, -1
    // M monsters arr of obj
    // CP - paralyze arr of obj by id
    // CC - paralyze arr of obj by id
    battle.actions = {L:{target:"Default"},R:{target:"Default"},C:battle.chat,D:-1,P:-1,F:-1,M:[],CP:{},CC:{}};
    setParaActions(raw_battle.paralyze, raw_battle.charm);
    parseTargets(raw_battle.targets);
    parseParalyzedHands(raw_battle.paralyzed_hand);

    var i, Ln;
    for (i = 0, Ln = raw_battle.monsters.length; i < Ln; ++i) {
        var m = raw_battle.monsters[i];
        m.is_elemental = !m.owner || (m.owner === "Nobody");
        m.allow_choose_target = false;
        if (m.is_elemental) {
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

        m.damage = getMonsterDamageByName(m.name);
        battle.monsters[m.owner].push(m);
        battle.actions.M.push({id:battle.targetsMap[m.name],target:m.target,old_target:m.target,under_control:true,owner:m.owner,name:m.name,
                                  status:m.status,d:m.damage,hp:m.hp});
    }

    for (i = 0, Ln = raw_battle.warlocks.length; i < Ln; ++i) {
        var w = raw_battle.warlocks[i];
        if (!battle.monsters[w.name]) {
            battle.monsters[w.name] = [];
        }
        prepareWarlock(w);
        battle.warlocks.unshift(w);
    }
    battle.actions.L.target = battle.player_target_L;
    battle.actions.L.g = battle.player_gesture_L;
    battle.actions.L.s = {n:battle.player_spell_L, gp: "", g:battle.player_gesture_L};
    battle.actions.R.target = battle.player_target_R;
    battle.actions.R.g = battle.player_gesture_R;
    battle.actions.R.s = {n:battle.player_spell_R, gp: "", g:battle.player_gesture_R};
    battle.player_name = battle.warlocks[0].name;
    battle.player_changed_mind = battle.warlocks[0].changed_mind;
    battle.enemy_name = battle.warlocks.length > 1 ? battle.warlocks[1].name : "Nobody";
    for (i = 0, Ln = battle.actions.M.length; i < Ln; ++i) {
        if (!battle.actions.M[i].target  || (battle.actions.M[i].target === "Nobody")) {
            battle.actions.M[i].target = battle.enemy_name;
            battle.actions.M[i].old_target = battle.enemy_name;
        }
        if (battle.actions.M[i].owner !== battle.player_name) {
            battle.actions.M[i].under_control = false;
        }
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
        arr_m.warlock_idx = i;
        var sprite = cWarlockObject.createObject(iWarlocks, {l_warlock: arr_m, l_ratio: mainWindow.ratioObject, l_IconInfoObj: cIconObject, x: 0, y: curr_y, height: G_WARLOCK_HEIGHT * mainWindow.ratioObject, width: battleWindow.width});
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

function prepareHint() {
    if (battle.hint && battle.hint.length > 0) {
        ltTutorial.tutorialData = battle.hint;
        ltTutorial.text = battle.hint[0];
        ltTutorial.visible = true;
    }
}

function prepareGUI() {
    prepareElemental();
    prepareWarlocks();
    prepareChat();
    prepareHint();
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
        post_request += "PARALYZE" + battle.paralyze[i] + "$" + actions.CP[battle.paralyze[i]].h + "#";
    }
    console.log("prepareOrder", "point6", post_request);
    for(i = 0, Ln = battle.charm.length; i < Ln; ++i) {
        pc_gv = actions.CC[battle.charm[i]].g;
        if (!pc_gv || (pc_gv === '')) {
            pc_gv = "-"
        }
        if (pc_gv === ">") {
            pc_gv = "&gt;"
        }
        post_request += "DIRECTHAND" + battle.charm[i] + "$" + actions.CC[battle.charm[i]].h + "#";
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

function getOrdersForReview(dictionary) {
    console.log("getOrdersForReview", JSON.stringify(battle.actions));
    var res = [];
    var actions = battle.actions;
    if (actions.C !== "") {
        res.push({type:"C",v:"Say: " + actions.C});
        console.log("getOrdersForReview", "point1", JSON.stringify(res));
    }

    if ((actions.L.g === "P") && actions.R.g === "P") {
        res.push({type:"LH",v:dictionary.getStringByCode("TitleAction_p"),c:"red"});
    }
    // gesture
    res.push({type:"LH",v:getTextForHandAction("LH", actions.L, battle.targetsMap, dictionary),c:"snow"});
    console.log("getOrdersForReview", "point2", JSON.stringify(res));
    res.push({type:"RH",v:getTextForHandAction("RH", actions.R, battle.targetsMap, dictionary),c:"snow"});
    console.log("getOrdersForReview", "point3", JSON.stringify(res));

    if (actions.D !== -1) {
        res.push({type:"D",v:getSpecActionText("D", actions.D, dictionary),c:"snow"});
        console.log("getOrdersForReview", "point4", JSON.stringify(res));
    }
    if (actions.P !== -1) {
        res.push({type:"P",v:getSpecActionText("P", actions.P, dictionary),c:"snow"});
        console.log("getOrdersForReview", "point5", JSON.stringify(res));
    }
    if (actions.F === 1) {
        res.push({type:"F",v:battle.fire,c:"snow"});
        console.log("getOrdersForReview", "point6", JSON.stringify(res));
    }
    var i, Ln, m_obj, pc_gv;
    for(i = 0, Ln = battle.paralyze.length; i < Ln; ++i) {
        res.push({type:"CP",v:getCharmActionText("CP", actions.CP[battle.paralyze[i]], battle.targetsMap[battle.paralyze[i]], dictionary), c:"snow"});
        console.log("getOrdersForReview", "point7", JSON.stringify(res));
    }
    for(i = 0, Ln = battle.charm.length; i < Ln; ++i) {
        console.log("getOrdersForReview before charm", i, Ln, battle.charm[i], JSON.stringify(actions.CC[battle.charm[i]]));
        res.push({type:"CC",v:getCharmActionText("CC", actions.CC[battle.charm[i]], battle.targetsMap[battle.charm[i]], dictionary), c:"snow"});
        console.log("getOrdersForReview", "point8", JSON.stringify(res));
    }
    for(i = 0, Ln = actions.M.length; i < Ln; ++i) {
        m_obj = actions.M[i];
        console.log("mt_label", JSON.stringify(m_obj));
        res.push({type:"M",v:getMonsterActionText(m_obj, m_obj.target, battle.targetsMap, dictionary),c:"snow",action_idx:i});
        console.log("getOrdersForReview", "point9", JSON.stringify(res));
    }
    return res;
}

function getCharmDataByAction(action_data) {
    var arr = action_data.action === "charmed" ? battle.actions.CC : battle.actions.CP;
    var id = battle.targetsMap[action_data.warlock_name];
    var res = arr[id];
    res.id = id;
    res.warlock_idx = action_data.warlock_idx;
    if (!!action_data.hand) {
        res.h = action_data.hand;
    }

    return res;
}

function setCharm(H, G) {
    console.log("BU.setCharm", H, G, JSON.stringify(battle.currentCharm));
    var T = battle.currentCharm.t;
    battle.actions["C" + T][battle.currentCharm.id] = {h:H,g:G,t:T};
}

function checkIsMonsterCharmed(monster) {
    if (monster.is_elemental) {
        return false;
    }
    if (monster.owner === battle.player_name) {
        return false;
    }
    var lp = battle.warlocks[0];
    if (!lp["charm_" + (battle.currentHand === "L" ? "left" : "right")]) {
        return false;
    }
    if (!battle.actions[battle.currentHand].s) {
        return false;
    }
    var spell = battle.actions[battle.currentHand].s;
    var res =  (spell.id === C_SPELL_CHARM_MONSTER) || (lp.amnesia > 0) || (lp.charmed > 0)|| (lp.confused > 0) || (lp.paralized > 0) || (lp.maladroit > 0);
    if (res) {
        monster.allow_choose_target = true;
        battle.actions.M[monster.action_idx].under_control = true;
    }
    return res;
}
