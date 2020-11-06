function newUserRegistered() {
    var l_login = Qt.core.login;
    Qt.gameField.closeChild();
    Qt.gameField.showTipMessage(Qt.dictionary.getStringByCode("JustRegistered").replace("#login#", l_login), true);
    Qt.gameField.logEvent("registration_finished", {login: l_login});
}

function loadChallengesList() {
    /*if (!Qt.core) {
        return [];
    }*/

    return [{id:1,d:"test001 asd asd sd",s:1},{id:1,d:"test002 asd asd sd",s:1},{id:1,d:"test003 asd asd sd",s:1},
            {id:1,d:"test001 asd asd sd",s:0},{id:1,d:"test001 asd asd sd",s:0},{id:1,d:"test001 asd asd sd",s:0},
            {id:1,d:"test001 asd asd sd",s:2},{id:1,d:"test001 asd asd sd",s:2},{id:1,d:"test001 asd asd sd",s:2},{id:1,d:"test001 asd asd sd",s:2},{id:1,d:"test001 asd asd sd",s:2},{id:1,d:"test001 asd asd sd",s:2},{id:1,d:"test001 asd asd sd",s:2}];
    /*var list_str = Qt.core.battleList;
    console.log("loadChallengesList", need_return, list_str);
    var arr = (list_str && (list_str.length > 0) && Qt.core.allowedAccept) ? JSON.parse(list_str) : [];
    console.log("loadChallengesList", Qt.core.isAI, list_str, Qt.core.readyInBattles, Qt.core.waitingInBattles);
    return arr;*/
}
