function newUserRegistered() {
    var l_login = Qt.core.login;
    Qt.gameField.closeChild();
    Qt.gameField.showTipMessage(Qt.dictionary.getStringByCode("JustRegistered").replace("#login#", l_login), true);
    Qt.gameField.logEvent("registration_finished", {login: l_login});
}

function loadChallengesList(filter) {
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
