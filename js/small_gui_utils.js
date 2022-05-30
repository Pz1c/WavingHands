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
