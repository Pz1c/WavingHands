function linkActivated(url) {
    console.log("linkActivated", url);
    var arr_url = url.split("/");
    console.log("array", arr_url);
    var action = arr_url[1] || "";
    var battle_id = arr_url[2];
    var battle_type = arr_url[3];
    if (action.indexOf("delmess") !== -1) {
        var idx1 = action.indexOf("from=");
        idx1 += 5;
        mainWindow.gameCore.deleteMsg(action.substr(idx1));
    } else {
        switch(action) {
        case "challenge":
            if (battle_type === "accept") {
                mainWindow.gameCore.acceptChallenge(battle_id);
            } else {
                mainWindow.gameCore.rejectChallenge(battle_id);
            }
            break;
        case "battle":
            mainWindow.gameCore.getBattle(battle_id, battle_type);
            break;
        default:
            Qt.openUrlExternally("https://games.ravenblack.net" + url);
            return;
        }
    }
    mainWindow.closeChild();
}

function getLastActivityText(last_activity_in_sec) {
    if (!(last_activity_in_sec > 0)) {
        return "";
    }
    var date = new Date();
    var diff = Math.floor(date.getTime()/1000) - last_activity_in_sec;
    console.log("UPU.getLastActivityText", Math.floor(date.getTime()/1000), last_activity_in_sec, date.getTime(), diff);

    if (Math.floor(diff / (24 * 60 * 60)) > 0) {
        return "Last active ~" + Math.floor(diff / (24 * 60 * 60)) + " days ago";
    } else if (Math.floor(diff / (60 * 60)) > 0) {
        return "Last active ~" + Math.floor(diff / (60 * 60)) + " hours ago";
    } else if (Math.floor(diff / (60)) > 0) {
        return "Last active ~" + Math.floor(diff / (60)) + " minutes ago";
    } else {
        return "Active right now";
    }
}
