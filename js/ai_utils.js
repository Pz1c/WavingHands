var WARLOCK_HAND_NONE = 0;
var WARLOCK_HAND_LEFT = 1;
var WARLOCK_HAND_RIGHT = 2;

var SPELL_TYPE_SUMMON_MONSTER = 0;
var SPELL_TYPE_POISON = 1;
var SPELL_TYPE_CONFUSION = 2;
var SPELL_TYPE_DAMAGE = 3;
var SPELL_TYPE_SHIELD = 4;
var SPELL_TYPE_MAGIC_SHIELD = 5;
var SPELL_TYPE_MASSIVE = 6;
var SPELL_TYPE_HASTLE = 7;
var SPELL_TYPE_CHARM_MONSTER = 8;
var SPELL_TYPE_CURE = 9;
var SPELL_TYPE_SPEC = 10;
var SPELL_TYPE_DEATH = 11;
var SPELL_TYPE_RESIST = 12;
var SPELL_TYPE_ELEMENTAL = 13;
var SPELL_TYPE_REMOVE_ENCHANTMENT = 14;
var SPELL_TYPE_STAB = 15;

var SPELL_DISPEL_MAGIC = 0;
var SPELL_SUMMON_ICE_ELEMENTAL = 1;
var SPELL_SUMMON_FIRE_ELEMENTAL = 2;
var SPELL_MAGIC_MIRROR = 3;
var SPELL_LIGHTNING_BOLT = 4;
var SPELL_CURE_HEAVY_WOUNDS = 5;
var SPELL_CURE_LIGHT_WOUNDS = 6;
var SPELL_BLINDNESS1 = 7;
var SPELL_AMNESIA = 8;
var SPELL_CONFUSION = 9;
var SPELL_DISEASE = 10;
var SPELL_BLINDNESS2 = 11;
var SPELL_DELAY_EFFECT = 12;
var SPELL_POISON = 13;
var SPELL_PARALYSIS = 14;
var SPELL_SUMMON_GIANT = 15;
var SPELL_SUMMON_TROLL = 16;
var SPELL_SUMMON_OGRE = 17;
var SPELL_SUMMON_GOBLIN = 18;
var SPELL_FIREBALL = 19;
var SPELL_SHIELD = 20;
var SPELL_REMOVE_ENCHANTMENT = 21;
var SPELL_INVISIBILITY = 22;
var SPELL_CHARM_MONSTER = 23;
var SPELL_CHARM_PERSON = 24;
var SPELL_FINGER_OF_DEATH = 25;
var SPELL_HASTE = 26;
var SPELL_MAGIC_MISSILE = 27;
var SPELL_ANTI_SPELL = 28;
var SPELL_PERMANENCY = 29;
var SPELL_TIME_STOP1 = 30;
var SPELL_TIME_STOP2 = 31;
var SPELL_RESIST_COLD = 32;
var SPELL_FEAR = 33;
var SPELL_FIRE_STORM = 34;
var SPELL_CLAP_OF_LIGHTNING = 35;
var SPELL_CAUSE_LIGHT_WOUNDS = 36;
var SPELL_CAUSE_HEAVY_WOUNDS = 37;
var SPELL_COUNTER_SPELL1 = 38;
var SPELL_ICE_STORM = 39;
var SPELL_RESIST_HEAT = 40;
var SPELL_PROTECTION = 41;
var SPELL_COUNTER_SPELL2 = 42;
var SPELL_SURRENDER = 43;
var SPELL_STAB = 44;
var SPELL_DISEASE_FDF = 45;
var SPELL_PARALYSIS_FDF = 46;
var SPELL_PARALYSIS_FDFD = 47;

var SPELL_DEF_TARGER_NOBODY = 0;
var SPELL_DEF_TARGER_SELF = 1;
var SPELL_DEF_TARGER_ENEMY = 2;
var SPELL_DEF_TARGER_ENEMY_MONSTER = 3;
var SPELL_DEF_TARGER_SELF_MONSTER = 4;

var arr_no_fdf_inactive = [SPELL_DISEASE_FDF, SPELL_PARALYSIS_FDF, SPELL_PARALYSIS_FDFD];
var arr_fdf_inactive = [SPELL_DISEASE, SPELL_PARALYSIS];


var arr_spells = [
/* 0 */ {active:true,gesture:"cDPW",name:"Dispel Magic",type:SPELL_TYPE_REMOVE_ENCHANTMENT,priority:0,level:1,danger:10},
/* 1 */ {active:true,gesture:"cSWWS",name:"Summon Ice Elemental",type:SPELL_TYPE_ELEMENTAL,priority:4,level:0,danger:15},
/* 2 */ {active:true,gesture:"cWSSW",name:"Summon Fire Elemental",type:SPELL_TYPE_ELEMENTAL,priority:5,level:1,danger:15},
/* 3 */ {active:true,gesture:"cw",name:"Magic Mirror",type:SPELL_TYPE_MAGIC_SHIELD,priority:0,level:0,danger:10},
/* 4 */ {active:true,gesture:"DFFDD",name:"Lightning Bolt",type:SPELL_TYPE_DAMAGE,priority:1,level:1,danger:16},
/* 5 */ {active:true,gesture:"DFPW",name:"Cure Heavy Wounds",type:SPELL_TYPE_CURE,priority:0,level:1,danger:10},
/* 6 */ {active:true,gesture:"DFW",name:"Cure Light Wounds",type:SPELL_TYPE_CURE,priority:0,level:0,danger:10},
/* 7 */ {active:true,gesture:"DFWFd",name:"Blindness",type:SPELL_TYPE_CONFUSION,priority:4,level:0,danger:15},
/* 8 */ {active:true,gesture:"DPP",name:"Amnesia",type:SPELL_TYPE_CONFUSION,priority:6,level:1,danger:12},
/* 9 */ {active:true,gesture:"DSF",name:"Confusion/Maladroitness",type:SPELL_TYPE_CONFUSION,priority:6,level:0,danger:12},
/* 10*/ {active:true,gesture:"DSFFFc",name:"Disease",type:SPELL_TYPE_POISON,priority:4,level:1,danger:17},
/* 11*/ {active:true,gesture:"DWFFd",name:"Blindness",type:SPELL_TYPE_CONFUSION,priority:4,level:0,danger:15},
/* 12*/ {active:true,gesture:"DWSSSP",name:"Delay Effect",type:SPELL_TYPE_SPEC,priority:0,level:0,danger:10},
/* 13*/ {active:true,gesture:"DWWFWD",name:"Poison",type:SPELL_TYPE_POISON,priority:3,level:1,danger:18},
/* 14*/ {active:true,gesture:"FFF",name:"Paralysis",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:12},
/* 15*/ {active:true,gesture:"WFPSFW",name:"Summon Giant",type:SPELL_TYPE_SUMMON_MONSTER,priority:4,level:4,danger:14},
/* 16*/ {active:true,gesture:"FPSFW",name:"Summon Troll",type:SPELL_TYPE_SUMMON_MONSTER,priority:3,level:3,danger:13},
/* 17*/ {active:true,gesture:"PSFW",name:"Summon Ogre",type:SPELL_TYPE_SUMMON_MONSTER,priority:2,level:2,danger:12},
/* 18*/ {active:true,gesture:"SFW",name:"Summon Goblin",type:SPELL_TYPE_SUMMON_MONSTER,priority:1,level:1,danger:11},
/* 19*/ {active:true,gesture:"FSSDD",name:"Fireball",type:SPELL_TYPE_DAMAGE,priority:3,level:1,danger:15},
/* 20*/ {active:true,gesture:"P",name:"Shield",type:SPELL_TYPE_SHIELD,priority:0,level:0,danger:10},
/* 21*/ {active:true,gesture:"PDWP",name:"Remove Enchantment",type:SPELL_TYPE_REMOVE_ENCHANTMENT,priority:0,level:0,danger:13},
/* 22*/ {active:true,gesture:"PPws",name:"Invisibility",type:SPELL_TYPE_CONFUSION,priority:3,level:0,danger:0,danger:13},
/* 23*/ {active:true,gesture:"PSDD",name:"Charm Monster",type:SPELL_TYPE_CHARM_MONSTER,priority:0,level:0,danger:12},
/* 24*/ {active:true,gesture:"PSDF",name:"Charm Person",type:SPELL_TYPE_CONFUSION,priority:4,level:2,danger:13},
/* 25*/ {active:true,gesture:"PWPFSSSD",name:"Finger of Death",type:SPELL_TYPE_DAMAGE,priority:1,level:0,danger:20},
/* 26*/ {active:true,gesture:"PWPWWc",name:"Haste",type:SPELL_TYPE_HASTLE,priority:2,level:0,danger:10},
/* 27*/ {active:true,gesture:"SD",name:"Magic Missile",type:SPELL_TYPE_DAMAGE,priority:0,level:0,danger:10},
/* 28*/ {active:true,gesture:"SPFP",name:"Anti-spell",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:14},
/* 29*/ {active:true,gesture:"SPFPSDW",name:"Permanency",type:SPELL_TYPE_SPEC,priority:3,level:0,danger:16},
/* 30*/ {active:true,gesture:"SPPc",name:"Time Stop",type:SPELL_TYPE_HASTLE,priority:3,level:0,danger:10},
/* 31*/ {active:true,gesture:"SPPFD",name:"Time Stop",type:SPELL_TYPE_HASTLE,priority:3,level:0,danger:10},
/* 32*/ {active:true,gesture:"SSFP",name:"Resist Cold",type:SPELL_TYPE_RESIST,priority:4,level:1,danger:10},
/* 33*/ {active:true,gesture:"SWD",name:"Fear (No CFDS)",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:12},
/* 34*/ {active:true,gesture:"SWWc",name:"Fire Storm",type:SPELL_TYPE_MASSIVE,priority:3,level:0,danger:16},
/* 35*/ {active:true,gesture:"WDDc",name:"Clap of Lightning",type:SPELL_TYPE_DAMAGE,priority:4,level:0,danger:15},
/* 36*/ {active:true,gesture:"WFP",name:"Cause Light Wounds",type:SPELL_TYPE_DAMAGE,priority:4,level:0,danger:12},
/* 37*/ {active:true,gesture:"WPFD",name:"Cause Heavy Wounds",type:SPELL_TYPE_DAMAGE,priority:3,level:0,danger:13},
/* 38*/ {active:true,gesture:"WPP",name:"Counter Spell",type:SPELL_TYPE_MAGIC_SHIELD,priority:0,level:0,danger:10},
/* 39*/ {active:true,gesture:"WSSc",name:"Ice Storm",type:SPELL_TYPE_MASSIVE,priority:0,level:3,danger:16},
/* 40*/ {active:true,gesture:"WWFP",name:"Resist Heat",type:SPELL_TYPE_RESIST,priority:0,level:4,danger:10},
/* 41*/ {active:true,gesture:"WWP",name:"Protection",type:SPELL_TYPE_SHIELD,priority:0,level:1,danger:10},
/* 42*/ {active:true,gesture:"WWS",name:"Counter Spell",type:SPELL_TYPE_MAGIC_SHIELD,priority:0,level:1,danger:10},
/* 43*/ {active:true,gesture:"p",name:"Surrender",type:SPELL_TYPE_SPEC,priority:-100,level:0,danger:0},
/* 44*/ {active:true,gesture:">",name:"Stab",type:SPELL_TYPE_STAB,priority:-1,level:0,danger:0},
/* 45*/ {active:true,gesture:"DSFDFc",name:"Disease",type:SPELL_TYPE_POISON,priority:4,level:1,danger:17},
/* 46*/ {active:true,gesture:"FDF",name:"Paralysis",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:12},
/* 47*/ {active:true,gesture:"FDFD",name:"Paralysis",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:12}
];

function checkSpell(spell, all_ids, all_types, all_hands, not_ids_empty, search_types, search_ids, not_ids, turn_from, turn_to, hands) {
    if (!all_types && (search_types.indexOf(spell.st) === -1)) {
        return false;
    }
    if (!all_ids && (search_ids.indexOf(spell.id) === -1)) {
        return false;
    }
    if (!not_ids_empty && (not_ids.indexOf(spell.id) !== -1)) {
        return false;
    }
    if (!all_hands && (hands.indexOf(spell.h) === -1)) {
        return false;
    }
    if ((spell.t < turn_from) || (spell.t > turn_to)) {
        return false;
    }
    return true;
}

function findSpell(spell_list, param) {
    var search_types = param.search_types, search_ids = param.search_ids, not_ids = param.not_ids, turn_from = param.turn_from, turn_to = param.turn_to, hands = param.hands;
    var all_ids = search_ids.length === 0;
    var all_types = search_types.length === 0;
    var all_hands = hands.length === 2;
    var not_ids_empty = not_ids.length === 0;
    for (var i = 0, Ln = spell_list.length; i < Ln; ++i) {
        console.log("findSpell", JSON.stringify(spell_list[i]), JSON.stringify(param));
        if (!spell_list[i].active) {
            continue;
        }

        if (checkSpell(spell_list[i], all_ids, all_types, all_hands, not_ids_empty, search_types, search_ids, not_ids, turn_from, turn_to, hands)) {
            console.log("FIND");
            return spell_list[i];
        }
    }
    return false;
}

function getGestureBySpell(spell, idx) {
    console.log("getGestureBySpell", JSON.stringify(spell), idx);
    if (!idx || !spell) {
        idx = 0;
    }
    if (!spell.g) {
        return false;
    }

    return spell.g.substr(spell.a + idx, 1);
}

function setGestureBySpell(warlock, spell) {
    /*if (!spell.g) {
        return;
    }*/

    var gesture = getGestureBySpell(spell);
    if (!gesture) {
        return;
    }

    Qt.mainWindow.changeGesture(gesture, spell.h === WARLOCK_HAND_LEFT);
}

function getAntispellFilter(enemy_spell) {
    switch(enemy_spell.st) {
    case SPELL_TYPE_SUMMON_MONSTER: return {search_types:[SPELL_TYPE_CHARM_MONSTER, SPELL_TYPE_MAGIC_SHIELD], search_ids:[], not_ids:[SPELL_MAGIC_MIRROR], turn_from:enemy_spell.t, turn_to:enemy_spell.t};
    case SPELL_TYPE_POISON: return {search_types:[SPELL_TYPE_MAGIC_SHIELD, SPELL_TYPE_REMOVE_ENCHANTMENT], search_ids:[], not_ids:[], turn_from:enemy_spell.t, turn_to:enemy_spell.t};
    case SPELL_TYPE_CONFUSION: return {search_types:[SPELL_TYPE_MAGIC_SHIELD], search_ids:[], not_ids:[], turn_from:enemy_spell.t, turn_to:enemy_spell.t};
    case SPELL_TYPE_DAMAGE:
        if (enemy_spell.id !== SPELL_MAGIC_MISSILE) {
            return {search_types:[SPELL_TYPE_MAGIC_SHIELD], search_ids:[], not_ids:[], turn_from:enemy_spell.t, turn_to:enemy_spell.t};
        }
        break;
    case SPELL_TYPE_SHIELD:
        break;
    case SPELL_TYPE_MAGIC_SHIELD:
        break;
    case SPELL_TYPE_MASSIVE: return {search_types:[SPELL_TYPE_MAGIC_SHIELD], search_ids:[], not_ids:[], turn_from:enemy_spell.t, turn_to:enemy_spell.t};
    case SPELL_TYPE_HASTLE:
        break;
    case SPELL_TYPE_CHARM_MONSTER:
        break;
    case SPELL_TYPE_CURE:
        break;
    case SPELL_TYPE_SPEC:
        break;
    case SPELL_TYPE_DEATH:  return {search_types:[], search_ids:[SPELL_ANTI_SPELL, SPELL_CHARM_PERSON, SPELL_AMNESIA, SPELL_CONFUSION, SPELL_PARALYSIS], not_ids:[], turn_from:0, turn_to:enemy_spell.t - 1};
    case SPELL_TYPE_RESIST:
    break;
    case SPELL_TYPE_ELEMENTAL: return {search_types:[], search_ids:[enemy_spell.l === 0 ? SPELL_RESIST_COLD : SPELL_RESIST_HEAT], not_ids:[], turn_from:0, turn_to:enemy_spell.t};
    case SPELL_TYPE_REMOVE_ENCHANTMENT:
    break;
    case SPELL_TYPE_STAB:
    break;
    }
    return false;
}

function getAntispellByFilter(self, filter) {
    var anti_spell = findSpell([self.bsL, self.bsR], filter);
    if (!anti_spell) {
        anti_spell = findSpell(self.spells, filter);
    }
    return anti_spell;
}

function destroyEnemySpell(self, enemy_spell) {
    var arr_hands = [];
    if (self.gL === '') {
        arr_hands.push(WARLOCK_HAND_LEFT);
    }
    if (self.gR === '') {
        arr_hands.push(WARLOCK_HAND_RIGHT);
    }

    console.log("destroyEnemySpell", arr_hands, JSON.stringify(enemy_spell));

    if (arr_hands.length === 0) {
        return false;
    }

    var anti_spell, need_search = false;
    var anti_spell_param = {search_types:[SPELL_TYPE_CONFUSION], search_ids:[], not_ids:[], turn_from:0, turn_to:enemy_spell.t - 1};
    anti_spell_param.hands = arr_hands;
    anti_spell = getAntispellByFilter(self, anti_spell_param);
    if (!anti_spell) {
        anti_spell_param = getAntispellFilter(enemy_spell)
        if (!anti_spell_param) {
            return ;
        }
        anti_spell_param.hands = arr_hands;
        anti_spell = getAntispellByFilter(self, anti_spell_param);
        if (0) {

        }
    }

    console.log("destroyEnemySpell", JSON.stringify(enemy_spell), JSON.stringify(anti_spell));

    if (anti_spell) {
        anti_spell.anti_spell = true;
        if (anti_spell.h === WARLOCK_HAND_LEFT) {
            self.bsL = anti_spell;
        } else {
            self.bsR = anti_spell;
        }
    }
}

function compareSpells(spell_left, spell_right) {
    if (spell_left.ng === 'X') {
        return WARLOCK_HAND_RIGHT;
    }

    if (spell_right.ng === 'X') {
        return WARLOCK_HAND_LEFT;
    }

    if (spell_left.anti_spell && !spell_right.anti_spell) {
        return WARLOCK_HAND_LEFT;
    }
    if (!spell_left.anti_spell && spell_right.anti_spell) {
        return WARLOCK_HAND_RIGHT;
    }
    return spell_left.p > spell_right.p ? WARLOCK_HAND_LEFT : WARLOCK_HAND_RIGHT;
}

function isSpellsNormal(self) {
    var bad_hand = compareSpells(self.bsL, self.bsR) === WARLOCK_HAND_LEFT ? WARLOCK_HAND_RIGHT : WARLOCK_HAND_LEFT;
    var need_change = false;
    console.log("isSpellsNormal", JSON.stringify(self.bsL), JSON.stringify(self.bsR), bad_hand);

    if (!need_change) {
        if ((self.bsL.ng === 'X') || (self.bsR.ng === 'X')) {
            console.log("isSpellsNormal", "spell not set for hand L");
            need_change = true;
        }
    }

    if (!need_change) {
        if ((self.bsL.st === self.bsR.st) && (self.bsL.t === self.bsR.t)) {
            console.log("isSpellsNormal", "same type in same turn");
            need_change = true;
        }
    }

    if (!need_change) {
        if (((self.bsL.ng === 'C') || (self.bsR.ng === 'C')) && (self.bsL.t === self.bsR.t)) {
            console.log("isSpellsNormal", "half clap");
            need_change = true;
        }
    }

    if (!need_change) {
        if (bad_hand === WARLOCK_HAND_RIGHT) {
            if ((self.bsL.th === 1) && (self.bsL.ng !== self.bsR.ng)) {
                console.log("isSpellsNormal", "wrong two hand gesture");
                need_change = true;
            }
        } else {
            if ((self.bsR.th === 1) && (self.bsR.ng !== self.bsL.ng)) {
                console.log("isSpellsNormal", "wrong two hand gesture");
                need_change = true;
            }
        }
    }

    if (!need_change) {
        for (var i = 0; i < 3; ++i) {
            if ((getGestureBySpell(self.bsL, i) === 'P') && (getGestureBySpell(self.bsR, i) === 'P')) {
                console.log("isSpellsNormal", "try surrender");
                need_change = true;
                break;
            }
        }
    }

    if (need_change) {
        if (bad_hand === WARLOCK_HAND_LEFT) {
            self.bsL.change = true;
        } else {
            self.bsR.change = true;
        }
    }

    return !need_change;
}

function copyObject(from, to) {
    for(var key in from) {
        to[key] = from[key];
    }
}

function setNextSpell(self) {
    var hand_left = !!self.bsL.change;
    var arr_not_ids, hand_id, priority, next_gesture = '';
    if (hand_left) {
        if (self.LnotIds.indexOf(self.bsL.id) === -1) {
            self.LnotIds.push(self.bsL.id);
        }
        arr_not_ids = self.LnotIds;
        priority = self.bsL.p;
        hand_id = self.bsL.h;
        if ((self.maladroit > 0) || (self.bsR.th === 1)) {
            next_gesture = self.bsR.ng;
        }
    } else {
        if (self.RnotIds.indexOf(self.bsR.id) === -1) {
            self.RnotIds.push(self.bsR.id);
        }
        arr_not_ids = self.RnotIds;
        priority = self.bsR.p;
        hand_id = self.bsR.h;
        if ((self.maladroit > 0) || (self.bsL.th === 1)) {
            next_gesture = self.bsL.ng;
        }
    }
    console.log("setNextSpell", hand_left, arr_not_ids, priority, hand_id);
    for (var i = 0, Ln = self.spells.length; i < Ln; ++i) {
        console.log("check spell", JSON.stringify(self.spells[i]));
        if (self.spells[i].h !== hand_id) {
            console.log("wrong hand", self.spells[i].h, hand_id);
            continue;
        }
        if (arr_not_ids.indexOf(self.spells[i].id) !== -1) {
            console.log("banned id", self.spells[i].id, arr_not_ids);
            continue;
        }
        if ((next_gesture !== '') && (self.spells[i].ng !== next_gesture)) {
            console.log("wrong gesture", self.spells[i].ng, next_gesture);
            continue;
        }

        /*if (priority > self.spells[i].p) {
            console.log("wrong priority", self.spells[i].p, priority);
            continue;
        }*/
        if (hand_left) {
            console.log("setNextSpell", JSON.stringify(self.bsL), JSON.stringify(self.spells[i]));
            copyObject(self.spells[i], self.bsL);
            self.bsR.change = false;
            console.log("setNextSpell", JSON.stringify(self.bsL));
        } else {
            console.log("setNextSpell", JSON.stringify(self.bsR), JSON.stringify(self.spells[i]));
            copyObject(self.spells[i], self.bsR);
            self.bsR.change = false;
            //self.bsR = self.spells[i];
            console.log("setNextSpell", JSON.stringify(self.bsR));
        }
        break;
    }
}

function setMonsterTargetByName(monster_name, target) {
    console.log("setMonsterTargetByName", monster_name, target);
    for(var i = 0, Ln = mtMonsterObj.length; i < Ln; ++i) {
        console.log("setMonsterTargetByName", mtMonsterObj[i].mt_id, mtMonsterObj[i].mt_label);
        if (mtMonsterObj[i].mt_label === monster_name) {
            mtMonsterObj[i].setTarget(target);
            return;
        }
    }
}

function getMonsterDangerBySpellID(spell_id) {
    switch(spell_id) {
    case SPELL_SUMMON_GIANT: return 4;
    case SPELL_SUMMON_TROLL: return 3;
    case SPELL_SUMMON_OGRE: return 2;
    case SPELL_SUMMON_GOBLIN: return 1;
    case SPELL_SUMMON_FIRE_ELEMENTAL: return battle.self.fireproof > 0 ? 0 : 3;
    case SPELL_SUMMON_ICE_ELEMENTAL: return battle.self.coldproof > 0 ? 0 : 3;
    default: return 0;
    }
}

function getMonsterHPBySpellID(spell_id) {
    switch(spell_id) {
    case SPELL_SUMMON_GIANT: return 4;
    case SPELL_SUMMON_TROLL: return 3;
    case SPELL_SUMMON_OGRE: return 2;
    case SPELL_SUMMON_GOBLIN: return 1;
    case SPELL_SUMMON_FIRE_ELEMENTAL:
    case SPELL_SUMMON_ICE_ELEMENTAL: return 3;
    default: return 0;
    }
}

function getMonsterTypeBySpellID(spell_id) {
    switch(spell_id) {
    case SPELL_SUMMON_FIRE_ELEMENTAL:
    case SPELL_SUMMON_ICE_ELEMENTAL: return "e";
    default: return "m";
    }
}

function compareMonster(a,b) {
  if (a.danger < b.danger) return +1;
  if (a.danger > b.danger) return -1;
  return 0;
}

var targets = [];
function prepareTargetsArray() {
    console.log("prepareTargetsArray");
    targets = [];
    if (battle.enemy.summon_left > 0) {
        targets.push({type:getMonsterTypeBySpellID(battle.enemy.summon_left),name:"LH:"+battle.enemy.name,danger:getMonsterDangerBySpellID(battle.enemy.summon_left),hp:getMonsterHPBySpellID(battle.enemy.summon_left),under_attack:0});
    }
    if (battle.enemy.summon_right > 0) {
        targets.push({type:getMonsterTypeBySpellID(battle.enemy.summon_left),name:"RH:"+battle.enemy.name,danger:getMonsterDangerBySpellID(battle.enemy.summon_right),hp:getMonsterHPBySpellID(battle.enemy.summon_right),under_attack:0});
    }
    for(var i = 0, Ln = battle.monsters.length; i < Ln; ++i) {
        var mob = battle.monsters[i];
        if (!mob.under_control) {
            targets.push({type:"m",name:mob.name,danger:mob.strength,hp:mob.hp,under_attack:0});
        }
    }
    targets.push({type:"w",name:battle.enemy.name,danger:-1,under_attack:0,hp:battle.enemy.hp});
    targets.push({type:"s",name:battle.self.name,danger:-2,under_attack:0,hp:battle.self.hp});
    console.log("prepareTargets", JSON.stringify(targets));
    targets.sort(compareMonster);
    console.log("prepareTargets", JSON.stringify(targets));
}

function getBestTarget(attack, type, is_monster) {
    if (!type) {
        type = "all";
    }
    console.log("getBestTarget", attack, type, JSON.stringify(targets));

    var target_idx = -1, i, Ln, trg;
    for (i = 0, Ln = targets.length; i < Ln; ++i) {
        trg = targets[i];
        if ((type !== "all") && (trg.type !== type)) {
            continue;
        }
        if (is_monster && (trg.type === "s")) {
            continue;
        }
        if (trg.hp - trg.under_attack <= 0) {
            continue;
        }
        console.log("check 1", trg.hp, trg.under_attack, attack, trg.hp - trg.under_attack - attack);
        if (trg.hp - trg.under_attack - attack === 0) {
            target_idx = i;
            break;
        }
    }
    if (target_idx === -1) {
        for (i = 0, Ln = targets.length; i < Ln; ++i) {
            trg = targets[i];
            if ((type !== "all") && (trg.type !== type)) {
                continue;
            }
            if (trg.hp - trg.under_attack <= 0) {
                continue;
            }
            if (is_monster && (trg.type === "s")) {
                continue;
            }
            console.log("check 2", trg.hp, trg.under_attack, attack, trg.hp - trg.under_attack - attack);
            if (trg.hp - trg.under_attack - attack < 0) {
                target_idx = i;
                break;
            }
        }
    }
    if (target_idx === -1) {
        for (i = 0, Ln = targets.length; i < Ln; ++i) {
            trg = targets[i];
            if ((type !== "all") && (trg.type !== type)) {
                continue;
            }
            if (trg.hp - trg.under_attack <= 0) {
                continue;
            }
            if (trg.hp - trg.under_attack > 0) {
                target_idx = i;
                break;
            }
            if (is_monster && (trg.type === "s")) {
                continue;
            }
        }
    }
    if ((target_idx === -1) && (type !== "all")) {
        target_idx = 0;
    }

    return target_idx;
}

function setTargetsForMonsters(do_charm_monster) {
    console.log("setTargetsForMonsters", do_charm_monster);
    var i, Ln, mob;

    if (battle.self.summon_left > 0) {
        battle.monsters.push({under_control:1,name:"LH:"+battle.self.name,danger:getMonsterDangerBySpellID(battle.self.summon_left)});
    }
    if (battle.self.summon_right > 0) {
        battle.monsters.push({under_control:1,name:"RH:"+battle.self.name,danger:getMonsterDangerBySpellID(battle.self.summon_right)});
    }

    for(i = 0, Ln = battle.monsters.length; i < Ln; ++i) {
        mob = battle.monsters[i];
        console.log("setTargetsForMonsters", JSON.stringify(mob));
        if (do_charm_monster || mob.under_control) {
            var target_idx = getBestTarget(mob.strength, "all", true);
            targets[target_idx].under_attack += mob.strength;
            setMonsterTargetByName(mob.name, targets[target_idx].name);
        } else if (!mob.under_control) {
            setMonsterTargetByName(mob.name, battle.enemy.name);
        }
    }
}

function setTargetForCharmed() {
    console.log("setTargetForCharmed", cpPersonObj.length);
    for (var i = 0, Ln = cpPersonObj.length; i < Ln; ++i) {
        console.log("setTargetForCharmed", cpPersonObj[i].pc_target_name, battle.self.name, battle.enemy.bsL.p, battle.enemy.bsR.p);
        if (cpPersonObj[i].pc_target_name === battle.self.name) {
            cpPersonObj[i].setHandGesture("LH", battle.self.gL);
        } else {
            cpPersonObj[i].setHandGesture(battle.enemy.bsL.p > battle.enemy.bsR.p ? "LH" : "RH", "-");
        }
    }
}

function setTargetForParalyzed() {
    console.log("setTargetForParalyzed", pParalyzeObj.length);
    for (var i = 0, Ln = pParalyzeObj.length; i < Ln; ++i) {
        console.log("setTargetForParalyzed", pParalyzeObj[i].pc_target_name, battle.self.name, battle.enemy.bsL.p, battle.enemy.bsR.p);
        if (pParalyzeObj[i].pc_target_name === battle.self.name) {
            pParalyzeObj[i].setHand(battle.self.bsL.p > battle.self.bsR.p ? "RH" : "LH");
        } else {
            pParalyzeObj[i].setHand(battle.enemy.bsL.p > battle.enemy.bsR.p ? "LH" : "RH");
        }
    }
}

function prepareBattleWarlock() {
    console.log("prepareBattleWarlock");
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        if (battle.warlocks[i].player) {
            battle.self = battle.warlocks[i];
            battle.self.gL = '';
            battle.self.gR = '';
            battle.self.LnotIds = [];
            battle.self.RnotIds = [];
        } else {
            battle.enemy = battle.warlocks[i];
        }
    }
    delete battle.warlocks;
    console.log("processBattle", "enemy", JSON.stringify(battle.enemy));
    console.log("processBattle", "self", JSON.stringify(battle.self));
    if (!battle.self.bsL.g) {
        battle.self.bsL = {id:-1,name:"",g:"XXX",t:1000,st:100,p:-100,h:1,l:100,a:3,ng:"X",th:0};
    }
    if (!battle.self.bsR.g) {
        battle.self.bsL = {id:-1,name:"",g:"XXX",t:1000,st:100,p:-100,h:2,l:100,a:3,ng:"X",th:0};
    }
}

function selectGesture() {
    var left_processed = battle.enemy.bsL.p > battle.enemy.bsR.p;
    if (left_processed) {
        destroyEnemySpell(battle.self, battle.enemy.bsL);
        console.log("destroyEnemySpell", "L", JSON.stringify(battle.enemy.bsL), JSON.stringify(battle.self.bsL), JSON.stringify(battle.self.bsR));
    }
    destroyEnemySpell(battle.self, battle.enemy.bsR);
    console.log("destroyEnemySpell", "R", JSON.stringify(battle.enemy.bsR), JSON.stringify(battle.self.bsL), JSON.stringify(battle.self.bsR));
    if (!left_processed) {
        destroyEnemySpell(battle.self, battle.enemy.bsL);
        console.log("destroyEnemySpell", "L", JSON.stringify(battle.enemy.bsL), JSON.stringify(battle.self.bsL), JSON.stringify(battle.self.bsR));
    }

    var idx = 0;
    while(!isSpellsNormal(battle.self)) {
        setNextSpell(battle.self);
        if (++idx > 50) {
            break;
        }
    }

    var left_first = battle.self.bsL.p < battle.self.bsR.p;/*!(battle.self.bsL.anti_spell && !battle.self.bsR.anti_spell) &&
             ((!battle.self.bsL.anti_spell && battle.self.bsR.anti_spell) ||
              (battle.self.bsL.p < battle.self.bsR.p));*/
    console.log("setGestureBySpell", left_first, battle.self.bsL, battle.self.bsR);
    setGestureBySpell(battle.self, left_first ? battle.self.bsL : battle.self.bsR);
    setGestureBySpell(battle.self, left_first ? battle.self.bsR : battle.self.bsL);
}

function getSpellByName(spell_name) {
    for(var i = 0, Ln = arr_spells.length; i < Ln; ++i) {
        if (arr_spells[i].n === spell_name) {
            return arr_spells[i];
        }
    }
}

function setTargetForChoosenSpell(is_right) {
    var current_spell_name = is_right ? cbRHS.currentText : cbLHS.currentText;
    console.log("setTargetForChoosenSpell", is_right, current_spell_name);
    if (!current_spell_name) {
        return ;
    }

    var current_spell = getSpellByName(current_spell_name);
    console.log("setTargetForChoosenSpell", JSON.stringify(current_spell));
    if (!current_spell) {
        return ;
    }

    var target_idx = -1, attack = 0;
    do {
        if (current_spell.st === SPELL_TYPE_DAMAGE) {
            target_idx = getBestTarget(current_spell.dmg);
            attack = current_spell.dmg;
            break;
        }
        if (current_spell.st === SPELL_TYPE_CHARM_MONSTER) {
            target_idx = getBestTarget(4, "m");
            attack = 4;
            if (target_idx === -1) {
                target_idx = getBestTarget(0, "s");
                attack = 0;
            }
            break;
        }
        //if (current_spell)

    }while(0);

    if (target_idx !== -1) {
        targets[target_idx].under_attack += attack;
        setTargetForSpell(is_right, targets[target_idx].name);
    }
}

function setTargetsForSpells() {
    console.log("setTargetsForSpells")
    getPossibleSpell(false, battle.self.bsL.t === 1 ? battle.self.bsL.n : "");
    getPossibleSpell(true, battle.self.bsR.t === 1 ? battle.self.bsR.n : "");
    setTargetForChoosenSpell(false);
    setTargetForChoosenSpell(true);
}

function getBotMsgByTurn(turn_num) {
    switch(turn_num) {
    case 1: return "Hi, warlock, I am a golem created for your training, try to beat me. Also join to community https://fb.com/WarlocksDuel/";
    case 2: return "Try to break enemy plane, using Amnesia, Paralysis, Confusion, Charm Person, Fear";
    case 3: return "Summon monsters to get your opponent busy ...SFW";
    case 4: return "Use Invisibility and Blindness to keep your gesture in secret, also you cannot be targeted";
    case 5: return "Dispel Magic remove all magic effects even mosters";
    case 6: return "Shield keep your in safe under monster attack, even Elementals";
    case 7: return "Cast Invisibility or Blindness on monster to eraise them";
    case 8: return "Cast Counter Spell on the enemy when enemy try to cast Summon spell and even Giant will be absorbed, but not elemental";
    case 9: return "Finger of Death can be reflected by Magic Mirror cw";
    default: return "";
    }
}

function processBattle(isAI) {
    console.log("processBattle", isAI/*JSON.stringify(battle)*/);

    var i, Ln;
    arr_spells = JSON.parse(Qt.core.getSpellBook());
    /*arr_spells[10].active = !battle.isFDF;
    arr_spells[14].active = !battle.isFDF;
    arr_spells[45].active = battle.isFDF;
    arr_spells[46].active = battle.isFDF;
    arr_spells[47].active = battle.isFDF;*/
    /*for(i = 0, Ln = arr_spells.length; i < Ln; ++i) {
        arr_spells[i].active = (battle.isFDF && (arr_fdf_inactive.indexOf(i) === -1)) || (!battle.isFDF && (arr_no_fdf_inactive.indexOf(i) === -1));
    }*/

    prepareBattleWarlock();
    selectGesture();
    var do_charm_monster = (battle.self.bsL.id === SPELL_CHARM_MONSTER) || (battle.self.bsR.id === SPELL_CHARM_MONSTER);
    prepareTargetsArray();
    setTargetsForSpells();
    setTargetsForMonsters(do_charm_monster);
    setTargetForCharmed();
    setTargetForParalyzed();
    console.log("processBattle", "complete", JSON.stringify(battle.enemy));
    //tSendOrderTimer.start();
    if (isAI) {
        teChatMsg.text = getBotMsgByTurn(battle.turn_num);
        sendOrderEx();
    } else {
        console.log("processBattle", "not AI");
    }
    //spellDecision(battle);
    //checkSpellCast(battle);
    //printBattle(battle, false);
    //return battle;
    //*/
}
