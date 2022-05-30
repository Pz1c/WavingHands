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

function processHintAction(a, restore) {
    console.log("BWU.processHintAction", JSON.stringify(a), restore);
    switch(a.action) {
    case "highlight": return processHighlightHintAction(a, restore);
    }
}

function processAllHintAction(actions, restore) {
    var i, Ln = actions.length;
    rTutOverlay.opacity = (((Ln === 0) && !restore) || ((Ln !== 0) && restore)) ? 0.5 : 0.1;
    for (i = 0; i < Ln; ++i) {
        processHintAction(actions[i], restore);
    }
}


function showTutorialData(diff) {
    var log_msg = mainWindow.gBattle.turn_num === 1;
    var code = log_msg ? "Play_Tutorial_Click_" : "Play_Turn_Message_Click_";
    if (diff !== 0) {
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
        ltTutorial.visible = false;
        return;
    }
    var hint = ltTutorial.tutorialData[ltTutorial.tutorialDataIdx];
    if (!hint) {
        if (log_msg) {
            mainWindow.logEvent(code + "Gotit");
        }
        ltTutorial.visible = false;
        return;
    }
    console.log("showTutorialData", diff, JSON.stringify(hint));
    ltTutorial.color = hint.color_bg;
    ltTTT.text = hint.txt;

    processAllHintAction(ltTutorial.tutorialData[ltTutorial.tutorialDataIdx].actions, false);
}
