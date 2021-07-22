#ifndef QWARLOCK_H
#define QWARLOCK_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include "qspell.h"
//#include "qwarlockspellchecker.h"
#include "qwarlockutils.h"
#include "qmonster.h"

#define WARLOCK_GESTURE_LENGTH 8

class QWarlock
{
public:
    QWarlock(QString Name, QString Status, QString LeftGestures, QString RightGestures, bool Player, bool isAI = false);
    QWarlock(QWarlock *CopyFrom);
    ~QWarlock();

    QString name();
    QString status();
    QString leftGestures();
    QString rightGestures();
    QString separatedString();
    bool player() const;

    void setPossibleSpells(const QList<QSpell *> &possibleSpells, QWarlock *enemy, const QList<QMonster *> &monsters);
    void checkSpells();

    QString possibleLeftGestures() const;
    QString possibleRightGestures() const;
    void setPossibleGestures(QString left, QString right);
    void emulateTurn(const QString &left, const QString &right);

    bool isParaFDF() const;
    void setIsParaFDF(bool isParaFDF);
    void setParalyzedHand(int Hand);

    bool isParaFC() const;
    void setIsParaFC(bool newIsParaFC);

    int maladroit() const;

    const QList<QSpell *> &possibleSpells() const;

    void setIsMaladroit(bool newIsMaladroit);

protected:
    void parseStatus();
    void checkPossibleGesture();
    QSpell *getSpellByFilter(const QList<QSpell *> &sl, int CastFrom, int CastTo, int SpellType, const QList<int> &notID, const QList<int> &byID) const;
    QSpell *getAntiSpell(const QList<QSpell *> &sl, const QSpell *s) const;

private:
    void setPriceForMap(QMap<QString, qreal> &L, QMap<QString, qreal> &R, const QString &NG, qreal price);

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
    int _disease;
    int _amnesia;
    int _maladroit;
    bool _surrender;
    bool _active;
    int _mshield;
    int _delay;
    int _time_stop;
    int _haste;
    int _permanency;
    int _blindness;
    int _invisibility;
    QString _name;
    QString _status;
    QString _leftGestures;
    QString _rightGestures;
    QString _possibleLeftGestures;
    QString _possibleRightGestures;
    bool _AI;
    bool _player;
    QList<QSpell *> _possibleSpells;
    QString _gestureR;
    QString _gestureL;
    QSpell *_bestSpellL;
    QSpell *_bestSpellR;
    bool _isParaFDF;
    bool _isParaFC;
    bool _isMaladroit;

};

#endif // QWARLOCK_H
