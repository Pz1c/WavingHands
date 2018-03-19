var idx_l = 0, idx_r = 1;
var arr_warlock = ['enemy', 'self'];
var arr_hand = ['L', 'R'];

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
/* 43*/ {gesture:"p",name:"Surrender",type:SPELL_TYPE_SPEC,priority:-10,level:0,danger:0},
/* 44*/ {gesture:">",name:"Stab",type:SPELL_TYPE_STAB,priority:-1,level:0,danger:0}
];

function checkSpellChar(left, right, spell) {
    if (left !== spell.toUpperCase()) {
        return false;
    }
    if ((spell.toUpperCase() !== spell) && (left !== right)) {
        return false;
    }
    return true;
}

function checkStriktSpell(left, right, spell) {
    var Ln = spell.length;
    if (left.length < Ln) {
        return false;
    }

    for (var i = 0; i < Ln; ++i) {
        if (!checkSpellChar(left.substr(i, 1), right.substr(i, 1), spell.substr(i, 1))) {
            return false;
        }
    }

    return true;
}

function checkSpellPosible(left, right, spell, strict) {
    //console.log('checkSpellPosible', left, right, spell);
    if (strict) {
      return checkStriktSpell(left, right, spell) ? 0 : -1;
    }

    var Ln = spell.length - 1, GLn = left.length < right.length ? left.length : right.length;
    if (Ln == 0) {
        return 0;
    }
    var work_spell = spell.substr(0, Ln);
    //console.log('checkSpellPosible', work_spell, Ln, GLn);
    var perhapse;
    var spell_idx;
    var spell_char;
    for (var i = 0; i < Ln; ++i) {
        perhapse = true;
        spell_idx = -1;
        for (var j = Ln - i; j > 0; --j) {
            spell_char = work_spell.substr(++spell_idx, 1);
            left_char = left.substr(GLn - j, 1);
            right_char = right.substr(GLn - j, 1);
            //console.log(i, spell_idx, 'L=' + left_char, 'R=' + right_char, 'S=' + spell_char);
            if ((GLn - j < 0) || !checkSpellChar(left_char, right_char, spell_char)) {
                perhapse = false;
                break;
            }
        }
        if (perhapse) {
            return i + 1;
        }
    }
    return -1;
}

function checkPossibleSpells(battle) {
  battle.enemy.possible_spell = [];
  battle.self.possible_spell = [];
  for (var i = 0, Ln = arr_spells.length; i < Ln; ++i) {
    //console.log('checkPossibleSpells', i, arr_spells[i].gesture, arr_spells[i].name);
    for (var w = 0; w < 2; ++w) {
      for (var h = 0; h < 2; ++h) {
        var turn_to_cast = checkSpellPosible(h === 0 ? battle[arr_warlock[w]].left : battle[arr_warlock[w]].right, h === 0 ? battle[arr_warlock[w]].right : battle[arr_warlock[w]].left, arr_spells[i].gesture, false);
        //console.log(arr_warlock[w], arr_hand[h], turn_to_cast);
        if (turn_to_cast > 0) {
          battle[arr_warlock[w]].possible_spell.push({spell_id:i,turn:turn_to_cast,hand:arr_hand[h]});
        }
      }
    }
  }
  //console.log('checkPossibleSpells', battle);
}

function calcSpellPriority(warlock_code, priority, danger, turn) {
  if (warlock_code === arr_warlock[1]) {// self
    return priority - turn;
  } else { // enemy
    return danger - turn;
  }
}

function checkWarlockSpells(warlock_code, battle) {
  battle[warlock_code].priority_max_L = 0;
  battle[warlock_code].priority_max_R = 0;
  battle[warlock_code].priority_sum_L = 0;
  battle[warlock_code].priority_sum_R = 0;

  var sps = battle[warlock_code].possible_spell;
  for (var i = 0, Ln = sps.length; i < Ln; ++i) {
    var spell = arr_spells[sps[i].spell_id];
    for (var h = 0; h < 2; ++h) {
      var priority = calcSpellPriority(warlock_code, spell.priority, spell.danger, sps[i].turn);
      battle[warlock_code]['priority_sum_' + arr_hand[h]] += priority;
      if (!battle[warlock_code]['priority_max_' + arr_hand[h]] || (battle[warlock_code]['priority_max_' + arr_hand[h]] < priority)) {
        battle[warlock_code]['priority_max_' + arr_hand[h]] = priority;
      }
      battle[warlock_code].possible_spell[i].level = spell.level;
      battle[warlock_code].possible_spell[i].priority = priority;
    }
  }
}

function checkMostPossibleSpells(warlock_code, battle) {
  battle[warlock_code].spell_L = {};
  battle[warlock_code].spell_R = {};
  var sps = battle[warlock_code].possible_spell;
  for (var i = 0, Ln = sps.length; i < Ln; ++i) {
    for (var h = 0; h < 2; ++h) {
      if (sps[i].hand !== arr_hand[h]) {
        continue;
      }
      if (sps[i].priority === battle[warlock_code]['priority_max_' + arr_hand[h]]) {
        battle[warlock_code]['spell_' + arr_hand[h]] = sps[i];
      }
    }
  }
  console.log('checkMostPossibleSpells', warlock_code, battle[warlock_code].spell_L, battle[warlock_code].spell_R);
}

function getChipHand(warlock) {
  return warlock.priority_sum_L > warlock.priority_sum_R ? 'L' : 'R';
}

function checkIsPossibleDestroySpell(battle, enemy_spell) {
  var ps = battle.self.possible_spell;
  for (var i = 0, Ln = ps.length; i < Ln; ++i) {
    var pss = battle.self.possible_spell[i];
    switch(arr_spells[pss.spell_id].type) {
      case SPELL_TYPE_REMOVE_ENCHANTMENT:
      case SPELL_TYPE_MAGIC_SHIELD:
        if (pss.turn === enemy_spell.turn) {
          return pss;
        }
        break;
      case SPELL_TYPE_CONFUSION:
        if (pss.turn + 1 >= enemy_spell.turn) {
          return pss;
        }
        break;
    }
  }
  if (enemy_spell.turn >= 4) {
    return {spell_id:SPELL_AMNESIA,turn:3,hand:getChipHand(battle.self)};
  }
  if (enemy_spell.turn === 3) {
    return {spell_id:SPELL_COUNTER_SPELL1,turn:3,hand:getChipHand(battle.self)};
  }
  if (enemy_spell.turn === 2) {
    return {spell_id:SPELL_MAGIC_MIRROR,turn:2,hand:'L'};
  }

  return 0;
}

function setGestureBySpell(battle, self_spell) {
  console.log('setGestureBySpell', self_spell);
  var gesture = arr_spells[self_spell.spell_id].gesture;
  var chr = gesture.substr(gesture.length - self_spell.turn, 1);
  if (chr.toUpperCase() === chr) {
    battle.self['gesture_' + self_spell.hand] = chr;
  } else {
    battle.self.gesture_L = chr.toUpperCase();
    battle.self.gesture_R = battle.self.gesture_L;
  }
}

function getAntiSpell(battle, enemy_spell) {
  var asd = checkIsPossibleDestroySpell(battle, enemy_spell);
  if (asd) {
    setGestureBySpell(battle, asd);
  }
  /*switch(enemy_spell.type) {
    //case SPELL_TYPE_CHARM_MONSTER:
    case SPELL_TYPE_CONFUSION:
    case SPELL_TYPE_DAMAGE:
    case SPELL_TYPE_POISON:
    case SPELL_TYPE_DEATH:

      break;
    case SPELL_TYPE_MASSIVE:
      break;
  }*/
}

function spellDecision(battle) {
  battle.self.gesture_L = '';
  battle.self.gesture_R = '';
  getAntiSpell(battle, battle.enemy.spell_L.turn < battle.enemy.spell_R.turn ? battle.enemy.spell_L : battle.enemy.spell_R);
  getAntiSpell(battle, battle.enemy.spell_L.turn < battle.enemy.spell_R.turn ? battle.enemy.spell_R : battle.enemy.spell_L);

  if (battle.self.gesture_L === '') {
    console.log('spellDecision', 'L', battle.self.spell_L);
    if (!battle.self.spell_L) {
      battle.self.spell_L = {spell_id:SPELL_CONFUSION,hand:'L',turn:3};
    }
    setGestureBySpell(battle, battle.self.spell_L);
  }
  if (battle.self.gesture_R === '') {
    console.log('spellDecision', 'R', battle.self.spell_R);
    if (!battle.self.spell_R) {
      battle.self.spell_R = {spell_id:SPELL_CHARM_PERSON,hand:'R',turn:4};
    }
    setGestureBySpell(battle, battle.self.spell_R);
  }

  console.log('spellDecision', 'L=' + battle.self.gesture_L, 'R=' + battle.self.gesture_R);
}

function checkSpellCast(battle) {
  battle.self.cast_spell_L = '';
  battle.self.cast_spell_R = '';

  var new_L = battle.self.left + battle.self.gesture_L;
  var new_R = battle.self.right + battle.self.gesture_R;
  battle.self.spell_L = [];
  battle.self.spell_L_max_priority = 0;
  battle.self.spell_L_max_priority_idx = -1;
  battle.self.spell_R = [];
  battle.self.spell_R_max_priority = 0;
  battle.self.spell_R_max_priority_idx = -1;
  for(var i, Ln = arr_spells.length; i < Ln; ++i) {
    if (checkSpellPosible(new_L, new_R, arr_spells[i].gesture, true) >= 0) {
      battle.self.spell_L.push(i);
      if (battle.self.spell_L_max_priority < arr_spells[i].priority) {
        battle.self.spell_L_max_priority = arr_spells[i].priority;
        battle.self.spell_L_max_priority_idx = battle.self.spell_L.length - 1;
      }
    }
    if (checkSpellPosible(new_R, new_L, arr_spells[i].gesture, true) >= 0) {
      battle.self.spell_R.push(i);
      if (battle.self.spell_R_max_priority < arr_spells[i].priority) {
        battle.self.spell_R_max_priority = arr_spells[i].priority;
        battle.self.spell_R_max_priority_idx = battle.self.spell_R.length - 1;
      }
    }
  }
  if (battle.self.spell_L_max_priority_idx != -1) {
    battle.self.cast_spell_L = arr_spells[battle.self.spell_L[battle.self.spell_L_max_priority_idx]].name;
  }
  if (battle.self.spell_R_max_priority_idx != -1) {
    battle.self.cast_spell_R = arr_spells[battle.self.spell_R[battle.self.spell_R_max_priority_idx]].name;
  }
  console.log('checkSpellCast', battle.self.cast_spell_L, battle.self.cast_spell_R);
}

function printBattle(battle) {
  var i = 0, Ln = 0;
  for (i = 0, Ln = battle.enemy.possible_spell.length; i < Ln; ++i) {
    console.log('enemy', battle.enemy.left, battle.enemy.right, arr_spells[battle.enemy.possible_spell[i].spell_id].gesture, arr_spells[battle.enemy.possible_spell[i].spell_id].name, battle.enemy.possible_spell[i]);
  }
  for (i = 0, Ln = battle.self.possible_spell.length; i < Ln; ++i) {
    console.log('self', battle.self.left, battle.self.right, arr_spells[battle.self.possible_spell[i].spell_id].gesture, arr_spells[battle.self.possible_spell[i].spell_id].name, battle.self.possible_spell[i]);
  }

}


function processBattle(battle) {
  checkPossibleSpells(battle);
  for (i = 0; i < 2; ++i) {
    checkWarlockSpells(arr_warlock[i], battle);
    checkMostPossibleSpells(arr_warlock[i], battle);
  }

  spellDecision(battle);
  checkSpellCast(battle);
  //printBattle(battle, false);
  return battle;
}
