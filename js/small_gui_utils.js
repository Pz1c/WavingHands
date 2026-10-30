var paraCFMap = {"C":"F","S":"D","W":"P"};
var paraFCMap = {"F":"C","S":"D","W":"P"};

function getNextParalyzedGesture(LastGesture, IsFC) {
    var res = IsFC ? paraFCMap[LastGesture] : paraCFMap[LastGesture];
    if (res) {
        return res;
    }
    return LastGesture;
}

function getIconByGesture(G, with_prefix) {
    //if (G === ' ') {
        //G = '-';
    //}
    if (G === '>') {
        G = '_';
    }
    if (G === '?') {
        G = '=';
    }
    var res = '';
    if (with_prefix) {
        res += "g_";
    }
    res += G.toLowerCase();
    return res;
}

function getFullIconPathByGesture(G) {
    return "qrc:/res/g_" + getIconByGesture(G) + ".png";
}

function getMonsterNameByStrength(Strength) {
    switch(Strength) {
    case 1: return "Goblin";
    case 2: return "Ogre";
    case 3: return "Trol";
    case 4: return "Giant";
    default: return "Unknown";
    }
}

function local_log_spell_event(spell_name) {
    mainWindow.logEvent("Play_Gesture_Spell", {Spell:spell_name});
}

function getTargetTitle(IsSpell, Title, TargetName, Permanency, Delay, NoColor) {
    var res = "", dict = mainWindow.warlockDictionary;
    if (IsSpell && (Title.indexOf("Summon") !== -1) && (Title.indexOf("Elemental") === -1)) {
        res = dict.getStringByCode("TargetTitleSummon").replace("%1", Title);
    } else if (IsSpell) {
        res = dict.getStringByCode("TargetTitleCast").replace("%1", Title);
        if (Title.indexOf("Counter") !== -1) {
            res += dict.getStringByCode("TargetTitleSpell");
        }
        if (Permanency) {
            res += dict.getStringByCode("TargetTitlePermanent");
        }
        if (Delay) {
            res += dict.getStringByCode("TargetTitleBank");
        }

        res += dict.getStringByCode("TargetTitleAt");
    } else {
        res = dict.getStringByCode("TargetTitleDirect").replace("%1", Title);
        res = res.replace("Right Hand Monster", "Right Hand");
        res = res.replace("Left Hand Monster", "Left Hand");
    }
    if (!NoColor) {
        res += '<font color="#10C9F5">';
    }
    res += TargetName;
    if (!NoColor) {
        res += '</font>';
    }

    return res;
}
