var C_NG_PLAYER_CODE = 'player';
var C_NG_BOT_CODE = 'bot';
var G_BATTLE_LIST = [[],[]];
var G_CHALLENGE_LIST = [];
var G_PROFILE = {elo:1500,feedback:true,rate_us:true,finished_game_count:0,sbl:-1,win_vs_bot:0,win_vs_warlock:0,last_activity:0,online:1};
var V_BTN_ACTION = [C_NG_BOT_CODE, C_NG_PLAYER_CODE];
var V_BEST_BATTLE_ID = 0;
var G_ACCOUNT_LIST = [];
var ARR_MAIN_MENU = [{c:"spellbook",t:"SpellbookActionTitle"}, {c:"top",t:"TopList"}, {c:"feedback",t:"Feedback"},
                     //{c:"battle_with_friend",t:"DuelWithFriend"}, /*{c:"switch_account",t:"SwitchAccount"},*/
                     {c:"rateus",t:"RateUs"}, {c:"rules",t:"miRulesTitle"},
                     {c:"logout",t:"miLogoutTitle"}/*, {c:"refresh",t:"Refresh"}*/];

const SPELL_CHARM_MONSTER = 23;

function getMainMenuList() {
    return ARR_MAIN_MENU;
}

function mainMenuActionEx(code) {
    logEvent("menu_action", {action:code});
    var def_type = 1013;
    switch(code) {
    case "refresh": return core.scanState(false);
    case "spellbook": return showWndSpellbook();
    case "rules": return Qt.openUrlExternally("https://games.ravenblack.net/rules");
    case "feedback": return showFeedbackWnd();
    case "rateus": return showRateUsWnd();
    case "top": return getHallOfFame();
    case "player_score": return showUserScoreWnd();
    case "battle_with_friend": return showShareWnd();
    case "logout": return confirmLogout();
    case "spellbook_levelup":
        def_type = 22;
        break;
    }
    showErrorWnd({id:-1,type:def_type});
}

function mainMenuAction(code) {
    console.log("mainMenuAction", code);
    mainMenuActionEx(code);
}

function newUserRegistered() {
    var l_login = core.login;
    closeChild();
    lPlayerTitle.text = "Hi, " + l_login;
    //showTipMessage(warlockDictionary.getStringByCode("JustRegistered").replace("#login#", l_login), true);
    showAfterRegWnd();
    logEvent("registration_finished", {login: l_login});
    userProfileChanged();
}

function fixActivBattleDesc(arr) {
    var curr_date_sec = Math.floor((new Date()).getTime()/1000);
    for (var i = 0, Ln = arr.length; i < Ln; ++i) {
        var item = arr[i];
        if (!item || !item.el || (item.el.length < 2)) {
            arr[i].online = false;
            continue;
        }
        var str = core.getWarlockStats(item.el);
        console.log("fixActivBattleDesc", item.el, str, curr_date_sec);
        var wd = JSON.parse(str);
        arr[i].online = curr_date_sec - wd.last_activity <= 5 * 60;

    }
}

function newBattleList() {
    var bl = core.battleList;
    console.log("newBattleList", bl);
    G_BATTLE_LIST = JSON.parse(bl);
    fixActivBattleDesc(G_BATTLE_LIST[0]);
    lvActiveBattle.model = G_BATTLE_LIST[0];
    lvFinishedBattle.model = G_BATTLE_LIST[1];
    ltFinishedBattle.visible = G_BATTLE_LIST[1].length > 0;
    logEvent("ListGames_View");
}

function loadBattleList(filter) {
    return G_BATTLE_LIST[filter === 1 ? 0 : 1];
}

function loadChallengeList() {
    var str = core.challengeList, i, Ln;
    console.log("loadChallengeList", core.isAI, str);
    var arr = JSON.parse(str);
    var first_call = !(G_CHALLENGE_LIST.length > 0);
    G_CHALLENGE_LIST = [];
    for(i = 0, Ln = arr.length; i < Ln; ++i) {
        if (!arr[i].active || (arr[i].size !== 2)) {
            continue;
        }
        G_CHALLENGE_LIST.push(arr[i]);
    }
    if (first_call) {
        console.log("loadChallengeList", JSON.stringify(G_CHALLENGE_LIST));
    }

    if (!core.isAI) {
        return;
    }
    var cli, bot_cnt = 0;
    for (i = 0, Ln = G_CHALLENGE_LIST.length; i < Ln; ++i) {
        cli = G_CHALLENGE_LIST[i];
        if (cli.for_bot) {
            console.log("loadChallengeList", "accept", JSON.stringify(cli));
            core.aiAcceptChallenge(cli.id);
            // battle item example
            // {"is_new_btn":0,"logins":"Galbarad(2)","fast":1,"level":"Very friendly","parafc":1,"maladroit":1,"desc":"TRANING BOT ONLY",
            // "battle_id":86536,"for_bot":true,"level_color":"#c6e5bc","friendly":2,"with_bot":false,"need":1,"active":true,"total_count":2}
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
    //bbNewGame.text = V_BTN1_TITLE;
    //bbNewBotGame.text = V_BTN2_TITLE;
}

function userProfileChanged() {
    var old_elo = G_PROFILE.elo;
    var str = core.playerJson();
    console.log("userProfileChanged", str, old_elo);
    G_PROFILE = JSON.parse(str);
    if (old_elo !== G_PROFILE.elo) {
        prepareNewGameBtn(G_PROFILE.elo);
    }
    tbScoreText.text = G_PROFILE.elo;
    var old_sbl = playerSpellbookLevel;
    if (playerSpellbookLevel !== G_PROFILE.sbl) {
        playerSpellbookLevel = G_PROFILE.sbl;
        if (old_sbl < G_PROFILE.sbl) {
            //showSBLWnd();
            afterCloseAction = "spellbook_levelup";
        }
    }
}

function getSpellBookLevelUpHint(exept_max) {
    if (exept_max && (G_PROFILE.sbl >= 5)) {
        return "";
    }

    if ((G_PROFILE.sbl === 3) && (G_PROFILE.win_vs_bot === 2)) {
        return warlockDictionary.getStringByCode("SpellbookHint3.1");
    } else {
        return warlockDictionary.getStringByCode("SpellbookHint" + G_PROFILE.sbl);
    }
}

function getJoinDialogText(b) {
    var txt = warlockDictionary.getStringByCode(b.need === 1 ? "DialogStartBattle" : "DialogJoinBattle");
    if (b.need === 1) {
        txt = txt.replace("#NEED", b.need - 1);
    }
    txt = txt.replace("#ID", '#' + b.id);
    txt = txt.replace("#FL", b.level);
    txt = txt.replace("#LOGINS", b.participants);
    txt += "\n" + b.desc;
    return txt;
}

function startGameWithPlayerEx(skip_search) {
    console.log("startGameWithPlayer", JSON.stringify(G_CHALLENGE_LIST));
    var best_idx = -1, battle, bb;
    if (!skip_search) {
        for (var i = 0, Ln = G_CHALLENGE_LIST.length; i < Ln; ++i) {
            battle = G_CHALLENGE_LIST[i];
            if (!battle.active || (battle.size > 2)) {
                continue;
            }

            if ((battle.level === 1) && (battle.need === 1)) {
                var creator = JSON.parse(core.getWarlockStats(battle.participant, 1));
                if (creator.found && (G_PROFILE.elo > creator.elo + 200)) {
                    continue;
                }
                logEvent("accept_callenge", {source:"btn_new",Type:"Training",With:"Random warlock",against:creator});
                core.acceptChallenge(battle.id, false);
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
                if (bb.level < battle.level) {
                    continue;
                }
                if (bb.fast && !battle.fast) {
                    continue;
                }
                best_idx = i;
            }
        }
    }
    //if (best_idx === -1) {
        var desc = "Created with android app Warlock's Duel.";
        if (G_PROFILE.elo >= 1700) {
            desc += " Elo " + (G_PROFILE.elo - 200) + " or more please.";
        }
        logEvent("create_callenge", {source:"btn_new",Type:"Training",With:"Random warlock"});
        core.createNewChallenge(1, 0, 1, 1, 2, 1, desc);
    /*} else {
        V_BEST_BATTLE_ID = G_CHALLENGE_LIST[best_idx].battle_id;
        mdNoGesture.text = getJoinDialogText(G_CHALLENGE_LIST[best_idx]);
        mdNoGesture.dialogType = 2;
        mdNoGesture.visible = true;
    }*/
}

function startGameWithPlayer(WarlockName, FriendlyLevel, OnlineGame) {
    if (!WarlockName) {
        showErrorWnd({id:-1,type:19});
    } else {
        core.createNewChallenge(1, 1, 1, 1, FriendlyLevel, 1, "Welcome to fight", WarlockName);
    }
}

function joinBattleDialogResult(accept) {
    if (accept && V_BEST_BATTLE_ID) {
        core.acceptChallenge(V_BEST_BATTLE_ID);
    } else {
        core.createNewChallenge(1, 0, 1, 1, 2, 1, "Welcome to fight");
    }
}

function startGameWithBotEx(silence) {
    console.log("startGameWithBot", JSON.stringify(G_CHALLENGE_LIST));
    var bot_battle_id = core.getBotBattle();
    if (bot_battle_id) {
        if (!silence) {
            showErrorWnd({id:-1,type:21});
        }
    } else {
        for (var i = 0, Ln = G_CHALLENGE_LIST.length; i < Ln; ++i) {
            if (G_CHALLENGE_LIST[i].active && G_CHALLENGE_LIST[i].with_bot) {
                //logEvent("accept_callenge", {source:"btn_new",level:"very friendly",with:"bot",against:""});
                logEvent("accept_callenge", {source:"btn_new",Type:"Training",With:"Bot"});
                core.acceptChallenge(G_CHALLENGE_LIST[i].id, false);
                return;
            }
        }
        logEvent("create_callenge", {source:"btn_new",Type:"Training",With:"Bot"});
        core.createNewChallenge(1, 0, 1, 1, 2, 2, "TRANING BOT ONLY");
    }
}

function startGameWithBot() {
    showErrorWnd({id:-1,type:20});
}

function startGameByCode(action_code, WarlockName, FriendlyLevel, OnlineGame) {
    switch(action_code) {
         case 'player':
             startGameWithPlayer(WarlockName, FriendlyLevel, OnlineGame);
             break;
         case 'bot':
             if ((G_PROFILE.finished_game_count < 3) || (G_PROFILE.elo <= 1500)) {
                startGameWithBotEx();
             } else {
                startGameWithBot();
             }
             break;
    }
    core.setTimerInterval(10, 10000);
    console.log("!!!FEEDBACK CHECK", G_PROFILE.feedback, G_PROFILE.finished_game_count);
    if (!G_PROFILE.feedback && (G_PROFILE.finished_game_count >= 2)) {
        G_PROFILE.feedback = true;
        core.setParamValue("feedback", "true");
        //
        afterCloseAction = "feedback";
        //showFeedbackWnd();
        console.log("!!!FEEDBACK CHECK", "after", G_PROFILE.feedback);
    }
}

function startGame(actionIdx) {
    startGameByCode(V_BTN_ACTION[actionIdx]);
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
    //cbLoginAs.model = G_ACCOUNT_LIST;
    //cbLoginAs.visible = G_ACCOUNT_LIST.length > 2;

    console.log("prepareLoginMenu", JSON.stringify(G_ACCOUNT_LIST));
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
