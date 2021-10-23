function cleanUpWindow(ratioObject) {
    ltDesc.visible = true;
    bbAction.visible = false;
    bbSkipAction.visible = false;
    bbAction.transparent = false;
    bbAction.gradient = gDef;
    bbAction.font.underline = false;
    svMain.anchors.top = ltDesc.bottom;
    svMain.anchors.topMargin = 20 * ratioObject;
}

function prepareDataType12(l_data, dict) {
    l_data.action = "leave";
    ltTitle.text = dict.getStringByCode('DefNotStartTitle');
    ltShortDesc.text = dict.getStringByCode('DefNotStartSTitle');
    ltError.text = "";
    if (l_data.d) {
        var tarr = l_data.d.split(" ");
        if (!tarr[0] || (tarr[0] === '')) {
            ltError.text = "No one other warlock in then circle<br>";
        } else {
            ltError.text = "Warlocks waiting in the circle: " + tarr[0].replace(",", ", ") + "<br>";
        }
        var tarr2 = tarr[1].split("/");
        ltError.text += "Waiting for: " + (tarr2[1] * 1 - tarr2[0] * 1) + " warlocks<br><br>";
    }
    ltError.text += dict.getStringByCode('DefNotStartDesc');
    bbAction.text = "Leave";
    bbAction.visible = true;
}

function prepareDataType11(l_data, dict) {
    ltTitle.text = dict.getStringByCode('DefErrTitle');
    ltShortDesc.text = dict.getStringByCode('JoinBattleError');
    ltError.text = l_data.d ? replaceAll(l_data.d, "''", '"') : "Unknown error";
}

function prepareDataType10(l_data, dict) {
    l_data.action = "reg_info";
    ltTitle.text = dict.getStringByCode('DefErrTitle');
    ltShortDesc.text = dict.getStringByCode('BattleCreateError');
    ltError.text = dict.getStringByCode('BattleCreateErrorDetails');
    bbAction.gradient = undefined;
    bbAction.font.underline = true;
    bbAction.text = dict.getStringByCode('LearnMore');
    bbAction.transparent = true;
    bbAction.visible = true;
}

function prepareDataType9(l_data, dict) {
    ltDesc.visible = false;
    svMain.anchors.topMargin = 0;
    ltTitle.text = l_data.t;
    ltShortDesc.text = l_data.st ? l_data.st : "";
    ltError.text = replaceAll(l_data.d, "''", '"');
}

function prepareDataType8(l_data, dict) {
    ltTitle.text = "Orders Submitted";
    ltShortDesc.text = "Your orders are in for this turn";
    ltError.text = l_data.d;
    bbAction.text = "Force Turn";//dict.getStringByCode("MonsterSetTarget");
    if (l_data.fst > 0) {
        bbAction.visible = true;
        l_data.action = "force";
    }
}

function prepareMonsterDetails(l_data, dict) {
    icon.source = "qrc:/res/" + l_data.icon + ".png";
    icon.visible = true;
    icon.text = l_data.hp;
    var details = "Life: " + l_data.hp + "<br>Power: "+l_data.strength;
    if (l_data.name.indexOf("RH:") === 0) {
        ltTitle.text = dict.getStringByCode("RH_summon");
        ltShortDesc.text = dict.getStringByCode("WH_summon");
        if (l_data.hp === "R") {
            details = "Life: Unknown<br>Power: Unknown";
        } else {
            ltTitle.text += " ("+BGU.getMonsterNameByStrength(l_data.strength)+")";
        }
    } else if (l_data.name.indexOf("LH:") === 0) {
        ltTitle.text = dict.getStringByCode("LH_summon");
        ltShortDesc.text = dict.getStringByCode("WH_summon");
        if (l_data.hp === "L") {
            details = "Life: Unknown<br>Power: Unknown";
        } else {
            ltTitle.text += " ("+BGU.getMonsterNameByStrength(l_data.strength) +")";
        }
    } else {
      ltTitle.text = l_data.name;
      ltShortDesc.text = dict.getStringByCode("Warlock");
    }
    details += "<br>Owner: " + l_data.owner + "<br>Target: " + l_data.target;
    ltError.text = details;
    bbAction.text = dict.getStringByCode("MonsterSetTarget");
    bbAction.visible = l_data.under_control;
}

function prepareHPDetails(l_data, dict) {
    icon.source = "qrc:/res/heart_small.png";
    icon.text = l_data.value;
    icon.visible = true;
    ltTitle.text = l_data.warlock_name;
    ltShortDesc.text = dict.getStringByCode("Warlock");
    ltError.text = "Life: " + l_data.value;
}

function prepareSpellDescription(spell_code, spell_icon) {
    if (spell_icon) {
        icon.source = "qrc:/res/"+spell_icon+".png";
        icon.visible = true;
    } else {
        icon.visible = false;
    }

    ltTitle.text = dict.getStringByCode(spell_code);
    ltShortDesc.text = dict.getStringByCode(spell_code + "_short_desc");
    ltError.font.pixelSize = 21 * mainWindow.ratioFont;
    ltError.text = dict.getStringByCode(spell_code + "_desc");
}

function prepareNotReadyFunctionalMsg(l_data, dict) {
    l_data.action = 'joinus';
    icon.source = "qrc:/res/summon.png";
    icon.visible = true;
    bbAction.visible = true;
    bbAction.text = dict.getStringByCode("JoinUs");
    ltTitle.text = dict.getStringByCode("FnNotReadyTitle");
    ltShortDesc.text = dict.getStringByCode("FnNotReady_short_desc");
    ltError.font.pixelSize = 21 * mainWindow.ratioFont;
    ltError.text = dict.getStringByCode("FnNotReady_desc");
}

function prepareFeedback(l_data, dict) {
    bbAction.visible = true;
    bbAction.text = dict.getStringByCode("Feedback");
    bbSkipAction.visible = true;
    bbSkipAction.text = dict.getStringByCode("Skip");
    ltDesc.visible = false;
    ltTitle.text = dict.getStringByCode("FeedbackTitle");
    ltShortDesc.text = dict.getStringByCode("Feedback_short_desc");
    ltError.text = "";
}

function prepareRateUs(l_data, dict) {
    bbAction.visible = true;
    bbAction.text = dict.getStringByCode("RateUsAction");
    bbSkipAction.visible = true;
    bbSkipAction.text = dict.getStringByCode("SendFeedback");
    ltDesc.visible = false;
    ltTitle.text = dict.getStringByCode("RateUs");
    ltShortDesc.text = dict.getStringByCode("RateUs_short_desc");
    ltError.text = "";
}

function prepareUserScore(l_data, dict) {
    ltDesc.visible = true;
    ltTitle.text = l_data.name + ": Stats";
    ltShortDesc.visible = false;
    ltError.text = "Score: " + l_data.elo + "<br>Played: " + l_data.played + "<br>Won: " + l_data.won + "<br>Died: " + l_data.died;
}

function replaceAll(str, find, replace) {
  return str.replace(new RegExp(find, 'g'), replace);
}

function prepareBattleHistory(l_data, dict) {
    ltDesc.visible = false;
    ltTitle.visible = true;
    ltTitle.text = dict.getStringByCode("BattleHistoryTitle");
    ltShortDesc.visible = false;
    svMain.anchors.top = ltTitle.bottom;
    ltError.text = replaceAll(l_data.history, "&quot;", '"');
}
