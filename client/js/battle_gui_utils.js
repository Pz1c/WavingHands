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
    GL = GL.toLowerCase();
    GR = GR.toLowerCase();
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

function strToArr2D(str, sep1, sep2, skip_empty) {
    var res = [];
    var arr1 = str.split(sep1);
    for (var i = 0, Ln = arr1.length; i < Ln; ++i) {
        if (arr1[i] === '') {
            continue;
        }

        res.push(arr1[i].split(sep2));
    }
    return res;
}

function cleanChildren(component) {
    console.log(component.id + " cleanShildren: " + component.children.length)
    for(var i = component.children.length; i > 0; i--) {
        console.log("destroying: " + i)
        component.children[i-1].destroy();
    }
}
