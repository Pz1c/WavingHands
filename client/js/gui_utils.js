var G_BATTLE_LIST = [[],[]];

function newUserRegistered() {
    var l_login = Qt.core.login;
    Qt.gameField.closeChild();
    Qt.gameField.showTipMessage(Qt.dictionary.getStringByCode("JustRegistered").replace("#login#", l_login), true);
    Qt.gameField.logEvent("registration_finished", {login: l_login});
}

function newBattleList() {
    G_BATTLE_LIST = JSON.parse(Qt.core.battleList);
    lvActiveBattle.model = G_BATTLE_LIST[0];
    lvFinishedBattle.model = G_BATTLE_LIST[1];
}

function loadBattleList(filter) {
    return G_BATTLE_LIST[filter === 1 ? 0 : 1];
}

function autoLogin(idx) {
    Qt.core.autoLogin(idx);
}

function showLoginMenu(menu_str) {
    console.log("showLoginMenu", menu_str);
    var arr = JSON.parse(menu_str);
    var idx = 100;

    /*while((mLoginsMenu.count > 0) && (--idx > 0)) {
        mLoginsMenu.removeItem(mLoginsMenu.itemAt(0))
    }*/
    var i, Ln;
    for (i = 0, Ln = 10; i < Ln; ++i) {
        mLoginsMenu.itemAt(i).visible = false;
    }

    for (i = 0, Ln = Math.min(10, arr.lst.length - 1); i < Ln; ++i) {
        var login = arr.lst[i].trim();
        if (login.length === 0) {
            break;
        }
        console.log("try to add", login);
        mLoginsMenu.itemAt(i).visible = true;
        mLoginsMenu.itemAt(i).text = login;
        //var item = ;
        //mLoginsMenu.addItem(MenuItem {id: "test_" + i, text: login, "onTriggered": function () { console.log("onTriggered", login) }});
        //mLoginsMenu.addSeparator();
    }
}

function isLoadingChanged() {
    var is_loading = Qt.core.isLoading;
    console.log("isLoadingChanged", is_loading);
    rLoading.visible = is_loading === 1;
}
