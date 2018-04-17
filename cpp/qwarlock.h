#ifndef QWARLOCK_H
#define QWARLOCK_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include "qspell.h"
//#include "qwarlockspellchecker.h"
#include "qwarlockutils.h"

#define WARLOCK_GESTURE_LENGTH 8

class QWarlock
{
public:
    QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player);
    ~QWarlock();

    QString name();
    QString status();
    QString leftGestures();
    QString rightGestures();
    QString separatedString();
    bool player() const;

    void setPossibleSpells(const QList<QSpell *> &possibleSpells, QWarlock *enemy = 0);
    void checkSpells();


    QString possibleLeftGestures() const;
    QString possibleRightGestures() const;
    void setPossibleGestures(QString left, QString right);

protected:
    void breakEnemySpell(QSpell *spell);
    void setAntispell(QWarlock *enemy = 0);
    void setSpellPriority(QWarlock *enemy = 0);
    void parseStatus();
    void checkPossibleGesture();
    int strValueToInt(QString val);
private:
    int _scared;
    int _confused;
    int _charmed;
    int _paralized;
    int _shield;
    int _coldproof;
    int _fireproof;
    int _hp;
    int _poison;
    int _desease;
    int _amnesia;
    int _maladroit;
    QString _name;
    QString _status;
    QString _leftGestures;
    QString _rightGestures;
    QString _possibleLeftGestures;
    QString _possibleRightGestures;
    bool _player;
    QList<QSpell *> _possibleSpells;
    QString _gestureR;
    QString _gestureL;
    QSpell *_bestSpellL;
    QSpell *_bestSpellR;
};

#endif // QWARLOCK_H
