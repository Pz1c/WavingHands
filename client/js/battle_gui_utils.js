var icon_status_code = ["scared","confused","charmed","paralized","shield","coldproof","fireproof","poison","desease","amnesia","maladroit","mshield","delay",
                        "time_stop","haste","permanency","blindness","invisibility"];
var icon_status_code_to_icon = {"confused":"maladroit","time_stop":"haste"};
var icon_status_spell = {"scared":"SWD","confused":"DSF","charmed":"PSDF","paralized":"FFF","shield":"WWP","coldproof":"SSFP","fireproof":"WWFP","poison":"DWWFWD","desease":"DSFFFc",
                         "amnesia":"DPP","maladroit":"DSF","mshield":"WWS","delay":"DWSSSP","time_stop":"SPPc","haste":"PWPWWc","permanency":"SPFPSDW","blindness":"DWFFd","invisibility":"PPws"};

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
    if (G === ' ') {
        G = '_';
    }
    if (G === '?') {
        G = '=';
    }
    return G.toLowerCase();
}

function preparePrintGestures(GL, GR) {
    var res = [], item, ll, lr;
    //GL = GL.toLowerCase();
    //GR = GR.toLowerCase();
    for(var i = 1, Ln = GL.length; i < Ln; ++i) {
        ll = getIconByGesture(GL.substr(i, 1));
        lr = getIconByGesture(GR.substr(i, 1));
        item = {l:'g_'+ll, r: 'g_' + lr, lv: ll !== '_', rv: lr !== '_'};
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
    var res = [], code, val, icon_name;
    for (var i = 0, Ln = icon_status_code.length; i < Ln; ++i) {
        code = icon_status_code[i];
        if (!w[code] || (w[code] === 0)) {
            continue;
        }
        icon_name = icon_status_code_to_icon[code] ? icon_status_code_to_icon[code] : code;
        val = w[code] === 999 ? "∞" : w[code];
        res.push({action: code, icon: icon_name, value: val, active: (w.control_paralyze && (code === "paralized")) || (w.control_charmed && (code === "charmed"))});
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
        return "Unknown";
    }
    // default spell
    if (action.s.gp === "?") {
        return "Default";
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
    res += action.g + " cast " + getSpellNameForOrderReview(action) + " spell on " + getSpellTargetForOrderReview(action, targetMap);
    return res;
}

function getSpecActionText(type, action, dict) {
    return dict.getStringByCode("TitleAction_" + type) + getHandTitleByIdx(action);
}

function getCharmActionText(type, action, target, dict) {

    return dict.getStringByCode("TitleAction_" + type).replace("%1", getHandTitleByIdx(action.h)).replace("%2", action.g) + target;
}

function getMonsterActionText(action, target, targetMap, dict) {
    return targetMap[action.id] + dict.getStringByCode("TitleAction_M") + target;
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
