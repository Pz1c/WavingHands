#include "qwarlock.h"

QWarlock::QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player)
{
    _name = Name;
    _status = Status;
    _leftGestures = LeftGestures;
    _rightGestures = RightGestures;
    _player = Player;
}

QString QWarlock::name() {
    return _name;
}

QString QWarlock::status() {
    return _status;
}

QString QWarlock::leftGestures() {
    return _leftGestures;
}

QString QWarlock::rightGestures() {
    return _rightGestures;
}

QString QWarlock::separatedString(QString &posible_spells) {
    //int Ln = _leftGestures.length();
    QString l = _leftGestures.right(WARLOCK_GESTURE_LENGTH);
    QString r = _rightGestures.right(WARLOCK_GESTURE_LENGTH);
    return QString("{\"name\":\"%1\",\"status\":\"%2\",\"L\":\"%3\",\"R\":\"%4\",\"spells\":%5,\"player\":}").arg(_name, _status, l, r, posible_spells, _player ? "true" : "false");
}

bool QWarlock::player() const
{
    return _player;
}

QString QWarlock::html(QString &posible_gestures) {
    int Ln = _leftGestures.length();
    QString res = QString("<tr><td colspan=%1>%2 Health: %3</td></tr>").arg(QString::number(Ln + 2), _name, _status);
    QString tr1 = "<tr><td>&nbsp;</td>";
    QString tr2 = "<tr><td>LH:</td>";
    QString tr3 = "<tr><td>RH:</td>";
    int start_idx = Ln <= 8 ? 0 : Ln - 8;
    for(int i = start_idx; i < Ln; ++i) {
        tr1.append(QString("<td>%1</td>").arg(QString::number(i)));
        tr2.append(QString("<td>%1</td>").arg(_leftGestures.at(i)));
        tr3.append(QString("<td>%1</td>").arg(_rightGestures.at(i)));
    }
    tr1.append("<td>&nbsp;</td>");
    QString lg;
    QString rg;
    QStringList ps = posible_gestures.split("#");
    int LCnt = 0, RCnt = 0;
    if (ps.count() == 0) {
        lg = "&nbsp;";
        rg = "&nbsp;";
    } else {
        foreach(QString s, ps) {
            if (s.isEmpty()) {
                continue;
            }
            QStringList pps = s.split(";");
            if (pps.count() != 2) {
                continue;
            }
            if (pps.at(0).compare("L") == 0) {
                lg.append(pps.at(1)).append(++LCnt % 2 == 0 ? "<br>" : " ");
            } else {
                rg.append(pps.at(1)).append(++RCnt % 2 == 0 ? "<br>" : " ");
            }
        }
    }

    tr2.append(QString("<td><font size=-3>%1</font></td>").arg(lg.trimmed()));
    tr3.append(QString("<td><font size=-3>%1</font></td>").arg(rg.trimmed()));
    return res.append(tr1).append("</tr>").append(tr2).append("</tr>").append(tr3).append("</tr>");
}
