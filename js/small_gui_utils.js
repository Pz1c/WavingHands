var paraCFMap = {"C":"F","S":"D","W":"P"};
var paraFCMap = {"F":"C","S":"D","W":"P"};

function getNextParalyzedGesture(LastGesture, IsFC) {
    var res = IsFC ? paraFCMap[LastGesture] : paraCFMap[LastGesture];
    if (res) {
        return res;
    }
    return LastGesture;
}

function getIconByGesture(G, with_prefix) {
    //if (G === ' ') {
        //G = '-';
    //}
    if (G === '>') {
        G = '_';
    }
    if (G === '?') {
        G = '=';
    }
    var res = '';
    if (with_prefix) {
        res += "g_";
    }
    res += G.toLowerCase();
    return res;
}

function getFullIconPathByGesture(G) {
    return "qrc:/res/g_" + getIconByGesture(G) + ".png";
}
