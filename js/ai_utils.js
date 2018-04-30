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


var arr_spells = [
/* 0 */ {gesture:"cDPW",name:"Dispel Magic",type:SPELL_TYPE_REMOVE_ENCHANTMENT,priority:0,level:1,danger:10},
/* 1 */ {gesture:"cSWWS",name:"Summon Ice Elemental",type:SPELL_TYPE_ELEMENTAL,priority:4,level:0,danger:15},
/* 2 */ {gesture:"cWSSW",name:"Summon Fire Elemental",type:SPELL_TYPE_ELEMENTAL,priority:5,level:1,danger:15},
/* 3 */ {gesture:"cw",name:"Magic Mirror",type:SPELL_TYPE_MAGIC_SHIELD,priority:0,level:0,danger:10},
/* 4 */ {gesture:"DFFDD",name:"Lightning Bolt",type:SPELL_TYPE_DAMAGE,priority:1,level:1,danger:16},
/* 5 */ {gesture:"DFPW",name:"Cure Heavy Wounds",type:SPELL_TYPE_CURE,priority:0,level:1,danger:10},
/* 6 */ {gesture:"DFW",name:"Cure Light Wounds",type:SPELL_TYPE_CURE,priority:0,level:0,danger:10},
/* 7 */ {gesture:"DFWFd",name:"Blindness",type:SPELL_TYPE_CONFUSION,priority:4,level:0,danger:15},
/* 8 */ {gesture:"DPP",name:"Amnesia",type:SPELL_TYPE_CONFUSION,priority:6,level:1,danger:12},
/* 9 */ {gesture:"DSF",name:"Confusion/Maladroitness",type:SPELL_TYPE_CONFUSION,priority:6,level:0,danger:12},
/* 10*/ {gesture:"DSFFFc",name:"Disease",type:SPELL_TYPE_POISON,priority:4,level:1,danger:17},
/* 11*/ {gesture:"DWFFd",name:"Blindness",type:SPELL_TYPE_CONFUSION,priority:4,level:0,danger:15},
/* 12*/ {gesture:"DWSSSP",name:"Delay Effect",type:SPELL_TYPE_SPEC,priority:0,level:0,danger:10},
/* 13*/ {gesture:"DWWFWD",name:"Poison",type:SPELL_TYPE_POISON,priority:3,level:1,danger:18},
/* 14*/ {gesture:"FFF",name:"Paralysis",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:12},
/* 15*/ {gesture:"WFPSFW",name:"Summon Giant",type:SPELL_TYPE_SUMMON_MONSTER,priority:4,level:4,danger:14},
/* 16*/ {gesture:"FPSFW",name:"Summon Troll",type:SPELL_TYPE_SUMMON_MONSTER,priority:3,level:3,danger:13},
/* 17*/ {gesture:"PSFW",name:"Summon Ogre",type:SPELL_TYPE_SUMMON_MONSTER,priority:2,level:2,danger:12},
/* 18*/ {gesture:"SFW",name:"Summon Goblin",type:SPELL_TYPE_SUMMON_MONSTER,priority:1,level:1,danger:11},
/* 19*/ {gesture:"FSSDD",name:"Fireball",type:SPELL_TYPE_DAMAGE,priority:3,level:1,danger:15},
/* 20*/ {gesture:"P",name:"Shield",type:SPELL_TYPE_SHIELD,priority:0,level:0,danger:10},
/* 21*/ {gesture:"PDWP",name:"Remove Enchantment",type:SPELL_TYPE_REMOVE_ENCHANTMENT,priority:0,level:0,danger:13},
/* 22*/ {gesture:"PPws",name:"Invisibility",type:SPELL_TYPE_CONFUSION,priority:3,level:0,danger:0,danger:13},
/* 23*/ {gesture:"PSDD",name:"Charm Monster",type:SPELL_TYPE_CHARM_MONSTER,priority:0,level:0,danger:12},
/* 24*/ {gesture:"PSDF",name:"Charm Person",type:SPELL_TYPE_CONFUSION,priority:4,level:2,danger:13},
/* 25*/ {gesture:"PWPFSSSD",name:"Finger of Death",type:SPELL_TYPE_DAMAGE,priority:1,level:0,danger:20},
/* 26*/ {gesture:"PWPWWc",name:"Haste",type:SPELL_TYPE_HASTLE,priority:2,level:0,danger:10},
/* 27*/ {gesture:"SD",name:"Magic Missile",type:SPELL_TYPE_DAMAGE,priority:0,level:0,danger:10},
/* 28*/ {gesture:"SPFP",name:"Anti-spell",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:14},
/* 29*/ {gesture:"SPFPSDW",name:"Permanency",type:SPELL_TYPE_SPEC,priority:3,level:0,danger:16},
/* 30*/ {gesture:"SPPc",name:"Time Stop",type:SPELL_TYPE_HASTLE,priority:3,level:0,danger:10},
/* 31*/ {gesture:"SPPFD",name:"Time Stop",type:SPELL_TYPE_HASTLE,priority:3,level:0,danger:10},
/* 32*/ {gesture:"SSFP",name:"Resist Cold",type:SPELL_TYPE_RESIST,priority:4,level:1,danger:10},
/* 33*/ {gesture:"SWD",name:"Fear (No CFDS)",type:SPELL_TYPE_CONFUSION,priority:5,level:0,danger:12},
/* 34*/ {gesture:"SWWc",name:"Fire Storm",type:SPELL_TYPE_MASSIVE,priority:3,level:0,danger:16},
/* 35*/ {gesture:"WDDc",name:"Clap of Lightning",type:SPELL_TYPE_DAMAGE,priority:4,level:0,danger:15},
/* 36*/ {gesture:"WFP",name:"Cause Light Wounds",type:SPELL_TYPE_DAMAGE,priority:4,level:0,danger:12},
/* 37*/ {gesture:"WPFD",name:"Cause Heavy Wounds",type:SPELL_TYPE_DAMAGE,priority:3,level:0,danger:13},
/* 38*/ {gesture:"WPP",name:"Counter Spell",type:SPELL_TYPE_MAGIC_SHIELD,priority:0,level:0,danger:10},
/* 39*/ {gesture:"WSSc",name:"Ice Storm",type:SPELL_TYPE_MASSIVE,priority:0,level:3,danger:16},
/* 40*/ {gesture:"WWFP",name:"Resist Heat",type:SPELL_TYPE_RESIST,priority:0,level:4,danger:10},
/* 41*/ {gesture:"WWP",name:"Protection",type:SPELL_TYPE_SHIELD,priority:0,level:1,danger:10},
/* 42*/ {gesture:"WWS",name:"Counter Spell",type:SPELL_TYPE_MAGIC_SHIELD,priority:0,level:1,danger:10},
/* 43*/ {gesture:"p",name:"Surrender",type:SPELL_TYPE_SPEC,priority:-100,level:0,danger:0},
/* 44*/ {gesture:">",name:"Stab",type:SPELL_TYPE_STAB,priority:-1,level:0,danger:0}
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

    return spell.g.substr(spell.a + idx, 1);
}

function setGestureBySpell(warlock, spell) {
    /*if (!spell.g) {
        return;
    }*/

    var gesture = getGestureBySpell(spell);
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
    }

    console.log("destroyEnemySpell", enemy_spell, anti_spell);

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


function processBattle() {
    //console.log("processBattle", JSON.stringify(battle));

    var idx_enemy, idx_self;
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
    //console.log("processBattle", JSON.stringify(battle.self), JSON.stringify(battle.enemy));
    if (!battle.self.bsL.g) {
        battle.self.bsL = {id:-1,n:"",g:"XXX",t:1000,st:100,p:-100,h:1,l:100,a:3,ng:"X",th:0};
    }
    if (!battle.self.bsR.g) {
        battle.self.bsL = {id:-1,n:"",g:"XXX",t:1000,st:100,p:-100,h:2,l:100,a:3,ng:"X",th:0};
    }

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
    console.log("processBattle", JSON.stringify(battle));
    tSendOrderTimer.start();
    //sendOrderEx();

    //spellDecision(battle);
    //checkSpellCast(battle);
    //printBattle(battle, false);
    //return battle;
    //*/
}
