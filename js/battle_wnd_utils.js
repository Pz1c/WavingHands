// the Warlock.qml item a highlight action is about, null when there is none
function getHintWarlock(a) {
    // a monster is shown inside the Warlock.qml item of its owner, a.object is the monster name
    // https://github.com/Pz1c/WavingHands/issues/227
    var warlock_idx = a.object_type === "monster" ? BU.map_monster_name_to_idx[a.object] : BU.map_warlock_name_to_idx[a.warlock_name];
    if ((warlock_idx === undefined) && a.warlock_name) {
        warlock_idx = BU.map_warlock_name_to_idx[a.warlock_name];
    }
    console.log("processHighlightHintAction", a.object_type, a.warlock_name, a.object, warlock_idx);
    if (!a.warlock_name) {
        return null;
    }

    var ww = iWarlocks.children[warlock_idx];
    return ww ? ww : null;
}

// https://github.com/Pz1c/WavingHands/issues/227
// The elemental serves nobody: its tile is in the bottom bar, under the news. When it runs amok the
// news shows it in the bottom left corner of the board, growing to the top right.
function processElementalHintAction(a, restore) {
    paHintElementalGrow.stop();
    iiHintElemental.scale = 1;
    iiHintElemental.color = iiHintElemental.bg_color;
    iiHintElemental.visible = !restore;
    if (!restore) {
        caHintElementalGrow.to = a.color;
        paHintElementalGrow.restart();
    }
}

function processHighlightHintAction(a, restore) {
    if (a.object_type === "elemental") {
        return processElementalHintAction(a, restore);
    }
    var ww = getHintWarlock(a);
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
    // https://github.com/Pz1c/WavingHands/issues/227 what a step plays over the board, and what it holds back
    naHintTextReveal.stop();
    ltTTT.opacity = 1;
    iTTIcons.opacity = 1;
    saHintFly.stop();
    iHintFly.visible = false;
    paHintBigPop.stop();
    iHintBig.visible = false;
    paHintElementalGrow.stop();
    iiHintElemental.visible = false;
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

// https://github.com/Pz1c/WavingHands/issues/227
// The icon of a cast spell grows out of the last letter of the spell, up to 384 in the middle of the
// caster's side of the board, and shrinks into the tile the spell leaves there: the status icon, the
// summoned monster, the HP card or the monster it was cast at. With no tile to land on it shrinks
// where it grew.
function processFlyHintAction(a) {
    var cw = iWarlocks.children[BU.map_warlock_name_to_idx[a.from]];
    if (!cw) {
        return;
    }
    var big = HINT_BIG_ICON * mainWindow.ratioObject, small = HINT_FLY_START * mainWindow.ratioObject;
    var mid = cw.mapToItem(iHintStage, 0.5 * cw.width, 0.5 * cw.height);
    // the whole icon stays on the board
    mid = {x: mid.x, y: Math.max(Math.min(mid.y, iHintStage.height - 0.5 * big), 0.5 * big)};
    var start = cw.newsGesturePoint(iHintStage);
    if (!start) {
        start = mid;
    }
    var tw = a.to ? getHintWarlock(a.to) : null;
    var end = tw ? tw.newsTilePoint(a.to, iHintStage) : null;
    if (!end) {
        end = {x: mid.x, y: mid.y, size: small};
    }
    console.log("processFlyHintAction", a.icon, JSON.stringify(start), JSON.stringify(mid), JSON.stringify(end));

    saHintFly.stop();
    iHintFly.source = "qrc:/res/" + a.icon + ".png";
    iHintFly.cx = start.x;
    iHintFly.cy = start.y;
    iHintFly.size = small;
    iHintFly.opacity = 1;
    naHintFlyGrowX.to = mid.x;
    naHintFlyGrowY.to = mid.y;
    naHintFlyGrow.to = big;
    naHintFlyLandX.to = end.x;
    naHintFlyLandY.to = end.y;
    naHintFlyLand.to = end.size;
    iHintFly.visible = true;
    saHintFly.restart();
}

// a tile that comes to the board in this step: hidden when the step starts, shown when its turn comes
function processTileHintAction(a) {
    var ww = getHintWarlock(a.target);
    if (ww) {
        ww.newsTileVisible(a.target, a.show);
    }
}

// The play on the board is over: the step's text and icon card show (the card pops in, or its small
// icon: the one that acts on the target), and after a blow its big outcome icon, which stays until
// the step is left.
function processRevealHintAction(a) {
    if (ltTTT.opacity < 1) {
        naHintTextReveal.restart();
    }
    iTTIcons.opacity = 1;
    if (a.pop === "small") {
        paHintSmallIconPop.restart();
    } else if (a.pop === "large") {
        paHintMainIconPop.restart();
    }
    if (a.big !== "") {
        iHintBig.source = "qrc:/res/" + a.big + ".png";
        iHintBig.visible = true;
        paHintBigPop.restart();
    }
}

// https://github.com/Pz1c/WavingHands/issues/227
// The temporary status icons and monster tiles a turn news step shows, and the real ones it does not
// show yet. Nothing to restore when the step is left: the next step brings its own set and
// closeTutorial clears them (Warlock.hintOnOff).
function processTempIconsAction(a, restore) {
    if (restore) {
        return;
    }
    var ww = iWarlocks.children[BU.map_warlock_name_to_idx[a.warlock_name]];
    if (ww) {
        ww.setTempIcons(a.statuses, a.monsters, a.hide_statuses, a.hide_monsters);
    }
}

// The turn news starts a moment after the board is built (wnd_battle.prepareHintWithCheck). The
// tiles of its first step are set at once, so what the news brings to the board is not seen before
// its turn, not even for a frame: neither what comes in a later step nor what the first one brings.
function prepareHintBoard() {
    var hint = BU.battle.hint, actions = hint && (hint.length > 0) ? hint[0].actions : [], a, ww, i, Ln;
    // the lists first, then the tiles in them
    for (i = 0, Ln = actions.length; i < Ln; ++i) {
        a = actions[i];
        ww = a.action === "temp_icons" ? findNewWarlock(a.warlock_name) : null;
        if (ww) {
            ww.setTempIcons(a.statuses, a.monsters, a.hide_statuses, a.hide_monsters);
        }
    }
    for (i = 0, Ln = actions.length; i < Ln; ++i) {
        a = actions[i];
        ww = a.appear ? findNewWarlock(a.warlock_name) : null;
        if (ww) {
            ww.newsTileVisible(a, false);
        }
    }
}

// The Warlock.qml item of that warlock while the board is being built: by name and from the end,
// the warlocks of the battle shown before are still on their way out of iWarlocks.
function findNewWarlock(name) {
    for (var i = iWarlocks.children.length - 1; i >= 0; --i) {
        var ww = iWarlocks.children[i];
        if (ww.l_warlock && (ww.l_warlock.name === name)) {
            return ww;
        }
    }
    return null;
}

function processHintAction(a, restore) {
    console.log("BWU.processHintAction", JSON.stringify(a), restore);
    switch(a.action) {
    case "highlight": return processHighlightHintAction(a, restore);
    case "icon": return processHighlightIconAction(a, restore);
    case "fly": return processFlyHintAction(a);
    case "tile": return processTileHintAction(a);
    case "reveal": return processRevealHintAction(a);
    case "temp_icons": return processTempIconsAction(a, restore);
    }
}

// https://github.com/Pz1c/WavingHands/issues/306
// https://github.com/Pz1c/WavingHands/issues/227
// A turn news (hint) step is played on the board, its text and its icon card come when the play is over.
// A cast: all the gestures go dark, the letters of the spell light up one after another (S -> F -> W),
// the spell's icon grows out of the last one and shrinks into the tile the spell leaves on the board,
// the tile shows and then lights up. An attack: the attacker grows, then the one that takes the blow
// (the target or its shield), then the big outcome icon shows with the text.
const HINT_FIRST_FRAME_DELAY = 50;  // all dark
const HINT_LETTER_DELAY = 200;
const HINT_FLY_TIME = 350;          // the spell icon grows and shrinks (saHintFly)
const HINT_TILE_DELAY = 50;         // a new tile shows before it lights up
const HINT_GROW_TIME = 150;         // the attacker, then its target (Warlock.qml)
const HINT_BIG_ICON = 384;
const HINT_FLY_START = 64;          // as big as a gesture on its card

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
            frame.push({t:g.t,l:g.l,r:g.r,lv:g.lv,rv:g.rv,la:g.la && (i <= lit),ra:g.ra && (i <= lit),
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

// https://github.com/Pz1c/WavingHands/issues/227
// The step's text only comes when its play on the board is over, so a tap that comes earlier does
// not skip the step: it shows the rest of it at once (the spell icon that is on its way is dropped).
// Returns false when the step has nothing left to play.
function finishHintAnimation() {
    if (hint_frames.length === 0) {
        return false;
    }
    var frames = hint_frames, i, j, Ln, LnJ;
    stopHintAnimation();
    for (i = 0, Ln = frames.length; i < Ln; ++i) {
        for (j = 0, LnJ = frames[i].actions.length; j < LnJ; ++j) {
            if (frames[i].actions[j].action !== "fly") {
                processHintAction(frames[i].actions[j], false);
            }
        }
    }
    saHintFly.stop();
    iHintFly.visible = false;
    return true;
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
    var letters = [], actors = [], targets = [], appearing = [], icon = null, a, lf, i, j, Ln, frame_actions;
    stopHintAnimation();
    for (i = 0, Ln = actions.length; i < Ln; ++i) {
        a = actions[i];
        if ((a.action === "highlight") && (a.object_type === "warlock") && (a.object === "hp")) {
            targets.push(a);
            continue;
        }
        // https://github.com/Pz1c/WavingHands/issues/227
        // a monster that acts (or the elemental) comes before the ones that are hit and the hearts; a
        // status icon (the shield that took the blow, the enchantment that landed) comes with those
        if ((a.action === "highlight") && ((a.object_type === "monster") || (a.object_type === "status") || (a.object_type === "elemental"))) {
            if (a.role === "actor") {
                actors.push(a);
            } else {
                targets.push(a);
            }
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

    // a tile this step brings to the board (the lists have it by now, the temp_icons actions are played)
    // is not seen until its turn in the step, and it is where a cast spell's icon lands
    var landing = null, grow = false;
    for (i = 0, Ln = targets.length; i < Ln; ++i) {
        a = targets[i];
        if (a.appear) {
            processHintAction({action:"tile",target:a,show:false}, false);
            appearing.push({action:"tile",target:a,show:true});
        }
        if (!landing || (a.appear && !landing.appear)) {
            landing = a;
        }
        grow = grow || a.grow;
    }

    var delay = HINT_FIRST_FRAME_DELAY, played = targets.length > 0;
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
        played = true;
    }

    if (icon && icon.fly_from && icon.large_icon) {
        hint_frames.push({delay:delay,actions:[{action:"fly",icon:icon.large_icon,from:icon.fly_from,to:landing}]});
        delay = HINT_FLY_TIME;
        played = true;
    }

    if (actors.length > 0) {
        hint_frames.push({delay:delay,actions:actors});
        delay = HINT_GROW_TIME;
        played = true;
    }

    if (appearing.length > 0) {
        hint_frames.push({delay:delay,actions:appearing});
        delay = HINT_TILE_DELAY;
    }

    var reveal = {action:"reveal",pop:"",big:""};
    if (icon) {
        // the small icon is what acts on the target, without one the whole card pops
        reveal.pop = icon.small_icon && (icon.small_icon !== "") ? "small" : "large";
        var pop_item = reveal.pop === "small" ? iTTISmallIcon : rTTIMainIcon;
        pop_item.scale = 0;
        pop_item.opacity = 0;
        reveal.big = icon.big_icon ? icon.big_icon : "";
    }
    if (grow) {
        // the ones that take the blow grow, then its outcome shows
        hint_frames.push({delay:delay,actions:targets});
        hint_frames.push({delay:HINT_GROW_TIME,actions:[reveal]});
    } else {
        hint_frames.push({delay:delay,actions:targets.concat([reveal])});
    }
    if (played) {
        naHintTextReveal.stop();
        ltTTT.opacity = 0;
        iTTIcons.opacity = 0;
    }
    console.log("BWU.playHintActions", JSON.stringify(hint_frames));
    scheduleNextHintFrame();
}

function processAllHintAction(actions, restore) {
    var i, Ln = actions.length, shown = 0;
    for (i = 0; i < Ln; ++i) {
        // the temporary icons come with every step, they do not make it a step that shows something
        if (actions[i].action !== "temp_icons") {
            ++shown;
        }
    }
    rTutOverlay.opacity = (((shown === 0) && !restore) || ((shown !== 0) && restore)) ? 0.5 : 0.3;
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
    if ((diff > 0) && !skip_restore && finishHintAnimation()) {
        return;
    }
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
