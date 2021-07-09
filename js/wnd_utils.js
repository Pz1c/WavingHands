var wnd_error = "wnd_error.qml";
//var wnd_battle_result = "wnd_battle_result.qml";
var wnd_new_user = "wnd_new_user.qml";
var wnd_register = "wnd_register.qml";
var wnd_login = "wnd_login.qml";
var wnd_profile = "wnd_profile.qml";
var wnd_spellbook = "wnd_spellbook.qml";
var wnd_battle = "wnd_battle.qml";
var wnd_chat = "wnd_chat.qml";
var wnd_gesture = "wnd_gesture.qml";
var wnd_charm = "wnd_charm.qml";
var wnd_orders = "wnd_orders.qml";
var wnd_spellbook = "wnd_spellbook.qml";
var wnd_spell = "wnd_spell.qml";

var arr_forbiddent_to_cache = [wnd_error];

var arr_wnd_instance = [];
var arr_wnd_obj = [];
var arr_wnd_stack = [];
var current_wnd_code;

function getQmlFullName(name) {
    return "qrc:///qml/windows/" + name;
}

function needToCache(wnd_name) {
    return arr_forbiddent_to_cache.indexOf(wnd_name) === -1;
}

function closeChilds(close_current) {
    console.log("WNDU.closeChilds", close_current);
    while(arr_wnd_stack.length > 0) {
        closeChild();

        if (close_current) {
            break;
        }
    }
}

var max_z_index = 0;

function showWnd(wnd_name, close_current, close_all_stack, add_in_stack, only_create) {
    logEvent("open_wnd", {name:wnd_name,max_z:max_z_index});
    console.log("showWnd", wnd_name, close_current, close_all_stack, add_in_stack);
    //add_in_stack = true;

    var wnd_obj;
    current_wnd_code = wnd_name;
    if (!arr_wnd_obj[wnd_name]) {
        wnd_obj = {code: wnd_name, full_name: getQmlFullName(wnd_name)};
        wnd_obj.wnd = Qt.createComponent(wnd_obj.full_name);
        arr_wnd_obj[wnd_name] = wnd_obj;
        console.log("add into arr_wnd_obj", wnd_name);
    } else {
        wnd_obj = arr_wnd_obj[wnd_name];
    }
    wnd_obj.only_create = only_create;

    if (close_all_stack || close_current) {
        closeChilds(close_current);
    }

    wnd_obj.add_in_stack = add_in_stack ? true : false;
    /*if (add_in_stack) {
        arr_wnd_stack.push(wnd_obj);
    }*/

    if (needToCache(wnd_name) && arr_wnd_instance[wnd_name]) {
        if (only_create) {
            return;
        }
        arr_wnd_instance[wnd_name].z = ++max_z_index;
        //arr_wnd_instance[wnd_name].code = wnd_name;
        arr_wnd_instance[wnd_name].showWnd();
        if (add_in_stack) {
            console.log("arr_wnd_stack.push1", wnd_name);
            arr_wnd_stack.push({item:arr_wnd_instance[wnd_name],code:wnd_name});
        }
        //Qt.loader.forceActiveFocus();
    } else if (wnd_obj.wnd.status === Component.Ready) {
        finishedShowWnd(wnd_obj);
    } else {
        console.log("Current status", wnd_obj.wnd.status, wnd_obj.wnd.errorString());
        wnd_obj.wnd.statusChanged.connect(finishedShowWnd);
    }
}

function finishedShowWnd(wnd_obj) {
    console.log("finishedShowWnd", current_wnd_code, wnd_obj.code, wnd_obj, arr_wnd_obj[current_wnd_code]);

    if (!wnd_obj) {
        wnd_obj = arr_wnd_obj[current_wnd_code];
    }

    if (!wnd_obj) {
        return;
    }

    if (wnd_obj.wnd.status === Component.Ready) {
        console.log("before create object");
        wnd_obj.item = wnd_obj.wnd.createObject(mainWindow.mainContainer, {x: 0, y: 0, visible:!wnd_obj.only_create,z:++max_z_index,code:wnd_obj.code});
        //wnd_obj.item.code = wnd_obj.code;
        console.log("create item", /*wnd_obj.code, wnd_obj.item.code*/);
        if (wnd_obj.add_in_stack) {
            console.log("arr_wnd_stack.push2", wnd_obj.code);
            arr_wnd_stack.push({item:wnd_obj.item,code:wnd_obj.code});
        }
        //Qt.loader.forceActiveFocus();
    } else if (wnd_obj.wnd.status === Component.Error) {
        console.log("Error loading component:", wnd_obj.wnd.errorString());
    } else {
        console.log("Unknown status", wnd_obj.wnd.status);
    }
    mainWindow.keyListener.focus = true;
}
//*/

function processEscape() {
    console.log("processEscape", arr_wnd_stack.length, is_game_in_progress);
    if (arr_wnd_stack.length > 0) {
        var wnd_last_name = arr_wnd_stack[arr_wnd_stack.length - 1].code;
        console.log("processEscape", wnd_last_name);
        closeChild();
        /*if (wnd_last_name === wnd_main_menu) {
            Qt.quit();
        } else  if (arr_wnd_stack.length === 0) {
            if (!is_game_in_progress) {
                //showMainMenu();
                Qt.quit();
            }
            return;
        } */
    } else {
        mdNoGesture.dialogType = 0;
        mdNoGesture.text = mainWindow.warlockDictionary.getStringByCode("ConfirmExit");
        mdNoGesture.visible = true;
    }
}

function closeChild() {
    console.log("closeChild", arr_wnd_stack.length/*, Qt.loader.focus*/);
    for(var i = 0, Ln = arr_wnd_stack.length; i < Ln; ++i) {
        console.log("closeChild", i, arr_wnd_stack[i].code, arr_wnd_stack[i], arr_wnd_stack[i].item ? arr_wnd_stack[i].item.z : "undefine");
    }

    if (arr_wnd_stack.length > 0) {
        var tmp_wnd = arr_wnd_stack.pop();
        console.log("closeChild", tmp_wnd.code, tmp_wnd);
        if (tmp_wnd.item) {
            if (needToCache(tmp_wnd.item.code)) {
                console.log("closeChild", "hide wnd");
                tmp_wnd.item.hideWnd();
            } else {
                tmp_wnd.item.hideWnd();
                tmp_wnd.item.destroy();
                console.log("closeChild", "destroy");
                //tmp_wnd.item = 0;
            }
        }
    }
    mainWindow.keyListener.forceActiveFocus();
}

function storeWnd(wnd) {
    if (!arr_wnd_instance[current_wnd_code]) {
        console.log("storeWnd", "add", current_wnd_code);
        arr_wnd_instance[current_wnd_code] = wnd;
    }
}

function refreshWindow(wnd_name, force) {
    if (arr_wnd_instance[wnd_name]) {
        console.log("refreshWindow", "find wnd, try refresh", wnd_name, arr_wnd_instance[wnd_name].visible);
        if (arr_wnd_instance[wnd_name].visible || force) {
            arr_wnd_instance[wnd_name].initFields();
            return true;
        }
    } else {
        console.log("refreshWindow", "wnd not found", wnd_name);
        return false;
    }    
}

function hideWindow(wnd_name) {
    if (arr_wnd_instance[wnd_name]) {
        console.log("hideWindow", wnd_name, "found");
        if (arr_wnd_instance[wnd_name].visible) {
            arr_wnd_instance[wnd_name].hideWnd();
        }
    } else {
        console.log("hideWindow", wnd_name, "not found");
    }
}

function prepareGameWindows() {

}

function showBattleResult() {
    showWnd(wnd_battle_result, 0, 0, 1);
}

function showProfileWindow() {
    showWnd(wnd_profile, 0, 0, 1);
}

function showSpellbookWindow() {
    showWnd(wnd_spellbook, 0, 0, 1);
}

function showErrorWnd(error, chat) {
    mainWindow.gERROR = error;
    if (!mainWindow.gERROR.type) {
        mainWindow.gERROR.type = 0;
    }
    if (!mainWindow.gERROR.id) {
        mainWindow.gERROR.id = -1;
    }
    if (error.close_current !== 1) {
        error.close_current = 0;
    }

    var wnd_name;
    switch(error.type) {
    case 3:
        wnd_name = wnd_chat;
        break;
    case 4:
        wnd_name = wnd_spell;
        break;
    case 7:
    case 8:
    case 9:
        wnd_name = wnd_spell;
        break;
    default:
        wnd_name = wnd_error;
        break;
    }

    showWnd(wnd_name, error.close_current, 0, 1);
}

function showNewUserMenu() {
    showWnd(wnd_new_user, 1, 0, 1);
}

function showRegisterFlow() {
    showWnd(wnd_register, 1, 0, 1);
}

function showLogin() {
    showWnd(wnd_login, 1, 0, 1);
}

function showBattle() {
    showWnd(wnd_battle, 0, 1, 1);
}

function showGesture() {
    showWnd(wnd_gesture, 0, 0, 1);
}

function showCharm() {
    showWnd(wnd_charm, 0, 0, 1);
}

function showOrders() {
    showWnd(wnd_orders, 0, 0, 1);
}

function showSpellbook() {
    showWnd(wnd_spellbook, 0, 0, 1);
}
