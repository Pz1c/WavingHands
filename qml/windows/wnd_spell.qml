// source https://qt-project.org/forums/viewthread/26455

import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/qml/components"

import "qrc:/js/battle_gui_utils.js" as BGU

InfoWindow {
    id: dMainItem

    Item {
        id: dialogWindow
        anchors.fill: content_item
        z: 15

        Item {
            id: iItem
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 24 * mainWindow.ratioObject
            anchors.right: parent.right
            anchors.rightMargin: 24 * mainWindow.ratioObject
            anchors.bottom: parent.bottom
            z: 17

            Text {
                id: ltTitle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 70 * mainWindow.ratioObject
                font.pixelSize: 42 * mainWindow.ratioFont
                color: "#10C9F5"
                horizontalAlignment: Text.AlignLeft
                //fontSizeMode: Text.VerticalFit
            }

            Text {
                id: ltShortDesc
                anchors.top: ltTitle.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                //height: 50 * mainWindow.ratioObject
                color: "#FEE2D6"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: 28 * mainWindow.ratioFont
                //fontSizeMode: Text.VerticalFit
                wrapMode: Text.WordWrap
            }

            Text {
                id: ltDesc
                anchors.top: ltShortDesc.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: 28 * mainWindow.ratioFont
                color: "#10C9F5"
                text: "Details:"
                horizontalAlignment: Text.AlignLeft
                //fontSizeMode: Text.VerticalFit
            }

            ScrollView {
                id: svMain
                anchors.top: ltDesc.bottom
                anchors.topMargin: 20 * mainWindow.ratioObject
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AsNeeded

                Text {
                    id: ltError
                    width: svMain.width
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                    font.pixelSize: 28 * mainWindow.ratioFont
                    color: "#FEE2D6"
                    horizontalAlignment: Text.AlignJustify
                }
            }

            BtnBig {
                id: bbAction
                text_color: "#ABF4F4"
                text: dict.getStringByCode("MonsterSetTarget")
                bg_color_active: "#551470"
                border_color_active: "#551470"
                radius: 30
                visible: false
                z: 30

                gradient: Gradient {
                    GradientStop { position: 0.0 ; color: "#905B93" }
                    GradientStop { position: 0.75; color: "#551470" }
                    GradientStop { position: 1.0 ; color: "#551470" }
                }

                width: 366 * ratioObject
                height: 96 * ratioObject
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 144 * ratioObject
                anchors.horizontalCenter: parent.horizontalCenter


                onClicked: {
                    console.log("chooseMonsterTarget", JSON.stringify(l_data));
                    if (l_data.action === "force") {
                        mainWindow.gameCore.forceSurrender(l_data.id, l_data.fst);
                    } else if (l_data.under_control) {
                        mainWindow.chooseMonsterTarget(ltTitle.text);
                    }
                }
            }
        }
    }

    onCancel: {
        mainWindow.processEscape();
    }

    function showWnd() {
        initErrFields();
        visible = true;
    }

    function hideWnd() {
        visible = false;
    }

    function replaceAll(str, find, replace) {
      return str.replace(new RegExp(find, 'g'), replace);
    }

    property string last_line: "";
    property string red_desc: "";
    property int last_turn: 0;
    property int curr_turn: 0;

    function processTurnMessage(turn_msg) {
        var result = "", lines = turn_msg.split("<FONT "), line, sub_line, sslines, ssln, ssslines, sssline, def_color = "#FEE2D6", current_color = def_color, idx1, idx2;
        for (var i = 0, Ln = lines.length; i < Ln; ++i) {
            //line = replaceAll(lines[i], "</FONT>", "").trim();
            line = lines[i].trim();
            console.log("wnd_spell.processTurnMessage", "main", line, i, Ln, curr_turn, last_turn);
            if (!line || (line === "")) {
                continue;
            }
            idx1 = line.indexOf('COLOR="');
            if (idx1 !== -1) {
                idx1 += 16;
                current_color = line.substr(idx1 - 9, 7);
            } else {
                idx1 = 0;
                current_color = def_color;
            }
            sub_line = line.substr(idx1);
            console.log("wnd_spell.processTurnMessage", "sub", current_color, sub_line);
            sslines = sub_line.split("</FONT>");
            for(var j = 0, LnJ = sslines.length; j < LnJ; ++j) {
                ssln = sslines[j].trim();
                if (!ssln || (ssln === "")) {
                    continue;
                }
                ssslines = ssln.split("<BR>");
                for(var k = 0, LnK = ssslines.length; k < LnK; ++k) {
                    sssline = ssslines[k].trim();
                    if (!sssline || (sssline === "")) {
                        continue;
                    }
                    if ((current_color !== "#CCCCCC") || (curr_turn === 0)) {
                        result += '<font color="'+current_color+'">'+sssline+'</font><br>';
                    } else {
                        continue;
                    }
                    last_line = sssline;
                    if ((curr_turn === last_turn) && (current_color === "#FF6666")) {
                        red_desc += sssline + "<br>";
                    }
                }
                current_color = def_color
            }
        }
        return result;
    }

    function processFinishedBattleText(txt) {
        var marr = replaceAll(/*replaceAll(*/replaceAll(txt, "''", '"')/*, "<FONT COLOR=", "<FONTCOLOR=")*/, "<TABLE CELLPADDING=0 CELLSPACING=0 BORDER=0 ><TR>", "<U>Turn").split("<U>Turn"), turn_txt, idx1;
        //console.log(JSON.stringify(marr));
        //ltShortDesc.text = "Turn " + marr[0].replace('<U>', '').replace('</U>', '').trim();
        var first = true, second = true, result = "", line;
        for (var i = 0, Ln = marr.length; i < Ln; ++i) {
            line = marr[i].trim();
            if (!line || (line === "")) {
                continue;
            }
            console.log("processFinishedBattleText", "line", line, first, second);

            if (first) {
                last_turn = line.substr(0, line.indexOf(" ")) * 1;
                //ltTitle.text = marr[i];//.substr(0, marr[i].indexOf(" ") - 1);
                ltShortDesc.text = "Turn " + line.replace('<U>', '').replace('</U>', '').trim();
                first = false;
                continue;
            }

            if ((i === Ln - 1) && (line.indexOf("monoturn") !== -1)) {
                continue;
            }

            idx1 = line.indexOf("</U>");
            if (idx1 === -1) {
                continue;
            }
            if (second) {
                second = false;
            } else {
                //result += "<br>";
            }
            curr_turn = line.substr(0, idx1) * 1;
            result += "<font color=\"#10C9F5\">Turn " + curr_turn + ":</font><br>";
            idx1 += 4;
            turn_txt = line.substr(idx1);
            console.log("wnd_spell.processFinishedBattleText", turn_txt);
            result += processTurnMessage(turn_txt);
        }
        return result;
    }

    function initErrFields() {
        console.log("wnd_spell.initFields", JSON.stringify(mainWindow.gERROR));
        if (!mainWindow.gERROR || (!mainWindow.gERROR.spell && !mainWindow.gERROR.data && (mainWindow.gERROR.type !== 8) && (mainWindow.gERROR.type !== 9))) {
            return;
        }

        var spell_code = mainWindow.gERROR.spell;
        if (mainWindow.gERROR.type >= 8) { // 8, 9
            l_data = mainWindow.gERROR;
        } else {
            l_data = spell_code ? {} : mainWindow.gERROR.data;
        }
        ltDesc.visible = true;
        bbAction.visible = false;
        svMain.anchors.topMargin = 20 * mainWindow.ratioObject;
        if (l_data && (l_data.action || (l_data.type && (l_data.type >= 8)))) {
          icon.visible = false;
          if (l_data.type && (l_data.type === 9)) {
            last_line = "";
            red_desc  = "";
            ltDesc.visible = false;
            svMain.anchors.topMargin = 0;
            ltTitle.text = l_data.t;
            ltError.text = processFinishedBattleText(l_data.d);
            if (last_line != "") {
                ltShortDesc.text = last_line + "<br>" + red_desc;
            }
          } else if (l_data.type && (l_data.type === 8)) {
            ltTitle.text = "Orders Submited";
            ltShortDesc.text = "Your orders are in for this turn";
            ltError.text = l_data.d;
            bbAction.text = "Force Turn"//dict.getStringByCode("MonsterSetTarget");
            if (l_data.fst > 0) {
                bbAction.visible = true;
                l_data.action = "force";
            }
          } else if (l_data.action === 'm') {
            icon.source = "qrc:/res/" + l_data.icon + ".png";
            icon.visible = true;
            icon.text = l_data.hp;
            var details = "Life: Unknown<br>Power: Unknown<br>Owner: " + l_data.owner + "<br>Target: " + l_data.target;
            if (l_data.hp === "R") {
              ltTitle.text = dict.getStringByCode("RH_summon");
              ltShortDesc.text = dict.getStringByCode("WH_summon");
            } else if (l_data.hp === "L") {
                ltTitle.text = dict.getStringByCode("LH_summon");
                ltShortDesc.text = dict.getStringByCode("WH_summon");
            } else {
              ltTitle.text = l_data.name;
              ltShortDesc.text = dict.getStringByCode("Warlock");
              details = "Life: " + l_data.hp + "<br>Power: "+l_data.strength+"<br>Owner: " + l_data.owner + "<br>Target: " + l_data.target;
            }
            ltError.text = details;
            bbAction.text = dict.getStringByCode("MonsterSetTarget");
            bbAction.visible = l_data.under_control;
          } else if (l_data.action === 'hp') {
            icon.source = "qrc:/res/heart_small.png";
            icon.text = l_data.value;
            icon.visible = true;
            ltTitle.text = l_data.warlock_name;
            ltShortDesc.text = dict.getStringByCode("Warlock");
            ltError.text = "Life: " + l_data.value;
          }
          //ltError.font.pixelSize = 28 * mainWindow.ratioFont;
        } else if (spell_code) {
            var spell_icon = BGU.map_spell_to_icon[spell_code];
            if (spell_icon) {
                icon.source = "qrc:/res/"+spell_icon+".png";
                icon.visible = true;
            } else {
                icon.visible = false;
            }

            ltTitle.text = dict.getStringByCode(spell_code);
            ltShortDesc.text = dict.getStringByCode(spell_code + "_short_desc");
            //ltError.font.pixelSize = 21 * mainWindow.ratioFont;
            ltError.text = dict.getStringByCode(spell_code + "_desc");
        }

        mainWindow.gERROR = {};
        console.log("wnd_spell.initFields", ltTitle.text, ltShortDesc.text);
    }

    Component.onCompleted: {
        mainWindow.storeWnd(dMainItem);
        initErrFields();
    }
}
