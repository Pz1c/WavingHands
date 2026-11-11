Qt.include("battle_gui_utils.js");

function replaceAll(str, find, replace) {
  return str.replace(new RegExp(find, 'g'), replace);
}

function prepareDataTypeFinishedGame(l_data, dict) {
    switch(l_data.sc) {
    case "Win":
        iiIcon.source = "qrc:/res/stars.png";
        break;
    case "Lose":
        iiIcon.source = "qrc:/res/RIP2.png";
        break;
    case "Draw":
        iiIcon.source = "qrc:/res/stab.png";
        break;
    case "Unknown":
        iiIcon.source = "qrc:/res/summon.png";
        break;
    }

    ltGesture.text = l_data.t;
    ltTitle.text = dict.getStringByCode("FinishedGameResultTitle" + l_data.sc);
    // somebody else's battle (https://github.com/Pz1c/WavingHands/issues/121): the result of the warlock it is shown for
    if (l_data.spectator && ((l_data.sc === "Win") || (l_data.sc === "Lose"))) {
        ltTitle.text = dict.getStringByCode("SpectatorResultTitle" + l_data.sc).replace("%1", l_data.spectator);
    }
    ltShortDesc.text = l_data.st ? replaceAll(l_data.st, "<br>", " ") : "";
    ltError.text = "<font color=\"#0654C0\">" + dict.getStringByCode("LastTurnChat") + "</font><br />" + replaceAll(replaceAll(l_data.lc, "''", '"'), '&quot;', '"');
    bbSkipAction.text = dict.getStringByCode("RematchWith").replace("%1", l_data.enemy);
    bbSkipAction.visible = !l_data.with_bot && !l_data.spectator;
}

function prepareDataTypeSpell(spell_code, dict) {
    var icon = map_spell_to_icon[spell_code];
    if (icon) {
        iiIcon.source = "qrc:/res/" + icon + ".png";
    }
    ltGesture.text = spell_code;
    ltTitle.text = dict.getStringByCode(spell_code);
    ltShortDesc.text = dict.getStringByCode(spell_code + "_short_desc");
    ltError.text = dict.getStringByCode(spell_code + "_desc");
}

