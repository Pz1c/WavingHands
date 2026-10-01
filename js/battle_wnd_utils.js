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
    paHintMainIconPop.stop();
    paHintSmallIconPop.stop();
    rTTIMainIcon.scale = 1;
    rTTIMainIcon.opacity = 1;
    iTTISmallIcon.scale = 1;
    iTTISmallIcon.opacity = 1;
    rTTIMainIcon.visible = false;
    iTTIMainIcon.visible = false;
    iTTISmallIcon.visible = false;
    tTTIMainIconText.text = "";
    tTTIMainIconText.visible = false;
    tTTIMainIconTitle.text = "";
    tTTIMainIconTitle.visible = false;
}

function processHighlightIconAction(a, restore) {
    console.log("processHighlightIconAction", JSON.stringify(a), restore);

    if (restore) {
        prepareIconToHint();
    }else {
        rTTIMainIcon.border.color = a.border_color;
        rTTIMainIcon.color = a.background_color;
        rTTIMainIcon.visible = true;
        if (a.large_icon && (a.large_icon !== "")) {
            if (a.large_icon === "heart") {
                if ((a.text + " ").indexOf("- ") === 0) {
                    a.large_icon = "heart_small";
                } else {
                    a.large_icon = "spellbook";
                }
            }

            iTTIMainIcon.source = "qrc:/res/" + a.large_icon + ".png";
            iTTIMainIcon.visible = true;
        }
        if (a.small_icon && (a.small_icon !== "")) {
            iTTISmallIcon.source = "qrc:/res/" + a.small_icon + ".png";
            iTTISmallIcon.visible = true;
        }
        if (a.text && (a.text !== "")) {
            tTTIMainIconText.color = a.border_color;
            tTTIMainIconText.text = a.text;
            tTTIMainIconText.visible = true;
        }
        if (a.title && (a.title !== "")) {
            tTTIMainIconTitle.color = a.border_color;
            tTTIMainIconTitle.text = a.title;
            tTTIMainIconTitle.visible = true;
        }
    }
}

function processPopHintAction(a) {
    if (a.icon === "small") {
        paHintSmallIconPop.restart();
    } else {
        paHintMainIconPop.restart();
    }
}

function processHintAction(a, restore) {
    console.log("BWU.processHintAction", JSON.stringify(a), restore);
    switch(a.action) {
    case "highlight": return processHighlightHintAction(a, restore);
    case "icon": return processHighlightIconAction(a, restore);
    case "pop": return processPopHintAction(a);
    }
}

// https://github.com/Pz1c/WavingHands/issues/306
// A turn news (hint) step doesn't show everything at once: the letters of the cast spell light
// up one after another (S -> F -> W), then the step's icon pops in (the amnesia, the goblin),
// and only then the target's heart gets its background.
const HINT_FIRST_FRAME_DELAY = 150;
const HINT_LETTER_DELAY = 200;
const HINT_TARGET_DELAY = 300; // lets the icon pop finish first

var hint_frames = [];

// Splits a gestures highlight into frames: the first one has none of the spell's letters lit,
// every next one lights one more, oldest first. lp/rp mark the letter that has just lit up,
// Warlock.qml pops it.
function getHintLetterFrames(a) {
    var res = [], i, lit, frame, g, Ln = a.data.length, first = Ln;
    for (i = 0; i < Ln; ++i) {
        if (a.data[i].la || a.data[i].ra) {
            first = i;
            break;
        }
    }
    if (first === Ln) {
        return [a];
    }

    for (lit = first - 1; lit < Ln; ++lit) {
        frame = [];
        for (i = 0; i < Ln; ++i) {
            g = a.data[i];
            frame.push({l:g.l,r:g.r,lv:g.lv,rv:g.rv,la:g.la && (i <= lit),ra:g.ra && (i <= lit),
                        lp:g.la && (i === lit),rp:g.ra && (i === lit)});
        }
        res.push({action:a.action,warlock_name:a.warlock_name,object_type:a.object_type,object:a.object,data:frame});
    }
    return res;
}

function stopHintAnimation() {
    tHintFrame.stop();
    hint_frames = [];
}

function scheduleNextHintFrame() {
    if (hint_frames.length === 0) {
        return;
    }
    tHintFrame.interval = hint_frames[0].delay;
    tHintFrame.restart();
}

function playNextHintFrame() {
    var frame = hint_frames.shift();
    if (!frame) {
        return;
    }
    for (var i = 0, Ln = frame.actions.length; i < Ln; ++i) {
        processHintAction(frame.actions[i], false);
    }
    scheduleNextHintFrame();
}

function playHintActions(actions) {
    var letters = [], targets = [], icon = null, a, lf, i, j, Ln, frame_actions;
    stopHintAnimation();
    for (i = 0, Ln = actions.length; i < Ln; ++i) {
        a = actions[i];
        if ((a.action === "highlight") && (a.object_type === "warlock") && (a.object === "hp")) {
            targets.push(a);
            continue;
        }
        if ((a.action === "highlight") && (a.object_type === "warlock") && (a.object === "gestures")) {
            lf = getHintLetterFrames(a);
            a = lf.shift();
            if (lf.length > 0) {
                letters.push(lf);
            }
        } else if (a.action === "icon") {
            icon = a;
        }
        processHintAction(a, false);
    }

    var delay = HINT_FIRST_FRAME_DELAY;
    for (i = 0; ; ++i) {
        frame_actions = [];
        for (j = 0, Ln = letters.length; j < Ln; ++j) {
            if (i < letters[j].length) {
                frame_actions.push(letters[j][i]);
            }
        }
        if (frame_actions.length === 0) {
            break;
        }
        hint_frames.push({delay:delay,actions:frame_actions});
        delay = HINT_LETTER_DELAY;
    }

    if (icon) {
        // the small icon is what acts on the target, without one the whole card pops
        var pop = icon.small_icon && (icon.small_icon !== "") ? "small" : "large";
        var pop_item = pop === "small" ? iTTISmallIcon : rTTIMainIcon;
        pop_item.scale = 0;
        pop_item.opacity = 0;
        hint_frames.push({delay:delay,actions:[{action:"pop",icon:pop}]});
        delay = HINT_TARGET_DELAY;
    }

    if (targets.length > 0) {
        hint_frames.push({delay:delay,actions:targets});
    }
    console.log("BWU.playHintActions", JSON.stringify(hint_frames));
    scheduleNextHintFrame();
}

function processAllHintAction(actions, restore) {
    var i, Ln = actions.length;
    rTutOverlay.opacity = (((Ln === 0) && !restore) || ((Ln !== 0) && restore)) ? 0.5 : 0.3;
    if (!restore) {
        playHintActions(actions);
        return;
    }
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
    stopHintAnimation();
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
    if (hint.font_size && (hint.font_size >= 21) && (hint.font_size <= 48)) {
        ltTTT.font.pixelSize = hint.font_size * mainWindow.ratioFont;
    }
    prepareWarockToHint(false);
    prepareIconToHint();

    processAllHintAction(ltTutorial.tutorialData[ltTutorial.tutorialDataIdx].actions, false);
}
