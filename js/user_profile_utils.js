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
