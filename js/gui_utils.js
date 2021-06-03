var C_NG_PLAYER_CODE = 'player';
var C_NG_BOT_CODE = 'bot';
var G_BATTLE_LIST = [[],[]];
var G_CHALLENGE_LIST = [];
var G_PROFILE = {elo:1500};
var V_BTN_ACTION = [C_NG_BOT_CODE, C_NG_PLAYER_CODE];
var V_BEST_BATTLE_ID = 0;
var G_ACCOUNT_LIST = [];

function newUserRegistered() {
    var l_login = core.login;
    closeChild();
    lPlayerTitle.text = "Hi, " + l_login;
    showTipMessage(warlockDictionary.getStringByCode("JustRegistered").replace("#login#", l_login), true);
    logEvent("registration_finished", {login: l_login});
}

function newBattleList() {
    var bl = core.battleList;
    console.log("newBattleList", bl);
    G_BATTLE_LIST = JSON.parse(bl);
    lvActiveBattle.model = G_BATTLE_LIST[0];
    lvFinishedBattle.model = G_BATTLE_LIST[1];
}

function loadBattleList(filter) {
    return G_BATTLE_LIST[filter === 1 ? 0 : 1];
}

function loadChallengeList() {
    var str = core.challengeList;
    console.log("loadChallengeList", core.isAI, str);
    G_CHALLENGE_LIST = JSON.parse(str);
    if (!core.isAI) {
        return;
    }
    var cli, bot_cnt = 0;
    for(var i = 0, Ln = G_CHALLENGE_LIST.length; i < Ln; ++i) {
        cli = G_CHALLENGE_LIST[i];
        if (cli.for_bot) {
            console.log("loadChallengeList", "accept", JSON.stringify(cli));
            core.aiAcceptChallenge(cli.id);
            return;
        } else if (cli.with_bot) {
            ++bot_cnt;
            console.log("loadChallengeList", "++bot_cnt", JSON.stringify(cli));
        }
    }
    if (bot_cnt < 1) {
        core.aiCreateNewChallenge();
    } else {
        core.aiLogin();
    }
}

function autoLogin(idx) {
    core.autoLogin(idx);
}

function prepareNewGameBtn(ELO) {
    var V_BTN1_TITLE, V_BTN2_TITLE;
    if (ELO <= 1500) {
        V_BTN1_TITLE = warlockDictionary.getStringByCode("TrainingGame");
        V_BTN2_TITLE = warlockDictionary.getStringByCode("NewGameWithPlayer");
        V_BTN_ACTION = [C_NG_BOT_CODE, C_NG_PLAYER_CODE];
    } else {
        V_BTN1_TITLE = warlockDictionary.getStringByCode("NewGame");
        V_BTN2_TITLE = warlockDictionary.getStringByCode("NewGameWithBot");
        V_BTN_ACTION = [C_NG_PLAYER_CODE, C_NG_BOT_CODE];
    }
    bbNewGame.text = V_BTN1_TITLE;
    bbNewBotGame.text = V_BTN2_TITLE;
}

function userProfileChanged() {
    var old_elo = G_PROFILE.elo;
    var str = core.playerJson();
    console.log("userProfileChanged", str, old_elo);
    G_PROFILE = JSON.parse(str);
    if (old_elo !== G_PROFILE.elo) {
        prepareNewGameBtn(G_PROFILE.elo);
    }
}

function getJoinDialogText(b) {
    var txt = warlockDictionary.getStringByCode(b.need === 1 ? "DialogStartBattle" : "DialogJoinBattle");
    if (b.need === 1) {
        txt = txt.replace("#NEED", b.need - 1);
    }
    txt = txt.replace("#ID", '#' + b.battle_id);
    txt = txt.replace("#FL", b.level);
    txt = txt.replace("#LOGINS", b.logins);
    txt += "\n" + b.desc;
    return txt;
}

function startGameWithPlayer() {
    console.log("startGameWithPlayer", JSON.stringify(G_CHALLENGE_LIST));
    var best_idx = -1, battle, bb;
    for (var i = 0, Ln = G_CHALLENGE_LIST.length; i < Ln; ++i) {
        battle = G_CHALLENGE_LIST[i];
        if (!battle.active || (battle.total_count > 2)) {
            continue;
        }

        if ((battle.friendly === 1) && (battle.need === 1)) {
            core.acceptChallenge(battle.battle_id, false);
            return;
        } else if (i !== best_idx) {
            if (best_idx === -1) {
                best_idx = 0;
                continue;
            }
            bb = G_CHALLENGE_LIST[best_idx];
            if (bb.need > battle.need) {
                continue;
            }
            if (bb.friendly < b.friendly) {
                continue;
            }
            if (bb.fast && !b.fast) {
                continue;
            }
            best_idx = i;
        }
    }
    if (best_idx === -1) {
        core.createNewChallenge(1, 0, 1, 1, 2, 1, "Welcome to fight");
    } else {
        V_BEST_BATTLE_ID = G_CHALLENGE_LIST[best_idx].battle_id;
        mdNoGesture.text = getJoinDialogText(G_CHALLENGE_LIST[best_idx]);
        mdNoGesture.dialogType = 2;
        mdNoGesture.visible = true;
    }
}

function joinBattleDialogResult(accept) {
    if (accept && V_BEST_BATTLE_ID) {
        core.acceptChallenge(V_BEST_BATTLE_ID);
    } else {
        core.createNewChallenge(1, 0, 1, 1, 2, 1, "Welcome to fight");
    }
}

function startGameWithBot() {
    console.log("startGameWithBot", JSON.stringify(G_CHALLENGE_LIST));
    for (var i = 0, Ln = G_CHALLENGE_LIST.length; i < Ln; ++i) {
        if (G_CHALLENGE_LIST[i].active && G_CHALLENGE_LIST[i].with_bot) {
            core.acceptChallenge(G_CHALLENGE_LIST[i].battle_id, false);
            return;
        }
    }
    core.createNewChallenge(1, 0, 1, 1, 2, 2, "TRANING BOT ONLY");
}

function startGame(actionIdx) {
    switch(V_BTN_ACTION[actionIdx]) {
         case 'player': return startGameWithPlayer();
         case 'bot': return startGameWithBot();
    }
}

function prepareLoginMenu(menu_str) {
    console.log("prepareLoginMenu", menu_str);
    var arr = JSON.parse(menu_str), i, Ln;
    G_ACCOUNT_LIST = [warlockDictionary.getStringByCode("LoginAs")];
    for (i = 0, Ln = arr.lst.length - 1; i < Ln; ++i) {
        var login = arr.lst[i].trim();
        if (login.length === 0) {
            break;
        }
        console.log("try to add", login);
        G_ACCOUNT_LIST.push(login);
    }
    cbLoginAs.model = G_ACCOUNT_LIST;
    cbLoginAs.visible = G_ACCOUNT_LIST.length > 2;

    console.log("prepareLoginMenu", cbLoginAs.visible, JSON.stringify(G_ACCOUNT_LIST), JSON.stringify(cbLoginAs.model));
}//*/

function isLoadingChanged() {
    var is_loading = core.isLoading;
    console.log("isLoadingChanged", is_loading);
    rLoading.visible = is_loading === 1;
    if (!rLoading.visible && !tbTop.visible) {
        tbTop.visible = true;
        rBody.visible = true;
    }
}

function linkActivated(link) {
    console.log("linkActivated", link)
    var a = link.split("/");
    switch(a[1]) {
        case "force_surrender": return mainWindow.gameCore.forceSurrender(a[2], a[3]);
        case "show_spell_desc": return mainWindow.showSpellDetails(a[2]);
    }
    return false;
}
