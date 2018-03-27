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

function findSpell(spell_list, search_types, search_ids, not_ids, turn_from, turn_to, hands) {
    var all_ids = search_ids.length === 0;
    var all_types = search_types.length === 0;
    var all_hands = hands.length === 2;
    var not_ids_empty = not_ids.length === 0;
    for (var i = 0, Ln = spell_list.length; i < Ln; ++i) {
        if (!all_types && (search_types.indexOf(spell_list[i].st) === -1)) {
            continue;
        }
        if (!all_ids && (search_ids.indexOf(spell_list[i].id) === -1)) {
            continue;
        }
        if (!not_ids_empty && (not_ids.indexOf(spell_list[i].id) !== -1)) {
            continue;
        }
        if (!all_hands && (hands.indexOf(spell_list[i].h) === -1)) {
            continue;
        }
        if ((spell_list[i].t < turn_from) || (spell_list[i].t > turn_to)) {
            continue;
        }
        return spell_list[i];
    }
    return false;
}

function setGestureBySpell(warlock, spell) {

}

function destroyEnemySpell(self, enemy_spell) {
  var hands = [];
  if (self.gL === '') {
      hands.push(WARLOCK_HAND_LEFT);
  }
  if (self.gR === '') {
      hands.push(WARLOCK_HAND_RIGHT);
  }
  if (hands.length === 0) {
      return false;
  }

  switch(enemy_spell.st) {
    case SPELL_TYPE_SUMMON_MONSTER:
        var spell = findSpell(self.spells, [SPELL_TYPE_CHARM_MONSTER, SPELL_TYPE_MAGIC_SHIELD], [], [SPELL_MAGIC_MIRROR], enemy_spell.t, enemy_spell.t, hands);
        if (spell) {
            setGestureBySpell(self, spell);
        }
    break;
    case SPELL_TYPE_POISON:
    break;
    case SPELL_TYPE_CONFUSION:
    break;
    case SPELL_TYPE_DAMAGE:
    break;
    case SPELL_TYPE_SHIELD:
    break;
    case SPELL_TYPE_MAGIC_SHIELD:
    break;
    case SPELL_TYPE_MASSIVE:
    break;
    case SPELL_TYPE_HASTLE:
    break;
    case SPELL_TYPE_CHARM_MONSTER:
    break;
    case SPELL_TYPE_CURE:
    break;
    case SPELL_TYPE_SPEC:
    break;
    case SPELL_TYPE_DEATH:
    break;
    case SPELL_TYPE_RESIST:
    break;
    case SPELL_TYPE_ELEMENTAL:
    break;
    case SPELL_TYPE_REMOVE_ENCHANTMENT:
    break;
    case SPELL_TYPE_STAB:
    break;
  }
}


function processBattle(battle) {
  for (var i = 0, Ln = battle.warlocks; i < Ln; ++i) {
    if (battle.warlocks[i].player) {
        battle.self = battle.warlocks[i];
    } else {
        battle.enemy = battle.warlocks[i];
    }
  }
  battle.self.gL = '';
  battle.self.gR = '';
  var left_processed = false;
  if ((battle.enemy.bsL.t > 0) && (!battle.enemy.bsR.t || (battle.enemy.bsL.p > battle.enemy.bsR.p))) {
    left_processed = true;
    destroyEnemySpell(battle.self, battle.enemy.bsL);
  }
  if (battle.enemy.bsR.t > 0) {
    destroyEnemySpell(battle.self, battle.enemy.bsR);
  }
  if (!left_processed && (battle.enemy.bsL.t > 0)) {
    destroyEnemySpell(battle.self, battle.enemy.bsL);
  }


  //spellDecision(battle);
  //checkSpellCast(battle);
  //printBattle(battle, false);
  //return battle;
  //*/
}
