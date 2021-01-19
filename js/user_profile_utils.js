function linkActivated(url) {
    console.log("linkActivated", url);
    var arr_url = url.split("/");
    console.log("array", arr_url);
    var action = arr_url[1];
    var battle_id = arr_url[2];
    var battle_type = arr_url[3];
    if (action.indexOf("delmess") !== -1) {
        var idx1 = action.indexOf("from=");
        idx1 += 5;
        Qt.core.deleteMsg(action.substr(idx1));
    } else {
        switch(action) {
        case "challenge":
            if (battle_type === "accept") {
                Qt.core.acceptChallenge(battle_id);
            } else {
                Qt.core.rejectChallenge(battle_id);
            }
            break;
        case "battle":
            Qt.core.getBattle(battle_id, battle_type);
            break;
        default:
            Qt.openUrlExternally("https://games.ravenblack.net" + url);
            return;
        }
    }
    Qt.mainWindow.closeChild();
}


function getTopPlayerColor(item, idx) {
    if (idx < 3) {
        return "lightgreen";
    } else {
        return "lightgrey";
    }
}

function getTopPlayerBorderColor(item, idx) {
    if ((item.ladder === 0) && (item.melee === 0)) {
        return "grey";
    } if (item.ladder > item.melee) {
        return "red";
    } else {
        return "purple";
    }
}

function getTopPlayerBorderWidth(item, idx) {
    if (idx < 3) {
        return 3;
    } else {
        return 1;
    }
}


function getTopPlayerDesc(item) {
    var res = "";
    if (item.ladder > 0) {
        if (res.length > 0) {
            res += ', ';
        }
        res += "L:" + item.ladder;
    }
    if (item.melee > 0) {
        if (res.length > 0) {
            res += ', ';
        }
        res += "M: " + item.melee;
    }
    if (item.played > 0) {
        if (res.length > 0) {
            res += ', ';
        }
        res += "P:" + item.played + " (w:" + item.won + ", d:" + item.dead + ")";
    }
    if (res.length > 0) {
        res += ', ';
    }
    res += "E:" + item.elo;

    return item.login + " - " + res;
}

/*function getTopPlayerTooltip(item) {
    var res = "Ladder:" + item.ladder;
    res += ', ';
    res += "Melee: " + item.melee;
    res += ', ';
    res += "Ppayed:" + item.played + " (win:" + item.won + ", death:" + item.dead + ")";
    res += ', ';
    res += "Elo:" + item.elo;

    return res;
}*/

