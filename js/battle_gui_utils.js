var icon_status_code = ["scared","confused","charmed","paralized","shield","coldproof","fireproof","poison","disease","amnesia","maladroit","mshield","delay",
                        "time_stop","haste","permanency","blindness","invisibility"];
var icon_status_code_to_icon = {"confused":"maladroit","time_stop":"time_stop"};
var icon_status_spell = {"scared":"SWD","confused":"DSF","charmed":"PSDF","paralized":"FFF","shield":"WWP","coldproof":"SSFP","fireproof":"WWFP","poison":"DWWFWD","disease":"DSFFFc",
                         "amnesia":"DPP","maladroit":"DSF","mshield":"WWS","delay":"DWSSSP","time_stop":"SPPc","haste":"PWPWWc","permanency":"SPFPSDW","blindness":"DWFFd","invisibility":"PPws"};

var map_spell_to_icon = {"SWD":"scared","DSF":"confused","PSDF":"charmed","FFF":"paralized","WWP":"shield","SSFP":"coldproof","WWFP":"fireproof",
            "DWWFWD":"poison","DSFFFc":"disease","DPP":"amnesia","DSF":"maladroit","WWS":"mshield","WPP":"mshield","DWSSSP":"delay","PWPWWc":"haste",
            "SPFPSDW":"permanency","DWFFd":"blindness","DWFWd":"blindness","PPws":"invisibility","cWSSW":"elemental_fire","cSWWS":"elemental_ice",
            "WFPSFW":"giant","FPSFW":"troll","PSFW":"ogre","SFW":"goblin","P":"shield","SPPc":"time_stop","SPPFD":"time_stop",
            "cDPW":"dispel_magic","cw":"magic_mirror", "DFFDD":"lightning_bolt","DFPW":"cure_heavy_wounds","DFW":"cure_light_wounds",
            "FSSDD":"fireball","PDWP":"remove_enchantment","PSDD":"charm_monster","PWPFSSSD":"finger_of_death",
            "SD":"magic_missile","SPFP":"anti_spell","SWWc":"fire_storm","WDDc":"clap_of_lightning","WFP":"cause_light_wounds",
            "WPFD":"cause_heavy_wounds","WSSc":"ice_storm", "p":"RIP2",">":"stab","DSFDFc":"disease","FDF":"paralysis","FDFD":"paralysis"};

var map_spell_name_to_gesture = {"Dispel Magic":["cDPW"],"Counter Spell":["WPP", "WWS"],"Magic Mirror":["cw"],"Summon Goblin":["SFW"],"Summon Ogre":["PSFW"],
    "Summon Troll":["FPSFW"],"Summon Giant":["WFPSFW"],"Summon Fire Elemental":["cWSSW"],"Summon Ice Elemental":["cSWWS"],
    "Haste":["PWPWWc"],"Time Stop":["SPPFD", "SPPc"],"Protection":["WWP"],"Resist Heat":["WWFP"],"Resist Cold":["SSFP"],
    "Paralysis":["FFF", "FDF", "FDFD"],"Amnesia":["DPP"],"Fear":["SWD"],"Confusion":["DSF"],"Maladroitness":["DSF"],"Charm Monster":["PSDD"],
    "Charm Person":["PSDF"],"Disease":["DSFFFc"],"Poison":["DWWFWD"],"Cure Light Wounds":["DFW"],"Cure Heavy Wounds":["DFPW"],"Anti-spell":["SPFP"],
    "Blindness":["DWFFd","DFWFd"],"Invisibility":["PPws"],"Permanency":["SPFPSDW"],"Delay Effect":["DWSSSP"],"Remove Enchantment":["PDWP"],
    "Shield":["P"],"Magic Missile":["SD"],"Cause Light Wounds":["WFP"],"Cause Heavy Wounds":["WPFD"],"Lightning Bolt":["DFFDD"],"Clap of Lightning":["WDDc"],
    "Fireball":["FSSDD"],"Finger of Death":["PWPFSSSD"],"Fire Storm":["SWWc"],"Ice Storm":["WSSc"],"Stab":[">"]};


var C_SPELL_DISPEL_MAGIC = 0;
var C_SPELL_SUMMON_ICE_ELEMENTAL = 1;
var C_SPELL_SUMMON_FIRE_ELEMENTAL = 2;
var C_SPELL_MAGIC_MIRROR = 3;
var C_SPELL_LIGHTNING_BOLT = 4;
var C_SPELL_CURE_HEAVY_WOUNDS = 5;
var C_SPELL_CURE_LIGHT_WOUNDS = 6;
var C_SPELL_BLINDNESS1 = 7;
var C_SPELL_AMNESIA = 8;
var C_SPELL_CONFUSION = 9;
var C_SPELL_DISEASE = 10;
var C_SPELL_BLINDNESS2 = 11;
var C_SPELL_DELAY_EFFECT = 12;
var C_SPELL_POISON = 13;
var C_SPELL_PARALYSIS = 14;
var C_SPELL_SUMMON_GIANT = 15;
var C_SPELL_SUMMON_TROLL = 16;
var C_SPELL_SUMMON_OGRE = 17;
var C_SPELL_SUMMON_GOBLIN = 18;
var C_SPELL_FIREBALL = 19;
var C_SPELL_SHIELD = 20;
var C_SPELL_REMOVE_ENCHANTMENT = 21;
var C_SPELL_INVISIBILITY = 22;
var C_SPELL_CHARM_MONSTER = 23;
var C_SPELL_CHARM_PERSON = 24;
var C_SPELL_FINGER_OF_DEATH = 25;
var C_SPELL_HASTE = 26;
var C_SPELL_MAGIC_MISSILE = 27;
var C_SPELL_ANTI_SPELL = 28;
var C_SPELL_PERMANENCY = 29;
var C_SPELL_TIME_STOP1 = 30;
var C_SPELL_TIME_STOP2 = 31;
var C_SPELL_RESIST_COLD = 32;
var C_SPELL_FEAR = 33;
var C_SPELL_FIRE_STORM = 34;
var C_SPELL_CLAP_OF_LIGHTNING = 35;
var C_SPELL_CAUSE_LIGHT_WOUNDS = 36;
var C_SPELL_CAUSE_HEAVY_WOUNDS = 37;
var C_SPELL_COUNTER_SPELL1 = 38;
var C_SPELL_ICE_STORM = 39;
var C_SPELL_RESIST_HEAT = 40;
var C_SPELL_PROTECTION = 41;
var C_SPELL_COUNTER_SPELL2 = 42;
var C_SPELL_SURRENDER = 43;
var C_SPELL_STAB = 44;
var C_SPELL_DISEASE_FDF = 45;
var C_SPELL_PARALYSIS_FDF = 46;
var C_SPELL_PARALYSIS_FDFD = 47;


function copyObject(from, to, except) {
    var check_exclude = except && Array.isArray(except) && (except.length > 0);
    for(var key in from) {
        if (check_exclude && except.include(key)) {
            continue;
        }

        to[key] = from[key];
    }
}

function getIconByGesture(G) {
    //if (G === ' ') {
        //G = '-';
    //}
    if (G === '>') {
        G = '_';
    }
    if (G === '?') {
        G = '=';
    }
    return G.toLowerCase();
}

function getFullIconPathByGesture(G) {
    return "qrc:/res/g_" + getIconByGesture(G) + ".png";
}

function getSpellIconByGesture(G) {
    if (map_spell_to_icon[G]) {
        return map_spell_to_icon[G];
    } else {
        return "stars_purple";
    }
}

function getFullSpellIconByGesture(G) {
    return "qrc:/res/" + getSpellIconByGesture(G) + ".png";
}

function preparePrintGestures(GL, GR, mscL, mscR, maxLength) {
    var res = [], item, ll, lr, start_idx = 1, Ln = GL.length;
    //GL = GL.toLowerCase();
    //GR = GR.toLowerCase();
    if (maxLength) {
        start_idx = Math.max(1, Ln - maxLength);
    }

    for(var i = start_idx; i < Ln; ++i) {
        ll = getIconByGesture(GL.substr(i, 1));
        lr = getIconByGesture(GR.substr(i, 1));
        item = {l:'g_'+ll, r: 'g_' + lr, lv: ll !== ' ', rv: lr !== ' ', la: i >= Ln - mscL, ra: i >= Ln - mscR};
        if (!item.lv) {
            item.l = "g__";
        }
        if (!item.rv) {
            item.r = "g__";
        }

        res.push(item);
    }
    return res;
}

function getMonsterIconByNameEx(name) {
    if (name.indexOf("Goblin") !== -1) {
        return "goblin";
    } else if (name.indexOf("Ogre") !== -1) {
        return "ogre";
    } else if (name.indexOf("Troll") !== -1) {
        return "troll";
    } else if (name.indexOf("Giant") !== -1) {
        return "giant";
    } else {
        return "summon";
    }
}

function getMonsterIconByName(name) {
    var res = getMonsterIconByNameEx(name);
    if ((res !== "summon") && (name.indexOf("(") !== -1)) {
        res = "new_" + res;
    }

    return res;
}

function getMonsterIconBySummonHP(HP) {
    switch(HP) {
    case 1: return "goblin";
    case 2: return "ogre";
    case 3: return "troll";
    case 4: return "giant";
    default: return "summon";
    }
}

function getMonsterDamageByName(name) {
    if (name.indexOf("Goblin") !== -1) {
        return 1;
    } else if (name.indexOf("Ogre") !== -1) {
        return 2;
    } else if (name.indexOf("Troll") !== -1) {
        return 3;
    } else if (name.indexOf("Giant") !== -1) {
        return 4;
    }else if (name.indexOf("Elemental") !== -1) {
        return 3;
    } else {
        return -1;
    }
}

function strToArr2D(str, sep1, sep2, skip_empty) {
    var res = [];
    var arr1 = str.split(sep1);
    for (var i = 0, Ln = arr1.length; i < Ln; ++i) {
        if (arr1[i] === '') {
            continue;
        }

        res.push(arr1[i].split(sep2));
    }
    return res;
}

function cleanChildren(component) {
    console.log(component.id + " cleanShildren: " + component.children.length)
    for(var i = component.children.length; i > 0; i--) {
        console.log("destroying: " + i)
        component.children[i-1].destroy();
    }
}

function prepareStatusIcon(w) {
    var res = [], code, val, icon_name, a;
    for (var i = 0, Ln = icon_status_code.length; i < Ln; ++i) {
        code = icon_status_code[i];
        if (!w[code] || (w[code] === 0)) {
            continue;
        }
        icon_name = icon_status_code_to_icon[code] ? icon_status_code_to_icon[code] : code;
        val = w[code] === 999 ? "∞" : w[code];
        a = {action: code, icon: icon_name, value: val, active: true, active_action: (w.control_paralyze && (code === "paralized")) || (w.control_charmed && (code === "charmed"))};
        if (a.active && (code === "paralized")) {
            a.lgL = w.lgL;
            a.lgR = w.lgR;
        }

        res.push(a);
    }

    return res;
}

function getSpellNameForOrder(action) {
    // strange gestures
    if ((action.g === ">") || (action.g === "?") || (action.g === "-")) {
        return "";
    }
    // default spell
    if (action.s.gp === "?") {
        return "";
    }

    if (!action.s.n) {
        return "";
    }

    return action.s.n.replace(" ", "+");
}

function getSpellNameForOrderReview(action) {
    // strange gestures
    if (action.g === ">") {
        return "Stab";
    }

    if ((action.g === "?") || (action.g === "-")) {
        return "Unknown spell";
    }
    // default spell
    if ((action.s.gp === "?") || (action.s.n === "Default")) {
        return "default spell";
    }

    return action.s.n;
}

function getSpellTargetForOrder(action, targetMap) {
    if (!action.target || !targetMap[action.target]) {
        return "";
    }
    return targetMap[action.target].replace(" ", "+");
}

function getSpellTargetForOrderReview(action, targetMap) {
    if (!action.target || !targetMap[action.target]) {
        return "default target";
    }
    if (action.target.indexOf(":") !== -1) {
        return "New monster (" + action.target.substr(0, 1) + ")";
    }

    return action.target;
}

// res.push({type:"RH",g:actions.R.g,s:getSpellNameForOrderReview(actions.R),t:getSpellTargetForOrderReview(actions.R, battle.targetsMap)});
function getTextForHandAction(hand, action, targetMap, dict, exist_completed_spell) {
    var spell_name = getSpellNameForOrderReview(action);
    var target_name = getSpellTargetForOrderReview(action, targetMap);
    var res;
    if ((spell_name === "default spell") && (target_name === "default target") && (exist_completed_spell === 0)) {
        if (hand === "LH") {
            res = "Left hand gesture";
        } else {
            res = "Right hand gesture";
        }
    } else {
        res = "cast " + spell_name + "<br>at " + target_name;
    }
    return res;
}

function getSpecActionText(type, action, dict) {
    return dict.getStringByCode("TitleAction_" + type) + getHandTitleByIdx(action);
}

function getCharmActionText(type, action, target, dict) {
    return dict.getStringByCode("TitleAction_" + type).replace("%1", getHandTitleByIdx(action.h)).replace("%2", action.g) + target;
}

function getMonsterActionText(action, target, targetMap, dict) {
    var name = targetMap[action.id];
    if (!target) {
        target = "Default";
    }
    if (name.indexOf(":") !== -1) {
        name = "New monster (" + action.id.substr(0, 1) + ")";
    }

    var t = dict.getStringByCode("TitleAction_M").replace("%1", name).replace("%2", target);

    return t;
}

function getHandTitleByIdx(idx) {
    switch(idx) {
        case 0: return "None";
        case 1:
        case "LH": return "Left hand";
        case 3: return "Two-hand";
        default: return "Right hand";
    }
}


function getHandByIdx(idx) {
    switch(idx) {
        case 0: return "#";
        case 1: return "LH#";
        default: return "RH#";
    }
}

function getMonsterNameByStrength(Strength) {
    switch(Strength) {
    case 1: return "Goblin";
    case 2: return "Ogre";
    case 3: return "Troll";
    case 4: return "Giant";
    default: return "Unknown";
    }
}

function replaceAll(str, find, replace) {
  return str.replace(new RegExp(find, 'g'), replace);
}

function parseSpellByText(txt) {
    var arr = txt.split(" ");
    var walock_name;
    var spell_name = "";
    var target_name = "";
    var at_found = false;
    for (var i = 0, Ln = arr.length; i < Ln; ++i) {
        if (arr[i] === "at") {
            at_found = true;
            continue;
        } else if (arr[i] === "casts") {
            continue;
        } else if (!walock_name) {
            walock_name = arr[i];
        } else if (!at_found) {
            spell_name += arr[i] + " ";
        } else if (at_found) {
            target_name += arr[i].replace(".", "") + " ";
        }
    }
    target_name = target_name.trim();
    if (target_name === "himself") {
        target_name = walock_name;
    }

    spell_name = spell_name.trim();

    console.log("parseSpellByText", txt, walock_name, spell_name, target_name);
    return {warlock:walock_name,spell:spell_name,target:target_name};
}

function checkIsSpellPossibeForWarlock(Gesture, Left, Right) {
    var res = [0, 0];
    console.log("checkIsSpellPossibeForWarlock", Gesture, Left, Right);
    var gl = Gesture.length;
    var L = replaceAll(Left, " ", "");
    var R = replaceAll(Right, " ", "");
    L = L.substr(L.length - gl);
    R = R.substr(R.length - gl);
    var not_found;
    for (var i = gl - 1; i >= 0; --i) {
        not_found = true;
        var g = Gesture.substr(i, 1);
        var gU = g.toUpperCase();
        var Lg = L.substr(i, 1);
        var Rg = R.substr(i, 1);
        var need_both = g !== gU;
        console.log("checkIsSpellPossibeForWarlock", g, need_both, L, R, Lg, Rg, i);
        if (need_both) {
            if ((Lg === gU) && (Rg === gU)) {
                res = [gl - i, gl - i];
                not_found = false;
            } else {
                return [0, 0];
            }
        } else {
            if ((Lg === gU) && ((i === gl - 1) || (res[0] === gl - i - 1))) {
                res[0] = gl - i;
                not_found = false;
            }
            if ((Rg === gU) && ((i === gl - 1) || (res[1] === gl - i - 1))) {
                res[1] = gl - i;
                not_found = false;
            }
        }
        if (not_found) {
            return [0, 0];
        }
    }

    return res;
}

function higlightWarlockGestureBySpell(warlock, spell_obj) {
    var arr_g = map_spell_name_to_gesture[spell_obj.spell];
    console.log("higlightWarlockGestureBySpell", spell_obj.spell, JSON.stringify(arr_g));
    if (!arr_g) {
        return {action:"none"};
    }
    for (var i = 0, Ln = arr_g.length; i < Ln; ++i) {
        var lrsl = checkIsSpellPossibeForWarlock(arr_g[i], warlock.L, warlock.R);
        if ((lrsl[0] !== 0) || (lrsl[1] !== 0)) {
            return {action:"highlight",warlock_name:warlock.name,object_type:"warlock",object:"gestures",data:preparePrintGestures(warlock.L, warlock.R, lrsl[0], lrsl[1], 5)}
        }
    }

    return {action:"none"};
}

function getMessageActionBySpell(txt, battle) {
    var res = [];
    var spell_obj = parseSpellByText(txt);
    var target_found = false;
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        if (battle.warlocks[i].name === spell_obj.warlock) {
            res.push(higlightWarlockGestureBySpell(battle.warlocks[i], spell_obj));
        } else {
            res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].L, battle.warlocks[i].R, 0, 0, 5)});
        }
        if (battle.warlocks[i].name === spell_obj.target) {
            res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"hp",data:[],color:spell_obj.target === spell_obj.warlock ? "#10C9F5" : "#FEE2D6"});
            target_found = true;
        }
        if (!target_found) {
            var w = battle.warlocks[i];
            for (var j = 0, LnJ = w.monsters; j < LnJ; ++j) {
                var m = w.monsters[j];
                if (m.name === spell_obj.target) {
                    res.push({action:"highlight",warlock_name:m.name,object_type:"monster",object:m.name,data:[],color:"#FEE2D6"});
                    target_found = true;
                    break;
                }
            }
        }
    }


    console.log("getMessageActionBySpell", JSON.stringify(spell_obj), JSON.stringify(res));
    return res;
}

function parseSimpleAttack(txt, stop_word) {
    var prepare_txt = stop_word ? txt.replace(stop_word, "attacks") : txt;
    var arr = replaceAll(prepare_txt.replace(",", " #").replace("for", "#"), '  ', ' ').split(" ");

    var aggressor = arr[0];
    var target = "";
    var target_start = false;

    for(var i = 1, Ln = arr.length; i < Ln; ++i) {
        if (!arr[i]) {
            continue;
        }
        if (arr[i] === 'attacks') {
            target_start = true;
            continue;
        }
        if (arr[i] === '#') {
            break;
        }
        if (target_start) {
            if (target !== "") {
                target += ' ';
            }
            target += arr[i];
        } else {
            aggressor += " " + arr[i];
        }
    }
    var res = {aggressor:aggressor,target:target,success:txt.indexOf("damage") !== -1 ? 1 : 0};
    console.log("parseSimpleAttack", JSON.stringify(arr), JSON.stringify(res));
    return res;
}

function parseAttackByText(txt) {
    var res = {};
    if (txt.indexOf(" attacks ") !== -1) {
        res = parseSimpleAttack(txt);
    } else if (txt.indexOf(" swings wildly ") !== -1) {
        res = parseSimpleAttack(txt, "swings wildly for");
    } else if (txt.indexOf("tries to attack") !== -1) {
        res = parseSimpleAttack(txt, "tries to attack");
    }

    //res = {aggressor:aggressor,target:target,success:txt.indexOf("damage") !== -1 ? 1 : 0};
    console.log("parseAttackByText", txt, JSON.stringify(res));
    return res;
}

function getMessageActionByAttack(txt, battle) {
    var res = [];
    var attack_obj = parseAttackByText(txt);
    var aggressor_found = false, target_found = false, w, m, j, LnJ;
    if (attack_obj.aggressor) {
        for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
            //console.log("getMessageActionByAttack", i, Ln, battle.warlocks[i].name);
            if (!aggressor_found) {
                if (battle.warlocks[i].name === attack_obj.aggressor) {
                    res.push(higlightWarlockGestureBySpell(battle.warlocks[i], {warlock:attack_obj.aggressor,spell:"Stab",target:attack_obj.target}));
                    aggressor_found = true;
                } else {
                    w = battle.warlocks[i];
                    for (j = 0, LnJ = w.monsters; j < LnJ; ++j) {
                        m = w.monsters[j];
                        if (m.name === attack_obj.aggressor) {
                            res.push({action:"highlight",warlock_name:m.name,object_type:"monster",object:m.name,data:[],color:"#10C9F5"});
                            aggressor_found = true;
                            break;
                        }
                    }
                }
            }

            if (!target_found) {
                if (attack_obj.success && (battle.warlocks[i].name === attack_obj.target)) {
                    res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"hp",data:[],color:attack_obj.target === attack_obj.aggressor ? "#10C9F5" : "#FEE2D6"});
                } else {
                    w = battle.warlocks[i];
                    for (j = 0, LnJ = w.monsters; j < LnJ; ++j) {
                        m = w.monsters[j];
                        if (m.name === attack_obj.target) {
                            res.push({action:"highlight",warlock_name:m.name,object_type:"monster",object:m.name,data:[],color:attack_obj.target === attack_obj.aggressor ? "#10C9F5" : "#FEE2D6"});
                            target_found = true;
                            break;
                        }
                    }
                }
            }

            if (aggressor_found && target_found) {
                break;
            }
        }
    }

    console.log("getMessageActionByAttack", JSON.stringify(attack_obj), JSON.stringify(res));
    return res;
}

function getMessageActionByRow(row, battle) {
    var res = [];
    if (row.txt.indexOf(" casts ") !== -1) {
        // process spell casting
        return getMessageActionBySpell(row.txt, battle);
    } else if (row.color === "#FF6666") {
        // TODO process dies messages
    } else {//if (row.txt.indexOf(" attacks ") !== -1) {
        // process attack message
        return getMessageActionByAttack(row.txt, battle);
    }

    return res;
}
