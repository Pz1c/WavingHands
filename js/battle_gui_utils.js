var icon_status_code = ["scared","confused","charmed","paralized","shield","coldproof","fireproof","poison","disease","amnesia","maladroit","mshield","delay",
                        "time_stop","haste","permanency","blindness","invisibility"];
var icon_status_code_to_icon = {"confused":"maladroit","time_stop":"time_stop"};
var icon_status_spell = {"scared":"SWD","confused":"DSF","charmed":"PSDF","paralized":"FFF","shield":"WWP","coldproof":"SSFP","fireproof":"WWFP","poison":"DWWFWD","disease":"DSFFFc",
                         "amnesia":"DPP","maladroit":"DSF","mshield":"WWS","delay":"DWSSSP","time_stop":"SPPc","haste":"PWPWWc","permanency":"SPFPSDW","blindness":"DWFFd","invisibility":"PPws"};

var map_spell_to_icon = {"SWD":"scared","DSF":"confused","PSDF":"charmed","FFF":"paralized","WWP":"shield","SSFP":"coldproof","WWFP":"fireproof",
            "DWWFWD":"poison","DSFFFc":"disease","DPP":"amnesia","DSF":"maladroit","WWS":"mshield","WPP":"mshield","DWSSSP":"delay","PWPWWc":"haste",
            "SPFPSDW":"permanency","DWFFd":"blindness","DFWFd":"blindness","PPws":"invisibility","cWSSW":"elemental_fire","cSWWS":"elemental_ice",
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
var arr_distruption_spell = ["Amnesia", "Paralysis", "Confusion", "Fear", "Maladroitness", "Charm Monster", "Charm Person", "Anti-spell"];

// https://github.com/Pz1c/WavingHands/issues/227
// An enchantment cast at a warlock lights his status icon up in the turn news, not his heart. When
// the icon is not in his end-of-turn status (a one-turn shield, a spell a Counter Spell absorbed)
// the news shows a temporary one, see prepareNewsTempIcons. mirror and dispel are news-only codes.
var map_spell_name_to_status = {"Shield":"shield","Protection":"shield","Counter Spell":"mshield","Resist Heat":"fireproof","Resist Cold":"coldproof",
    "Paralysis":"paralized","Amnesia":"amnesia","Fear":"scared","Confusion":"confused","Maladroitness":"maladroit","Charm Person":"charmed",
    "Disease":"disease","Poison":"poison","Blindness":"blindness","Invisibility":"invisibility","Haste":"haste","Time Stop":"time_stop",
    "Delay Effect":"delay","Permanency":"permanency","Magic Mirror":"mirror","Dispel Magic":"dispel"};
var map_news_status_to_icon = {"mirror":"magic_mirror","dispel":"dispel_magic"};
var arr_harmful_status_spell = arr_distruption_spell.concat(["Disease", "Poison", "Blindness"]);

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
        if (check_exclude && (except.indexOf(key) !== -1)) {
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

// icon name for a spell gesture code, undefined when there is no dedicated icon
function getIconBySpell(spell_code) {
    return map_spell_to_icon[spell_code];
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
    } else if ((name.indexOf("Fire") !== -1) && (name.indexOf("Elemental") !== -1)) {
        return "elemental_fire";
    } else if ((name.indexOf("Ice") !== -1) && (name.indexOf("Elemental") !== -1)) {
        return "elemental_ice";
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
    var spell_fail = "";
    if (txt.indexOf("but misses due to invisibility") !== -1) {
        spell_fail = "Invisibility";
    } else if (txt.indexOf("but misses due to bl") !== -1) {
        spell_fail = "Blindness";
    }

    console.log("parseSpellByText", txt, walock_name, spell_name, target_name, spell_fail);
    return {warlock:walock_name,spell:spell_name,target:target_name,fail:spell_fail};
}

function checkIsSpellPossibeForWarlock(Gesture, Left, Right) {
    var res = [0, 0];
    console.log("checkIsSpellPossibeForWarlock", Gesture, Left, Right);
    var gl = Gesture.length;
    var L = replaceAll(Left, " ", "");
    var R = replaceAll(Right, " ", "");
    L = L.substr(L.length - gl);
    R = R.substr(R.length - gl);
    //console.log("checkIsSpellPossibeForWarlock", Gesture, L, R);
    var not_found;
    for (var i = gl - 1; i >= 0; --i) {
        not_found = true;
        var g = Gesture.substr(i, 1);
        var gU = g.toUpperCase();
        var Lg = L.substr(i, 1);
        var Rg = R.substr(i, 1);
        var need_both = g !== gU;
        console.log("checkIsSpellPossibeForWarlock", g, need_both, L, R, Lg, Rg, i, JSON.stringify(res));
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
            } else {
                res[0] = 0;
            }

            if ((Rg === gU) && ((i === gl - 1) || (res[1] === gl - i - 1))) {
                res[1] = gl - i;
                not_found = false;
            } else {
                res[1] = 0;
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

function getSpellIconActionBySpell(spell_obj) {
    //console.log("getSpellIconActionBySpell", JSON.stringify(spell_obj));
    var res = {action:"icon",large_icon:"",small_icon:spell_obj.fail.toLowerCase(),title:"",text:"",background_color:"#210430",border_color:"#FEE2D6"};
    var arr_g = map_spell_name_to_gesture[spell_obj.spell];
    res.large_icon = map_spell_to_icon[arr_g[0]];
    if (spell_obj.spell.indexOf("Cure") !== -1) {
        res.background_color = "#0654C0";
        res.border_color = "#210430";
    } else {
        var arr_dmg_spell = ['>','SD','WFP','WPFD','WDDc','DFFDD','FSSDD','PWPFSSSD'];
        for (var i = 0, Ln = arr_g.length; i < Ln; ++i) {
            if (arr_dmg_spell.indexOf(arr_g[i]) !== -1) {
                res.background_color = "#FEE2D6";
                res.border_color = "#210430";
                break;
            }
        }
    }

    return res;
}

// https://github.com/Pz1c/WavingHands/issues/227
// A Shield, a Counter Spell or a Dispel Magic cast at somebody earlier in the turn decides which
// defense icon a deflected attack or an absorbed spell at him gets later in the turn news.
function noteDefense(target, spell_obj) {
    if (spell_obj.fail !== "") {
        return;
    }
    switch (spell_obj.spell) {
    case "Shield":
    case "Protection":
        target.defended_by = "shield";
        break;
    case "Counter Spell":
        target.defended_by = "counter";
        break;
    case "Dispel Magic":
        target.defended_by = "dispel";
        break;
    }
}

// the warlock or the monster with that name in the current battle, null when there is none
function findBattleObject(battle, name) {
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        if (w.name === name) {
            return w;
        }
        var arr_m = getNewsMonsters(w);
        for (var j = 0, LnJ = arr_m.length; j < LnJ; ++j) {
            if (arr_m[j].name === name) {
                return arr_m[j];
            }
        }
    }
    return null;
}

// defense.png / defense_by_counter.png / defense_by_dispell.png by what protected the target this turn;
// by_counter is what the turn text itself says when no spell cast at the target was seen
function getDefenseIcon(target, by_counter) {
    var by = target && target.defended_by ? target.defended_by : (by_counter ? "counter" : "shield");
    switch (by) {
    case "counter": return "defense_by_counter";
    case "dispel":  return "defense_by_dispell";
    default:        return "defense";
    }
}

// 1 to 4 points of damage have their own heart icon (1damage.png ... 4damage.png) that shows the
// amount itself, more keeps the heart with the number on it
function applyDamageIcon(icon_action, damage) {
    var d = parseInt(damage, 10);
    if ((d >= 1) && (d <= 4)) {
        icon_action.large_icon = d + "damage";
        icon_action.text = "";
    }
}

function getMessageActionBySpell(obj, battle) {
    var res = [];
    var spell_obj = obj.obj;
    var target_found = false;
    // https://github.com/Pz1c/WavingHands/issues/227
    // an enchantment lights the target's status icon up, a summon the new monster (prepareNewsMonsters
    // paired the rows), anything else the target's heart
    var status_code = map_spell_name_to_status[spell_obj.spell];
    obj.txt = spell_obj.spell;
    obj.font_size = 42;
    if (spell_obj.fail !== "") {
        obj.txt += " fail";
        obj.font_size = 28;
    }

    res.push(getSpellIconActionBySpell(spell_obj));
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        if (battle.warlocks[i].name === spell_obj.warlock) {
            // hightlight spell gestures
            var hga = higlightWarlockGestureBySpell(battle.warlocks[i], spell_obj);
            if (hga.action !== "none") {
                res.push(hga);
            } else {
                res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].L, battle.warlocks[i].R, 0, 0, 5)});
            }

            //https://github.com/Pz1c/WavingHands/issues/312
            //res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"hp",data:[],color:spell_obj.target === battle.warlocks[i].name ? "#10C9F5" : "#FEE2D6"});
        } else {
            // switch off hightlighting to all other warlocks gestures
            res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].L, battle.warlocks[i].R, 0, 0, 5)});
        }
        if (battle.warlocks[i].name === spell_obj.target) {
            if (spell_obj.spell === "Cause Heavy Wounds") {
                battle.warlocks[i].got_heavy_wounds = true;
            }
            noteDefense(battle.warlocks[i], spell_obj);
            if (arr_distruption_spell.indexOf(spell_obj.spell) !== -1) {
                if (!battle.warlocks[i].arr_distruption) {
                    battle.warlocks[i].arr_distruption = [];
                }
                battle.warlocks[i].arr_distruption.push(spell_obj.spell);
            }

            if (status_code) {
                res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"status",object:status_code,data:[],role:"target",color:"#10C9F5"});
            } else if (!obj.summon) {
                res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"hp",data:[],color:"#10C9F5"});
            }
            target_found = true;
        }
        if (!target_found) {
            var w = battle.warlocks[i];
            var arr_m = getNewsMonsters(w);
            for (var j = 0, LnJ = arr_m.length; j < LnJ; ++j) {
                var m = arr_m[j];
                if (m.name === spell_obj.target) {
                    if (spell_obj.spell === "Cause Heavy Wounds") {
                        m.got_heavy_wounds = true;
                    }
                    noteDefense(m, spell_obj);
                    if (arr_distruption_spell.indexOf(spell_obj.spell) !== -1) {
                        if (!m.arr_distruption) {
                            m.arr_distruption = [];
                        }
                        m.arr_distruption.push(spell_obj.spell);
                    }
                    res.push({action:"highlight",warlock_name:w.name,object_type:"monster",object:m.name,data:[],role:"target",color:"#FEE2D6"});
                    target_found = true;
                    break;
                }
            }
        }
        //if (target_found) {
        //    break;
        //}
    }


    if (obj.summon) {
        // the monster this cast brings (a temporary tile when it is gone at the end of the turn)
        res.push({action:"highlight",warlock_name:obj.summon.owner,object_type:"monster",object:obj.summon.key,data:[],role:"target",color:"#10C9F5"});
    }

    //console.log("getMessageActionBySpell", JSON.stringify(spell_obj), JSON.stringify(res));
    return res;
}

function parseSimpleAttack(txt, stop_word) {
    var prepare_txt = stop_word ? txt.replace(stop_word, "attacks") : txt;
    var arr = replaceAll(prepare_txt.replace(",", " #").replace("for", "#").replace(".", " ."), '  ', ' ').split(" ");

    var damage = 0;
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
            if (arr[i + 2] === "damage") {
                damage = arr[i + 1];
            }

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
    var res = {aggressor:aggressor,target:target,damage:damage,success:damage > 0 ? 1 : 0,shield:txt.indexOf("shield") !== -1 ? 1 : 0};
    //console.log("parseSimpleAttack", JSON.stringify(arr), JSON.stringify(res));
    return res;
}

function parseStrangeAttack(txt, battle) {
    var res = {aggressor:"",target:"",spell:"",damage:0,shield:false,mirror:false,counter_spell:false};
    if (txt.indexOf("fizzles slightly") !== -1) {
        res.counter_spell = true;
        txt = txt.replace("shield fizzles slightly", "is hit by a Anti-spell");
    } else if (txt.indexOf("Tiny holes in") !== -1) {
        res.counter_spell = true;
        txt = txt.replace("Tiny holes in ", "").replace("shield are sealed up", "is hit by a Cure Light Wounds");
    } else if (txt.indexOf("Scales start to grow over") !== -1) {
        txt = txt.replace("Scales start to grow over ", "").replace("eyes!", "is hit by a Blindness");
    } else if (txt.indexOf("is healed") !== -1) {
        txt = txt.replace("is healed.", "is hit by a Cure Light Wounds");
    } else if (txt.indexOf("shield blurs for a moment") !== -1) {
        res.counter_spell = true;
        txt = txt.replace("shield blurs for a moment", "is hit by a Distruption");
    } else if (txt.indexOf("A magic missile") !== -1) {
        res.shield = true;
        txt = txt.replace("A magic missile bounces off ", "").replace("shield", "is hit by a Magic Missile");
    } else if (txt.indexOf("Holes open up in") !== -1) {
        res.counter_spell = true;
        txt = txt.replace("Holes open up in ", "").replace("shield, but then close up again.", "is hit by a Cause Light Wounds for 2 damage");
    } else if (txt.indexOf("Wounds appear") !== -1) {
        txt = txt.replace("Wounds appear all over ", "").replace("body!", "is hit by a Cause Light Wounds for 2 damage");
    } else if (txt.indexOf("reflected") !== -1) {
        var idx1 = txt.indexOf(" spell");
        res.spell = txt.substr(4, idx1 - 4).trim();
        //console.log("parseStrangeAttack magic mirror", res.spell);
        idx1 = txt.indexOf("from") + 5;
        var idx2 = txt.indexOf("'s", idx1);
        txt = txt.substr(idx1, idx2 - idx1) + " is hit by a " + res.spell;
        res.spell = "";
        res.mirror = true;
    }


    var arr = replaceAll(txt, ",", " semicolon").replace(".", " dot").replace("is hit by a", "attack_spell")
            .replace("!", " !").replace("'s", " ").split(" ");
    //console.log("parseStrangeAttack", arr);
    var target_finished = false, spell_started = false, spell_finished = false;
    for(var i = 0, Ln = arr.length; i < Ln; ++i) {
        if (arr[i] === "") {
            continue;
        }

        var word_not_upper = arr[i].charAt(0).toUpperCase() !== arr[i].charAt(0);
        target_finished = target_finished || word_not_upper;
        if (!target_finished || (i === 0)) {
            res.target += " " + arr[i];
            continue;
        }
        if (arr[i] === "attack_spell") {
            spell_started = true;
            continue;
        }
        spell_finished = spell_started && (spell_finished || word_not_upper);
        if (spell_started && !spell_finished) {
            res.spell += " " + arr[i];
            continue;
        }

        if ((arr[i] === "damage") && (arr[i - 2] === "for")) {
            res.damage = arr[i - 1];
        }

        if (!spell_started && ((arr[i] === "coordination") || (arr[i] === "maladroit") || (arr[i] === "trips"))) {
            res.spell = battle.maladroit ? "Maladroitness" : "Confusion";
        }

        if (!spell_started && ((arr[i] === "intrigued") || (arr[i] === "charmed"))) {
            res.spell = "Charm Person";
        }

        if (!spell_started && ((arr[i] === "glassy-eyed") || ((arr[i] === "instincts") && (arr[i - 1] === "baser")))) {
            res.spell = "Charm Monster";
        }

        if (!spell_started && ((arr[i] === "stiffen") || (arr[i] === "paralysed") ||
                               ((arr[i] === "move") && (arr[i - 1] === "can't")))) {
            res.spell = "Paralysis";
        }

        if (!spell_started && ((arr[i] === "fear") || (arr[i] === "scared"))) {
            res.spell = "Fear";
        }

        if (!spell_started && ((arr[i] === "blank") || (arr[i] === "forgets"))) {
            res.spell = "Amnesia";
        }

        if (!spell_started && ((arr[i] === "sick") || (arr[i] === "nauseous") || (arr[i] === "pale")
                               || (arr[i] === "breathing") || (arr[i] === "feverishly") || (arr[i] === "weakly")
                               || (arr[i] === "coughing"))) {
            res.spell = "Disease";
        }

        if (!spell_started && ((arr[i] === "shimmering") && (arr[i - 1] === "thick"))) {
            res.spell = "Protection";
        }

        if (!spell_started && ((arr[i] === "shimmering") && (arr[i - 1] !== "thick"))) {
            res.spell = "Shield";
        }

        if (!spell_started && ((arr[i] === "glowing") || (arr[i] === "par alysed"))) {
            res.spell = "Counter Spell";
        }

        if (!spell_started && ((arr[i] === "shimmer") || (arr[i] === "par alysed"))) {
            res.spell = "Invisibility";
        }

        if (!spell_started && (((arr[i] === "sparkling") && (arr[i + 1] === "frost")) )) {
            res.spell = "Resist Heat";
        }

        if (!spell_started && (((arr[i] === "warm") && (arr[i + 1] === "glow")) )) {
            res.spell = "Resist Cold";
        }

        if (!spell_started && (((arr[i] === "grounded") && (arr[i - 2] === "energies")) )) {
            res.spell = "Remove Enchantment";
            //res.counter_spell = true;
        }

        if (!spell_started && (((arr[i] === "banked") && (arr[i - 2] === "cast")) )) {
            res.spell = "Delay Effect";
            //res.counter_spell = true;
        }
    }

    res.target = res.target.trim();
    res.spell = res.spell.trim();

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
    } else {
        // message like "Galbarad starts to lose coordination" or "Smelly Goblin is hit by a Magic Missile, for 1 damage."
        // moved to getMessageActionByOther
        // res = parseStrangeAttack(txt);
    }

    //res = {aggressor:aggressor,target:target,success:txt.indexOf("damage") !== -1 ? 1 : 0};
    //console.log("parseAttackByText", txt, JSON.stringify(res));
    return res;
}

function getMessageActionByAttack(attack_obj, battle) {
    var res = [];
    var icon_action = {action:"icon",large_icon:"",small_icon:"",title:"",text:"",background_color:"#210430",border_color:"#FEE2D6"};
    //var attack_obj = parseAttackByText(txt);
    var aggressor_found = attack_obj.aggressor === "", target_found = attack_obj.target === "", w, m, j, LnJ, arr_m;
    if (attack_obj.aggressor || attack_obj.target) {
        for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
            //console.log("getMessageActionByAttack", i, Ln, battle.warlocks[i].name);
            if (battle.warlocks[i].name !== attack_obj.aggressor) {
                res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].L, battle.warlocks[i].R, 0, 0, 5)});
            }

            if (!aggressor_found) {
                if (battle.warlocks[i].name === attack_obj.aggressor) {
                    res.push(higlightWarlockGestureBySpell(battle.warlocks[i], {warlock:attack_obj.aggressor,spell:"Stab",target:attack_obj.target}));
                    aggressor_found = true;
                } else {
                    w = battle.warlocks[i];
                    arr_m = getNewsMonsters(w);
                    for (j = 0, LnJ = arr_m.length; j < LnJ; ++j) {
                        m = arr_m[j];
                        if (m.name === attack_obj.aggressor) {
                            res.push({action:"highlight",warlock_name:w.name,object_type:"monster",object:m.name,data:[],role:"actor",color:"#10C9F5"});
                            aggressor_found = true;
                            break;
                        }
                    }
                }
            }

            if (!target_found) {
                if (attack_obj.success && (battle.warlocks[i].name === attack_obj.target)) {
                    res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"hp",data:[],color:attack_obj.target === attack_obj.aggressor ? "#10C9F5" : "#FEE2D6"});
                    icon_action.large_icon = "heart";
                    icon_action.text = battle.warlocks[i].hp;
                    icon_action.title = battle.warlocks[i].name;
                    target_found = true;
                } else if (attack_obj.shield && (battle.warlocks[i].name === attack_obj.target)) {
                    // https://github.com/Pz1c/WavingHands/issues/227 the shield that took the blow lights up
                    res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"status",object:getDefenseStatusCode(battle.warlocks[i], false),data:[],role:"target",color:"#FEE2D6"});
                    target_found = true;
                } else {
                    w = battle.warlocks[i];
                    arr_m = getNewsMonsters(w);
                    for (j = 0, LnJ = arr_m.length; j < LnJ; ++j) {
                        m = arr_m[j];
                        if (m.name === attack_obj.target) {
                            res.push({action:"highlight",warlock_name:w.name,object_type:"monster",object:m.name,data:[],role:"target",color:attack_obj.target === attack_obj.aggressor ? "#10C9F5" : "#FEE2D6"});
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

        icon_action.small_icon = getMonsterIconByNameEx(attack_obj.aggressor);
        if (icon_action.small_icon === "summon") {
            icon_action.small_icon = "sword";
        }
        if (attack_obj.damage > 0) {
            icon_action.text = "- " + attack_obj.damage;
        }

        // https://github.com/Pz1c/WavingHands/issues/227
        if (attack_obj.shield) {
            icon_action.large_icon = getDefenseIcon(findBattleObject(battle, attack_obj.target), false);
        } else if (icon_action.large_icon === "heart") {
            applyDamageIcon(icon_action, attack_obj.damage);
        } else if (icon_action.large_icon === "") {
            icon_action.large_icon = getMonsterIconByNameEx(attack_obj.target);
        }
        res.push(icon_action);
    }
    //console.log("getMessageActionByAttack", JSON.stringify(attack_obj), JSON.stringify(res));
    return res;
}

function getMessageActionByOther(obj, battle) {
    var res = [];
    //var obj = parseStrangeAttack(msg);
    //console.log("getMessageActionByOther", JSON.stringify(obj));
    var icon_action = {action:"icon",large_icon:"",small_icon:"",title:"",text:"",background_color:"#210430",border_color:"#FEE2D6"};
    var target_found = false;
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].L, battle.warlocks[i].R, 0, 0, 5)});
        if (w.name === obj.target) {
            icon_action.large_icon = "heart";
            icon_action.text = w.hp;
            icon_action.title = w.name;
            target_found = true;
            if (obj.spell === "Disease") {
                if (w.disease > 0) {
                    icon_action.text = w.disease;
                } else if (w.poison > 0) {
                    icon_action.text = w.poison;
                    obj.spell = "Poison";
                } else {
                    // looks like effect removed
                    icon_action.text = "Cured";
                    obj.cured = true;
                }
            } else if (obj.spell === "Cause Light Wounds") {
                if (w.got_heavy_wounds) {
                    obj.spell = "Cause Heavy Wounds";
                    obj.damage = 3;
                }
            } else if (obj.spell === "Distruption") {
                if (w.arr_distruption) {
                    obj.spell = battle.warlocks[i].arr_distruption.shift();
                }
            }
            // https://github.com/Pz1c/WavingHands/issues/227
            // a status row lights the status icon up (a temporary one when the warlock has not got it
            // at the end of the turn), a wound the heart
            var status_code = ((obj.damage > 0) || obj.cured) ? "" : getNewsStatusCode(obj, w);
            if (status_code) {
                res.push({action:"highlight",warlock_name:w.name,object_type:"status",object:status_code,data:[],role:"target",
                          color:isHarmfulNews(obj) ? "#FEE2D6" : "#10C9F5"});
            } else {
                res.push({action:"highlight",warlock_name:w.name,object_type:"warlock",object:"hp",data:[],color:"#FEE2D6"});
            }
            target_found = true;
            //break;
        }

        // the row is about one of this warlock's monsters: "Smelly Goblin is hit by a Magic Missile",
        // "Green Troll is covered by a shimmering shield", "Big Goblin is summoned to serve Bob"
        // https://github.com/Pz1c/WavingHands/issues/227
        var arr_m = getNewsMonsters(w);
        for (var j = 0, LnJ = arr_m.length; j < LnJ; ++j) {
            var m = arr_m[j];
            if (m.name !== obj.target) {
                continue;
            }
            if (obj.spell === "Distruption") {
                if (m.arr_distruption) {
                    obj.spell = m.arr_distruption.shift();
                }
                target_found = true;
            }
            // hit by a spell or an attack: the hit color; nothing happened to it (it just appeared): the actor color
            res.push({action:"highlight",warlock_name:w.name,object_type:"monster",object:m.name,data:[],role:"target",
                      color:((obj.spell !== "") || (obj.damage > 0)) ? "#FEE2D6" : "#10C9F5"});
            break;
        }
    }

    if (obj.damage > 0) {
        icon_action.text = "- " + obj.damage;
        // https://github.com/Pz1c/WavingHands/issues/227
        if (icon_action.large_icon === "heart") {
            applyDamageIcon(icon_action, obj.damage);
        }
    }

    if (!target_found) {
        icon_action.large_icon = getMonsterIconByNameEx(obj.target);
    }
    if (obj.spell !== "") {
        var arr_g = map_spell_name_to_gesture[obj.spell];
        if (arr_g) {
            icon_action.small_icon = map_spell_to_icon[arr_g[0]];
        }
        if ((obj.spell === "Disease") || (obj.spell === "Poison")) {
            icon_action.large_icon = "";
        }
        // a warlock is now protected ("is covered by a glowing / shimmering / thick shimmering shield"):
        // the defense icon, the small one tells by which spell
        // https://github.com/Pz1c/WavingHands/issues/227
        if (target_found && !(obj.damage > 0) &&
                ((obj.spell === "Shield") || (obj.spell === "Counter Spell") || (obj.spell === "Protection"))) {
            icon_action.large_icon = "defense";
            icon_action.text = "";
        }
    }
    // https://github.com/Pz1c/WavingHands/issues/227
    if (obj.shield || obj.counter_spell) {
        icon_action.large_icon = getDefenseIcon(findBattleObject(battle, obj.target), obj.counter_spell);
        icon_action.text = "";
    }
    if (obj.mirror) {
        icon_action.large_icon = "magic_mirror";
        icon_action.text = "";
    }

    res.push(icon_action);
    //console.log("getMessageActionByOther", JSON.stringify(res));
    return res;
}

function parseDeathMessage(msg) {
    var res = {target:""};
    res.target = msg.replace(" dies.", "");
    return res;
}

function getMessageActionByDeath(obj, battle) {
    var res = [];
    var icon = getMonsterIconByNameEx(obj.target);
    if (icon === "summon") {
        icon = "";
    }
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        res.push({action:"highlight",warlock_name:w.name,object_type:"warlock",object:"gestures",data:preparePrintGestures(w.L, w.R, 0, 0, 5)});
        // https://github.com/Pz1c/WavingHands/issues/227 the one who died: the heart, or the monster's (temporary) tile
        if (w.name === obj.target) {
            res.push({action:"highlight",warlock_name:w.name,object_type:"warlock",object:"hp",data:[],color:"#FEE2D6"});
            continue;
        }
        var arr_m = getNewsMonsters(w);
        for (var j = 0, LnJ = arr_m.length; j < LnJ; ++j) {
            if (arr_m[j].name === obj.target) {
                res.push({action:"highlight",warlock_name:w.name,object_type:"monster",object:arr_m[j].name,data:[],role:"target",color:"#FEE2D6"});
                break;
            }
        }
    }
    res.push({action:"icon",large_icon:"RIP2",small_icon:icon,title:"",text:"",background_color:"#210430",border_color:"#FEE2D6"});
    return res;
}

function getMessageTypeByRow(row) {
    if (row.color === "#CCCCCC") {
        return "gesture";
    } else if ((row.txt.indexOf(" casts ") !== -1) && (row.txt.indexOf(" banked ") === -1)) {
        // process spell casting
        return "spell";
    } else if (row.color === "#FF6666") {
        return "death";
    } else if (((row.txt.indexOf(" attack") !== -1) || (row.txt.indexOf(" swing ") !== -1)) &&
               (row.txt.indexOf("too scared") === -1) &&
               (row.txt.indexOf("can't move to attack") === -1)&&
               (row.txt.indexOf("trips on its feet") === -1)) {
        // process attack message
        return "attack";
    } else {
        return "other";
    }
}

function parseHistoryMessage(obj, battle) {
    switch(obj.row_type) {
    case "gesture": return {};
    case "spell":   return parseSpellByText(obj.txt);
    case "death":   return parseDeathMessage(obj.txt);
    case "attack":  return parseAttackByText(obj.txt);
    case "other":   return parseStrangeAttack(obj.txt, battle);
    }
}

function getActionByHistoryMessage(obj, battle) {
    switch(obj.row_type) {
    case "gesture": return [];
    case "spell":   return getMessageActionBySpell(obj, battle);
    case "death":   return getMessageActionByDeath(obj.obj, battle);
    case "attack":  return getMessageActionByAttack(obj.obj, battle);
    case "other":   return getMessageActionByOther(obj.obj, battle);
    }
}


function prepareAndSortRealAction(actions, battle) {
    var i, Ln, j, LnJ, tmp;
    // first step, parse and set id
    for (i = 0, Ln = actions.length; i < Ln; ++i) {
        actions[i].id = i;
        actions[i].depends_on = [];
        actions[i].move_to = i;
        actions[i].row_type = getMessageTypeByRow(actions[i]);
        actions[i].obj = parseHistoryMessage(actions[i], battle);
        // https://github.com/Pz1c/WavingHands/issues/227
        // "the monster Bob is summoning with his left hand" is that monster once prepareNewsMonsters named it
        if (actions[i].summon_target && actions[i].obj && actions[i].obj.target) {
            actions[i].obj.target = actions[i].summon_target.key;
        }
        actions[i].new_action = getActionByHistoryMessage(actions[i], battle);
        actions[i].checked = (actions[i].row_type === "death") || (actions[i].color === "#F5C88E")
                    || (actions[i].color === "#CCCCCC");
    }

    for (i = 0, Ln = actions.length; i < Ln; ++i) {
        console.log("sortRealAction", i, actions[i].txt, JSON.stringify(actions[i].obj));
        if (actions[i].checked) {
            continue;
        }
        if ((actions[i].type === 2) && (actions[i].txt.indexOf("shakes his head") !== -1)) {
            actions[i].type = 1;
            actions[i].checked = true;
            continue;
        }

        if (actions[i].row_type === "spell") {
            if (actions[i].obj.fail !== "") {
                actions[i].checked = true;
                continue;
            }

            for (j = i + 1; j < Ln; ++j) {
                console.log("sortRealAction", i, j, JSON.stringify(actions[j].obj));
                if (((actions[i].obj.spell === actions[j].obj.spell) && (actions[i].obj.target === actions[j].obj.target))
                        || (actions[i].summon_id && (actions[i].summon_id === actions[j].summoned_id))) {
                    actions[j].depends_on.push(actions[i].id);
                    actions[i].checked = true;
                    actions[j].checked = true;
                    console.log("sortRealAction", i, j, "FOUND");
                    if (i !== j - 1) {
                        actions.splice(j, 0, JSON.parse(JSON.stringify(actions[i])));
                        actions[i].type = 3;
                        ++Ln;
                        /*console.log("sortRealAction", "AFTER SPLICE", i, Ln);
                        for (var k = 0, LnK = actions.length; k < LnK; ++k) {
                            console.log("sortRealAction", k, LnK, actions[k].txt, actions[k].type);
                        }*/
                    }
                    break;
                }
            }
        }
    }
}

function processHintText(obj) {
    console.log("processHintText.1", obj.txt, obj.font_size);
    if (obj.txt.indexOf(" attacks ") !== -1) {
        obj.txt = obj.txt.substring(0, obj.txt.indexOf(" attacks ") + 8);
        obj.font_size = 28;
    } else if (obj.txt.indexOf(" is hit by a") !== -1) {
        obj.txt = obj.txt.substring(0, obj.txt.indexOf(" is hit by a") + 7);
        obj.font_size = 28;
    } else if (obj.txt.indexOf(" hand is paralysed") !== -1) {
        obj.txt = obj.obj.target + " is paralyzed";
        obj.font_size = 28;
    } else if (obj.txt.indexOf(" starts to lose coordination") !== -1) {
        obj.txt = obj.txt.substring(0, obj.txt.indexOf(" starts to lose coordination")) + " loses coordination";
        obj.font_size = 28;
    }
    console.log("processHintText.2", obj.txt, obj.font_size);
}

// https://github.com/Pz1c/WavingHands/issues/227
// Turn news icons for what the end-of-turn board does not show any more.
//
// Before the rows are parsed, prepareNewsMonsters gives the monsters that are gone (died this turn, a
// summon a Counter Spell absorbed) a temporary tile object in their owner's news_monsters list, pairs
// every "X casts Summon Y at Z" row with its "N is summoned to serve Z" row and resolves a target like
// "the monster X is summoning with his left hand" to that monster. After the rows are parsed and
// sorted, prepareNewsTempIcons works out which temporary status icons and tiles every step shows.
var SUMMON_SPELL_TYPES = ["Goblin", "Ogre", "Troll", "Giant"];

function escapeRegExp(str) {
    return str.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");
}

function getNewsMonsters(w) {
    return w.news_monsters ? w.news_monsters : w.monsters;
}

function getNewsStatusIcon(code) {
    if (icon_status_code_to_icon[code]) {
        return icon_status_code_to_icon[code];
    }
    return map_news_status_to_icon[code] ? map_news_status_to_icon[code] : code;
}

function findWarlockByName(battle, name) {
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        if (battle.warlocks[i].name === name) {
            return battle.warlocks[i];
        }
    }
    return null;
}

function isWarlockName(battle, name) {
    return findWarlockByName(battle, name) !== null;
}

// the owner's name of a monster on the end-of-turn board (real_only) or in the news, "" when there is none
function findMonsterOwner(battle, name, real_only) {
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        var arr_m = real_only ? w.monsters : getNewsMonsters(w);
        for (var j = 0, LnJ = arr_m.length; j < LnJ; ++j) {
            if (arr_m[j].name === name) {
                return w.name;
            }
        }
    }
    return "";
}

// does the warlock's end-of-turn status row have that icon
function hasStatusIcon(battle, warlock_name, code) {
    var w = findWarlockByName(battle, warlock_name);
    if (!w || !w.statusIcons) {
        return false;
    }
    for (var i = 0, Ln = w.statusIcons.length; i < Ln; ++i) {
        if (w.statusIcons[i].action === code) {
            return true;
        }
    }
    return false;
}

// the status icon key of what protected the target this turn (noteDefense), by_counter is what the
// row itself says when no spell cast at the target was seen
function getDefenseStatusCode(target, by_counter) {
    var by = target && target.defended_by ? target.defended_by : (by_counter ? "counter" : "shield");
    switch (by) {
    case "counter": return "mshield";
    case "dispel":  return "dispel";
    default:        return "shield";
    }
}

// the status icon an "other" row is about: the shield that absorbed or deflected something, the
// mirror that reflected it, the enchantment that got or left the target; "" for a wound and the like
function getNewsStatusCode(obj, target) {
    if (obj.shield || obj.counter_spell) {
        return getDefenseStatusCode(target, obj.counter_spell);
    }
    if (obj.mirror) {
        return "mirror";
    }
    var code = map_spell_name_to_status[obj.spell];
    return code ? code : "";
}

// orange for what hurts (an absorbed or deflected hit, a disruption, a disease), blue for a protection
function isHarmfulNews(obj) {
    if (obj.shield || obj.counter_spell) {
        return true;
    }
    if (obj.mirror) {
        return false;
    }
    return arr_harmful_status_spell.indexOf(obj.spell) !== -1;
}

// the hand whose gestures completed the spell this turn: "left", "right", "both" or ""
function getSpellHand(w, spell_name) {
    var arr_g = map_spell_name_to_gesture[spell_name];
    if (!w || !arr_g) {
        return "";
    }
    for (var i = 0, Ln = arr_g.length; i < Ln; ++i) {
        var lr = checkIsSpellPossibeForWarlock(arr_g[i], w.L, w.R);
        var l = lr[0] === arr_g[i].length, r = lr[1] === arr_g[i].length;
        if (l && r) {
            return "both";
        }
        if (l || r) {
            return l ? "left" : "right";
        }
    }
    return "";
}

function addTempNewsMonster(battle, owner, m) {
    m.temp = true;
    m.action = "m";
    m.owner = owner;
    m.status = "";
    m.enchantment_icon = "";
    m.is_checkbox = false;
    m.is_elemental = false;
    m.allow_choose_target = false;
    battle.temp_monsters[m.name] = m;
    battle.temp_monster_owner[m.name] = owner;
    var w = findWarlockByName(battle, owner);
    if (w) {
        w.news_monsters.push(m);
    }
}

// the owner of a monster that is gone: the battle history names who it was summoned to serve; in a
// duel it is whoever it did not attack this turn
function resolveGoneMonsterOwner(battle, name, rows) {
    var m, hist = battle.battle_hist ? battle.battle_hist.replace(/<[^>]*>/g, "") : "";
    var re = new RegExp(escapeRegExp(name) + " is summoned to serve ([^.<]+)\\.");
    m = re.exec(hist);
    if (m && isWarlockName(battle, m[1].trim())) {
        return m[1].trim();
    }
    if (battle.warlocks.length === 2) {
        re = new RegExp("^" + escapeRegExp(name) + " (?:attacks|swings wildly for|tries to attack) (.+?)(?:,| for | but |\\.|$)");
        for (var i = 0, Ln = rows.length; i < Ln; ++i) {
            m = re.exec(rows[i].txt);
            if (!m) {
                continue;
            }
            var victim = m[1].trim();
            var victim_owner = isWarlockName(battle, victim) ? victim : findMonsterOwner(battle, victim, false);
            if (victim_owner) {
                return battle.warlocks[0].name === victim_owner ? battle.warlocks[1].name : battle.warlocks[0].name;
            }
        }
    }
    return "";
}

// the summon a "the monster X is summoning with his left hand" target means: X's summon of that hand
function findSummonRecord(summons, caster, hand) {
    var first = null;
    for (var i = 0, Ln = summons.length; i < Ln; ++i) {
        if (summons[i].caster.toLowerCase() !== caster.toLowerCase()) {
            continue;
        }
        if (!first) {
            first = summons[i];
        }
        if ((summons[i].hand === hand) || (summons[i].hand === "both")) {
            return summons[i];
        }
    }
    return first;
}

function prepareNewsMonsters(rows, battle) {
    var i, Ln, j, LnJ, m, txt, rec, owner, name, strength;
    var summons = [], summoned = [];
    var re_cast = /^(\S+) casts Summon (Goblin|Ogre|Troll|Giant) at ([^,.]+)/;
    var re_summoned = /^(.+?) is summoned to serve ([^.]+)\.?$/;
    var re_dies = /^(.+?) dies\.?$/;
    var re_summoning = /the monster (\S+) is summoning with (?:his|her|its|their) (left|right) hand/;
    battle.temp_monsters = {};
    battle.temp_monster_owner = {};
    for (i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        battle.warlocks[i].news_monsters = battle.warlocks[i].monsters.slice();
    }
    for (i = 0, Ln = rows.length; i < Ln; ++i) {
        txt = rows[i].txt;
        if ((txt.indexOf(" banked ") === -1) && (m = re_cast.exec(txt))) {
            owner = m[3].trim();
            if ((owner === "himself") || (owner === "herself") || (owner === "itself")) {
                owner = m[1];
            } else if (!isWarlockName(battle, owner)) {
                // cast at a monster: its controller gets the new one
                var mo = findMonsterOwner(battle, owner, false), ms = re_summoning.exec(owner);
                owner = mo ? mo : (ms ? ms[1] : m[1]);
            }
            rec = {id:summons.length + 1,caster:m[1],owner:owner,type:m[2],hand:getSpellHand(findWarlockByName(battle, m[1]), "Summon " + m[2]),name:"",key:"",real:false};
            summons.push(rec);
            rows[i].summon = rec;
            rows[i].summon_id = rec.id;
        } else if ((m = re_summoned.exec(txt))) {
            summoned.push({row:rows[i],name:m[1].trim(),owner:m[2].trim(),rec:null});
        }
    }
    // the casts and the arrivals: same controller, same kind, in order
    for (i = 0, Ln = summons.length; i < Ln; ++i) {
        rec = summons[i];
        for (j = 0, LnJ = summoned.length; j < LnJ; ++j) {
            if (summoned[j].rec || (summoned[j].owner.toLowerCase() !== rec.owner.toLowerCase()) || (summoned[j].name.indexOf(rec.type) === -1)) {
                continue;
            }
            summoned[j].rec = rec;
            summoned[j].row.summoned_id = rec.id;
            rec.name = summoned[j].name;
            break;
        }
        rec.real = (rec.name !== "") && (findMonsterOwner(battle, rec.name, true) !== "");
        if (rec.real) {
            rec.key = rec.name;
            rec.owner = findMonsterOwner(battle, rec.name, true);
            continue;
        }
        strength = SUMMON_SPELL_TYPES.indexOf(rec.type) + 1;
        rec.key = rec.name !== "" ? rec.name : ("New " + rec.type + " of " + rec.owner);
        if (!battle.temp_monsters[rec.key]) {
            addTempNewsMonster(battle, rec.owner, {name:rec.key,icon:rec.name !== "" ? getMonsterIconByName(rec.name) : "new_" + rec.type.toLowerCase(),
                                                   text:strength,hp:strength,strength:strength,damage:strength});
        }
    }
    // an arrival without a cast row (a banked summon fired) that is gone too
    for (j = 0, LnJ = summoned.length; j < LnJ; ++j) {
        name = summoned[j].name;
        if (summoned[j].rec || battle.temp_monsters[name] || !isWarlockName(battle, summoned[j].owner) || (findMonsterOwner(battle, name, true) !== "")) {
            continue;
        }
        strength = getMonsterDamageByName(name);
        addTempNewsMonster(battle, summoned[j].owner, {name:name,icon:getMonsterIconByName(name),text:strength > 0 ? strength : "",hp:strength,strength:strength,damage:strength});
    }
    // the ones on the board when the turn began that died
    for (i = 0, Ln = rows.length; i < Ln; ++i) {
        if (!(m = re_dies.exec(rows[i].txt))) {
            continue;
        }
        name = m[1].trim();
        if (isWarlockName(battle, name) || battle.temp_monsters[name] || (findMonsterOwner(battle, name, true) !== "")) {
            continue;
        }
        owner = resolveGoneMonsterOwner(battle, name, rows);
        if (!owner) {
            continue;
        }
        strength = getMonsterDamageByName(name);
        addTempNewsMonster(battle, owner, {name:name,icon:getMonsterIconByName(name),text:"",hp:0,strength:strength,damage:strength,pre_existing:true});
    }
    // targets like "the monster X is summoning with his left hand"
    for (i = 0, Ln = rows.length; i < Ln; ++i) {
        if ((m = re_summoning.exec(rows[i].txt))) {
            rec = findSummonRecord(summons, m[1], m[2]);
            if (rec) {
                rows[i].summon_target = rec;
            }
        }
    }
    console.log("prepareNewsMonsters", JSON.stringify(summons), JSON.stringify(battle.temp_monster_owner));
}

function getTempState(state, warlock_name) {
    if (!state[warlock_name]) {
        state[warlock_name] = {statuses:[],monsters:[]};
    }
    return state[warlock_name];
}

function ensureTempStatus(state, warlock_name, code) {
    var s = getTempState(state, warlock_name);
    for (var i = 0, Ln = s.statuses.length; i < Ln; ++i) {
        if (s.statuses[i].action === code) {
            return;
        }
    }
    s.statuses.push({action:code,icon:getNewsStatusIcon(code),value:"",text:"",active:false,active_action:false,is_checkbox:false,temp:true});
}

function ensureTempMonster(state, m) {
    var s = getTempState(state, m.owner);
    for (var i = 0, Ln = s.monsters.length; i < Ln; ++i) {
        if (s.monsters[i].name === m.name) {
            return;
        }
    }
    s.monsters.push(JSON.parse(JSON.stringify(m)));
}

function removeTempIcon(state, r) {
    var name, s, i;
    for (name in state) {
        s = state[name];
        if (r.status && (name === r.warlock)) {
            for (i = s.statuses.length - 1; i >= 0; --i) {
                if (s.statuses[i].action === r.status) {
                    s.statuses.splice(i, 1);
                }
            }
        }
        if (r.monster) {
            for (i = s.monsters.length - 1; i >= 0; --i) {
                if (s.monsters[i].name === r.monster) {
                    s.monsters.splice(i, 1);
                }
            }
        }
    }
}

function snapshotTempIcons(state) {
    return JSON.parse(JSON.stringify(state));
}

// A temporary status icon shows from the first step that lights it up to the end of the news (a
// Counter Spell that absorbed the enchantment takes it away), a gone monster from the first step that
// mentions it (from the start when it was on the board as the turn began) until its death row.
// Every row gets the set its step shows; returns the set the steps before the first row show.
function prepareNewsTempIcons(rows, battle) {
    var i, Ln, j, LnJ, a, row, key, code, remove, state = {};
    for (key in battle.temp_monsters) {
        if (battle.temp_monsters[key].pre_existing) {
            ensureTempMonster(state, battle.temp_monsters[key]);
        }
    }
    var initial = snapshotTempIcons(state);
    for (i = 0, Ln = rows.length; i < Ln; ++i) {
        row = rows[i];
        if (row.type >= 2) {
            continue;
        }
        remove = [];
        for (j = 0, LnJ = row.new_action.length; j < LnJ; ++j) {
            a = row.new_action[j];
            if (a.action !== "highlight") {
                continue;
            }
            if ((a.object_type === "status") && !hasStatusIcon(battle, a.warlock_name, a.object)) {
                ensureTempStatus(state, a.warlock_name, a.object);
            } else if ((a.object_type === "monster") && battle.temp_monsters[a.object]) {
                ensureTempMonster(state, battle.temp_monsters[a.object]);
            }
        }
        if ((row.row_type === "other") && row.obj.counter_spell && (code = map_spell_name_to_status[row.obj.spell])) {
            remove.push({warlock:row.obj.target,status:code});
        }
        if (row.row_type === "death") {
            remove.push({monster:row.obj.target});
        }
        row.temp_icons = snapshotTempIcons(state);
        for (j = 0, LnJ = remove.length; j < LnJ; ++j) {
            removeTempIcon(state, remove[j]);
        }
    }
    return initial;
}

// one "temp_icons" action per warlock, Warlock.qml puts the set in its lists
function getTempIconActions(snap, battle) {
    var res = [], w, s;
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        w = battle.warlocks[i];
        s = snap && snap[w.name] ? snap[w.name] : {statuses:[],monsters:[]};
        res.push({action:"temp_icons",warlock_name:w.name,statuses:s.statuses,monsters:s.monsters});
    }
    return res;
}
