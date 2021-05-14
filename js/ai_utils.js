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

function getBotMsgByTurn(turn_num) {
    switch(turn_num) {
    case 1: return "Hi, warlock, I am a golem created for your training, try to beat me. Every letter represents a hand gesture. A set of gestures like DPP is a spell (Amnesia).";
    case 2: return "Here are some more useful spells: SD (Magic Missile) deals 1 damage. P (shield) will defend against it. But P/P with both hands will make you surrender.";
    case 3: return "Enchantments like DPP (Amnesia), DSF (Maladroit), SWD (Fear) disrupt your enemy. When two are cast at the same Warlock they cancel out.";
    case 4: return "Check your spell book to learn more spells...";
    case 5: return "Summon a goblin (SFW) to fight for you. Cast it on yourself, and don't forget to set it's target. Stab ('>') is a simple way to hit a goblin. Shield ('P') and Protection (WWP) to defend against it.";
    case 6: return "Inflict direct damage with Cause light or heavy wounds (WFP, WPFD), and heal up (DFW, DFPW)";
    case 7: return "There are two ways to counter a spell (WPP, WWS). Cast them at yourself to prevent spells and monsters from hitting you. Cast at an opponent to prevent him from summoning a monster.";
    case 8: return "You're doing good! Keep up. When you get a bit more training you can start playing against other players and friends.";
    case 9: return "Games with other players are played in turns. You can play up-to 3 games at the same time for free, and you're given up to 3 days to play each turn.";
    case 10: return "When you start playing against real players, you'll be seeing a lot of these spells: Charm Person (PSDF) to force a gesture (or a no gesture '-'), and Charm Monster (PSDD) to take control of a monster, and change it's target";
    case 11: return "You will also see players concealing their movements with Invisibility (PPws - notice the lower caps means you should w and s and both hands together)";
    case 12: return "Players will summon stronger monsters in addition to Goblins (SFW) - like Ogre (PSFW), Troll (FPSFW) and Giant (WFPSFW) - noticed the pattern ?";
    case 13: return "The most powerful spell around? That's Finger of Death (PWPFSSSD) to instantly kill an opponent. It can't be countered, but it can be mirrored (cw)";
    case 14: return "Playing with friends can be a lot more fun. Send them an invite. You can set up a private game to play together.";
    case 15: return "Every time you win, you can go up the ladder board. When you win against powerful warlocks your elo score will go up too to reflect your skill.";
    case 16: return "In melee games many 3-6 players play at the same time, often using Fire and Ice Elementals (cWSSW, cSWWS), Storms (SWWc, WSSc) and Protections (WWFP, SSFP)";
    case 17: return "Want to learn more? checkout the full rules (https://games.ravenblack.net/rules) and join our Facebook community (https://fb.com/WarlocksDuel/)";
    default: return "";
    }
}

function copyObject(from, to) {
    for(var key in from) {
        to[key] = from[key];
    }
}

function prepareWarlock(container, spell_list, clean) {
    container.gL = '';
    container.gR = '';
    container.LnotIds = [];
    container.RnotIds = [];
    container.spellbook = [];
    for (var i = 0, Ln = spells.length; i < Ln; ++i) {
        container.spellbook[i] = {t:99,idx:-1};
    }

    var new_spell = [], ls;
    for (var s = 0, LnS = spell_list.length; s < LnS; ++s) {
        ls = spell_list[s];
        if (clean && ls.active && (ls.t < ls.g.length)) {
            new_spell.push(ls);
        } else {
            if (ls.t < container.spellbook[ls.id].t) {
                container.spellbook[ls.id].t = ls.t;
                container.spellbook[ls.id].idx = s;
            }
        }
    }
    if (clean) {
        container.spells = new_spell;
    }
}

function prepareBattleWarlock() {
    console.log("prepareBattleWarlock");
    var s, LnS, ls;
    for (var i = 0, Ln = battle.warlocks.length; i < Ln; ++i) {
        if (battle.warlocks[i].player) {
            ai.self = battle.warlocks[i];
            prepareWarlock(ai.self, battle.warlocks[i].spells, false);
        } else if ((battle.warlocks[i].active) && !ai.enemy) {
            ai.enemy = battle.warlocks[i];
            prepareWarlock(ai.enemy, ai.enemy.spells, true);
        }
    }

    console.log("processBattle", "enemy", JSON.stringify(ai.enemy));
    console.log("processBattle", "self", JSON.stringify(ai.self));
    if (!ai.self.bsL.g) {
        ai.self.bsL = {id:-1,name:"",g:"XXX",t:1000,st:100,p:-100,h:1,l:100,a:3,ng:"X",th:0};
    }
    if (!ai.self.bsR.g) {
        ai.self.bsL = {id:-1,name:"",g:"XXX",t:1000,st:100,p:-100,h:2,l:100,a:3,ng:"X",th:0};
    }
}

function calculateHP(al) {
    if (ai.self.hp <= 10) {
        al.heal += 1;
    } else if (ai.self.hp <= 5) {
        al.heal += 2;
    }
}

function getDamageByName(monster_name) {
    var hand = (monster_name.indexOf("LH:") !== -1 ? WARLOCK_HAND_LEFT : WARLOCK_HAND_RIGHT), s;
    for (var i = 0, Ln = battle.enemy.spells.length; i < Ln; ++i) {
        s = battle.enemy.spells[i];
        if ((s.hand !== hand) || (s.id < 0) || (s.t !== 1)) {
            continue;
        }
        if (spells[s.id].type !== SPELL_TYPE_SUMMON_MONSTER) {
            continue;
        }

        return spells[s.id].dmg;
    }
    return 0;
}

function calculateMonster(al) {
    var ep = 0, sp = 0, m;
    for(var i = 0, Ln = battle.actions.M.length; i < Ln; ++i) {
        m = battle.actions.M[i];
        if (m.d === -1) {
            if (m.under_control) {
                continue;
            } else {
                m.d = getDamageByName(d.name);
            }
        }

        if (m.under_control) {
          ep += m.d;
        } else {
          sp += m.d;
        }
    }
    al.monster = sp - ep;
    if (al.monster <= 0) {
        return;
    }
    if (battle.self.shield > 1) { // SPELL_PROTECTION
        al.monster -= battle.self.shield;
    }
    if (battle.self.invisibility > 0) {
        al.monster -= battle.self.invisibility;
    }
}

function calculateSpell(al) {
    for (var i = 0, Ln = battle.enemy.spells.length; i < Ln; ++i) {
        s = battle.enemy.spells[i];

        if ((s.id === SPELL_FINGER_OF_DEATH) && (s.t <= 5)) {
            al.confuse_enemy += 7 - s.t;
            continue;
        }

        if ((spells[s.id].type === SPELL_TYPE_SUMMON_MONSTER) && (spells[s.id].l >= 2) && (spells[s.id].t <= 2)) {
            al.confuse_enemy += spells[s.id].l;
            continue;
        }

        if (spells[s.id].type === SPELL_TYPE_CONFUSION) {
            al.confuse += (spells[s.id].length - s.t - 1);
            if ((s.t === 1) && ((s.id === SPELL_PARALYSIS) || (s.id === SPELL_PARALYSIS_FDF) || (s.id === SPELL_PARALYSIS_FDFD))) {
                al.confuse += 3;
            }
            continue;
        }

        if ((battle.self.fireproof > 0) && ((s.id === SPELL_FIRE_STORM) || (s.id === SPELL_SUMMON_FIRE_ELEMENTAL) || (s.id === SPELL_FIREBALL))) {

        }

        if (s.id === SPELL_FIRE_STORM) {

            continue;
        }



    }
}

function calculateDanger() {
    var al = {heal:0,monster:0,confuse:0,damage:0,confuse_enemy:1};
    //calculateHP(al);
    //calculateMonster();
    console.log("AI.calculateDanger", JSON.stringify(al));
    ai.action_list = al;
}

/*
    // L left  obj
    // R Right obj
    // C Chat  text
    // D Delay int 0 - none, 1 left, 2 right, -1 unavailable
    // P Permanent int 0 - none, 1 left, 2 right, -1 unavailable
    // F Fire int 0, 1, -1
    // M monsters arr of obj
    // CP - paralyze arr of obj by id
    // CC - paralyze arr of obj by id
    battle.actions = {L:{target:"Default"},R:{target:"Default"},C:"",D:-1,P:-1,F:-1,M:[],CP:{},CC:{}};

*/

var ai, battle;

function processBattle(Battle, isAI) {
    try {
        ai = {};
        battle = Battle;
        arr_spells = JSON.parse(mainWindow.gameCore.getSpellBook());
        //console.log("processBattle", isAI, JSON.stringify(arr_spells));
        console.log("processBattle", JSON.stringify(battle));
        prepareBattleWarlock();
        calculateDanger();
        console.log("processBattle", "complete", JSON.stringify(ai));
    } catch(error) {
        console.error(error);
    }

    if (isAI) {
        battle.actions.C = getBotMsgByTurn(battle.turn_num);
    } else {
        console.log("processBattle", "not AI");
    }

}
