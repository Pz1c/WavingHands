.import "battle_gui_utils.js" as BGU

.import QtQuick 2.0 as QQ

var battle = {};
var map_warlock_name_to_idx = {};
// monster name -> index of the Warlock.qml item that shows it (the owner's), for the turn news
// https://github.com/Pz1c/WavingHands/issues/227
var map_monster_name_to_idx = {};
var cWarlockObject,cIconObject;
const G_WARLOCK_HEIGHT = 474;

// Classic player colors: one wizard icon per color (res/wizard_<color>.png). The color is
// pseudo-random per game (hashed from the battle id and the warlock name), so every game
// comes up with a different assignment, yet the same game shows the same colors everywhere.
var WIZARD_COLORS = ["black", "red", "blue", "green", "grey", "brown"];

function wizardColorIdx(seed) {
    var hash = 2166136261;
    for (var i = 0; i < seed.length; ++i) {
        hash ^= seed.charCodeAt(i);
        hash = (hash * 16777619) >>> 0; // FNV-1a
    }
    return hash % WIZARD_COLORS.length;
}

// Assign the warlock his game color; if the hash already picked it for another warlock of
// this battle, take the next free one, so two players in a battle never look the same.
function assignWizardColor(w) {
    if (!battle.wiz_colors) {
        battle.wiz_colors = {};
    }
    var idx = wizardColorIdx(w.name + "#" + battle.id);
    for (var k = 0; k < WIZARD_COLORS.length; ++k) {
        var col = WIZARD_COLORS[(idx + k) % WIZARD_COLORS.length];
        if (!battle.wiz_colors[col]) {
            battle.wiz_colors[col] = true;
            return col;
        }
    }
    return WIZARD_COLORS[idx]; // more warlocks than colors (monsters battles): reuse
}

// Current page <SELECT NAME=LH/RH> is stored as "value;label#value;label#".
// "X" is "As Right": it is not a gesture of its own.
function pageGestureValues(str) {
    var res = [], parts = (str || "").split("#");
    for (var i = 0, Ln = parts.length; i < Ln; ++i) {
        var value = parts[i].split(";")[0];
        if (value !== "") {
            res.push(value);
        }
    }
    return res;
}

function prepareWarlock(w) {
    w.color = assignWizardColor(w);
    w.monsters = battle.monsters[w.name];
    w.read_only = battle.read_only;
    w.print_g = BGU.preparePrintGestures(w.L, w.R, w.smcL, w.smcR);
    if (!w.id) {
        w.id = battle.targetsMap[w.name];
    }
    w.paralyzed_hand = battle.paralyzed_hand[w.id];
    if (!w.paralyzed_hand) {
        w.paralyzed_hand = "";
    }
    // The current form is authoritative; do not let a stale analyzed fh from
    // the previous turn's history lock a hand.
    w.fh = (w.paralyzed_hand === "LH") ? 1 : ((w.paralyzed_hand === "RH") ? 2 : 0);
    if (w.fh > 0) {
        w.paralyzed_hand = w.fh === 1 ? "LH" : "RH";
        if (battle.actions.CP[w.id]) {
            battle.actions.CP[w.id].h = w.paralyzed_hand;
        }
        if (battle.actions.CC[w.id]) {
            battle.actions.CC[w.id].h = w.paralyzed_hand;
        }
    }

    w.control_paralyze = battle.paralyze.indexOf(w.id) !== -1;
    w.control_charmed  = battle.charm.indexOf(w.id) !== -1;
    w.statusIcons = BGU.prepareStatusIcon(w);
    if (w.player) {
        var left = pageGestureValues(battle.page_plg);
        var right = pageGestureValues(battle.page_prg);
        var normal_left = [];
        for (var gi = 0, Gn = left.length; gi < Gn; ++gi) {
            if (left[gi] !== "X") {
                normal_left.push(left[gi]);
            }
        }
        // "As Right" means the left hand uses the right hand's options.
        if ((normal_left.length === 0) && (left.indexOf("X") !== -1)) {
            normal_left = right;
        }
        if (normal_left.length > 0) {
            w.plg = normal_left.join(",");
        }
        if (right.length > 0) {
            w.prg = right.join(",");
        }
    }
    w.banked_spell = w.bank;//player && */(battle.fire !== "") ? battle.fire : "";
    w.turn_num = battle.turn_num;
    if (w.player) {
        battle.L = w.L;
        battle.R = w.R;
        battle.player_name = w.name;
        battle.player_gesture_L = w.pgL;
        battle.player_gesture_R = w.pgR;
        battle.player_spell_L = w.psL;
        battle.player_spell_R = w.psR;
        battle.player_target_L = w.ptL;
        battle.player_target_R = w.ptR;
        battle.ngL = battle.player_gesture_L;
        battle.ngR = battle.player_gesture_R;
        battle.player_has_bank = w.delay > 0;
        battle.player_has_permanency = w.permanency > 0;
        battle.player_under_control = (w.charmed > 0) || (w.paralized > 0);
        console.log("prepareWarlock", battle.player_under_control, JSON.stringify(w));
    }

    return w;
}

function parseTargets(targets_str) {
    battle.targetsList = [];
    battle.targetsMap = {};
    var strs = BGU.strToArr2D(targets_str, '#', ';', true);
    var m_owner;
    console.log("parseTargets", JSON.stringify(strs));
    var title, obj_id;
    for (var i = 0, Ln = strs.length; i < Ln; ++i) {
        obj_id = strs[i][0];
        title = strs[i][1];
        if (title === " ") {
            title = "Default";
        }
        battle.targetsList.push(title);
        battle.targetsMap[title] = obj_id;
        battle.targetsMap[obj_id] = title;
    }
}

function setParaActions(paralyze, charm) {
    battle.paralyze = [];
    battle.charm = [];
    var i, Ln;
    if (paralyze !== "") {
        var tp = paralyze.split(";");
        for(i = 0, Ln = tp.length; i < Ln; ++i) {
            if (!tp[i]) {
                continue;
            }
            battle.paralyze.push(tp[i]);
            battle.actions.CP[tp[i]] = {h:"LH",g:"-",t:"P"};
        }
    }
    if (charm !== "") {
        var tc = charm.split(";");
        for(i = 0, Ln = tc.length; i < Ln; ++i) {
            if (!tc[i]) {
                continue;
            }
            battle.charm.push(tc[i]);
            battle.actions.CC[tc[i]] = {h:"LH",g:"-",t:"C"};
        }
    }
}

function parseParalyzedHands(arr) {
    battle.paralyzed_hand = {};
    var item;
    for (var i = 0, Ln = arr.length; i < Ln; ++i) {
        item = arr[i];
        battle.paralyzed_hand[item[0]] = item[1];
    }
}

// Turn news for a beginner (below the Master Warlock Spellbook, level 5) is limited to the three
// things they can act on: somebody cast a spell, somebody took damage, a poison/disease counter
// changed. Gestures, deaths, shields, counters, mirrors, charms etc. stay in the battle history.
function isBeginnerTurnNews(row) {
    switch (row.row_type) {
    case "spell":
        return true;
    case "attack":
        return row.obj.damage > 0;
    case "other":
        if ((row.obj.spell === "Poison") || (row.obj.spell === "Disease")) {
            return true;
        }
        return (row.obj.damage > 0) && !row.obj.shield && !row.obj.counter_spell && !row.obj.mirror;
    default:
        return false;
    }
}

// ---------------------------------------------------------------------------
// Last-turn damage and healing animations
//
// Turn news shows a hit as a banner icon today. Instead, the damage number is
// attached to the row that caused it (the spell cast, the stab, the monster
// attack, the elemental's "runs amok" row, or the magic mirror reflection).
// The damage row itself is consumed and only the cause row is shown. Cure
// spells work the same way, with a green +number on the casting row.
// ---------------------------------------------------------------------------

function getTurnNewsDamage(row) {
    var damage = row.obj ? parseInt(row.obj.damage, 10) : 0;
    return isNaN(damage) ? 0 : damage;
}

function isTurnNewsHeal(row) {
    return !!row.obj && /\bis healed\b/i.test(row.txt || "");
}

function getTurnNewsHealAmount(spell) {
    return spell === "Cure Heavy Wounds" ? 2 : 1;
}

function findTurnNewsTargetObject(target) {
    if (!target) {
        return null;
    }
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        if (!w) {
            continue;
        }
        if (target.type === "warlock") {
            if (w.name === target.name) {
                return w;
            }
            continue;
        }
        var monsters = w.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === target.name) {
                return monsters[j];
            }
        }
    }
    return null;
}

function isPreviousTurnBattle(previous_battle) {
    return !!previous_battle && (previous_battle.id === battle.id) &&
            ((parseInt(previous_battle.turn_num, 10) + 1) === parseInt(battle.turn_num, 10));
}

function getPreviousTurnTargetHp(previous_battle, target) {
    if (!isPreviousTurnBattle(previous_battle)) {
        return null;
    }
    for (var i = 0, Ln = previous_battle.warlocks.length; i < Ln; ++i) {
        var w = previous_battle.warlocks[i];
        if (!w) {
            continue;
        }
        if ((target.type === "warlock") && (w.name === target.name)) {
            var hp = parseInt(w.hp, 10);
            return isNaN(hp) ? null : hp;
        }
        var monsters = w.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === target.name) {
                var mhp = parseInt(monsters[j].hp, 10);
                return isNaN(mhp) ? null : mhp;
            }
        }
    }
    return null;
}

function cloneTurnNewsElemental(el) {
    var hp = el ? parseInt(el.hp, 10) : 0;
    if (isNaN(hp) || (hp < 0)) {
        hp = 0;
    }
    return {name:(el && el.name) ? el.name : "", hp:hp,
            type:(el && (el.type === "ice")) ? "ice" : "fire", action:"m"};
}

function getSummonedMonsterHp(name) {
    if (name.indexOf("Goblin") !== -1) {
        return 1;
    } else if (name.indexOf("Ogre") !== -1) {
        return 2;
    } else if (name.indexOf("Troll") !== -1) {
        return 3;
    } else if (name.indexOf("Giant") !== -1) {
        return 4;
    }
    return 1;
}

function findTurnNewsMonsterData(name) {
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        if (!w) {
            continue;
        }
        var monsters = w.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === name) {
                return monsters[j];
            }
        }
    }
    return null;
}

function getTurnNewsSummonInfo(row) {
    var txt = (row.txt || "").replace(/^\s+|\s+$/g, "");
    var match = /^(.+?)\s+is summoned to serve\s+(.+?)\.?$/i.exec(txt);
    if (!match) {
        return null;
    }
    return {name:match[1], owner:normalizeTurnNewsName(match[2])};
}

// "A Fire Elemental appears in a furious roar of flame!"
// "An Ice Elemental appears in a sudden rush of arctic wind!"
function getTurnNewsElementalAppearance(row) {
    var match = /\bA(?:n)?\s+(Fire|Ice)\s+Elemental\s+appears/i.exec(row.txt || "");
    return match ? match[1].toLowerCase() : "";
}

// Elementals leave the board in several ways: opposite storms, a storm of the
// same type, the opposite elemental, Resist Heat, blindness, and so on.
function isTurnNewsElementalDestroyed(row) {
    var txt = row.txt || "";
    if (!/Elemental/i.test(txt)) {
        return false;
    }
    if (/appears|casts\s+Summon|runs\s+amok|at\s+bay/i.test(txt)) {
        return false;
    }
    return /\b(destroy(?:ed|s)?|melt(?:s|ed)?|shattered|extinguish(?:ed)?|snuffed|quench(?:ed)?|doused|dissipat(?:ed|es)?|evaporat(?:ed|es)?|flies\s+away|vanishes|fades|dispelled|engulfed|consumed|nullif(?:y|ied|ies)|annihilat(?:e|ed|es)|cancel(?:led|s)?|dies)\b/i.test(txt);
}

function getTurnNewsTransitionRow(real_actions, idx) {
    var i, Ln = real_actions.length, row;
    for (i = idx; i < Ln; ++i) {
        row = real_actions[i];
        if ((row.type < 2) && !row.turn_news_hidden) {
            return row;
        }
    }
    for (i = idx - 1; i >= 0; --i) {
        row = real_actions[i];
        if ((row.type < 2) && !row.turn_news_hidden) {
            return row;
        }
    }
    return null;
}

function addTurnNewsTransitionAction(real_actions, idx, action) {
    var row = getTurnNewsTransitionRow(real_actions, idx);
    if (!row) {
        return;
    }
    if (!row.new_action) {
        row.new_action = [];
    }
    row.new_action.push(action);
    row.turn_news_force_show = true;
}

function findTurnNewsArrival(news_summons, row) {
    if (!news_summons || !news_summons.arrivals) {
        return null;
    }
    for (var i = 0, Ln = news_summons.arrivals.length; i < Ln; ++i) {
        if (news_summons.arrivals[i].row === row) {
            return news_summons.arrivals[i];
        }
    }
    return null;
}

function prepareTurnNewsTransitions(real_actions, previous_battle, news_summons) {
    var previous_valid = isPreviousTurnBattle(previous_battle);
    var elemental_summoned = false, i, Ln, row;
    for (i = 0, Ln = real_actions.length; i < Ln; ++i) {
        if (getTurnNewsElementalAppearance(real_actions[i])) {
            elemental_summoned = true;
            break;
        }
    }

    if (!battle.read_only) {
        for (i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
            var w = battle.warlocks[i];
            if (w) {
                // The order-form placeholders are not part of the turn-start board.
                w.news_placeholders_visible = false;
            }
        }
    }

    if (battle.read_only) {
        battle.turn_news_elemental = undefined;
        return;
    }
    if (previous_valid) {
        var pe = previous_battle.elemental;
        battle.turn_news_elemental = (pe && (parseInt(pe.hp, 10) > 0))
                ? cloneTurnNewsElemental(pe) : null;
    } else if (elemental_summoned) {
        battle.turn_news_elemental = null;
    } else {
        battle.turn_news_elemental = (battle.elemental && (parseInt(battle.elemental.hp, 10) > 0))
                ? cloneTurnNewsElemental(battle.elemental) : null;
    }

    for (i = 0, Ln = real_actions.length; i < Ln; ++i) {
        row = real_actions[i];
        var arrival = findTurnNewsArrival(news_summons, row);
        var summon = arrival
                ? {name:arrival.name, owner:arrival.owner, rec:arrival.rec}
                : (news_summons ? null : getTurnNewsSummonInfo(row));
        if (summon) {
            var summon_data = findTurnNewsMonsterData(summon.name);
            var summon_icon = summon_data
                    ? summon_data.icon
                    : BGU.getMonsterIconByName(summon.name);
            if (summon_icon.indexOf("new_") === 0) {
                summon_icon = summon_icon.substring(4);
            }
            var summon_hp = summon_data
                    ? summon_data.hp
                    : getSummonedMonsterHp(summon.name);
            var transition_idx = i;
            if (summon.rec) {
                for (var ci = 0, LnC = real_actions.length; ci < LnC; ++ci) {
                    if (real_actions[ci].summon === summon.rec) {
                        transition_idx = ci;
                        break;
                    }
                }
            }
            addTurnNewsTransitionAction(real_actions, transition_idx,
                    {action:"summon", object_type:"monster",
                     object:summon.name,
                     warlock_name:getMonsterOwnerName(summon.name) || summon.owner,
                     icon:summon_icon, hp:summon_hp});
        }

        var appearance = getTurnNewsElementalAppearance(row);
        if (appearance) {
            addTurnNewsTransitionAction(real_actions, i,
                    {action:"elemental", visible:true, type:appearance, hp:3});
        }
        if (isTurnNewsElementalDestroyed(row)) {
            addTurnNewsTransitionAction(real_actions, i,
                    {action:"elemental", visible:false, type:""});
        }

        if (row.row_type === "death") {
            var target = normalizeTurnNewsName(row.obj ? row.obj.target : "");
            // "Azi surrenders." is colored like a death row but nobody dies.
            if (/\bsurrenders?\b/i.test(target)) {
                target = "";
            }
            if (target) {
                var owner = getMonsterOwnerName(target);
                var is_warlock = false;
                for (var wi = 0, LnW = battle.warlocks.length; wi < LnW; ++wi) {
                    if (battle.warlocks[wi] && (battle.warlocks[wi].name === target)) {
                        is_warlock = true;
                        break;
                    }
                }
                addTurnNewsTransitionAction(real_actions, i,
                        {action:"death",
                         object_type:(owner || !is_warlock) ? "monster" : "warlock",
                         object:target, warlock_name:owner || target});
            }
        }
    }
}

function addTurnNewsDamageTotal(totals, target, damage) {
    if (!target || !(damage > 0)) {
        return;
    }
    var key = target.type + ":" + target.name;
    totals[key] = (totals[key] || 0) + damage;
}

function addTurnNewsHpAction(row, target, amount) {
    if (!row || !target || !amount) {
        return;
    }
    if (!row.new_action) {
        row.new_action = [];
    }
    row.new_action.push({action:"hp", warlock_name:target.owner,
                         object_type:target.type, object:target.name, amount:amount});
}

function addTurnNewsHeal(cause_row, heal_row, target, amount) {
    if (!cause_row || !target || !(amount > 0)) {
        return null;
    }
    if (!cause_row.new_action) {
        cause_row.new_action = [];
    }
    var action = {action:"heal", warlock_name:target.owner,
                  object_type:target.type, object:target.name, amount:amount};
    cause_row.new_action.push(action);

    if (heal_row !== cause_row) {
        var actions = heal_row.new_action || [];
        for (var i = 0, Ln = actions.length; i < Ln; ++i) {
            var a = actions[i];
            if ((a.action !== "highlight") || ((a.object_type === "warlock") && (a.object !== "hp")) ||
                    ((a.object_type === "monster") && (a.role !== "target"))) {
                continue;
            }
            if (!hasTurnNewsTargetAction(cause_row, a)) {
                cause_row.new_action.push(a);
            }
        }
        heal_row.turn_news_hidden = true;
    }

    cause_row.turn_news_has_heal = true;
    return action;
}

function findTurnNewsHealCause(target, causes) {
    if (!target) {
        return null;
    }
    var target_name = normalizeTurnNewsName(target);
    for (var i = 0, Ln = causes.length; i < Ln; ++i) {
        if (causes[i].claimed) {
            continue;
        }
        if (normalizeTurnNewsName(causes[i].target) === target_name) {
            causes[i].claimed = true;
            return causes[i];
        }
    }
    return null;
}

// Distribute the healed points over this turn's cure spells. Without the
// previous battle (opening an old game) fall back to the spell's nominal value.
// The distribution lets a cure at full health show no +number and still starts
// the life-point card at the value the previous turn ended with.
function prepareTurnNewsHp(previous_battle, damage_totals, heal_pairings) {
    var i, Ln;

    // A read-only battle has no turn news. A snapshot replay already shows the
    // board at the start of the turn, so its cards start from the live units.
    // The old reconstruction is only for a live board that already shows the
    // resolved turn.
    if (battle.read_only || !isPreviousTurnBattle(previous_battle)) {
        return;
    }

    var by_target = {}, pairing, target, key;

    for (i = 0, Ln = heal_pairings.length; i < Ln; ++i) {
        pairing = heal_pairings[i];
        key = pairing.target.type + ":" + pairing.target.name;
        if (!by_target[key]) {
            by_target[key] = {target:pairing.target, entries:[]};
        }
        by_target[key].entries.push(pairing);
    }

    for (key in by_target) {
        var group = by_target[key];
        target = group.target;
        var hp_object = findTurnNewsTargetObject(target);
        if (!hp_object) {
            continue;
        }
        var final_hp = parseInt(hp_object.hp, 10);
        if (isNaN(final_hp)) {
            final_hp = 0;
        }
        var previous_hp = getPreviousTurnTargetHp(previous_battle, target);
        var damage_total = damage_totals[key] || 0;
        var nominal = 0, entries = group.entries;
        for (i = 0, Ln = entries.length; i < Ln; ++i) {
            nominal += entries[i].nominal;
        }

        var actual = nominal;
        if (previous_hp !== null) {
            actual = final_hp - previous_hp + damage_total;
            if (actual < 0) {
                actual = 0;
            }
        }

        var remaining = actual;
        for (i = 0, Ln = entries.length; i < Ln; ++i) {
            var healed = Math.min(entries[i].nominal, remaining);
            entries[i].action.amount = healed;
            remaining -= healed;
        }
        // Anything left over belongs to the last cure of this target.
        if (remaining > 0) {
            entries[entries.length - 1].action.amount += remaining;
        }
    }

    // Set every life-point card's first value. Warlocks and monsters that had no
    // damage or healing this turn simply keep the value from the previous turn.
    for (i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        if (!w) {
            continue;
        }
        target = {type:"warlock", name:w.name};
        var w_start = getPreviousTurnTargetHp(previous_battle, target);
        if (w_start === null) {
            key = "warlock:" + w.name;
            w_start = parseInt(w.hp, 10) + (damage_totals[key] || 0);
            var w_group = by_target[key];
            if (w_group) {
                var w_healed = 0;
                for (var j = 0, LnJ = w_group.entries.length; j < LnJ; ++j) {
                    w_healed += w_group.entries[j].action.amount || 0;
                }
                w_start -= w_healed;
            }
        }
        if (isNaN(w_start) || (w_start < 0)) {
            w_start = 0;
        }
        w.hp = w_start;

        var monsters = w.monsters || [];
        for (var k = 0, LnK = monsters.length; k < LnK; ++k) {
            var m = monsters[k];
            target = {type:"monster", name:m.name};
            var m_start = getPreviousTurnTargetHp(previous_battle, target);
            if (m_start === null) {
                key = "monster:" + m.name;
                m_start = parseInt(m.hp, 10) + (damage_totals[key] || 0);
                var m_group = by_target[key];
                if (m_group) {
                    var m_healed = 0;
                    for (var q = 0, LnQ = m_group.entries.length; q < LnQ; ++q) {
                        m_healed += m_group.entries[q].action.amount || 0;
                    }
                    m_start -= m_healed;
                }
            }
            if (isNaN(m_start) || (m_start < 0)) {
                m_start = 0;
            }
            m.hp = m_start;
            m.text = m.hp;
        }
    }
}

function normalizeTurnNewsName(name) {
    return (name || "").replace(/[\s.!;,]+$/, "");
}

function getMonsterOwnerName(name) {
    if (!name) {
        return "";
    }
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        var w = battle.warlocks[i];
        if (!w) {
            continue;
        }
        var monsters = w.monsters || [];
        for (var j = 0, LnJ = monsters.length; j < LnJ; ++j) {
            if (monsters[j].name === name) {
                return w.name;
            }
        }
    }
    var owners = battle.news_summons && battle.news_summons.arrivals;
    if (owners) {
        for (var k = 0, LnK = owners.length; k < LnK; ++k) {
            if (owners[k].name === name) {
                return owners[k].owner;
            }
        }
    }
    return "";
}

// A death removes the monster from every roster and index.
function removeDeadMonsterTarget(name) {
    if (!name || !battle) {
        return;
    }

    if (battle.monsters) {
        for (var owner in battle.monsters) {
            var monsters = battle.monsters[owner] || [];
            for (var i = monsters.length - 1; i >= 0; --i) {
                if (monsters[i] && (monsters[i].name === name)) {
                    monsters.splice(i, 1);
                }
            }
        }
    }
    delete map_monster_name_to_idx[name];

    var id = battle.targetsMap ? battle.targetsMap[name] : "";
    if (battle.targetsList) {
        var idx = battle.targetsList.indexOf(name);
        if (idx !== -1) {
            battle.targetsList.splice(idx, 1);
        }
    }
    if (battle.targetsMap) {
        delete battle.targetsMap[name];
        if (id && (battle.targetsMap[id] === name)) {
            delete battle.targetsMap[id];
        }
    }

    if (battle.actions && battle.actions.M) {
        for (var k = battle.actions.M.length - 1; k >= 0; --k) {
            if (battle.actions.M[k] && (battle.actions.M[k].name === name)) {
                battle.actions.M.splice(k, 1);
            }
        }
        for (var n = 0; n < battle.actions.M.length; ++n) {
            var action = battle.actions.M[n];
            if (!action) {
                continue;
            }
            action.action_idx = n;
            var live_monster = battle.monsters && action.owner ? battle.monsters[action.owner] : null;
            if (!live_monster) {
                continue;
            }
            for (var q = 0; q < live_monster.length; ++q) {
                if (live_monster[q] && (live_monster[q].name === action.name)) {
                    live_monster[q].action_idx = n;
                    break;
                }
            }
        }
    }
}

function getTurnNewsTarget(row) {
    var actions = row.new_action || [];
    for (var i = 0, Ln = actions.length; i < Ln; ++i) {
        var a = actions[i];
        if (a.action !== "highlight") {
            continue;
        }
        if ((a.object_type === "warlock") && (a.object === "hp")) {
            return {type:"warlock", name:a.warlock_name, owner:a.warlock_name};
        }
        if ((a.object_type === "monster") && (a.role === "target")) {
            var owner = getMonsterOwnerName(a.object);
            if (owner) {
                return {type:"monster", name:a.object, owner:owner};
            }
        }
    }

    // Rows whose target is no longer represented by a highlight (for example a
    // monster that died during the turn) can still name a Warlock target.
    var target = normalizeTurnNewsName(row.obj ? row.obj.target : "");
    if (target) {
        var m_owner = getMonsterOwnerName(target);
        if (m_owner) {
            return {type:"monster", name:target, owner:m_owner};
        }
        for (var j = 0, LnJ = battle.warlocks.length; j < LnJ; ++j) {
            var w = battle.warlocks[j];
            if (w && (w.name === target)) {
                return {type:"warlock", name:target, owner:target};
            }
        }
    }
    return null;
}

function getTurnNewsElementalCause(row) {
    var match = /(Fire|Ice)\s+Elemental\s+runs\s+amok/i.exec(row.txt || "");
    return match ? match[1].toLowerCase() : "";
}

function getTurnNewsElementalDamage(row) {
    var txt = (row.txt || "").toLowerCase();
    if ((txt.indexOf("burnt") !== -1) || (txt.indexOf("burned") !== -1)) {
        return "fire";
    }
    if ((txt.indexOf("frozen") !== -1) || (txt.indexOf("chilled") !== -1)) {
        return "ice";
    }
    return "";
}

// The server's reflection row has the spell name in the text. parseStrangeAttack
// keeps it in obj.mirror_spell; this fallback covers the "reflects" wording too.
function findTurnNewsSpellName(txt) {
    var best = "";
    for (var name in BGU.map_spell_name_to_gesture) {
        if ((name === "Magic Mirror") || (name === "Stab")) {
            continue;
        }
        if ((txt.indexOf(name) !== -1) && (name.length > best.length)) {
            best = name;
        }
    }
    return best;
}

function getTurnNewsReflection(row) {
    if (!row.obj) {
        return null;
    }
    if (!row.obj.mirror && !/reflected|reflects/i.test(row.txt || "")) {
        return null;
    }
    var spell = row.obj.mirror_spell || findTurnNewsSpellName(row.txt || "");
    var target = getTurnNewsTarget(row);
    var target_name = target ? target.name : (row.obj.target || "");
    if (!spell || !target_name) {
        return null;
    }
    return {spell:spell, target:target_name};
}

function isTurnNewsStab(row) {
    return row.obj && ((row.obj.spell === "Stab") || / stab/i.test(row.txt || ""));
}

// Poison and disease counters are turn news, but they are not one of the hit
// events that get a floating damage number.
function isTurnNewsCounterDamage(row) {
    var txt = (row.txt || "").toLowerCase();
    return ((row.obj && ((row.obj.spell === "Poison") || (row.obj.spell === "Disease"))) ||
            /\bis (poisoned|diseased)\b/.test(txt));
}

function getTurnNewsMonsterAttacker(row) {
    var candidates = [];
    if (row.obj && row.obj.aggressor) {
        candidates.push(row.obj.aggressor);
    }
    if (row.obj && row.obj.spell) {
        candidates.push(row.obj.spell);
    }
    for (var i = 0, Ln = candidates.length; i < Ln; ++i) {
        if (getMonsterOwnerName(candidates[i])) {
            return candidates[i];
        }
    }

    // "Bob is hit by Smelly Goblin for 1 damage" has no separate attack row.
    // Only accept the monster after "hit by"; before it is the victim, not the attacker.
    var txt = row.txt || "";
    var hit_by = txt.indexOf("hit by");
    if (hit_by !== -1) {
        for (var j = 0, LnJ = battle.warlocks.length; j < LnJ; ++j) {
            var w = battle.warlocks[j];
            if (!w) {
                continue;
            }
            var monsters = w.monsters || [];
            for (var k = 0, LnK = monsters.length; k < LnK; ++k) {
                var pos = txt.indexOf(monsters[k].name);
                if ((pos !== -1) && (pos > hit_by)) {
                    return monsters[k].name;
                }
            }
        }
    }
    return "";
}

function addTurnNewsMonsterAttackIcon(row, monster_name) {
    if (!monster_name) {
        monster_name = getTurnNewsMonsterAttacker(row);
    }
    if (!monster_name || !getMonsterOwnerName(monster_name)) {
        return;
    }
    if (!row.new_action) {
        row.new_action = [];
    }
    var actions = row.new_action;
    for (var i = 0, Ln = actions.length; i < Ln; ++i) {
        var a = actions[i];
        if ((a.action === "highlight") && (a.object_type === "monster") &&
                (a.object === monster_name) && (a.role === "actor")) {
            a.news_icon = "stab";
            return;
        }
    }
    row.new_action.push({action:"highlight", warlock_name:monster_name, object_type:"monster",
                         object:monster_name, data:[], role:"actor", color:"#10C9F5", news_icon:"stab"});
}

function hasTurnNewsTargetAction(row, target_action) {
    var actions = row.new_action || [];
    for (var i = 0, Ln = actions.length; i < Ln; ++i) {
        var a = actions[i];
        if ((a.action === "highlight") && (a.object_type === target_action.object_type) &&
                (a.object === target_action.object) && (a.warlock_name === target_action.warlock_name) &&
                (a.role === target_action.role)) {
            return true;
        }
    }
    return false;
}

function removeTurnNewsHitIcon(row) {
    var actions = row.new_action || [], res = [];
    var is_attack = (row.row_type === "attack");
    for (var i = 0, Ln = actions.length; i < Ln; ++i) {
        var a = actions[i];
        if (a.action === "icon") {
            // A stab/monster attack row carries its whole message; the new
            // monster marker and floating number replace its old banner icon.
            if (is_attack) {
                continue;
            }
            var icon = a.large_icon || "";
            if ((icon === "heart") || (icon.indexOf("damage") !== -1) ||
                    ((a.text || "").indexOf("- ") === 0)) {
                continue;
            }
        }
        res.push(a);
    }
    row.new_action = res;
}

function addTurnNewsDamage(cause, damage_row, target, amount) {
    if (!cause || !target || !(amount > 0)) {
        return false;
    }
    if (!cause.new_action) {
        cause.new_action = [];
    }

    cause.new_action.push({action:"damage", warlock_name:target.owner,
                           object_type:target.type, object:target.name, amount:amount});

    // Move the victim's highlight from the effect row to the cause row. This is
    // what lights up every target of Fire Storm or a running elemental.
    if (cause !== damage_row) {
        var actions = damage_row.new_action || [];
        for (var i = 0, Ln = actions.length; i < Ln; ++i) {
            var a = actions[i];
            if ((a.action !== "highlight") || ((a.object_type === "warlock") && (a.object !== "hp")) ||
                    ((a.object_type === "monster") && (a.role !== "target"))) {
                continue;
            }
            if (!hasTurnNewsTargetAction(cause, a)) {
                cause.new_action.push(a);
            }
        }
    }

    cause.turn_news_has_damage = true;
    removeTurnNewsHitIcon(cause);
    if (cause !== damage_row) {
        damage_row.turn_news_hidden = true;
    }
    return true;
}

function getTurnNewsSpellAliases(spell) {
    if ((spell === "Cause Light Wounds") || (spell === "Cause Heavy Wounds")) {
        return {"Cause Light Wounds":true, "Cause Heavy Wounds":true};
    }
    return null;
}

function findTurnNewsSpellCause(spell, target, causes) {
    var aliases = getTurnNewsSpellAliases(spell);
    var storm = (spell === "Fire Storm") || (spell === "Ice Storm");
    for (var i = 0, Ln = causes.length; i < Ln; ++i) {
        var cause = causes[i];
        if (cause.claimed && !storm) {
            continue;
        }
        if (aliases) {
            if (!aliases[cause.spell]) {
                continue;
            }
        } else if (cause.spell !== spell) {
            continue;
        }
        if (!storm && cause.target && target && (cause.target !== target)) {
            continue;
        }
        if (!storm) {
            cause.claimed = true;
        }
        return cause;
    }
    return null;
}

function findTurnNewsReflectionCause(spell, target, causes) {
    var aliases = getTurnNewsSpellAliases(spell);
    var fallback = null, fallback_count = 0;
    for (var i = 0, Ln = causes.length; i < Ln; ++i) {
        var cause = causes[i];
        if (cause.claimed) {
            continue;
        }
        if (aliases ? !aliases[cause.spell] : (cause.spell !== spell)) {
            continue;
        }
        // The reflection row names the mirror owner; the damage row names the
        // original caster. damage_target was resolved from the original cast.
        if (cause.damage_target) {
            if (target === cause.damage_target) {
                cause.claimed = true;
                return cause;
            }
            continue;
        }
        if (target && (cause.target === target)) {
            cause.claimed = true;
            return cause;
        }
        if (!fallback) {
            fallback = cause;
        }
        ++fallback_count;
    }
    if ((fallback_count === 1) && fallback) {
        fallback.claimed = true;
        return fallback;
    }
    return null;
}

function findTurnNewsAttackCause(target, causes) {
    for (var i = 0, Ln = causes.length; i < Ln; ++i) {
        var cause = causes[i];
        if (cause.claimed || !cause.row.obj) {
            continue;
        }
        var cause_target = normalizeTurnNewsName(cause.row.obj.target);
        var target_name = normalizeTurnNewsName(target);
        if (cause_target && target_name && (cause_target === target_name)) {
            cause.claimed = true;
            return cause;
        }
    }
    return null;
}

function findTurnNewsElementalCause(element, causes) {
    for (var i = 0, Ln = causes.length; i < Ln; ++i) {
        if (causes[i].type === element) {
            return causes[i];
        }
    }
    return null;
}

function prepareTurnNewsDamage(real_actions, previous_battle) {
    var spell_causes = [], attack_causes = [], reflection_causes = [], elemental_causes = [];
    var heal_causes = [], heal_pairings = [], damage_totals = {};
    var reflection_rows = [];
    var i, Ln, row, cause;

    for (i = 0, Ln = real_actions.length; i < Ln; ++i) {
        row = real_actions[i];
        row.turn_news_hidden = false;
        row.turn_news_has_damage = false;
        if (row.type >= 2) {
            continue;
        }

        var element = getTurnNewsElementalCause(row);
        if (element) {
            elemental_causes.push({row:row, type:element});
        }

        var reflection = getTurnNewsReflection(row);
        if (reflection) {
            reflection_rows.push({row:row, spell:reflection.spell, target:reflection.target});
        }

        if ((row.row_type === "spell") && row.obj && !row.obj.fail) {
            spell_causes.push({row:row, spell:row.obj.spell, target:row.obj.target, claimed:false});
            if ((row.obj.spell === "Cure Light Wounds") || (row.obj.spell === "Cure Heavy Wounds")) {
                heal_causes.push({row:row, spell:row.obj.spell, target:row.obj.target, claimed:false});
            }
        }

        if (row.row_type === "attack") {
            attack_causes.push({row:row, claimed:false});
            addTurnNewsMonsterAttackIcon(row);
        }

        var row_damage = getTurnNewsDamage(row);
        if (row_damage > 0) {
            var row_target = getTurnNewsTarget(row);
            addTurnNewsDamageTotal(damage_totals, row_target, row_damage);
        }
    }

    // Resolve the spell's original caster so reflected damage can be matched to
    // the mirror row even though the two rows name different people.
    for (i = 0, Ln = reflection_rows.length; i < Ln; ++i) {
        var reflection_row = reflection_rows[i];
        var reflection_aliases = getTurnNewsSpellAliases(reflection_row.spell);
        var damage_target = "";
        for (var j = 0, LnJ = spell_causes.length; j < LnJ; ++j) {
            var spell_cause = spell_causes[j];
            if (reflection_aliases ? !reflection_aliases[spell_cause.spell] : (spell_cause.spell !== reflection_row.spell)) {
                continue;
            }
            if (normalizeTurnNewsName(spell_cause.target) !== normalizeTurnNewsName(reflection_row.target)) {
                continue;
            }
            damage_target = spell_cause.row.obj ? (spell_cause.row.obj.warlock || "") : "";
            break;
        }
        reflection_causes.push({row:reflection_row.row, spell:reflection_row.spell,
                                target:reflection_row.target, damage_target:damage_target, claimed:false});
    }

    for (i = 0, Ln = real_actions.length; i < Ln; ++i) {
        row = real_actions[i];
        if (row.type >= 2) {
            continue;
        }

        if (isTurnNewsHeal(row)) {
            target = getTurnNewsTarget(row);
            if (!target) {
                console.log("prepareTurnNewsHeal: no target for", row.txt, JSON.stringify(row.obj));
                continue;
            }
            cause = findTurnNewsHealCause(target.name, heal_causes);
            if (cause) {
                var heal_amount = getTurnNewsHealAmount(cause.spell);
                var heal_action = addTurnNewsHeal(cause.row, row, target, heal_amount);
                if (heal_action) {
                    heal_pairings.push({target:target, nominal:heal_amount, action:heal_action});
                }
                continue;
            }
            console.log("prepareTurnNewsHeal: no cause for", row.txt, JSON.stringify(row.obj));
            continue;
        }

        var damage = getTurnNewsDamage(row);
        if (!(damage > 0)) {
            continue;
        }
        var target = getTurnNewsTarget(row);
        if (!target) {
            console.log("prepareTurnNewsDamage: no target for", row.txt, JSON.stringify(row.obj));
            continue;
        }

        // Attack rows (stabs and monster attacks) carry their own cause.
        if (row.row_type === "attack") {
            addTurnNewsDamage(row, row, target, damage);
            for (var j = 0, LnJ = attack_causes.length; j < LnJ; ++j) {
                if (attack_causes[j].row === row) {
                    attack_causes[j].claimed = true;
                    break;
                }
            }
            continue;
        }

        if (isTurnNewsCounterDamage(row)) {
            addTurnNewsHpAction(row, target, -damage);
            continue;
        }

        // A reflection row may itself contain the reflected damage.
        var self_reflection = getTurnNewsReflection(row);
        if (self_reflection) {
            addTurnNewsDamage(row, row, target, damage);
            continue;
        }

        // parseStrangeAttack can stop at the first lower-case word ("an Ice
        // Storm", "Clap of Lightning"), so prefer the full name found in the text.
        var damage_spell = findTurnNewsSpellName(row.txt || "");
        if (!damage_spell && row.obj && row.obj.spell) {
            damage_spell = row.obj.spell;
        }

        // Otherwise the damage row may still belong to a nearby reflection row.
        if (damage_spell) {
            cause = findTurnNewsReflectionCause(damage_spell, target.name, reflection_causes);
            if (cause) {
                addTurnNewsDamage(cause.row, row, target, damage);
                continue;
            }
        }

        if (damage_spell) {
            cause = findTurnNewsSpellCause(damage_spell, target.name, spell_causes);
            if (cause) {
                addTurnNewsDamage(cause.row, row, target, damage);
                continue;
            }
        }

        if (isTurnNewsStab(row)) {
            cause = findTurnNewsAttackCause(target.name, attack_causes);
            if (cause) {
                addTurnNewsDamage(cause.row, row, target, damage);
                continue;
            }
        }

        var element_damage = getTurnNewsElementalDamage(row);
        if (element_damage) {
            cause = findTurnNewsElementalCause(element_damage, elemental_causes);
            if (cause) {
                addTurnNewsDamage(cause.row, row, target, damage);
                continue;
            }
        }

        // A monster attack without a separate attack row ("Bob is hit by a Goblin").
        var monster_attacker = getTurnNewsMonsterAttacker(row);
        if (monster_attacker) {
            addTurnNewsMonsterAttackIcon(row, monster_attacker);
            addTurnNewsDamage(row, row, target, damage);
            continue;
        }

        console.log("prepareTurnNewsDamage: no cause for", row.txt, JSON.stringify(row.obj));
        addTurnNewsHpAction(row, target, -damage);
    }

    prepareTurnNewsHp(previous_battle, damage_totals, heal_pairings);
}

function prepareTurnActionInfo(last_turn_hist, previous_battle) {
    console.log("battle_utils.prepareTurnActionInfo", JSON.stringify(last_turn_hist), JSON.stringify(battle.hint));
    var new_hint = [], i, Ln;
    for(i = 0, Ln = battle.hint.length; i < Ln; ++i) {
        var action = [];
        if (battle.hint[i].icon) {
            action.push({action:"icon",large_icon:battle.hint[i].icon,small_icon:"",title:"",text:"",background_color:"#10C9F5",border_color:"#210430"});
        }
        new_hint.push({txt:battle.hint[i].txt,color_bg:"#FEE2D6",actions:action});
    }

    var real_actions = [], new_action;
    for(i = 0, Ln = last_turn_hist.length; i < Ln; ++i) {
        new_action = [];
        if ((last_turn_hist[i].type === 0)) {
            // chat message
            if (battle.with_bot) {
                continue;
            }
            new_action.push({action:"icon",large_icon:"chat",small_icon:"",title:"",text:"",background_color:"#210430",border_color:"#E7FFFF"});
        } else if (last_turn_hist[i].type === 2) {
            real_actions.push(last_turn_hist[i]);
            continue;
        } else if (last_turn_hist[i].type === 1) {
            real_actions.push(last_turn_hist[i]);
            continue;
            /*new_action = BGU.getMessageActionByRow(last_turn_hist[i], battle); // before change color
            if ((last_turn_hist[i].color === "#FF6666") || (last_turn_hist[i].color === "#FF8888")) {
                last_turn_hist[i].color = "#FEE2D6";
            } else {
                last_turn_hist[i].color = "#10C9F5";
            }*/
        }
        if (!last_turn_hist[i].font_size) {
            last_turn_hist[i].font_size = 21;
        }

        new_hint.push({color_bg:last_turn_hist[i].color,font_size:last_turn_hist[i].font_size,txt:BGU.replaceAll(last_turn_hist[i].txt, '&quot;', '"'),actions:new_action});
    }

    // Pair the summon rows once; the event playback mutates the live board.
    var news_summons = BGU.prepareNewsMonsters(real_actions, battle);
    BGU.prepareAndSortRealAction(real_actions, battle);
    prepareTurnNewsDamage(real_actions, previous_battle);
    prepareTurnNewsTransitions(real_actions, previous_battle, news_summons);
    var beginner = mainWindow.playerSpellbookLevel < 5;

    for (i = 0, Ln = real_actions.length; i < Ln; ++i) {
        //new_action = BGU.getMessageActionByRow(real_actions[i], battle); // before change color
        if (real_actions[i].turn_news_hidden) {
            continue;
        }
        if (real_actions[i].type >= 2) {
            continue;
        }
        if (beginner && !isBeginnerTurnNews(real_actions[i]) &&
                !real_actions[i].turn_news_has_damage && !real_actions[i].turn_news_force_show) {
            continue;
        }
        // https://github.com/Pz1c/WavingHands/issues/311
        BGU.processHintText(real_actions[i]);

        if ((real_actions[i].color === "#FF6666") || (real_actions[i].color === "#FF8888")) {
            real_actions[i].color = "#FEE2D6";
        } else {
            real_actions[i].color = "#10C9F5";
        }
        if (!real_actions[i].font_size) {
            real_actions[i].font_size = 21;
        }

        //BGU.replaceAll(real_actions[i].txt, '&quot;', '"')
        var silent_death = (real_actions[i].row_type === "death");
        var row_actions = real_actions[i].new_action;
        var status_remove = BGU.getStatusRemovalAction(real_actions[i]);
        if (status_remove) {
            row_actions = row_actions.concat([status_remove]);
        }
        new_hint.push({color_bg:real_actions[i].color,font_size:real_actions[i].font_size,txt:real_actions[i].txt,
                       turn_news_silent:silent_death,
                       actions:row_actions});
    }

    battle.hint = new_hint;
}

function prepareMonster(m) {
    m.is_elemental = !m.owner || (m.owner === "Nobody");
    m.allow_choose_target = false;
    if (m.is_elemental) {
        battle.elemental = m;
        battle.elemental.type = m.name.indexOf("Fire") !== -1 ? "fire" : "ice";
        battle.elemental.action = "m";
        return false;
    }

    if ((m.name.indexOf(":") !== -1) && (m.strength > 0)) {
        m.icon = "new_" + BGU.getMonsterIconByName(BGU.getMonsterNameByStrength(m.strength));
        if (m.icon.indexOf("elemental") !== -1) {
            m.icon = "new_summon";
        }
    } else {
        m.icon = BGU.getMonsterIconByName(m.name);
    }

    m.action = "m";
    m.action_idx = battle.actions.M.length;

    if (!battle.monsters[m.owner]) {
        battle.monsters[m.owner] = [];
    }

    m.damage = BGU.getMonsterDamageByName(m.name);

    m.enchantment_icon = "";
    if (m.status.indexOf("-") !== -1) {
        var status = m.status.toLowerCase();
        m.enchantment = "";
        m.enchantment_icon = "qrc:/res/target_default.png";
        for (var i = 0, Ln = BGU.icon_status_code.length; i < Ln; ++i) {
            if (status.indexOf(BGU.icon_status_code[i]) !== -1) {
                if (m.enchantment !== "") {
                    m.enchantment = "many";
                    break;
                } else {
                    m.enchantment = BGU.icon_status_code[i];
                }
            }
        }

        if (m.enchantment !== "many") {
            var e_code = BGU.icon_status_code_to_icon[m.enchantment];
            if (!e_code) {
                e_code = m.enchantment;
            }
            var tmp_spell = BGU.icon_status_spell[e_code];
            if (tmp_spell) {
                var tmp_icon = BGU.map_spell_to_icon[tmp_spell];
                if (tmp_icon) {
                    m.enchantment_icon = "qrc:/res/"+tmp_icon+".png";
                }
            }
        }
    }

    return true;
}

function prepareBattle(raw_battle) {
    // The live board is the snapshot. The current page supplies history/order state.
    battle = {id:raw_battle.id,size:0,fire:raw_battle.fire,chat:raw_battle.chat,is_fdf:raw_battle.is_fdf,is_fc:raw_battle.is_fc,warlocks:[],elemental:{hp:0,type:"fire"},
        monsters:{},ngL:"",ngR:"",turn_num: raw_battle.turn_num,hint: raw_battle.hint, msg: raw_battle.msg, battle_hist: BGU.replaceAll(raw_battle.battle_hist, "&quot;", '"'),
        page_plg: raw_battle.plg, page_prg: raw_battle.prg,
        battle_chat: raw_battle.battle_chat, with_bot: raw_battle.with_bot, read_only: raw_battle.read_only, maladroit: raw_battle.maladroit,
        // https://github.com/Pz1c/WavingHands/issues/315: true while the site offers its SUICIDE checkbox.
        // battleInfo() sends 1/0; a battle prepared once already holds the bool. Note "0" is truthy in JS.
        suicide: (raw_battle.suicide === 1) || (raw_battle.suicide === "1") || (raw_battle.suicide === true)};
    // L left  obj
    // R Right obj
    // C Chat  text
    // D Delay int 0 - none, 1 left, 2 right, -1 unavailable
    // P Permanent int 0 - none, 1 left, 2 right, -1 unavailable
    // F Fire int 0, 1, -1
    // M monsters arr of obj
    // CP - paralyze arr of obj by id
    // CC - paralyze arr of obj by id
    // S Suicide int 0 - no, 1 - tick the site's SUICIDE checkbox (only offered while battle.suicide)
    battle.actions = {L:{target:"Default"},R:{target:"Default"},C:raw_battle.msg,D:-1,P:-1,F:-1,S:0,M:[],CP:{},CC:{}};
    setParaActions(raw_battle.paralyze, raw_battle.charm);
    parseTargets(raw_battle.targets);
    parseParalyzedHands(raw_battle.paralyzed_hand);

    var i, Ln;
    for (i = 0, Ln = raw_battle.monsters.length; i < Ln; ++i) {
        var m = raw_battle.monsters[i];
        m.read_only = battle.read_only;
        console.log("JS.prepareBattle.monsters", JSON.stringify(m));
        if (prepareMonster(m)) {
            battle.monsters[m.owner].push(m);
            battle.actions.M.push({id:battle.targetsMap[m.name],target:m.new_target,old_target:m.target,
                                   under_control:true,owner:m.owner,name:m.name,
                                   status:m.status,d:m.damage,hp:m.hp,action:"m",strength:m.damage,icon:m.icon});
        }
    }

    for (i = 0, Ln = raw_battle.warlocks.length; i < Ln; ++i) {
        var w = raw_battle.warlocks[i];
        if (!battle.monsters[w.name]) {
            battle.monsters[w.name] = [];
        }
        prepareWarlock(w);
        battle.warlocks.unshift(w);
        ++battle.size;
    }
    battle.actions.L.target = battle.player_target_L;
    battle.actions.L.g = battle.player_gesture_L;
    battle.actions.L.s = {n:battle.player_spell_L, gp: "", g:battle.player_gesture_L};
    battle.actions.R.target = battle.player_target_R;
    battle.actions.R.g = battle.player_gesture_R;
    battle.actions.R.s = {n:battle.player_spell_R, gp: "", g:battle.player_gesture_R};
    battle.player_name = battle.warlocks[0].name;
    battle.player_changed_mind = battle.warlocks[0].changed_mind;
    battle.enemy_name = battle.warlocks.length > 1 ? battle.warlocks[1].name : "Nobody";
    for (i = 0, Ln = battle.actions.M.length; i < Ln; ++i) {
        if (!battle.actions.M[i].target  || (battle.actions.M[i].target === "Nobody")) {
            battle.actions.M[i].target = battle.enemy_name;
            battle.actions.M[i].old_target = battle.enemy_name;
        }
        if (battle.actions.M[i].owner !== battle.player_name) {
            battle.actions.M[i].under_control = false;
        }
    }
    //delete battle.monsters;

    prepareTurnActionInfo(raw_battle.last_turn_hist, null);

    console.log("prepared battle", JSON.stringify(battle));
}

function prepareElemental() {
    var el = (battle.turn_news_elemental !== undefined) ? battle.turn_news_elemental : battle.elemental;
    if (el && (parseInt(el.hp, 10) > 0)) {
        iiElemental.text = el.hp;
        iiElemental.source = "qrc:/res/elemental_"+el.type+".png";
        iiElemental.l_data = el;
        iiElemental.visible = true;
    } else {
        iiElemental.visible = false;
    }
}

function showTurnNewsElemental(type, hp) {
    var shown = parseInt(hp, 10);
    if (isNaN(shown) || (shown <= 0)) {
        shown = 3;
    }
    battle.turn_news_elemental = {name:(type === "ice") ? "Ice Elemental" : "Fire Elemental",
                                  hp:shown, type:(type === "ice") ? "ice" : "fire", action:"m"};
    prepareElemental();
}

function hideTurnNewsElemental() {
    battle.turn_news_elemental = null;
    prepareElemental();
}

function resetTurnNewsElemental() {
    battle.turn_news_elemental = undefined;
    prepareElemental();
}

function prepareWarlocks() {
    console.log("iWarlocks");
    BGU.cleanChildren(iWarlocks);

    if (!cIconObject) {
        cIconObject = Qt.createComponent("qrc:///qml/components/IconInfo.qml");
    }
    if (!cWarlockObject) {
        cWarlockObject = Qt.createComponent("qrc:///qml/game_components/Warlock.qml");
    }
    if (cWarlockObject.status === QQ.Component.Error) {
        console.log("Error loading component: " + cWarlockObject.errorString());
        return ;
    }
    if (cWarlockObject.status === QQ.Component.Ready) {
        console.log("component ready");
        finishPrepareWarlockList();
    } else {
        console.log("connect to component ready signal");
        cWarlockObject.statusChanged.connect(finishPrepareWarlockList);
    }
}

function finishPrepareWarlockList() {
    console.log("finishPrepareWarlockList", battleWindow.height, battleWindow.width, battle.turn_num);
    if (cWarlockObject.status === QQ.Component.Error || cWarlockObject.status !== QQ.Component.Ready) {
        console.log("Error loading component:", cWarlockObject.errorString());
        return ;
    }
    var curr_y = 0;
    var total_height = 0;
    map_warlock_name_to_idx = {};
    map_monster_name_to_idx = {};
    for(var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        // The snapshot unit section is the turn-start board, so a Warlock who
        // dies during the news is still present here and can show his RIP card.
        var start_hp = battle.warlocks[i] ? parseInt(battle.warlocks[i].hp, 10) : 0;
        if (!battle.warlocks[i] || (!battle.read_only && !battle.warlocks[i].active &&
                (battle.monsters[battle.warlocks[i].name].length === 0) && !(start_hp > 0))) {
            continue;
        }
        var arr_m = battle.warlocks[i];
        arr_m.warlock_idx = i;
        map_warlock_name_to_idx[arr_m.name] = i;
        for (var j = 0, LnJ = arr_m.monsters.length; j < LnJ; ++j) {
            map_monster_name_to_idx[arr_m.monsters[j].name] = i;
        }
        //arr_m.turn_num = battle.turn_num;
        var sprite = cWarlockObject.createObject(iWarlocks, {l_warlock: arr_m, l_ratio: mainWindow.ratioObject, l_IconInfoObj: cIconObject, x: 0, y: curr_y, height: G_WARLOCK_HEIGHT * mainWindow.ratioObject, width: battleWindow.width});
        if (sprite === null) {
            console.log("Error creating object");
            continue;
        }
        sprite.clicked.connect(iconClick);
        sprite.doubleClicked.connect(iconDoubleClick);
        console.log("looks like created x: " + sprite.x + " y: " + sprite.y + " h: " + sprite.height+ " w: " + sprite.width);
        console.log("svWarlocks.w: " + iWarlocks.width);
        console.log("svWarlocks.h: " + iWarlocks.height);
        curr_y += sprite.height;
        total_height += sprite.height;
    }
    iWarlocks.height = total_height;
}

function prepareChat() {
    var with_msg = battle.chat > 0;
    console.log("prepareChat", with_msg, battle.chat);
    if (with_msg) {
        iiChat.source = "qrc:/res/chat_pimple.png";
    }
}

function prepareHint() {
    // A finished battle opened for viewing has nothing to click through: no turn news.
    if (battle.read_only) {
        return;
    }
    if (battle.hint && battle.hint.length > 0) {
        ltTutorial.tutorialData = battle.hint;
        ltTutorial.tutorialDataIdx = -1;
        playNextTurnNewsStep(1, true);
    }
}

function prepareGUI() {
    prepareElemental();
    prepareWarlocks();
    prepareChat();
    // need wait before warlocks finish creating
    //prepareHint();
    bbSendOrders.text = battle.read_only ? "Back" : "Submit";
}

function applyBattle() {
    var raw_battle = mainWindow.gBattle;
    prepareBattle(raw_battle);
    mainWindow.gBattle = battle;
    prepareGUI();
}

function prepareOrder() {
    console.log("prepareOrder", "before", JSON.stringify(battle.actions));
    var actions = battle.actions;
    var post_request = "say$";
    if (actions.C !== "") {
        post_request += actions.C.replace("$", "sign:dollar").replace("#", "sign:pos_number");
    }
    post_request += "#";
    console.log("prepareOrder", "point1", post_request);
    // gesture
    post_request += "LH$"+actions.L.g+"#";
    post_request += "RH$"+actions.R.g+"#";
    console.log("prepareOrder", "point2", post_request);
    post_request += "LHS$"+BGU.getSpellNameForOrder(actions.L)+"#";
    post_request += "RHS$"+BGU.getSpellNameForOrder(actions.R)+"#";
    console.log("prepareOrder", "point3", post_request);
    post_request += "LHT$"+BGU.getSpellTargetForOrder(actions.L, battle.targetsMap)+"#";
    post_request += "RHT$"+BGU.getSpellTargetForOrder(actions.R, battle.targetsMap)+"#";
    console.log("prepareOrder", "point4", post_request, JSON.stringify(battle.targetsMap));

    var i, Ln, m_obj, pc_gv;
    for(i = 0, Ln = actions.M.length; i < Ln; ++i) {
        m_obj = actions.M[i];
        console.log("mt_label", JSON.stringify(m_obj));
        // The live board starts at the snapshot, so a monster that died during
        // the news has already been removed here.
        if ((m_obj.id === undefined) || (m_obj.id === "")) {
            continue;
        }
        /*if (m_obj.target === m_obj.old_target) {
            continue;
        }*/
        var trg = battle.targetsMap[m_obj.target];
        if (!trg) {
            trg = "";
        } else {
            trg = BGU.replaceAll(trg, " ", "+");
        }

        post_request += m_obj.id + "$" + trg + "#";
    }
    console.log("prepareOrder", "point5", post_request);
    for(i = 0, Ln = battle.paralyze.length; i < Ln; ++i) {
        post_request += "PARALYZE" + battle.paralyze[i] + "$" + actions.CP[battle.paralyze[i]].h + "#";
    }
    console.log("prepareOrder", "point6", post_request);
    for(i = 0, Ln = battle.charm.length; i < Ln; ++i) {
        pc_gv = actions.CC[battle.charm[i]].g;
        if (!pc_gv || (pc_gv === '')) {
            pc_gv = "-"
        }
        if (pc_gv === ">") {
            pc_gv = "&gt;"
        }
        post_request += "DIRECTHAND" + battle.charm[i] + "$" + actions.CC[battle.charm[i]].h + "#";
        post_request += "DIRECTGESTURE" + battle.charm[i] + "$" + pc_gv + "#";
    }
    console.log("prepareOrder", "point7", post_request);

    if (actions.D !== -1) {
        post_request += "DELAY$" + BGU.getHandByIdx(actions.D);
    }
    console.log("prepareOrder", "point8", post_request);
    if (actions.P !== -1) {
        post_request += "PERM$" + BGU.getHandByIdx(actions.P);
    }
    console.log("prepareOrder", "point9", post_request);
    if (actions.F === 1) {
        post_request += "FIRE$1#"
    }
    // https://github.com/Pz1c/WavingHands/issues/315
    if ((actions.S === 1) && battle.suicide) {
        post_request += "SUICIDE$1#";
    }
    console.log("prepareOrder", "after", post_request);

    mainWindow.gameCore.sendOrders(post_request);
}

function checkSummonHint(action) {
    console.log("checkSummonHint", JSON.stringify(action));
    if ((action.s.n.indexOf("Summon") === -1) || (action.s.n.indexOf("Elemental") !== -1)) {
        return false;
    }
    if (action.target === battle.player_name) {
        return false;
    }
    for(var i=0, Ln = battle.monsters[battle.player_name].length; i < Ln; ++i) {
        if (battle.monsters[battle.player_name][i].name === action.target) {
            return false;
        }
    }

    return true;
}

function checkMonsterHint(monster) {
    console.log("checkMonsterHint", JSON.stringify(battle.monsters));
    for(var i=0, Ln = battle.monsters[battle.player_name].length; i < Ln; ++i) {
        if (battle.monsters[battle.player_name][i].name === monster.target) {
            return true;
        }
    }

    return false;
}


function getOrdersForReview(dictionary) {
    console.log("getOrdersForReview", JSON.stringify(battle.actions));
    var res = [];
    var actions = battle.actions;

    if ((actions.L.g === "P") && (actions.R.g === "P")) {
        res.push({row_type:"0",type:"PP",v:dictionary.getStringByCode("TitleAction_p"),c:"red",icon:"RIP2",icon_text:"",icon_visible:true,icon_width:60});
        //res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_pp"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    // https://github.com/Pz1c/WavingHands/issues/315: the site offers Suicide to a permanently enchanted warlock
    if (battle.suicide) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_suicide"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }

    if (actions.C !== "") {
        res.push({row_type:"0",type:"C",v:"Say: " + actions.C,c:"snow",icon:"chat",icon_text:"",icon_visible:true,icon_width:60});
        console.log("getOrdersForReview", "point1", JSON.stringify(res));
    }
    // gesture
    res.push({row_type:"0",type:"LH",v:BGU.getTextForHandAction("LH", actions.L, battle.targetsMap, dictionary, battle.completed_spell_L),
              c:"snow",icon:"g_" + BGU.getIconByGesture(actions.L.g),icon_text:"",icon_visible:true,icon_width:60});
    if ((actions.L.g === "C") && (actions.R.g !== "C")) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_clap"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    if (actions.L.target === "Nobody") {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_nobody"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    } else if (checkSummonHint(actions.L)) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_summon"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    console.log("getOrdersForReview", "point2", JSON.stringify(res));

    res.push({row_type:"0",type:"RH",v:BGU.getTextForHandAction("RH", actions.R, battle.targetsMap, dictionary, battle.completed_spell_R),
              c:"snow",icon:"g_" + BGU.getIconByGesture(actions.R.g),icon_text:"",icon_visible:true,icon_width:60});
    if ((actions.L.g !== "C") && (actions.R.g === "C")) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_clap"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    if (actions.R.target === "Nobody") {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_nobody"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    } else if (checkSummonHint(actions.L)) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_summon"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }
    console.log("getOrdersForReview", "point3", JSON.stringify(res));

    if ((actions.L.g === ">") && (actions.R.g === ">")) {
        res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_knifes"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
    }

    if (actions.D !== -1) {
        res.push({row_type:"0",type:"D",v:BGU.getSpecActionText("D", actions.D, dictionary),c:"snow",icon:"",icon_text:"",icon_visible:false,icon_width:0});
        console.log("getOrdersForReview", "point4", JSON.stringify(res));
    }
    if (actions.P !== -1) {
        res.push({row_type:"0",type:"P",v:BGU.getSpecActionText("P", actions.P, dictionary),c:"snow",icon:"",icon_text:"",icon_visible:false,icon_width:0});
        console.log("getOrdersForReview", "point5", JSON.stringify(res));
    }
    if (actions.F === 1) {
        res.push({row_type:"0",type:"F",v:battle.fire,c:"snow",icon:"",icon_text:"",icon_visible:false,icon_width:0});
        console.log("getOrdersForReview", "point6", JSON.stringify(res));
    }
    var i, Ln, m_obj, pc_gv;
    for(i = 0, Ln = battle.paralyze.length; i < Ln; ++i) {
        res.push({row_type:"0",type:"CP",v:BGU.getCharmActionText("CP", actions.CP[battle.paralyze[i]],
                  battle.targetsMap[battle.paralyze[i]], dictionary), c:"snow",icon:"paralized",icon_text:"",icon_visible:true,icon_width:60});
        console.log("getOrdersForReview", "point7", JSON.stringify(res));
    }
    for(i = 0, Ln = battle.charm.length; i < Ln; ++i) {
        console.log("getOrdersForReview before charm", i, Ln, battle.charm[i], JSON.stringify(actions.CC[battle.charm[i]]));
        res.push({row_type:"0",type:"CC",v:BGU.getCharmActionText("CC", actions.CC[battle.charm[i]], battle.targetsMap[battle.charm[i]], dictionary),
                     c:"snow",icon:"charmed",icon_text:"",icon_visible:true,icon_width:60});
        console.log("getOrdersForReview", "point8", JSON.stringify(res));
    }
    for(i = 0, Ln = actions.M.length; i < Ln; ++i) {
        m_obj = actions.M[i];
        console.log("m_obj", JSON.stringify(m_obj));
        if (!m_obj.under_control ||
                (m_obj.id === undefined) ||
                (m_obj.id === "")) {
            continue;
        }

        var obj = {row_type:"0",type:"M",v:BGU.getMonsterActionText(m_obj, m_obj.target, battle.targetsMap, dictionary),c:"snow",action_idx:i,icon:"",icon_text:"",icon_visible:true,icon_width:78};
        if (m_obj.name.indexOf(":") !== -1) {
            obj.icon = BGU.getMonsterIconBySummonHP(m_obj.hp);
            obj.icon_text = m_obj.name.substr(0, 1);
        } else {
            obj.icon = BGU.getMonsterIconByName(m_obj.name);
            obj.icon_text = m_obj.hp;
        }
        res.push(obj);
        if (m_obj.target === "Nobody") {
            res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_nobody"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
        } else if (m_obj.target === battle.player_name) {
            res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_monster1"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
        } else if (checkMonsterHint(m_obj)) {
            res.push({row_type:"1",type:"H",v:dictionary.getStringByCode("TitleActionHint_monster2"),c:"red",icon:"lightning_notice",icon_text:"",icon_visible:true,icon_width:60});
        }

        console.log("getOrdersForReview", "point9", JSON.stringify(res));
    }
    return res;
}

function getCharmDataByAction(action_data) {
    var arr = action_data.action === "charmed" ? battle.actions.CC : battle.actions.CP;
    var id = battle.targetsMap[action_data.warlock_name];
    var res = arr[id];
    res.id = id;
    res.warlock_idx = action_data.warlock_idx;
    if (!!action_data.hand) {
        res.h = action_data.hand;
    }

    return res;
}

function setCharm(H, G) {
    console.log("BU.setCharm", H, G, JSON.stringify(battle.currentCharm));
    var T = battle.currentCharm.t;
    battle.actions["C" + T][battle.currentCharm.id] = {h:H,g:G,t:T};
}

function checkIsMonsterCharmed(monster) {
    if (monster.is_elemental) {
        return false;
    }
    if (monster.owner === battle.player_name) {
        return false;
    }
    //var lp = battle.warlocks[0];
    /*console.log("checkIsMonsterCharmed", lp.charm_left, lp.charm_right);
    if (!lp["charm_" + (battle.currentHand === "L" ? "left" : "right")]) {
        return false;
    }*/
    if (!battle.actions[battle.currentHand] || !battle.actions[battle.currentHand].s) {
        return false;
    }
    var spell = battle.actions[battle.currentHand].s;
    var res =  (spell.id === BGU.C_SPELL_CHARM_MONSTER) || ((spell.id === -1) && spell.is_charm_monster && (spell.is_charm_monster === 1))
            || battle.player_under_control;
    if (res) {
        monster.allow_choose_target = true;
        battle.actions.M[monster.action_idx].allow_choose_target = true;
        battle.actions.M[monster.action_idx].under_control = true;
    }
    return res;
}

function getIconByGesture(g) {
    return BGU.getIconByGesture(g);
}

function getIconStatusSpell(code) {
    return BGU.icon_status_spell[code];
}

function getIconBySpell(spell_code) {
    return BGU.map_spell_to_icon[spell_code];
}
