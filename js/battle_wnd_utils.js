function processHighlightHintAction(a, restore) {
    var warlock_idx = BU.map_warlock_name_to_idx[a.warlock_name];
    console.log("processHighlightHintAction", a.warlock_name, warlock_idx);
    if (!a.warlock_name) {
        return;
    }

    var ww = iWarlocks.children[warlock_idx];
    if (!ww) {
        return;
    }
    if (restore) {
        ww.highlightActionRestore(a);
    }else {
        ww.highlightAction(a);
    }
}

function prepareIconToHint() {
    rTTIMainIcon.visible = false;
    iTTIMainIcon.visible = false;
    iTTISmallIcon.visible = false;
    tTTIMainIconText.text = "";
    tTTIMainIconText.visible = false;
}

function processHighlightIconAction(a, restore) {
    console.log("processHighlightIconAction", JSON.stringify(a), restore);

    if (restore) {
        prepareIconToHint();
    }else {
        rTTIMainIcon.border.color = a.border_color;
        rTTIMainIcon.color = a.background_color;
        rTTIMainIcon.visible = true;
        if (a.large_icon !== "") {
            iTTIMainIcon.source = "qrc:/res/" + a.large_icon + ".png";
            iTTIMainIcon.visible = true;
        }
        if (a.small_icon !== "") {
            iTTISmallIcon.source = "qrc:/res/" + a.small_icon + ".png";
            iTTISmallIcon.visible = true;
        }
        if (a.text !== "") {
            tTTIMainIconText.color = a.border_color;
            tTTIMainIconText.text = a.text;
            tTTIMainIconText.visible = true;
        }
    }
}

function processHintAction(a, restore) {
    console.log("BWU.processHintAction", JSON.stringify(a), restore);
    switch(a.action) {
    case "highlight": return processHighlightHintAction(a, restore);
    case "icon": return processHighlightIconAction(a, restore);
    }
}

function processAllHintAction(actions, restore) {
    var i, Ln = actions.length;
    rTutOverlay.opacity = (((Ln === 0) && !restore) || ((Ln !== 0) && restore)) ? 0.5 : 0.3;
    for (i = 0; i < Ln; ++i) {
        processHintAction(actions[i], restore);
    }
}


function prepareWarockToHint(restore) {
    for (var i = 0, Ln = iWarlocks.children.length; i < Ln; ++i) {
        iWarlocks.children[i].hintOnOff(restore);
    }
}

function closeTutorial() {
    ltTutorial.visible = false;
    prepareWarockToHint(true);
    prepareIconToHint();
}

function showTutorialData(diff, skip_restore) {
    var log_msg = mainWindow.gBattle.turn_num === 1;
    var code = log_msg ? "Play_Tutorial_Click_" : "Play_Turn_Message_Click_";
    if ((diff !== 0) && !skip_restore) {
        // restore prev changed
        processAllHintAction(ltTutorial.tutorialData[ltTutorial.tutorialDataIdx].actions, true);
        if (log_msg) {
            if (diff > 0) {
                mainWindow.logEvent(code + "Next");
            } else {
                mainWindow.logEvent(code + "Back");
            }
        }
    }

    ltTutorial.tutorialDataIdx += diff;
    if (ltTutorial.tutorialDataIdx < 0) {
        ltTutorial.tutorialDataIdx = 0;
    }

    if (ltTutorial.tutorialDataIdx >= ltTutorial.tutorialData.length) {
        closeTutorial();
        return;
    }
    var hint = ltTutorial.tutorialData[ltTutorial.tutorialDataIdx];
    if (!hint) {
        if (log_msg) {
            mainWindow.logEvent(code + "Gotit");
        }
        closeTutorial();
        return;
    }
    console.log("showTutorialData", diff, JSON.stringify(hint));
    ltTutorial.color = hint.color_bg;
    ltTTT.text = hint.txt;
    prepareWarockToHint(false);
    prepareIconToHint();

    processAllHintAction(ltTutorial.tutorialData[ltTutorial.tutorialDataIdx].actions, false);
}
