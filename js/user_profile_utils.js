function linkActivated(url) {
    console.log("linkActivated", url)
    var arr_url = url.split("/")
    console.log("array", arr_url)
    var battle_id = arr_url[2]
    var battle_type = arr_url[3]
    Qt.core.getBattle(battle_id, battle_type);
    Qt.mainWindow.closeChild();
}
