.import "battle_gui_utils.js" as BGU
.import QtQuick 2.0 as QQ

var battle = {};
var map_warlock_name_to_idx = {};
var cWarlockObject,cIconObject;
const G_WARLOCK_HEIGHT = 474;


function prepareWarlock(w) {
    w.monsters = battle.monsters[w.name];
    w.read_only = battle.read_only;
    w.print_g = BGU.preparePrintGestures(w.L, w.R, w.smcL, w.smcR);
    if (!w.id) {
        w.id = battle.targetsMap[w.name];
    }
    w.paralyzed_hand = battle.paralyzed_hand[w.id];
    if (!w.paralyzed_hand) {
        w.paralyzed_hand = "";
    }
    if (w.fh > 0) {
        w.paralyzed_hand = w.fh === 1 ? "LH" : "RH";
        if (battle.actions.CP[w.id]) {
            battle.actions.CP[w.id].h = w.paralyzed_hand;
        }
        if (battle.actions.CC[w.id]) {
            battle.actions.CC[w.id].h = w.paralyzed_hand;
        }
    }

    w.control_paralyze = battle.paralyze.indexOf(w.id) !== -1;
    w.control_charmed  = battle.charm.indexOf(w.id) !== -1;
    w.statusIcons = BGU.prepareStatusIcon(w);
    w.banked_spell = w.bank;//player && */(battle.fire !== "") ? battle.fire : "";
    w.turn_num = battle.turn_num;
    if (w.player) {
        battle.L = w.L;
        battle.R = w.R;
        battle.player_name = w.name;
        battle.player_gesture_L = w.pgL;
        battle.player_gesture_R = w.pgR;
        battle.player_spell_L = w.psL;
        battle.player_spell_R = w.psR;
        battle.player_target_L = w.ptL;
        battle.player_target_R = w.ptR;
        battle.ngL = battle.player_gesture_L;
        battle.ngR = battle.player_gesture_R;
        battle.player_has_bank = w.delay > 0;
        battle.player_has_permanency = w.permanency > 0;
        battle.player_under_control = (w.charmed > 0) || (w.paralized > 0);
        console.log("prepareWarlock", battle.player_under_control, JSON.stringify(w));
    }

    return w;
}

function parseTargets(targets_str) {
    battle.targetsList = [];
    battle.targetsMap = {};
    var strs = BGU.strToArr2D(targets_str, '#', ';', true);
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

function prepareTurnActionInfo(last_turn_hist) {
    console.log("battle_utils.prepareTurnActionInfo", JSON.stringify(last_turn_hist), JSON.stringify(battle.hint));
    var new_hint = [], i, Ln;
    for(i = 0, Ln = battle.hint.length; i < Ln; ++i) {
        new_hint.push({txt:battle.hint[i],color_bg:"#FEE2D6",actions:[]});
    }

    for(i = 0, Ln = last_turn_hist.length; i < Ln; ++i) {
        var new_action = [];
        if ((last_turn_hist[i].type === 0)) {
            // chat message
            if (battle.with_bot) {
                continue;
            }
            new_action.push({action:"icon",large_icon:"chat",small_icon:"",title:"",text:"",background_color:"#210430",border_color:"#E7FFFF"});
        } else if (last_turn_hist[i].type === 2) {
            continue;
        } else if (last_turn_hist[i].type === 1) {
            new_action = BGU.getMessageActionByRow(last_turn_hist[i], battle); // before change color
            if ((last_turn_hist[i].color === "#FF6666") || (last_turn_hist[i].color === "#FF8888")) {
                last_turn_hist[i].color = "#FEE2D6";
            } else {
                last_turn_hist[i].color = "#10C9F5";
            }
        }
        if (!last_turn_hist[i].font_size) {
            last_turn_hist[i].font_size = 21;
        }

        new_hint.push({color_bg:last_turn_hist[i].color,font_size:last_turn_hist[i].font_size,txt:BGU.replaceAll(last_turn_hist[i].txt, '&quot;', '"'),actions:new_action});
    }

    battle.hint = new_hint;
}

function prepareMonster(m) {
    m.is_elemental = !m.owner || (m.owner === "Nobody");
    m.allow_choose_target = false;
    if (m.is_elemental) {
        battle.elemental = m;
        battle.elemental.type = m.name.indexOf("Fire") !== -1 ? "fire" : "ice";
        battle.elemental.action = "m";
        return false;
    }

    if ((m.name.indexOf(":") !== -1) && (m.strength > 0)) {
        m.icon = "new_" + BGU.getMonsterIconByName(BGU.getMonsterNameByStrength(m.strength));
        if (m.icon.indexOf("elemental") !== -1) {
            m.icon = "new_summon";
        }
    } else {
        m.icon = BGU.getMonsterIconByName(m.name);
    }

    m.action = "m";
    m.action_idx = battle.actions.M.length;

    if (!battle.monsters[m.owner]) {
        battle.monsters[m.owner] = [];
    }

    m.damage = BGU.getMonsterDamageByName(m.name);

    m.enchantment_icon = "";
    if (m.status.indexOf("-") !== -1) {
        var status = m.status.toLowerCase();
        m.enchantment = "";
        m.enchantment_icon = "qrc:/res/target_default.png";
        for (var i = 0, Ln = BGU.icon_status_code.length; i < Ln; ++i) {
            if (status.indexOf(BGU.icon_status_code[i]) !== -1) {
                if (m.enchantment !== "") {
                    m.enchantment = "many";
                    break;
                } else {
                    m.enchantment = BGU.icon_status_code[i];
                }
            }
        }

        if (m.enchantment !== "many") {
            var e_code = BGU.icon_status_code_to_icon[m.enchantment];
            if (!e_code) {
                e_code = m.enchantment;
            }
            var tmp_spell = BGU.icon_status_spell[e_code];
            if (tmp_spell) {
                var tmp_icon = BGU.map_spell_to_icon[tmp_spell];
                if (tmp_icon) {
                    m.enchantment_icon = "qrc:/res/"+tmp_icon+".png";
                }
            }
        }
    }

    return true;
}

function prepareBattle(raw_battle) {
    battle = {id:raw_battle.id,size:0,fire:raw_battle.fire,chat:raw_battle.chat,is_fdf:raw_battle.is_fdf,is_fc:raw_battle.is_fc,warlocks:[],elemental:{hp:0,type:"fire"},
        monsters:{},ngL:"",ngR:"",turn_num: raw_battle.turn_num,hint: raw_battle.hint, msg: raw_battle.msg, battle_hist: raw_battle.battle_hist,
        battle_chat: raw_battle.battle_chat, with_bot: raw_battle.with_bot, read_only: raw_battle.read_only};
    // L left  obj
    // R Right obj
    // C Chat  text
    // D Delay int 0 - none, 1 left, 2 right, -1 unavailable
    // P Permanent int 0 - none, 1 left, 2 right, -1 unavailable
    // F Fire int 0, 1, -1
    // M monsters arr of obj
    // CP - paralyze arr of obj by id
    // CC - paralyze arr of obj by id
    battle.actions = {L:{target:"Default"},R:{target:"Default"},C:raw_battle.msg,D:-1,P:-1,F:-1,M:[],CP:{},CC:{}};
    setParaActions(raw_battle.paralyze, raw_battle.charm);
    parseTargets(raw_battle.targets);
    parseParalyzedHands(raw_battle.paralyzed_hand);

    var i, Ln;
    for (i = 0, Ln = raw_battle.monsters.length; i < Ln; ++i) {
        var m = raw_battle.monsters[i];
        m.read_only = battle.read_only;
        console.log("JS.prepareBattle.monsters", JSON.stringify(m));
        if (prepareMonster(m)) {
            battle.monsters[m.owner].push(m);
            battle.actions.M.push({id:battle.targetsMap[m.name],target:m.new_target,old_target:m.target,
                                   under_control:true,owner:m.owner,name:m.name,
                                   status:m.status,d:m.damage,hp:m.hp,action:"m",strength:m.damage,icon:m.icon});
        }
    }

    for (i = 0, Ln = raw_battle.warlocks.length; i < Ln; ++i) {
        var w = raw_battle.warlocks[i];
        if (!battle.monsters[w.name]) {
            battle.monsters[w.name] = [];
        }
        prepareWarlock(w);
        battle.warlocks.unshift(w);
        ++battle.size;
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
    //delete battle.monsters;

    prepareTurnActionInfo(raw_battle.last_turn_hist);

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
    BGU.cleanChildren(iWarlocks);

    if (!cIconObject) {
        cIconObject = Qt.createComponent("qrc:///qml/components/IconInfo.qml");
    }
    if (!cWarlockObject) {
        cWarlockObject = Qt.createComponent("qrc:///qml/game_components/Warlock.qml");
    }
    if (cWarlockObject.status === QQ.Component.Error) {
        console.log("Error loading component: " + cWarlockObject.errorString());
        return ;
    }
    if (cWarlockObject.status === QQ.Component.Ready) {
        console.log("component ready");
        finishPrepareWarlockList();
    } else {
        console.log("connect to component ready signal");
        cWarlockObject.statusChanged.connect(finishPrepareWarlockList);
    }
}

function finishPrepareWarlockList() {
    console.log("finishPrepareWarlockList", battleWindow.height, battleWindow.width, battle.turn_num);
    if (cWarlockObject.status === QQ.Component.Error || cWarlockObject.status !== QQ.Component.Ready) {
        console.log("Error loading component:", cWarlockObject.errorString());
        return ;
    }
    var curr_y = 0;
    var total_height = 0;
    for(var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        // https://github.com/Pz1c/WavingHands/issues/259
        if (!battle.warlocks[i] || (!battle.read_only && !battle.warlocks[i].active && (battle.monsters[battle.warlocks[i].name].length === 0))) {
            continue;
        }
        var arr_m = battle.warlocks[i];
        arr_m.warlock_idx = i;
        map_warlock_name_to_idx[arr_m.name] = i;
        //arr_m.turn_num = battle.turn_num;
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
    if (with_msg) {
        iiChat.source = "qrc:/res/chat_pimple.png";
    }
}

function prepareHint() {
    if (battle.hint && battle.hint.length > 0) {
        ltTutorial.tutorialData = battle.hint;
        ltTutorial.tutorialDataIdx = -1;
        showTutorialData(1, true);
        ltTutorial.visible = true;
    }
}

function prepareGUI() {
    prepareElemental();
    prepareWarlocks();
    prepareChat();
    // need wait before warlocks finish creating
    //prepareHint();
    bbSendOrders.text = battle.read_only ? "Back" : "Submit";
}

function applyBattle() {
    var raw_battle = mainWindow.gBattle;
    prepareBattle(raw_battle);
    mainWindow.gBattle = battle;
    prepareGUI();
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
    post_request += "LHS$"+BGU.getSpellNameForOrder(actions.L)+"#";
    post_request += "RHS$"+BGU.getSpellNameForOrder(actions.R)+"#";
    console.log("prepareOrder", "point3", post_request);
    post_request += "LHT$"+BGU.getSpellTargetForOrder(actions.L, battle.targetsMap)+"#";
    post_request += "RHT$"+BGU.getSpellTargetForOrder(actions.R, battle.targetsMap)+"#";
    console.log("prepareOrder", "point4", post_request, JSON.stringify(battle.targetsMap));

    var i, Ln, m_obj, pc_gv;
    for(i = 0, Ln = actions.M.length; i < Ln; ++i) {
        m_obj = actions.M[i];
        console.log("mt_label", JSON.stringify(m_obj));
        /*if (m_obj.target === m_obj.old_target) {
            continue;
        }*/
        var trg = battle.targetsMap[m_obj.target];
        if (!trg) {
            trg = "";
        } else {
            trg = BGU.replaceAll(trg, " ", "+");
        }

        post_request += m_obj.id + "$" + trg + "#";
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
        post_request += "DELAY$" + BGU.getHandByIdx(actions.D);
    }
    console.log("prepareOrder", "point8", post_request);
    if (actions.P !== -1) {
        post_request += "PERM$" + BGU.getHandByIdx(actions.P);
    }
    console.log("prepareOrder", "point9", post_request);
    if (actions.F === 1) {
        post_request += "FIRE$1#"
    }
    console.log("prepareOrder", "after", post_request);

    mainWindow.gameCore.sendOrders(post_request);
    mainWindow.processEscape();
}

function checkSummonHint(action) {
    console.log("checkSummonHint", JSON.stringify(action));
    if ((action.s.n.indexOf("Summon") === -1) || (action.s.n.indexOf("Elemental") !== -1)) {
        return false;
    }
    if (action.target === battle.player_name) {
        return false;
    }
    for(var i=0, Ln = battle.monsters[battle.player_name].length; i < Ln; ++i) {
        if (battle.monsters[battle.player_name][i].name === action.target) {
            return false;
        }
    }

    return true;
}

function checkMonsterHint(monster) {
    console.log("checkMonsterHint", JSON.stringify(battle.monsters));
    for(var i=0, Ln = battle.monsters[battle.player_name].length; i < Ln; ++i) {
        if (battle.monsters[battle.player_name][i].name === monster.target) {
            return true;
        }
    }

    return false;
}


function getOrdersForReview(dictionary) {
    console.log("getOrdersForReview", JSON.stringify(battle.actions));
    var res = [];
    var actions = battle.actions;

    if ((actions.L.g === "P") && (actions.R.g === "P")) {
        res.push({row_type:"0",type:"PP",v:dictionary.getStringByCode("TitleAction_p"),c:"red",icon:"RIP2",icon_text:"",icon_visible:true,icon_width:60});
        //res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_pp"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }

    if (actions.C !== "") {
        res.push({row_type:"0",type:"C",v:"Say: " + actions.C,c:"snow",icon:"chat",icon_text:"",icon_visible:true,icon_width:60});
        console.log("getOrdersForReview", "point1", JSON.stringify(res));
    }
    // gesture
    res.push({row_type:"0",type:"LH",v:BGU.getTextForHandAction("LH", actions.L, battle.targetsMap, dictionary, battle.completed_spell_L),
              c:"snow",icon:"g_" + BGU.getIconByGesture(actions.L.g),icon_text:"",icon_visible:true,icon_width:60});
    if ((actions.L.g === "C") && (actions.R.g !== "C")) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_clap"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    if (actions.L.target === "Nobody") {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_nobody"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    } else if (checkSummonHint(actions.L)) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_summon"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    console.log("getOrdersForReview", "point2", JSON.stringify(res));

    res.push({row_type:"0",type:"RH",v:BGU.getTextForHandAction("RH", actions.R, battle.targetsMap, dictionary, battle.completed_spell_R),
              c:"snow",icon:"g_" + BGU.getIconByGesture(actions.R.g),icon_text:"",icon_visible:true,icon_width:60});
    if ((actions.L.g !== "C") && (actions.R.g === "C")) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_clap"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    if (actions.R.target === "Nobody") {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_nobody"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    } else if (checkSummonHint(actions.L)) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_summon"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    console.log("getOrdersForReview", "point3", JSON.stringify(res));

    if ((actions.L.g === ">") && (actions.R.g === ">")) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_knifes"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }

    if (actions.D !== -1) {
        res.push({row_type:"0",type:"D",v:BGU.getSpecActionText("D", actions.D, dictionary),c:"snow",icon:"",icon_text:"",icon_visible:false,icon_width:0});
        console.log("getOrdersForReview", "point4", JSON.stringify(res));
    }
    if (actions.P !== -1) {
        res.push({row_type:"0",type:"P",v:BGU.getSpecActionText("P", actions.P, dictionary),c:"snow",icon:"",icon_text:"",icon_visible:false,icon_width:0});
        console.log("getOrdersForReview", "point5", JSON.stringify(res));
    }
    if (actions.F === 1) {
        res.push({row_type:"0",type:"F",v:battle.fire,c:"snow",icon:"",icon_text:"",icon_visible:false,icon_width:0});
        console.log("getOrdersForReview", "point6", JSON.stringify(res));
    }
    var i, Ln, m_obj, pc_gv;
    for(i = 0, Ln = battle.paralyze.length; i < Ln; ++i) {
        res.push({row_type:"0",type:"CP",v:BGU.getCharmActionText("CP", actions.CP[battle.paralyze[i]],
                  battle.targetsMap[battle.paralyze[i]], dictionary), c:"snow",icon:"paralized",icon_text:"",icon_visible:true,icon_width:60});
        console.log("getOrdersForReview", "point7", JSON.stringify(res));
    }
    for(i = 0, Ln = battle.charm.length; i < Ln; ++i) {
        console.log("getOrdersForReview before charm", i, Ln, battle.charm[i], JSON.stringify(actions.CC[battle.charm[i]]));
        res.push({row_type:"0",type:"CC",v:BGU.getCharmActionText("CC", actions.CC[battle.charm[i]], battle.targetsMap[battle.charm[i]], dictionary),
                     c:"snow",icon:"charmed",icon_text:"",icon_visible:true,icon_width:60});
        console.log("getOrdersForReview", "point8", JSON.stringify(res));
    }
    for(i = 0, Ln = actions.M.length; i < Ln; ++i) {
        m_obj = actions.M[i];
        console.log("m_obj", JSON.stringify(m_obj));
        if (!m_obj.under_control) {
            continue;
        }

        var obj = {row_type:"0",type:"M",v:BGU.getMonsterActionText(m_obj, m_obj.target, battle.targetsMap, dictionary),c:"snow",action_idx:i,icon:"",icon_text:"",icon_visible:true,icon_width:78};
        if (m_obj.name.indexOf(":") !== -1) {
            obj.icon = BGU.getMonsterIconBySummonHP(m_obj.hp);
            obj.icon_text = m_obj.name.substr(0, 1);
        } else {
            obj.icon = BGU.getMonsterIconByName(m_obj.name);
            obj.icon_text = m_obj.hp;
        }
        res.push(obj);
        if (m_obj.target === "Nobody") {
            res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_nobody"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
        } else if (m_obj.target === battle.player_name) {
            res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_monster1"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
        } else if (checkMonsterHint(m_obj)) {
            res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_monster2"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
        }

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
    //var lp = battle.warlocks[0];
    /*console.log("checkIsMonsterCharmed", lp.charm_left, lp.charm_right);
    if (!lp["charm_" + (battle.currentHand === "L" ? "left" : "right")]) {
        return false;
    }*/
    if (!battle.actions[battle.currentHand] || !battle.actions[battle.currentHand].s) {
        return false;
    }
    var spell = battle.actions[battle.currentHand].s;
    var res =  (spell.id === BGU.C_SPELL_CHARM_MONSTER) || ((spell.id === -1) && spell.is_charm_monster && (spell.is_charm_monster === 1))
            || battle.player_under_control;
    if (res) {
        monster.allow_choose_target = true;
        battle.actions.M[monster.action_idx].allow_choose_target = true;
        battle.actions.M[monster.action_idx].under_control = true;
    }
    return res;
}

function getIconByGesture(g) {
    return BGU.getIconByGesture(g);
}

function getIconStatusSpell(code) {
    return BGU.icon_status_spell[code];
}

function getIconBySpell(spell_code) {
    return BGU.map_spell_to_icon[spell_code];
}
