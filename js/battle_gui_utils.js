var icon_status_code = ["scared","confused","charmed","paralized","shield","coldproof","fireproof","poison","desease","amnesia","maladroit","mshield","delay",
                        "time_stop","haste","permanency","blindness","invisibility"];
var icon_status_code_to_icon = {"confused":"maladroit","time_stop":"haste"};
var icon_status_spell = {"scared":"SWD","confused":"DSF","charmed":"PSDF","paralized":"FFF","shield":"WWP","coldproof":"SSFP","fireproof":"WWFP","poison":"DWWFWD","desease":"DSFFFc",
                         "amnesia":"DPP","maladroit":"DSF","mshield":"WWS","delay":"DWSSSP","time_stop":"SPPc","haste":"PWPWWc","permanency":"SPFPSDW","blindness":"DWFFd","invisibility":"PPws"};

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

function preparePrintGestures(GL, GR, mscL, mscR) {
    var res = [], item, ll, lr;
    //GL = GL.toLowerCase();
    //GR = GR.toLowerCase();
    for(var i = 1, Ln = GL.length; i < Ln; ++i) {
        ll = getIconByGesture(GL.substr(i, 1));
        lr = getIconByGesture(GR.substr(i, 1));
        item = {l:'g_'+ll, r: 'g_' + lr, lv: ll !== ' ', rv: lr !== ' ', la: i >= Ln - mscL, ra: i >= Ln - mscR};
        res.push(item);
    }
    return res;
}

function getMonsterIconByName(name) {
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
        a = {action: code, icon: icon_name, value: val, active: (w.control_paralyze && (code === "paralized")) || (w.control_charmed && (code === "charmed"))};
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
    if (action.s.gp === "?") {
        return "Default spell";
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
        return "Default";
    }
    return action.target;
}

// res.push({type:"RH",g:actions.R.g,s:getSpellNameForOrderReview(actions.R),t:getSpellTargetForOrderReview(actions.R, battle.targetsMap)});
function getTextForHandAction(hand, action, targetMap, dict) {
    var res = dict.getStringByCode(hand) + ": ";
    res += action.g + " cast " + getSpellNameForOrderReview(action) + " on " + getSpellTargetForOrderReview(action, targetMap);
    return res;
}

function getSpecActionText(type, action, dict) {
    return dict.getStringByCode("TitleAction_" + type) + getHandTitleByIdx(action);
}

function getCharmActionText(type, action, target, dict) {

    return dict.getStringByCode("TitleAction_" + type).replace("%1", getHandTitleByIdx(action.h)).replace("%2", action.g) + target;
}

function getMonsterActionText(action, target, targetMap, dict) {
    var t = targetMap[action.id] + dict.getStringByCode("TitleAction_M") + target;
    if (!target) {
        t += "Default";
    }

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
