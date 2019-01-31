// windows
var wndUP, wndLoading, wndTmp, wndNew, wndErr;
// monster command
var cMonsterTarget,mtTaskList=[],mtMonsterObj,cpTaskList=[],pTaskList=[],cPersonCharmed,cParalyze,cpPersonObj,pParalyzeObj;
var cWarlockObject;
// target list
var arrTarget,arrTargetID,arrVisibleTarget,strVisibleTarget,arrRPG,arrLPG;
// challendge list
var arrChallengeList;
// interface constant
var dict, cChatMessage, cMosterFrom, default_spell_list;

var isAI = false;
var battle = {};

var WARLOCK_HAND_NONE = 0;
var WARLOCK_HAND_LEFT = 1;
var WARLOCK_HAND_RIGHT= 2;

Qt.include("ai_utils.js");

function sendOrder() {
    action_send_order = true;
    if (arrLPG[cbLHG.currentText] === "-" && arrRPG[cbRHG.currentText] === "-") {
        mdNoGesture.text = dict.getStringByCode("NoGestureForTurn")
        mdNoGesture.visible = true
    } else if (arrLPG[cbLHG.currentText] === "P" && arrRPG[cbRHG.currentText] === "P") {
        mdNoGesture.text = dict.getStringByCode("SurrenderForTurn")
        mdNoGesture.visible = true
    } else {
        sendOrderEx()
    }
}

function sendOrderEx() {
    // chat message
    var post_request = "say$";
    if ((teChatMsg.text !== cChatMessage) && (teChatMsg.text != "")) {
        post_request += teChatMsg.text.replace("$", "sign:dollar").replace("#", "sign:pos_number")
    }
    post_request += "#"
    // gesture
    var g = arrLPG[cbLHG.currentText]
    post_request += "LH$"+g+"#";
    g = arrRPG[cbRHG.currentText]
    console.log("cbLHT.currentText", cbLHT.currentText)
    console.log("cbRHT.currentText", cbRHT.currentText)
    post_request += "RH$"+g+"#";
    post_request += "LHS$"+(cbLHS.currentText === " " ? "" : cbLHS.currentText.replace(" ", "+"))+"#";
    post_request += "RHS$"+(cbRHS.currentText === " " ? "" : cbRHS.currentText.replace(" ", "+"))+"#";
    post_request += "LHT$"+arrTarget[cbLHT.currentText].replace(" ", "+")+"#";
    post_request += "RHT$"+arrTarget[cbRHT.currentText].replace(" ", "+")+"#";

    var i, Ln;
    for(i = 0, Ln = mtMonsterObj.length; i < Ln; ++i) {
        console.log("mt_label", mtMonsterObj[i].mt_id, mtMonsterObj[i].mt_value);
        if (!mtMonsterObj[i].mt_value || mtMonsterObj[i].mt_value === ' ') {
            continue;
        }

        post_request += mtMonsterObj[i].mt_id + "$" + arrTarget[mtMonsterObj[i].mt_value].replace(" ", "+") + "#"
    }

    for(i = 0, Ln = cpPersonObj.length; i < Ln; ++i) {
        console.log("pc_gesture_value", cpPersonObj[i].pc_gesture_value, cpPersonObj[i].pc_gesture_value.replace(">", "&gt;"))
        console.log("cpPersonObj", cpPersonObj[i])
        var pc_gv = cpPersonObj[i].pc_gesture_value;
        if (!pc_gv || pc_gv === '') {
            pc_gv = "-"
        }
        if (pc_gv === ">") {
            pc_gv = "&gt;"
        }

        post_request += "DIRECTHAND" + cpPersonObj[i].pc_target_id + "$" + cpPersonObj[i].pc_hand_value + "#"
        post_request += "DIRECTGESTURE" + cpPersonObj[i].pc_target_id + "$" + pc_gv + "#"
    }

    for(i = 0, Ln = pParalyzeObj.length; i < Ln; ++i) {
        post_request += "PARALYZE" + pParalyzeObj[i].p_target_id + "$" + pParalyzeObj[i].p_hand_value + "#"
    }

    if (lvaoDelay.visible) {
        post_request += "DELAY$"
        switch(cbDelay.currentIndex) {
            case 0:
                post_request += "#"
                break;
            case 1:
                post_request += "LH#"
                break;
            default:
                post_request += "RH#"
                break;
        }
    }

    if (lvaoPermanent.visible) {
        post_request += "PERM$"
        switch(cbPermanent.currentIndex) {
            case 0:
                post_request += "#"
                break;
            case 1:
                post_request += "LH#"
                break;
            default:
                post_request += "RH#"
                break;
        }
    }

    if (lvaoFire.visible && cbFire.checked) {
        post_request += "FIRE$1#"
    }

    teChatMsg.text = ""
    console.log("sendOrder", post_request);
    Qt.core.sendOrders(post_request);
    tScanTimer.interval = 5000;
    rMain.timerCounter = 30;
    tScanTimer.restart();
    //tSpellList.text = ''
    showList();
}

function acceptChallenge(link) {
    console.log("acceptChallenge", link)

    Qt.core.acceptChallenge(link.split("/")[2])
}

function loadChallengesList(need_return) {
    if (!Qt.core) {
        return;
    }

    var list_str = Qt.core.challengeList;
    var arr = (!Qt.core.isAI && Qt.core.allowedAccept) ? JSON.parse(list_str) : [];
    mainWindow.battles = arr;
    console.log("loadChallengesList", need_return, Qt.core.isAI, list_str, Qt.core.readyInBattles, Qt.core.waitingInBattles);
    if (need_return) {
        return arr;
    } else {
        tvChallengeList.model = arr;
    }
}

function getSpellList(right) {
    if (!arrLPG) {
        return ""
    }
    var l_c = arrLPG[cbLHG.currentText]
    var r_c = arrRPG[cbRHG.currentText]

    console.log("lct", cbLHG.currentText, "lc", l_c)
    console.log("rct", cbRHG.currentText, "rc", r_c)

    if (!l_c || l_c === ' ') {
        l_c = '-'
    }

    if (!r_c || r_c === ' ') {
        r_c = '-'
    }
    if (l_c === 'X') {
        l_c = r_c
    }
    if (r_c === 'X') {
        r_c = l_c
    }

    l_c = Qt.core.leftHand + l_c
    r_c = Qt.core.rightHand + r_c

    console.log("left gesture", l_c , " right gesture", r_c);
    var g_list = Qt.core.getSpellList(l_c, r_c, 0);
    console.log("g_list", g_list);

    return JSON.parse(g_list);
}

function getPossibleSpell(right) {
    var currLeftSpell = cbLHS.currentText, currRightSpell = cbRHS.currentText;
    var currLeftSpellIdx = 0, currRightSpellIdx = 0;
    var checkLeft = currLeftSpell !== '' && currLeftSpell !== ' ';
    var checkRight = currRightSpell !== '' && currRightSpell !== ' ';
    var arr = getSpellList(right);
    var arrL = [' '];
    var arrR = [' '];
    //var arr = g_list.split("#");
    for (var i = 0, Ln = arr.length; i < Ln; ++i) {
        //if (!arr[i] || arr[i] === '' || arr[i] === ' ') {
        //    continue;
        //}

        //var arr2 = arr[i];
        //if (!arr2[1] || arr2[1] === '' || arr2[1] === ' ') {
        //    continue;
        //}
        switch(arr[i].h) {
        case WARLOCK_HAND_LEFT:
            arrL.push(arr[i].n)
            if (checkLeft && currLeftSpell === arr[i].n) {
                currLeftSpellIdx = arrL.length - 1
            }
            break;
        case WARLOCK_HAND_RIGHT:
            arrR.push(arr[i].n)
            if (checkRight && currRightSpell === arr[i].n) {
                currRightSpellIdx = arrR.length - 1
            }
            break;
        }
    }

    cbLHS.model = arrL
    if (arrL.length > 1) {
        cbLHS.currentIndex = currLeftSpellIdx > 0 ? currLeftSpellIdx : 1
    }

    cbRHS.model = arrR
    if (arrR.length > 1) {
        cbRHS.currentIndex = currRightSpellIdx > 0 ? currRightSpellIdx : 1
    }
}

function loadSpellList() {
    var str = Qt.core.spellListHtml;
    console.log("loadSpellList", str);
    if (str.length === 0) {
        console.log("loadSpellList empty")
        mainWindow.spells = default_spell_list;
    } else {
        console.log("loadSpellList full")
        mainWindow.spells = JSON.parse(str);
        // tSpellList.text = lst
    }
}

function showReadyBattle() {
    showLoading();
    battle = {};
    battle.isFDF = Qt.core.isParaFDF === 1;
    setPosibleGesture(Qt.core.leftGesture, Qt.core.rightGesture);
    getPossibleSpell(0);
    getPossibleSpell(1);
    prepareStepMessage();
    prepareMonsterList();
    prepareWarlockList();
    prepareTargets(Qt.core.targets);
    createMonsterTarget(Qt.core.monsterCommandList);
    prepareCharmList(Qt.core.charmPerson);
    prepareParalyzeList(Qt.core.paralyze);
    prepareDelay(Qt.core.isDelay === 1);
    preparePermanent(Qt.core.isPermanent === 1);
    prepareFire(Qt.core.fire);
    prepareButton(1);
    flatMenuItem();
    hideLoading();
    if (Qt.core.isAI) {
        startSkynet();
    }
}

function flatMenuItem() {
    /*tbList.y = rChallengeList.visible ? 6 : 3
    tbBook.y = rSpellList.visible ? 6 : 3
    tbFight.y = rReadyBattle.visible ? 6 : 3*/
}

function showDuel() {
    rReadyBattle.visible = true
    rChallengeList.visible = false
    rSpellList.visible = false

    flatMenuItem()
}

function showSpellBook() {
    var str = Qt.core.spellListHtml;
    console.log("showSpellBook", str, default_spell_list);
    if (str.length > 0) {
        mainWindow.spells = JSON.parse(str);
    }else {
        mainWindow.spells = default_spell_list;
    }
    tvSpellList.model = mainWindow.spells;
    rReadyBattle.visible = false
    rChallengeList.visible = false
    rSpellList.visible = true

    flatMenuItem()
}

function showList() {
    console.log("showList");
    rReadyBattle.visible = false
    rChallengeList.visible = true
    rSpellList.visible = false

    flatMenuItem()
}

function prepareButton(state) {
    maDoit.enabled = state === 1
    rReadyBattle.visible = state === 1
    rChallengeList.visible = !rReadyBattle.visible
    rSpellList.visible = false
}

function cleanOrders() {
    console.log("cleanOrders");
    cbLHG.model = [" "];
    cbRHG.model = [" "];
    cbLHS.model = [" "];
    cbRHS.model = [" "];
    cbLHT.model = [" "];
    cbRHT.model = [" "];
    tStepMsg.text = "";
    tMonsterList.text = '';
    //tWarlockList.text = '';
    teChatMsg.text = '';
    cleanChildren(svMonsterOrders);
    cleanChildren(svWarlocks);
    cleanChildren(lvaoCharmPerson);
    cleanChildren(lvaoParalyze);
    lvaoDelay.visible = false;
    lvaoFire.visible = false;
    lvaoPermanent.visible = false;
    prepareButton(0);
}

function prepareFire(fire) {
    lvaoFire.visible = fire !== ''
    cbFire.text = fire
    resizeAddOrdersWnd(0, "f")
}

function prepareDelay(is_delay) {
    lvaoDelay.visible = is_delay
    resizeAddOrdersWnd(0, "d")
}

function preparePermanent(is_permanent) {
    lvaoPermanent.visible = is_permanent
    resizeAddOrdersWnd(0, "pr")
}

function prepareCharmList(char_person_list) {
    cleanChildren(lvaoCharmPerson);

    console.log("prepareCharmList: " + char_person_list)
    cpTaskList.push(char_person_list)
    if (!cPersonCharmed) {
        cPersonCharmed = Qt.createComponent("qrc:///qml/person_charmed.qml");
    }
    if (cPersonCharmed.status === Component.Error) {
        console.log("Error loading component: " + cPersonCharmed.errorString());
        return ;
    }

    if (cPersonCharmed.status === Component.Ready) {
        console.log("component ready");
        cpTaskList.pop();
        finishLoadPersonCharmedEx(char_person_list);
    } else {
        console.log("connect to component ready signal");
        component.statusChanged.connect(finishLoadPersonCharmed);
    }
}

function finishLoadPersonCharmed() {
    var char_person_list = cpTaskList.pop();
    finishLoadPersonCharmedEx(char_person_list);
}

function finishLoadPersonCharmedEx(char_person_list) {
    console.log("finishLoadPersonCharmedEx: " + char_person_list);
    if (cPersonCharmed.status === Component.Error || cPersonCharmed.status !== Component.Ready) {
        console.log("Error loading component:", cPersonCharmed.errorString());
        return ;
    }
    cpPersonObj = [];
    var arr = char_person_list.split(";")
    var arrL=[],currLIdx=0;
    var currLeft = 0;
    var curr_y = 3;
    var total_heght = 0;
    for(var i = 0, Ln = arr.length; i < Ln; ++i) {
        if (arr[i] === '') {
            continue;
        }
        console.log("current person_id: ", arr[i]);
        var label = "Force gesture for " + arrTargetID[arr[i]] + " hand"
        var sprite = cPersonCharmed.createObject(lvaoCharmPerson, {pc_label: label, pc_target_id: arr[i], pc_hand_value: "LH", pc_gesture_value: "-", x: 3, y: curr_y});
        if (sprite === null) {
            console.log("Error creating object");
            continue;
        }
        cpPersonObj[cpPersonObj.length] = sprite;
        console.log("looks like created: " + sprite.pc_label + " x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
        console.log("lvaoCharmPerson.width: " + lvaoCharmPerson.width);
        if (currLeft != 0) {
            console.log("currentleft.w: " + currLeft.width)
            if (currLeft.width + sprite.width + 6 < lvaoCharmPerson.width) {
                sprite.y = currLeft.y
                sprite.x = currLeft.x + currLeft.width + 3
                if (arrL[currLIdx + 1]) {
                    console.log("exists next arrL")
                    ++currLIdx
                    currLeft = arrL[currLIdx]
                } else {
                    console.log("currLeft = 0")
                    currLeft = 0
                }
            } else {
                console.log("push into arrL")
                arrL[arrL.length] = sprite
                curr_y += sprite.height + 3
            }
        } else {
            currLeft = sprite
            arrL[currLIdx] = sprite
            curr_y += sprite.height + 3
        }
        if (total_heght < sprite.y + sprite.height + 3) {
            total_heght = sprite.y + sprite.height + 3
        }
    }
    resizeAddOrdersWnd(total_heght, "cp");
}

function prepareParalyzeList(char_person_list) {
    cleanChildren(lvaoParalyze);

    console.log("prepareParalyzeList: " + char_person_list)
    pTaskList.push(char_person_list)
    if (!cParalyze) {
        cParalyze = Qt.createComponent("qrc:///qml/paralyze.qml");
    }
    if (cParalyze.status === Component.Error) {
        console.log("Error loading component: " + cParalyze.errorString());
        return ;
    }

    if (cParalyze.status === Component.Ready) {
        console.log("component ready");
        pTaskList.pop();
        finishLoadParalyzeEx(char_person_list);
    } else {
        console.log("connect to component ready signal");
        component.statusChanged.connect(finishLoadParalyze);
    }
}

function finishLoadParalyze() {
    var char_person_list = pTaskList.pop();
    finishLoadParalyzeEx(char_person_list);
}

function finishLoadParalyzeEx(char_person_list) {
    console.log("finishLoadParalyzeEx: " + char_person_list);
    if (cParalyze.status === Component.Error || cParalyze.status !== Component.Ready) {
        console.log("Error loading component:", cParalyze.errorString());
        return ;
    }
    pParalyzeObj = [];
    var arr = char_person_list.split(";")
    var arrL=[],currLIdx=0;
    var currLeft = 0;
    var curr_y = 3;
    var total_heght = 0;
    for(var i = 0, Ln = arr.length; i < Ln; ++i) {
        if (arr[i] === '') {
            continue;
        }
        console.log("current person_id: ", arr[i]);
        var label = "Paralyze " + arrTargetID[arr[i]] + " hand"
        var sprite = cParalyze.createObject(lvaoParalyze, {p_label: label, p_target_id: arr[i], x: 3, y: curr_y});
        if (sprite === null) {
            console.log("Error creating object");
            continue;
        }
        pParalyzeObj[pParalyzeObj.length] = sprite;
        console.log("looks like created: " + sprite.p_label + " x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
        console.log("lvaoParalyze.width: " + lvaoParalyze.width);
        if (currLeft != 0) {
            console.log("currentleft.w: " + currLeft.width)
            if (currLeft.width + sprite.width + 6 < lvaoParalyze.width) {
                sprite.y = currLeft.y
                sprite.x = currLeft.x + currLeft.width + 3
                if (arrL[currLIdx + 1]) {
                    console.log("exists next arrL")
                    ++currLIdx
                    currLeft = arrL[currLIdx]
                } else {
                    console.log("currLeft = 0")
                    currLeft = 0
                }
            } else {
                console.log("push into arrL")
                arrL[arrL.length] = sprite
                curr_y += sprite.height + 3
            }
        } else {
            currLeft = sprite
            arrL[currLIdx] = sprite
            curr_y += sprite.height + 3
        }
        if (total_heght < sprite.y + sprite.height + 3) {
            total_heght = sprite.y + sprite.height + 3
        }
    }
    resizeAddOrdersWnd(total_heght, "p");
}

function setPosibleGesture(left, right) {
    console.log("setPosibleGesture", left, " setPosibleGesture: ", right)
    var arr_l = left.split("#");
    var arr_r = right.split("#");
    var arr_l_res = []
    var arr_r_res = []
    arrLPG = []
    arrRPG = []
    var t, i, Ln;
    for (i = 0, Ln = arr_l.length; i < Ln; ++i) {
        t = arr_l[i].split(";")
        if (t[0] === '' || t[0] === ' ') {
            continue;
        }
        arr_l_res.push(t[1])
        arrLPG[t[1]] = t[0]
    }

    for (i = 0, Ln = arr_r.length; i < Ln; ++i) {
        t = arr_r[i].split(";")
        if (t[0] === '' || t[0] === ' ') {
            continue;
        }
        arr_r_res.push(t[0])
        arrRPG[t[1]] = t[0]
    }

    cbLHG.model = arr_l_res;
    cbRHG.model = arr_r_res;
}

function changeGesture(Gesture, Left) {
    var G = Gesture.toUpperCase();
    var arr1, arr2;
    if (Left) {
        arr1 = cbLHG.model;
        arr2 = cbRHG.model;
    } else {
        arr2 = cbLHG.model;
        arr1 = cbRHG.model;
    }
    var idx1 = -1, idx2 = -1;
    for (var i = 0, Ln = arr1.length; i < Ln; ++i) {
        if (arr1[i] === G) {
            idx1 = i;
            break;
        }
    }
    for (i = 0, Ln = arr2.length; i < Ln; ++i) {
        if (arr2[i] === G) {
            idx2 = i;
            break;
        }
    }

    if (idx1 != -1) {
        if (Left) {
            cbLHG.currentIndex = idx1;
        } else {
            cbRHG.currentIndex = idx1;
        }
    }

    if ((Gesture !== G) && (idx2 != -1)) {
        if (Left) {
            cbRHG.currentIndex = idx2;
        } else {
            cbLHG.currentIndex = idx2;
        }
    }
}

function resizeMonsterOrderWnd(height) {
    //console.log("resizeMonsterOrderWnd", height, " rMonsterOrders.height: ", rMonsterOrders.height)
    svMonsterOrders.height = height
    //console.log("resizeMonsterOrderWnd", height, " rMonsterOrders.height: ", rMonsterOrders.height)
    /*if (rMonsterOrders.height > height) {
        rMonsterOrders.height = height
    }*/
}

function resizeWarlockWnd(height) {
    //console.log("resizeWarlockWnd", height, " rWarlock.height: ", rWarlocks.height)
    svWarlocks.height = height
    /*if (rWarlocks.height > height) {
        rWarlocks.height = height
        //svrWarlocks.height = height
    }*/
}

function resizeAddOrdersWnd(height, type) {
    console.log("resizeAddOrdersWnd", height, " type: ", type)

    if (type === "cp") {
        lvaoCharmPerson.height = height;
    } else if (type === "p") {
        lvaoParalyze.height = height;
    } else if (type === "d") {
        lvaoDelay.height = height;
    } else if (type === "f") {
        lvaoFire.height = height;
    } if (type === "pr") {
        lvaoPermanent.height = height;
    }

    var total_height = lvaoCharmPerson.height + lvaoParalyze.height;
    if (lvaoDelay.visible) {
        total_height += lvaoDelay.height;
    }
    if (lvaoFire.visible) {
        total_height += lvaoFire.height;
    }
    if (lvaoPermanent.visible) {
        total_height += lvaoPermanent.height;
    }
    total_height += tStepMsg.height;
    total_height += tMonsterList.height;
    total_height += svWarlocks.height;
    total_height += svMonsterOrders.height;

    console.log("total_height", total_height, lvaoCharmPerson.height, lvaoParalyze.height, tStepMsg.height, tMonsterList.height, svWarlocks.height, svMonsterOrders.height,
                (lvaoDelay.visible ? lvaoDelay.height : -1), (lvaoFire.visible ? lvaoFire.height : -1), (lvaoPermanent.visible ? lvaoPermanent.height : -1));
    lvReadyBattle.contentHeight = total_height + 100;
}

function prepareStepMessage() {
    tStepMsg.text = Qt.core.finishedBattle
    console.log("prepareStepMessage", tStepMsg.text, tStepMsg.contentHeight, tStepMsg.height);
    tStepMsg.height = tStepMsg.contentHeight
    if (tStepMsg.text === '') {
        tStepMsg.height = 0;
    }
}

function prepareMonsterList() {
    tMonsterList.text = Qt.core.monsters;
    console.log("prepareMonsterList", tMonsterList.text, tMonsterList.contentHeight, tMonsterList.height);
    tMonsterList.height = tMonsterList.contentHeight
    if (tMonsterList.text === '') {
        tMonsterList.height = 0;
    }
}

function prepareWarlockList() {
    console.log("svWarlocks")
    if (!cWarlockObject) {
        cWarlockObject = Qt.createComponent("qrc:///qml/warlock.qml");
    }
    if (cWarlockObject.status === Component.Error) {
        console.log("Error loading component: " + cWarlockObject.errorString());
        return ;
    }

    if (cWarlockObject.status === Component.Ready) {
        console.log("component ready");
        finishPrepareWarlockList();
    } else {
        console.log("connect to component ready signal");
        cWarlockObject.statusChanged.connect(finishPrepareWarlockList);
    }
}

function finishPrepareWarlockList() {
    cleanChildren(svWarlocks);
    var str_warlock_list = Qt.core.warlocks
    console.log("finishPrepareWarlockList: " + str_warlock_list);
    if (cWarlockObject.status === Component.Error || cWarlockObject.status !== Component.Ready) {
        console.log("Error loading component:", cWarlockObject.errorString());
        return ;
    }
    var arr = JSON.parse(str_warlock_list);
    battle.warlocks = arr;
    var curr_y = 0;
    var total_height = 0;
    for(var i = 0, Ln = arr.length; i < Ln; ++i) {
        if (!arr[i]) {
            continue;
        }
        var arr_m = arr[i];
        var sprite = cWarlockObject.createObject(svWarlocks, {w_warlock_status: arr_m.name + ' ' + arr_m.status, w_left_g: arr_m.L, w_right_g: arr_m.R,
                                                     w_possible_spells: arr_m.spells, "height_koeff": Qt.mainWindow.height_koeff, x: 0, y: curr_y});
        if (sprite === null) {
            console.log("Error creating object");
            continue;
        }
        console.log("looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
        console.log("svWarlocks.w: " + svWarlocks.width);
        console.log("svWarlocks.h: " + svWarlocks.height);
        curr_y += sprite.height;
        total_height += sprite.height;
        /*if (i < 2) {
            if (total_heght < sprite.y + sprite.height + 3) {
                total_heght = sprite.y + sprite.height + 3
            }
        }*/
    }
    //svWarlocks.height = total_height;
    //lvWarlocks.height = total_heght
    resizeWarlockWnd(total_height);
}

function cleanChildren(component) {
    console.log("cleanShildren: " + component.children.length)
    for(var i = component.children.length; i > 0; i--) {
        console.log("destroying: " + i)
        component.children[i-1].destroy()
    }
}

function createMonsterTarget(str_monster_list) {
    cleanChildren(svMonsterOrders);

    console.log("createMonsterTarget: " + str_monster_list)
    mtTaskList.push(str_monster_list)
    if (!cMonsterTarget) {
        cMonsterTarget = Qt.createComponent("qrc:///qml/monster_target.qml");
    }
    if (cMonsterTarget.status === Component.Error) {
        console.log("Error loading component: " + cMonsterTarget.errorString());
        return ;
    }

    if (cMonsterTarget.status === Component.Ready) {
        console.log("component ready");
        mtTaskList.pop();
        finishLoadMonsterTargetEx(str_monster_list);
    } else {
        console.log("connect to component ready signal");
        cMonsterTarget.statusChanged.connect(finishLoadMonsterTarget);
    }
}

function finishLoadMonsterTarget() {
    var o = mtTaskList.pop();
    finishLoadMonsterTargetEx(o)
}

function finishLoadMonsterTargetEx(str_monster_list) {
    cleanChildren(svMonsterOrders);
    console.log("finishLoadMonsterTargetEx: " + str_monster_list);
    if (cMonsterTarget.status === Component.Error || cMonsterTarget.status !== Component.Ready) {
        console.log("Error loading component:", cMonsterTarget.errorString());
        return ;
    }
    mtMonsterObj = [];
    var arr = str_monster_list.split(";");
    //var arrL=[],currLIdx=0;
    //var currLeft = 0;
    var curr_y = 3;
    var total_heght = 0;
    for(var i = 0, Ln = arr.length; i < Ln; ++i) {
        if (arr[i] === '') {
            continue;
        }
        var arr_m = arr[i].split(",");
        var monster_id = arr_m[0];
        var monster_name = arr_m[1];
        var monster_target = arr_m[2];

        var sprite = cMonsterTarget.createObject(svMonsterOrders, {mt_id: monster_id, mt_label: monster_name, x: 3, y: curr_y, width: svMonsterOrders.width, mt_model: strVisibleTarget});
        if (sprite === null) {
            console.log("Error creating object");
            continue;
        }
        mtMonsterObj[mtMonsterObj.length] = sprite;
        console.log("looks like created: " + sprite.mt_label + " x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
        console.log("svMonsterOrders.w: " + svMonsterOrders.width);
        /*if (currLeft != 0) {
            console.log("currentleft.w: " + currLeft.width)
            if (currLeft.width + sprite.width + 6 < svMonsterOrders.width) {
                sprite.y = currLeft.y
                sprite.x = currLeft.x + currLeft.width + 3
                if (arrL[currLIdx + 1]) {
                    console.log("exists next arrL")
                    ++currLIdx
                    currLeft = arrL[currLIdx]
                } else {
                    console.log("currLeft = 0")
                    currLeft = 0
                }
            } else {
                console.log("push into arrL")
                arrL[arrL.length] = sprite
                curr_y += sprite.height + 3
            }
        } else {*/
            //currLeft = sprite
            //arrL[currLIdx] = sprite
            curr_y += sprite.height + 3
        //}
        if (total_heght < sprite.y + sprite.height + 3) {
            total_heght = sprite.y + sprite.height + 3
        }
    }
    resizeMonsterOrderWnd(total_heght);
}

function prepareTargets(str_traget) {
    console.log("str_traget", str_traget)
    arrTarget = [];
    arrTargetID = [];
    arrVisibleTarget = [];
    strVisibleTarget = "";
    var arr = str_traget.split("#");
    for(var i = 0, Ln = arr.length; i < Ln; ++i) {
        if (arr[i] === '') {
            continue;
        }
        var arr2 = arr[i].split(";");
        if (arr2.length !== 2) {
            continue;
        }
        console.log("arrTarget["+arr2[1]+"] = " + arr2[0])
        arrTarget[arr2[1]] = arr2[0];
        arrTargetID[arr2[0]] = arr2[1];
        arrVisibleTarget.push(arr2[1]);
        strVisibleTarget += arr2[1] + ";"
    }
    if (strVisibleTarget.length > 0) {
        strVisibleTarget = strVisibleTarget.substr(0, strVisibleTarget.length - 1);
    }
    cbLHT.model = arrVisibleTarget;
    cbRHT.model = arrVisibleTarget;
    console.log("strVisibleTarget", strVisibleTarget)
}

function showNewUserWnd() {
    showWindow("new_user.qml")
}

function hideNewUserWnd() {
    if (wndNew) {
        wndNew.destroy();
        wndNew = null;
    }
}

function showErrorWnd(wnd_name) {
    if (!wndErr) {
        wndErr = Qt.createComponent("qrc:///qml/" + wnd_name);
    }
    if (wndErr.status === Component.Ready) {
        finishedShowErrWindow();
    } else {
        console.log("Current status", wndErr.status)
        wndTmp.statusChanged.connect(finishedShowErrWindow);
    }
}

function finishedShowErrWindow() {
    console.log("finishedShowErrWindow")
    if (wndErr.status === Component.Ready) {
        wndErr.createObject(mainWindow, {})
    } else if (wndErr.status === Component.Error) {
        console.log("Error loading component:", wndErr.errorString())
    } else {
        console.log("Unknown status", wndErr.status)
    }
}

function showWindow(wnd_name, child_wnd) {
    console.log("try to show window:" + wnd_name)
    if (child_wnd) {
        child_wnd.destroy();
    }
    if (wnd_name === "error_message.qml") {
        showErrorWnd(wnd_name);
        return;
    }

    wndTmp = Qt.createComponent("qrc:///qml/" + wnd_name);
    wndTmp.window_name = wnd_name;
    if (wndTmp.status === Component.Ready) {
        finishedShowWindow();
    } else {
        console.log("Current status", wndTmp.status)
        wndTmp.statusChanged.connect(finishedShowWindow);
    }
}

function finishedShowWindow(wnd) {
    if (!wnd) {
        wnd = wndTmp;
    }

    console.log("finishedShowWindow")
    if (wnd.status === Component.Ready) {
        wnd.createObject(mainWindow, {})
    } else if (wnd.status === Component.Error) {
        console.log("Error loading component:", wnd.errorString())
    } else {
        console.log("Unknown status", wnd.status)
    }
}

function showLoading() {
    console.log("showLoading")
    if (!wndLoading) {
        wndLoading = Qt.createComponent("qrc:///qml/loading.qml").createObject(mainWindow, {});
    }
    wndLoading.visible = true;
}

function hideLoading() {
    console.log("hideLoading")
    if (!wndLoading) {
        wndLoading = Qt.createComponent("qrc:///qml/loading.qml").createObject(mainWindow, {});
    }
    wndLoading.visible = false;
}

function isLoadingChanged() {
    console.log("isLoadingChanged")
    if (Qt.core.isLoading === 1) {
        showLoading();
    } else {
        hideLoading()
    }
}

function linkActivated(link) {
    console.log("linkActivated", link)
    var a = link.split("/");
    switch(a[1]) {
        case "force_surrender":
            Qt.core.forceSurrender(a[2], a[3])
            break;
        case "show_spell_desc":
            Qt.mainWindow.showSpellDetails(a[2]);
    }
}

function startSkynet() {
    processBattle(battle);
}
