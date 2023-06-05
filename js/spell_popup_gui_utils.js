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
    ltShortDesc.text = l_data.st ? replaceAll(l_data.st, "<br>", " ") : "";
    ltError.text = "<font color=\"#0654C0\">LAst turn's chat</font><br />" + replaceAll(replaceAll(l_data.lc, "''", '"'), '&quot;', '"');
    bbSkipAction.text = "Rematch " + l_data.enemy;
    bbSkipAction.visible = !l_data.with_bot;
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

