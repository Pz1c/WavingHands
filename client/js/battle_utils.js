var battle = {};

function copyObject(from, to, except) {
    var check_exclude = except && Array.isArray(except) && (except.length > 0);
    for(var key in from) {
        if (check_exclude && except.include(key)) {
            continue;
        }

        to[key] = from[key];
    }
}

function preparePrintGestures(GL, GR) {
    var res = [], item, ll, lr;
    for(var i = 1, Ln = GL.length; i < Ln; ++i) {
        ll = GL.substr(i, 1);
        lr = GR.substr(i, 1);
        if (ll === ' ') {
            ll = '_';
        }
        if (lr === ' ') {
            lr = '_';
        }

        item = {l:'g_'+ll, r: 'g_' + lr, lv: ll !== '_', rv: lr !== '_'};
        res.push(item);
    }
    return res;
}


function prepareWarlock(w) {
    w.monsters = battle.monsters[w.name];
    w.print_g = preparePrintGestures(w.L, w.R);

    return w;
}

function parseTargets(targets_str) {
    battle.targetsList = [];
    battle.targetsMap = [];
    var strs = targets_str.split('#'), trgs;
    for(var i = 0, Ln = strs.length; i < Ln; ++i) {
        trgs = strs[i].split(",");
        battle.targetsList.push(trgs[1]);
        battle.targetsMap[trgs[0]] = trgs[1];
    }
}

function prepareBattle(raw_battle) {
    battle.warlocks = [];
    battle.elemental = {hp:0,type:"fire"};
    battle.monsters = [];

    var i, Ln;
    for(i = 0, Ln = raw_battle.monsters.length; i < Ln; ++i) {
        var m = raw_battle.monsters[i];
        if (!m.owner) {
            battle.elemental = m;
            battle.elemental.type = m.name.indexOf("Fire") !== -1 ? "fire" : "ice";
            continue;
        }

        if (!battle.monsters[m.owner]) {
            battle.monsters[m.owner] = [];
        }
        battle.monsters[m.owner].push(m);
    }

    for (i = 0, Ln = raw_battle.warlocks.length; i < Ln; ++i) {
        var w = raw_battle.warlocks[i];
        if (!battle.monsters[w.name]) {
            battle.monsters[w.name] = [];
        }
        prepareWarlock(w);
        battle.warlocks.unshift(w);
    }

    parseTargets(raw_battle.targets);
}
