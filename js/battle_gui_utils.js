var icon_status_code = ["scared","confused","charmed","paralized","shield","coldproof","fireproof","poison","disease","amnesia","maladroit","mshield","delay",
                        "time_stop","haste","permanency","blindness","invisibility"];
var icon_status_code_to_icon = {"confused":"maladroit","time_stop":"time_stop"};
var icon_status_spell = {"scared":"SWD","confused":"DSF","charmed":"PSDF","paralized":"FFF","coldproof":"SSFP","fireproof":"WWFP","poison":"DWWFWD","disease":"DSFFFc",
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
// Master's summon order; index + 1 is the monster strength.
var SUMMON_SPELL_TYPES = ["Goblin", "Ogre", "Troll", "Giant"];
var SUMMON_GESTURE_TYPES = SUMMON_SPELL_TYPES.concat(["Fire Elemental", "Ice Elemental"]);
var arr_distruption_spell = ["Amnesia", "Paralysis", "Confusion", "Fear", "Maladroitness", "Charm Monster", "Charm Person", "Anti-spell"];

// https://github.com/Pz1c/WavingHands/issues/227
// An enchantment cast at a warlock lights his status icon up in the turn news. The live
// status list is updated at the row where the spell lands. mirror and dispel are news-only codes.
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
        item = {t: i, l:'g_'+ll, r: 'g_' + lr, lv: ll !== ' ', rv: lr !== ' ', la: i >= Ln - mscL, ra: i >= Ln - mscR};
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

// A summon in progress ends the hand with a prefix of its spell. The
// placeholder is "RH:<owner>" / "LH:<owner>" and carries no monster name.
// Prefer the longest match: ...PSF is Ogre, not Goblin.
function getSummonFromGestures(gestures) {
    var text = (gestures || "").toUpperCase().replace(/[^A-Z]+$/, "");
    var best = null;
    var best_len = 0;
    for (var i = 0; i < SUMMON_GESTURE_TYPES.length; ++i) {
        var name = SUMMON_GESTURE_TYPES[i];
        var codes = map_spell_name_to_gesture["Summon " + name];
        if (!codes || !codes.length) {
            continue;
        }
        var elemental = (name.indexOf("Elemental") !== -1);
        var code = codes[0].toUpperCase();
        for (var n = Math.min(code.length, text.length); n >= 2; --n) {
            if ((text.lastIndexOf(code.substr(0, n)) === text.length - n) &&
                    (n > best_len)) {
                best = {
                    name:name,
                    strength:getMonsterDamageByName(name),
                    elemental:elemental,
                    icon:elemental ? getMonsterIconByNameEx(name) : ""
                };
                best_len = n;
                break;
            }
        }
    }
    return best;
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

function getSpellNameForOrderReview(action, dict) {
    // strange gestures
    if (action.g === ">") {
        return "Stab";
    }

    if ((action.g === "?") || (action.g === "-")) {
        return dict.getStringByCode("UnknownSpell");
    }
    // default spell
    if ((action.s.gp === "?") || (action.s.n === "Default")) {
        return dict.getStringByCode("DefaultSpell");
    }

    return action.s.n;
}

function getSpellTargetForOrder(action, targetMap) {
    if (!action.target || !targetMap[action.target]) {
        return "";
    }
    return targetMap[action.target].replace(" ", "+");
}

function getSpellTargetForOrderReview(action, targetMap, dict) {
    if (!action.target || !targetMap[action.target]) {
        return dict.getStringByCode("DefaultTarget");
    }
    if (action.target.indexOf(":") !== -1) {
        return dict.getStringByCode("NewMonsterHand").replace("%1", action.target.substr(0, 1));
    }

    return action.target;
}

// res.push({type:"RH",g:actions.R.g,s:getSpellNameForOrderReview(actions.R),t:getSpellTargetForOrderReview(actions.R, battle.targetsMap)});
function getTextForHandAction(hand, action, targetMap, dict, exist_completed_spell) {
    var spell_name = getSpellNameForOrderReview(action, dict);
    var target_name = getSpellTargetForOrderReview(action, targetMap, dict);
    var res;
    if ((spell_name === dict.getStringByCode("DefaultSpell")) && (target_name === dict.getStringByCode("DefaultTarget")) && (exist_completed_spell === 0)) {
        if (hand === "LH") {
            res = dict.getStringByCode("LeftHandGesture");
        } else {
            res = dict.getStringByCode("RightHandGesture");
        }
    } else {
        res = dict.getStringByCode("CastSpellAt").replace("%1", spell_name).replace("%2", target_name);
    }
    return res;
}

function getSpecActionText(type, action, dict) {
    return dict.getStringByCode("TitleAction_" + type) + getHandTitleByIdx(action, dict);
}

function getCharmActionText(type, action, target, dict) {
    return dict.getStringByCode("TitleAction_" + type).replace("%1", getHandTitleByIdx(action.h, dict)).replace("%2", action.g) + target;
}

function getMonsterActionText(action, target, targetMap, dict) {
    var name = targetMap[action.id];
    if (!name) {
        name = action.name || "Monster";
    }
    if (!target) {
        target = "Default";
    }
    if (name.indexOf(":") !== -1) {
        name = dict.getStringByCode("NewMonsterHand").replace("%1", action.id.substr(0, 1));
    }

    var t = dict.getStringByCode("TitleAction_M").replace("%1", name).replace("%2", target);

    return t;
}

function getHandTitleByIdx(idx, dict) {
    switch(idx) {
        case 0: return dict.getStringByCode("HandNone");
        case 1:
        case "LH": return dict.getStringByCode("HandLeft");
        case 3: return dict.getStringByCode("HandTwo");
        default: return dict.getStringByCode("HandRight");
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
            // A trailing clause is not part of the target name.
            if (arr[i] === "but") {
                break;
            }
            target_name += arr[i].replace(/[.,]/g, "") + " ";
        }
    }
    target_name = target_name.trim();
    if (target_name === "himself") {
        target_name = walock_name;
    }

    spell_name = spell_name.trim();
    var lower_spell = spell_name.toLowerCase();
    var banked_prefixes = ["his banked ", "her banked ",
                           "its banked ", "their banked "];
    var banked = false;
    for (var b = 0; b < banked_prefixes.length; ++b) {
        if (lower_spell.indexOf(banked_prefixes[b]) === 0) {
            spell_name = spell_name.substring(banked_prefixes[b].length);
            banked = true;
            break;
        }
    }
    var spell_fail = "";
    if (txt.indexOf("but misses due to invisibility") !== -1) {
        spell_fail = "Invisibility";
    } else if (txt.indexOf("but misses due to bl") !== -1) {
        spell_fail = "Blindness";
    }

    console.log("parseSpellByText", txt, walock_name, spell_name, target_name, spell_fail);
    return {warlock:walock_name,spell:spell_name,target:target_name,fail:spell_fail,banked:banked};
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
                // A shared gesture (the clap) must match on both hands, but
                // only extend a hand that has the rest of the spell as its
                // contiguous suffix.
                if (res[0] === gl - i - 1) {
                    res[0] = gl - i;
                }
                if (res[1] === gl - i - 1) {
                    res[1] = gl - i;
                }
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

function higlightWarlockGestureBySpell(warlock, spell_obj, battle) {
    var arr_g = map_spell_name_to_gesture[spell_obj.spell];
    console.log("higlightWarlockGestureBySpell", spell_obj.spell, JSON.stringify(arr_g));
    if (!arr_g) {
        return {action:"none"};
    }

    var L = warlock.L, R = warlock.R;
    var pL = warlock.pL, pR = warlock.pR;
    // Replay shows the full history; match the spell against the history
    // only up to the turn being played.
    if (battle && battle.replay && (warlock.replay_pL !== undefined)) {
        var end = battle.turn_num + 1;
        L = warlock.replay_pL.substr(0, end);
        R = warlock.replay_pR.substr(0, end);
        pL = L;
        pR = R;
    }

    for (var i = 0, Ln = arr_g.length; i < Ln; ++i) {
        var lrsl = checkIsSpellPossibeForWarlock(arr_g[i], L, R);
        if ((lrsl[0] !== 0) || (lrsl[1] !== 0)) {
            return {action:"highlight",warlock_name:warlock.name,object_type:"warlock",object:"gestures",data:preparePrintGestures(pL, pR, lrsl[0], lrsl[1], 5)}
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
        target.defended_by = "shield";
        break;
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

// the warlock or live monster with that name, null when there is none
function findBattleObject(battle, name) {
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        if (w.name === name) {
            return w;
        }
        var monsters = w.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === name) {
                return monsters[j];
            }
        }
    }
    return null;
}

function findMonsterByName(battle, name) {
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        var monsters = w.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j] && (monsters[j].name === name)) {
                return monsters[j];
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
    var missed = spell_obj.fail !== "";
    var target_found = false;
    // https://github.com/Pz1c/WavingHands/issues/227
    // an enchantment lights the target's status icon up, a summon the new monster (prepareNewsMonsters
    // paired the rows), anything else the target's heart
    var status_code = map_spell_name_to_status[spell_obj.spell];
    obj.txt = spell_obj.spell;
    obj.font_size = 42;

    res.push({action:"highlight",warlock_name:spell_obj.warlock,
              object_type:"warlock",object:"caster",role:"actor",
              data:[],color:"#10C9F5"});
    res.push(getSpellIconActionBySpell(spell_obj));
    if (spell_obj.banked) {
        // A banked cast empties the bank, so its icon has to go with this
        // event instead of lingering from the turn-start board.
        res.push({action:"banked_remove",warlock_name:spell_obj.warlock,
                  object_type:"banked",object:"banked"});
    }
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        if (battle.warlocks[i].name === spell_obj.warlock) {
            // hightlight spell gestures
            var hga = higlightWarlockGestureBySpell(battle.warlocks[i], spell_obj, battle);
            if (hga.action !== "none") {
                res.push(hga);
            } else {
                res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].pL, battle.warlocks[i].pR, 0, 0, 5)});
            }

            //https://github.com/Pz1c/WavingHands/issues/312
            //res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"hp",data:[],color:spell_obj.target === battle.warlocks[i].name ? "#10C9F5" : "#FEE2D6"});
        } else {
            // switch off hightlighting to all other warlocks gestures
            res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].pL, battle.warlocks[i].pR, 0, 0, 5)});
        }
        if (battle.warlocks[i].name === spell_obj.target) {
            if (!missed && (spell_obj.spell === "Cause Heavy Wounds")) {
                battle.warlocks[i].got_heavy_wounds = true;
            }
            noteDefense(battle.warlocks[i], spell_obj);
            if (!missed &&
                    (arr_distruption_spell.indexOf(spell_obj.spell) !== -1)) {
                if (!battle.warlocks[i].arr_distruption) {
                    battle.warlocks[i].arr_distruption = [];
                }
                battle.warlocks[i].arr_distruption.push(spell_obj.spell);
            }

            if (!missed && status_code) {
                // This is the cast row. A successful cast creates the status
                // with its end-of-turn counter and marks it fresh so it is not
                // decremented at the end of this turn.
                var cast_status = {action:"highlight",warlock_name:battle.warlocks[i].name,
                                   object_type:"status",object:status_code,
                                   icon:getNewsStatusIcon(status_code),
                                   data:[],role:"target",color:"#10C9F5"};
                cast_status.value = getNewsStatusValue(status_code, spell_obj.spell);
                if (spell_obj.spell === "Counter Spell") {
                    // The counter effect row pulses this status; creating it
                    // must not light it up on its own.
                    cast_status.pulse = false;
                }
                res.push(cast_status);
            } else if (!missed && !obj.summon) {
                res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"hp",data:[],color:"#10C9F5"});
            }
            target_found = true;
        }
        if (!target_found) {
            var owner = findMonsterOwner(battle, spell_obj.target);
            if (owner && !missed) {
                var m = findMonsterByName(battle, spell_obj.target);
                if (m) {
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
                }
                res.push({action:"highlight",warlock_name:owner,object_type:"monster",object:spell_obj.target,data:[],role:"target",color:"#FEE2D6"});
                target_found = true;
            }
        }
        //if (target_found) {
        //    break;
        //}
    }

    if (missed) {
        // The reason is already the status key. The icon belongs to the
        // target for invisibility and to the caster for blindness.
        var fail_code = spell_obj.fail.toLowerCase();
        var fail_owner = (spell_obj.fail === "Invisibility")
                ? spell_obj.target : spell_obj.warlock;
        res.push({action:"highlight",warlock_name:fail_owner,object_type:"status",
                  object:fail_code,icon:getNewsStatusIcon(fail_code),data:[],
                  role:"target",
                  color:isHarmfulNews({spell:spell_obj.fail}) ? "#FEE2D6" : "#10C9F5"});
    }

    if (obj.summon && !missed) {
        // the monster this cast brings
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

function newStrangeResult() {
    return {target:"", spell:"", damage:0};
}

function targetBefore(txt, marker) {
    var i = txt.indexOf(marker);
    return (i > 0) ? txt.slice(0, i).trim() : null;
}

function targetBetween(txt, start, end) {
    var i = txt.indexOf(start);
    if (i === -1) {
        return null;
    }
    i += start.length;
    var j = txt.indexOf(end, i);
    return (j === -1) ? null : txt.slice(i, j).trim();
}

function applyStrangeRule(txt, rule, battle) {
    if ((rule.exact !== undefined) && (txt !== rule.exact)) {
        return null;
    }
    if (rule.startsWith && !txt.startsWith(rule.startsWith)) {
        return null;
    }
    if (rule.endsWith && !txt.endsWith(rule.endsWith)) {
        return null;
    }
    if (rule.contains) {
        for (var i = 0, Ln = rule.contains.length; i < Ln; ++i) {
            if (txt.indexOf(rule.contains[i]) === -1) {
                return null;
            }
        }
    }

    var res = newStrangeResult();

    if (rule.summonAbsorbed) {
        var owner = targetBefore(txt, "'s ");
        var monster = targetBetween(txt, "'s ", " is absorbed into ");
        if ((owner === null) || (monster === null) || !txt.endsWith(" glow.")) {
            return null;
        }
        res.summoner = owner;
        res.spell = "Summon " + monster;
        res.countered_summon = true;
        res.kind = "apply";
        return res;
    }

    if (rule.targetBefore !== undefined) {
        res.target = targetBefore(txt, rule.targetBefore);
        if (res.target === null) {
            return null;
        }
    } else if (rule.targetBetween !== undefined) {
        res.target = targetBetween(txt, rule.targetBetween[0], rule.targetBetween[1]);
        if (res.target === null) {
            return null;
        }
    }

    if (rule.spell !== undefined) {
        res.spell = (typeof rule.spell === "function") ? rule.spell(battle) : rule.spell;
    }
    if (rule.spellBetween !== undefined) {
        var spell = targetBetween(txt, rule.spellBetween[0], rule.spellBetween[1]);
        if (spell === null) {
            return null;
        }
        res.spell = (rule.spellAliases && rule.spellAliases[spell]) || spell;
    }

    if (rule.damage !== undefined) {
        res.damage = rule.damage;
    }
    if (rule.damageBetween !== undefined) {
        var damage = targetBetween(txt, rule.damageBetween[0], rule.damageBetween[1]);
        if (damage === null) {
            return null;
        }
        res.damage = parseInt(damage, 10) || 0;
    }

    res.kind = rule.kind || "apply";
    if (rule.flags) {
        for (var key in rule.flags) {
            res[key] = rule.flags[key];
        }
    }
    if (rule.mirrorSpell) {
        res.mirror_spell = res.spell;
    }
    return res;
}

var EFFECT_RULES = [
    // Countered summon
    {summonAbsorbed: true},

    // Reflection
    {
        targetBetween: [" spell is reflected from ", "'s Magic Mirror."],
        spellBetween: ["The ", " spell is reflected from "],
        flags: {mirror:true},
        mirrorSpell: true
    },
    {
        targetBetween: [" spell is reflected from ", "'s magic mirror."],
        spellBetween: ["The ", " spell is reflected from "],
        flags: {mirror:true},
        mirrorSpell: true
    },

    // Whole-turn / non-spell rows
    {exact: "This turn took place outside of time."},
    {exact: "No Warlocks remaining. An ignominious end to a battle."},
    {exact: "The haze of an enchantment spell drifts aimlessly over the circle, and dissipates."},
    {exact: "A Fire Storm rages through the circle!", spell: "Fire Storm"},
    {exact: "An Ice Storm rages through the circle!", spell: "Ice Storm"},
    {exact: "All magical effects are erased! All other spells fail!", spell: "Dispel Magic"},

    // Bank and spell-specific outcomes
    {targetBefore: " banks a spell for later.", flags: {banked:true}},
    {
        targetBetween: ["Lightning sparks all around ", "'s shield."],
        spell: "Lightning Bolt",
        flags: {counter_spell:true, absorbed:true,
                absorbed_aliases:["Clap of Lightning"]}
    },
    {targetBefore: "'s half-done spells fizzle and die.", spell: "Anti-spell"},
    {
        targetBefore: " ignores ",
        contains: ["'s appeal to ", " baser instincts"],
        spell: "Charm Monster",
        flags: {failed:true}
    },
    {targetBefore: " begins glowing faintly.", flags: {ambiguous:true}},
    {targetBefore: " attempts to make the spell permanent!", spell: "Permanency"},

    // Counters and blocked spells
    {targetBefore: "'s shield blurs for a moment.", spell: "Distruption", flags: {counter_spell:true}},
    {targetBefore: "'s shield fizzles slightly.", spell: "Anti-spell", flags: {counter_spell:true}},
    {targetBefore: "'s shield flickers, but remains firm.", flags: {counter_spell:true}},
    {targetBefore: "'s shield intensifies momentarily.", kind: "result"},
    {
        targetBetween: ["Tiny holes in ", "'s shield are sealed up."],
        spell: "Cure Light Wounds",
        flags: {counter_spell:true}
    },
    {
        targetBetween: ["Holes open up in ", "'s shield, but then close up again."],
        spell: "Cause Light Wounds",
        flags: {counter_spell:true}
    },
    {
        targetBetween: ["A magic missile bounces off ", "'s shield."],
        spell: "Magic Missile",
        flags: {shield:true, absorbed:true}
    },
    {targetBefore: " is hit by Remove Enchantment", spell: "Remove Enchantment"},
    {targetBefore: "'s surrounding magical energies are grounded.", spell: "Remove Enchantment"},
    {
        targetBefore: " is hit by a Blindness spell, and is annihilated by the magical overload.",
        spell: "Blindness",
        flags: {annihilated: true}
    },

    // Damage and healing
    {
        targetBetween: ["A fireball strikes ", ", burning "],
        spell: "Fireball",
        damageBetween: [" for ", " damage."]
    },
    {
        targetBefore: " is hit by a bolt of lightning, for ",
        spell: "Lightning Bolt",
        damageBetween: [" is hit by a bolt of lightning, for ", " damage."]
    },
    {
        targetBefore: " is burnt in the raging Fire Storm, for ",
        spell: "Fire Storm",
        damageBetween: [" is burnt in the raging Fire Storm, for ", " damage."]
    },
    {
        targetBefore: " is frozen by the raging Ice Storm, for ",
        spell: "Ice Storm",
        damageBetween: [" is frozen by the raging Ice Storm, for ", " damage."]
    },
    {
        targetBetween: ["Wounds appear all over ", "'s body!"],
        spell: "Cause Light Wounds",
        damage: 2
    },
    {targetBefore: " is healed.", spell: "Cure Light Wounds"},
    {
        targetBefore: " is hit by a ",
        spellBetween: [" is hit by a ", ", for "],
        damageBetween: [", for ", " damage."],
        spellAliases: {"bolt of lightning":"Lightning Bolt"}
    },
    {
        targetBefore: " is burnt for ",
        damageBetween: [" is burnt for ", " damage."]
    },
    {
        targetBefore: " is frozen for ",
        damageBetween: [" is frozen for ", " damage."]
    },
    {targetBefore: " basks in the heat of the Fire Storm.", spell: "Fire Storm"},
    {targetBefore: " basks in the fiery heat."},
    {targetBefore: " basks in the freezing cold.", spell: "Ice Storm"},
    {targetBefore: " basks in the bitter cold.", spell: "Ice Storm"},

    // Enchantments and statuses
    {targetBefore: " is covered by a shimmering shield.", spell: "Shield"},
    {targetBefore: " is surrounded by a thick shimmering shield.", spell: "Protection"},
    {targetBefore: " is covered by a magical glowing shield.", spell: "Counter Spell"},
    {targetBefore: " is covered by a reflective shield.", spell: "Magic Mirror"},
    {targetBefore: " is covered in a coat of sparkling frost.", spell: "Resist Heat"},
    {targetBefore: " is covered in a warm glow.", spell: "Resist Cold"},
    {targetBefore: " starts to lose coordination.", spell: "Maladroitness"},
    {targetBefore: " is rendered maladroit!", spell: "Maladroitness"},
    {targetBefore: " looks a bit confused.", spell: "Confusion"},
    {
        targetBefore: " trips on its feet.",
        spell: function(battle) {
            return (battle && battle.maladroit) ? "Maladroitness" : "Confusion";
        }
    },
    {targetBefore: "'s hands start to stiffen.", spell: "Paralysis"},
    {targetBefore: "'s right hand is paralysed.", spell: "Paralysis", kind: "result"},
    {targetBefore: "'s left hand is paralysed.", spell: "Paralysis", kind: "result"},
    {targetBefore: " can't move to attack.", spell: "Paralysis", kind: "result"},
    {targetBefore: " cringes in fear.", spell: "Fear"},
    {targetBefore: " quakes in fear!", spell: "Fear"},
    {targetBefore: " is too scared to attack.", spell: "Fear", kind: "result"},
    {targetBefore: " starts to look blank.", spell: "Amnesia"},
    {targetBefore: " forgets ", spell: "Amnesia"},
    {targetBefore: " starts to look sick.", spell: "Disease"},
    {targetBefore: " looks nauseous.", spell: "Disease"},
    {targetBefore: " looks pale.", spell: "Disease"},
    {targetBefore: " breathes feverishly.", spell: "Disease"},
    {targetBefore: " staggers weakly.", spell: "Disease"},
    {targetBefore: " coughs weakly.", spell: "Disease"},
    {targetBefore: " looks intrigued by ", spell: "Charm Person"},
    {targetBefore: " looks, glassy-eyed, at ", spell: "Charm Monster"},
    {
        targetBetween: ["Scales start to grow over ", "'s eyes!"],
        spell: "Blindness"
    },
    {targetBefore: "'s eyes are covered with scales!", spell: "Blindness", kind: "result"},
    {
        targetBetween: ["The scales are removed from ", "'s eyes."],
        spell: "Blindness",
        kind: "result"
    },
    {targetBefore: " begins to shimmer.", spell: "Invisibility"},
    {
        targetBetween: ["There is a flash, and ", " disappears!"],
        spell: "Invisibility"
    },
    {targetBefore: " fades back into visibility.", spell: "Invisibility", kind: "result"},
    {targetBefore: " speeds up.", spell: "Haste"},
    {targetBefore: " flickers out of time!", spell: "Time Stop"},

    // Forced-gesture and other rows with no spell action
    {targetBefore: " confusedly makes the wrong gesture with his left hand."},
    {targetBefore: " confusedly makes the wrong gesture with his right hand."},
    {targetBefore: " makes a confused gesture, but luckily it's what he intended anyway!"},
    {targetBefore: " is charmed into making the wrong gesture with his left hand."},
    {targetBefore: " is charmed into making the wrong gesture with his right hand."},
    {targetBefore: " is charmed, but ends up making the gestures he intended anyway!"},
    {targetBefore: " is victorious!"},
    {targetBefore: " surrenders."},
    {targetBefore: " is summoned to serve "},
    {targetBefore: " tries to cast Clap of Lightning, but doesn't have the charge for it."},
    {targetBefore: " swings wildly for the monster "},
    {targetBefore: "'s right hand stab is wasted at a monster which wasn't summoned."},
    {targetBefore: " shakes his head and regains control, as enchantments cancel each other out."}
];

function parseSpellEffect(txt, battle) {
    for (var i = 0, Ln = EFFECT_RULES.length; i < Ln; ++i) {
        var res = applyStrangeRule(txt, EFFECT_RULES[i], battle);
        if (res) {
            return res;
        }
    }
    console.log("parseSpellEffect: unknown effect", txt);
    return newStrangeResult();
}

function parseAttackByText(txt) {
    var res = {};
    if (txt.indexOf(" attacks ") !== -1) {
        res = parseSimpleAttack(txt);
    } else if (txt.indexOf(" swings wildly ") !== -1) {
        res = parseSimpleAttack(txt, "swings wildly for");
    } else if (txt.indexOf(" stabs ") !== -1) {
        res = parseSimpleAttack(txt.replace(" stabs ", " attacks "));
    } else if (txt.indexOf("tries to attack") !== -1) {
        res = parseSimpleAttack(txt, "tries to attack");
    } else {
        // message like "Galbarad starts to lose coordination" or "Smelly Goblin is hit by a Magic Missile, for 1 damage."
        // moved to getMessageActionByOther
        // res = parseSpellEffect(txt);
    }

    //res = {aggressor:aggressor,target:target,success:txt.indexOf("damage") !== -1 ? 1 : 0};
    //console.log("parseAttackByText", txt, JSON.stringify(res));
    return res;
}

function getMessageActionByAttack(attack_obj, battle) {
    var res = [];
    var icon_action = {action:"icon",large_icon:"",small_icon:"",title:"",text:"",background_color:"#210430",border_color:"#FEE2D6"};
    //var attack_obj = parseAttackByText(txt);
    var aggressor_found = attack_obj.aggressor === "", target_found = attack_obj.target === "";
    if (attack_obj.aggressor || attack_obj.target) {
        for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
            //console.log("getMessageActionByAttack", i, Ln, battle.warlocks[i].name);
            if (battle.warlocks[i].name !== attack_obj.aggressor) {
                res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].pL, battle.warlocks[i].pR, 0, 0, 5)});
            }

            if (!aggressor_found) {
                if (battle.warlocks[i].name === attack_obj.aggressor) {
                    res.push(higlightWarlockGestureBySpell(battle.warlocks[i],
                                                           {warlock:attack_obj.aggressor,spell:"Stab",target:attack_obj.target},
                                                           battle));
                    aggressor_found = true;
                } else {
                    var aggressor_owner = findMonsterOwner(battle, attack_obj.aggressor);
                    if (aggressor_owner) {
                        res.push({action:"highlight",warlock_name:aggressor_owner,object_type:"monster",object:attack_obj.aggressor,data:[],role:"actor",color:"#10C9F5"});
                        aggressor_found = true;
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
                    var target_owner = findMonsterOwner(battle, attack_obj.target);
                    if (target_owner) {
                        res.push({action:"highlight",warlock_name:target_owner,object_type:"monster",object:attack_obj.target,data:[],role:"target",color:attack_obj.target === attack_obj.aggressor ? "#10C9F5" : "#FEE2D6"});
                        target_found = true;
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
    //var obj = parseSpellEffect(msg);
    //console.log("getMessageActionByOther", JSON.stringify(obj));
    var icon_action = {action:"icon",large_icon:"",small_icon:"",title:"",text:"",background_color:"#210430",border_color:"#FEE2D6"};
    var target_found = false;
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        res.push({action:"highlight",warlock_name:battle.warlocks[i].name,object_type:"warlock",object:"gestures",data:preparePrintGestures(battle.warlocks[i].pL, battle.warlocks[i].pR, 0, 0, 5)});
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
                    obj.countered_spell = w.arr_distruption.shift();
                }
            }
            if (obj.banked) {
                // Turn news runs on the turn-start board: the bank icon is not
                // filled yet, while the delay status it consumes is still listed.
                res.push({action:"status_remove",warlock_name:w.name,object_type:"status",object:"delay"});
                res.push({action:"highlight",warlock_name:w.name,object_type:"banked",object:"banked",
                          value:w.banked_spell,data:[],role:"target",color:"#10C9F5"});
                icon_action.large_icon = "banked_spell";
                icon_action.text = "";
            } else {
                // https://github.com/Pz1c/WavingHands/issues/227
                // An effect row only lights the status card that is already there.
                // It must not re-create or refresh the status: a carried status is
                // still decremented at the end of this turn.
                var status_code = ((obj.damage > 0) || obj.cured) ? "" : getNewsStatusCode(obj, w);
                if (status_code) {
                    var status_action = {action:"highlight",warlock_name:w.name,object_type:"status",object:status_code,
                                         icon:getNewsStatusIcon(status_code),data:[],role:"target",
                                         color:isHarmfulNews(obj) ? "#FEE2D6" : "#10C9F5"};
                    res.push(status_action);
                } else {
                    res.push({action:"highlight",warlock_name:w.name,object_type:"warlock",object:"hp",data:[],color:"#FEE2D6"});
                }
            }
            target_found = true;
            //break;
        }

        // the row is about one of this warlock's monsters: "Smelly Goblin is hit by a Magic Missile",
        // "Green Troll is covered by a shimmering shield", "Big Goblin is summoned to serve Bob"
        // https://github.com/Pz1c/WavingHands/issues/227
        if (findMonsterOwner(battle, obj.target) === w.name) {
            var m = findMonsterByName(battle, obj.target);
            if (m && (obj.spell === "Distruption")) {
                if (m.arr_distruption) {
                    obj.countered_spell = m.arr_distruption.shift();
                }
                target_found = true;
            }
            // hit by a spell or an attack: the hit color; nothing happened to it (it just appeared): the actor color
            res.push({action:"highlight",warlock_name:w.name,object_type:"monster",object:obj.target,data:[],role:"target",
                      color:((obj.spell !== "") || (obj.damage > 0)) ? "#FEE2D6" : "#10C9F5"});
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
        res.push({action:"highlight",warlock_name:w.name,object_type:"warlock",object:"gestures",data:preparePrintGestures(w.pL, w.pR, 0, 0, 5)});
        if (w.name === obj.target) {
            res.push({action:"highlight",warlock_name:w.name,object_type:"warlock",object:"hp",data:[],color:"#FEE2D6"});
            continue;
        }
        if (findMonsterOwner(battle, obj.target) === w.name) {
            res.push({action:"highlight",warlock_name:w.name,object_type:"monster",object:obj.target,data:[],role:"target",color:"#FEE2D6"});
        }
    }
    res.push({action:"icon",large_icon:"RIP2",small_icon:icon,title:"",text:"",background_color:"#210430",border_color:"#FEE2D6"});
    return res;
}

// A blinded monster is destroyed outright; the line is the spell's result,
// not the generic death row.
var re_blindness_annihilation =
        / is hit by a Blindness spell, and is annihilated by the magical overload\.?$/i;

function getMessageTypeByRow(row) {
    if (row.color === "#CCCCCC") {
        return "gesture";
    } else if (row.txt.indexOf(" casts ") !== -1) {
        // process spell casting
        return "spell";
    } else if (row.color === "#FF6666") {
        if (re_blindness_annihilation.test(row.txt)) {
            return "other";
        }
        return "death";
    } else if (((row.txt.indexOf(" attack") !== -1) || (row.txt.indexOf(" swing ") !== -1) ||
                (row.txt.indexOf(" stabs ") !== -1)) &&
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
    case "other":   return parseSpellEffect(obj.txt, battle);
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


function spellsPair(cast_spell, effect_spell) {
    if (cast_spell === effect_spell) {
        return true;
    }
    if ((cast_spell === "Clap of Lightning") &&
            (effect_spell === "Lightning Bolt")) {
        return true;
    }
    if ((cast_spell === "Cure Heavy Wounds") &&
            (effect_spell === "Cure Light Wounds")) {
        return true;
    }
    if ((cast_spell === "Cause Heavy Wounds") &&
            (effect_spell === "Cause Light Wounds")) {
        return true;
    }
    return (cast_spell === "Poison") && (effect_spell === "Disease");
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
        if (actions[i].txt.indexOf("shakes his head") !== -1) {
            actions[i].type = 1;
            actions[i].checked = true;
            actions[i].turn_news_force_show = true;

            // Both disruption enchantments cancel out, so remove every
            // disruption status the target could have.
            var target = actions[i].obj ? actions[i].obj.target : "";
            var codes = ["paralized", "confused", "charmed",
                         "scared", "amnesia", "maladroit"];
            if (target && actions[i].new_action) {
                for (var c = 0; c < codes.length; ++c) {
                    actions[i].new_action.push({
                        action:"status_remove",
                        warlock_name:target,
                        object_type:"status",
                        object:codes[c],
                        defer_to_end:true
                    });
                }
            }
            continue;
        }
        if (actions[i].checked) {
            continue;
        }

        if (actions[i].row_type === "spell") {
            if (actions[i].obj.fail !== "") {
                actions[i].checked = true;
                continue;
            }

            for (j = i + 1; j < Ln; ++j) {
                console.log("sortRealAction", i, j, JSON.stringify(actions[j].obj));
                if (actions[j].obj && actions[j].obj.kind === "result") {
                    continue;
                }
                if (((spellsPair(actions[i].obj.spell, actions[j].obj.spell)) && (actions[i].obj.target === actions[j].obj.target))
                        || (actions[i].summon_id && (actions[i].summon_id === actions[j].summoned_id))) {
                    actions[j].depends_on.push(actions[i].id);
                    actions[i].checked = true;
                    actions[j].checked = true;
                    console.log("sortRealAction", i, j, "FOUND");
                    if (i !== j - 1) {
                        // Move the cast row next to its result instead of keeping a hidden
                        // original and inserting a visible copy. Both rows stay checked so
                        // the outer loop does not process them again.
                        var moved = actions.splice(i, 1)[0];
                        actions.splice(j - 1, 0, moved);
                        --i;
                        /*console.log("sortRealAction", "AFTER MOVE", i, Ln);
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

function processHintText(obj, dict) {
    console.log("processHintText.1", obj.txt, obj.font_size);
    if (obj.txt.indexOf(" attacks ") !== -1) {
        obj.txt = obj.txt.substring(0, obj.txt.indexOf(" attacks ") + 8);
        obj.font_size = 28;
    } else if (obj.txt.indexOf(" stabs ") !== -1) {
        obj.txt = obj.txt.substring(0, obj.txt.indexOf(" stabs ") + 7);
        obj.font_size = 28;
    } else if (obj.txt.indexOf(" is hit by a") !== -1) {
        obj.txt = obj.txt.substring(0, obj.txt.indexOf(" is hit by a") + 7);
        obj.font_size = 28;
    } else if (obj.txt.indexOf(" hand is paralysed") !== -1) {
        obj.txt = dict.getStringByCode("HintIsParalyzed").replace("%1", obj.obj.target);
        obj.font_size = 28;
    } else if (obj.txt.indexOf(" starts to lose coordination") !== -1) {
        obj.txt = dict.getStringByCode("HintLosesCoordination").replace("%1", obj.txt.substring(0, obj.txt.indexOf(" starts to lose coordination")));
        obj.font_size = 28;
    }
    console.log("processHintText.2", obj.txt, obj.font_size);
}

// https://github.com/Pz1c/WavingHands/issues/227
// Turn news icons for what the end-of-turn board does not show any more.
//
// The turn news now mutates the live monster and status lists. The summon rows are still paired
// here so the event rows can name the owner of a monster that is not on the snapshot board yet.
var news_status_value = {"blindness":3,"invisibility":3,"poison":6,"disease":6,
                         "permanency":3,"fireproof":999,"coldproof":999,"haste":3,"time_stop":1,
                         "delay":3,"scared":1,"confused":1,"charmed":1,
                         "paralized":1,"amnesia":1,"maladroit":1};

function getNewsStatusIcon(code) {
    if (icon_status_code_to_icon[code]) {
        return icon_status_code_to_icon[code];
    }
    return map_news_status_to_icon[code] ? map_news_status_to_icon[code] : code;
}

function getNewsStatusValue(code, spell) {
    if (spell === "Protection") {
        return 2;
    }
    // Every instant status defaults to 0 and is removed when the news closes.
    return news_status_value[code] !== undefined ? news_status_value[code] : 0;
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

// owner from the summon records for a monster that is not on the live board yet
function findSummonOwner(battle, name) {
    var arrivals = battle.news_summons && battle.news_summons.arrivals;
    if (!arrivals) {
        return "";
    }
    for (var i = 0, Ln = arrivals.length; i < Ln; ++i) {
        if (arrivals[i].name === name) {
            return arrivals[i].owner;
        }
    }
    return "";
}

// owner of a live or part-of-this-turn monster; "" when there is none
function findMonsterOwner(battle, name) {
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        var monsters = w.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === name) {
                return w.name;
            }
        }
    }
    return findSummonOwner(battle, name);
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

// Scan and pair the summon rows once. prepareNewsMonsters() needs the records before
// BGU.prepareAndSortRealAction() (the annotations below are used by the parser), while
// prepareTurnNewsTransitions() needs the same records after parsing. Returning them keeps a
// single parser instead of three independent scans of the summon rows.
function collectNewsSummons(rows, battle) {
    var i, Ln, j, LnJ, m, txt, rec, owner;
    var casts = [], arrivals = [], byId = {};
    var re_cast = /^(\S+) casts (?:his |her |its |their )?(?:banked )?Summon (Goblin|Ogre|Troll|Giant) at ([^,.]+)/;
    var re_summoned = /^(.+?) is summoned to serve ([^.]+)\.?$/;
    var re_summoning = /the monster (\S+) is summoning with (?:his|her|its|their) (left|right) hand/;
    for (i = 0, Ln = rows.length; i < Ln; ++i) {
        txt = rows[i].txt;
        if ((m = re_cast.exec(txt))) {
            owner = m[3].trim();
            if ((owner === "himself") || (owner === "herself") || (owner === "itself")) {
                owner = m[1];
            } else if (!isWarlockName(battle, owner)) {
                // cast at a monster: its controller gets the new one
                var mo = findMonsterOwner(battle, owner), ms = re_summoning.exec(owner);
                owner = mo ? mo : (ms ? ms[1] : m[1]);
            }
            rec = {id:casts.length + 1,caster:m[1],owner:owner,type:m[2],hand:getSpellHand(findWarlockByName(battle, m[1]), "Summon " + m[2]),name:"",key:"",real:false};
            casts.push(rec);
            byId[rec.id] = rec;
            rows[i].summon = rec;
            rows[i].summon_id = rec.id;
        } else if ((m = re_summoned.exec(txt))) {
            arrivals.push({row:rows[i],name:m[1].trim(),owner:m[2].trim(),rec:null});
        }
    }
    // the casts and the arrivals: same controller, same kind, in order
    for (i = 0, Ln = casts.length; i < Ln; ++i) {
        rec = casts[i];
        for (j = 0, LnJ = arrivals.length; j < LnJ; ++j) {
            if (arrivals[j].rec || (arrivals[j].owner.toLowerCase() !== rec.owner.toLowerCase()) || (arrivals[j].name.indexOf(rec.type) === -1)) {
                continue;
            }
            arrivals[j].rec = rec;
            arrivals[j].row.summoned_id = rec.id;
            rec.name = arrivals[j].name;
            break;
        }
        rec.real = (rec.name !== "") && !!(battle.targetsMap && battle.targetsMap[rec.name]);
        if (rec.real) {
            rec.key = rec.name;
        } else {
            rec.key = rec.name !== "" ? rec.name : ("New " + rec.type + " of " + rec.owner);
        }
        // Master identifies the summoned monster even when it is not on the
        // end-of-turn board: strength from the summon type, icon from the name
        // or the new_<type> placeholder.
        rec.strength = SUMMON_SPELL_TYPES.indexOf(rec.type) + 1;
        rec.hp = rec.strength;
        rec.icon = rec.name !== "" ? getMonsterIconByName(rec.name)
                                   : "new_" + rec.type.toLowerCase();
    }
    // targets like "the monster X is summoning with his left hand"
    for (i = 0, Ln = rows.length; i < Ln; ++i) {
        if ((m = re_summoning.exec(rows[i].txt))) {
            rec = findSummonRecord(casts, m[1], m[2]);
            if (rec) {
                rows[i].summon_target = rec;
            }
        }
    }
    return {casts:casts,arrivals:arrivals,byId:byId};
}

function prepareNewsMonsters(rows, battle) {
    var news = collectNewsSummons(rows, battle);
    battle.news_summons = news;
    console.log("prepareNewsMonsters", JSON.stringify(news.casts));
    return news;
}

// A Counter Spell removes the enchantment it stopped. The row itself already
// lights the defense icon; this action removes the stopped status from the live
// status list.
function getStatusRemovalAction(row) {
    if ((row.row_type !== "other") || !row.obj || !row.obj.counter_spell || !row.obj.target) {
        return null;
    }
    var code = map_spell_name_to_status[row.obj.spell];
    if (!code) {
        return null;
    }
    return {action:"status_remove",warlock_name:row.obj.target,object_type:"status",object:code};
}
