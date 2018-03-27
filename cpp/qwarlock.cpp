#include "qwarlock.h"

QWarlock::QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player)
{
    _name = Name;
    _status = Status;
    _leftGestures = LeftGestures;
    _rightGestures = RightGestures;
    _player = Player;
}

QWarlock::~QWarlock() {
    if (_possibleSpells.isEmpty()) {
        return;
    }
    foreach(QSpell *s, _possibleSpells) {
        delete s;
    }
    _possibleSpells.clear();
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

QString QWarlock::separatedString() {
    //int Ln = _leftGestures.length();
    qDebug() << "QWarlock::separatedString" << _name << _bestSpellL << _bestSpellR << _possibleSpells;
    QString l = _leftGestures.right(WARLOCK_GESTURE_LENGTH);
    QString r = _rightGestures.right(WARLOCK_GESTURE_LENGTH);
    QString res;
    foreach(QSpell *s, _possibleSpells) {
        if (!res.isEmpty()) {
            res.append(",");
        }
        res.append(s->json());
    }
    res.prepend("[").append("]");
    QString sbsL = "{}";
    QString sbsR = "{}";
    if (_bestSpellL) {
        sbsL = _bestSpellL->json();
        qDebug() << "sbsL" << sbsL;
    }
    if (_bestSpellR) {
        sbsR = _bestSpellR->json();
        qDebug() << "sbsR" << sbsR;
    }

    return QString("{\"name\":\"%1\",\"status\":\"%2\",\"L\":\"%3\",\"R\":\"%4\",\"spells\":%5,\"player\":%6,\"bsL\":%7,\"bsR\":%8}").
            arg(_name, _status, l, r, res, _player ? "true" : "false", sbsL, sbsR);
}

bool QWarlock::player() const
{
    return _player;
}

void QWarlock::setPossibleSpells(const QList<QSpell *> &possibleSpells)
{
    qDebug() << "QWarlock::setPossibleSpells" << possibleSpells;
    _possibleSpells = possibleSpells;
    checkSpells();
}

QString QWarlock::html(QString &posible_gestures) {
    qDebug() << "QWarlock" << posible_gestures;
    return "";
}

void QWarlock::checkSpells() {
    qDebug() << "QWarlock::checkSpells" << _name << _possibleSpells;
    _bestSpellL = 0;
    _bestSpellR = 0;
    foreach(QSpell *s, _possibleSpells) {
        if ((s->hand() == WARLOCK_HAND_LEFT) && !_bestSpellL) {
            _bestSpellL = s;
        }
        if ((s->hand() == WARLOCK_HAND_RIGHT) && !_bestSpellR) {
            _bestSpellR = s;
        }
        if (_bestSpellL && _bestSpellR) {
            break;
        }
    }
    qDebug() << "QWarlock::checkSpells" << _bestSpellL << _bestSpellR;
}
