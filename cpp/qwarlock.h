#ifndef QWARLOCK_H
#define QWARLOCK_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include "qspell.h"
//#include "qwarlockspellchecker.h"
//#include "qwarlockutils.h"
#include "qmonster.h"

#define WARLOCK_GESTURE_LENGTH 8

class QWarlockSpellChecker;

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

    void setPossibleSpells(const QList<QSpell *> &possibleSpells);
    QString possibleLeftGestures() const;
    QString possibleRightGestures() const;
    void setPossibleGestures(QString left, QString right);
    void emulateTurn(const QString &left, const QString &right);

    bool isParaFDF() const;
    void setIsParaFDF(bool isParaFDF);
    void setParalyzedHand(int Hand, const QString &Gesture = "", bool SetPossibleGesture = true);

    bool isParaFC() const;
    void setIsParaFC(bool newIsParaFC);

    int maladroit() const;

    const QList<QSpell *> &possibleSpells() const;

    void setIsMaladroit(bool newIsMaladroit);

    void processDecision(QWarlock *enemy, QList<QMonster *> &monsters, const QString &paralyzed, const QString &charmed);

    const QString &id() const;
    void setId(const QString &newId);

    int isSummoning() const;

    int paralized() const;

    QString printOrders(QMap<QString, QString> &name_to_id) const;

    int forcedHand() const;

    void setMagicBookLevel(int newMagicBookLevel);

    int magicBookLevel() const;

    bool AI() const;

protected:
    void analyzeMonster(QList<QMonster *> &monsters, QWarlock *enemy);
    void processMonster(QList<QMonster *> &monsters, QWarlock *enemy);
    void analyzeEnemy(QWarlock *enemy, const QString &paralyzed, const QString &charmed);
    void breakEnemy(QWarlock *enemy);
    void processMaladroit();
    void attackEnemy(QWarlock *enemy);
    void validateSpellForTurn();
    void parseStatus();
    void checkPossibleGesture();
    void fillTurnToCast();
    void setSpellPriority(QWarlock *enemy, const QList<QMonster *> &monsters);
    void checkSpells();
    void targetSpell(const QWarlock *enemy, const QList<QMonster *> &monsters);
    QSpell *getSpellByFilter(const QList<QSpell *> &sl, int CastFrom, int CastTo, int SpellType, const QList<int> &notID, const QList<int> &byID, int Hand = -1) const;
    QSpell *getAntiSpell(const QList<QSpell *> &sl, const QSpell *s, const QWarlock *enemy) const;
    bool checkAntiSpell(const QSpell *as, const QSpell *s) const;
    QString getTargetForSpell(const QSpell *spell, const QWarlock *enemy, const QList<QMonster *> &monsters);
    int getTurnToCastBySpellID(int spell_id, int def_value = 5, int hand = -1) const;

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
    QString _id;
    QString _name;
    QString _status;
    QString _leftGestures;
    QString _rightGestures;
    QString _possibleLeftGestures;
    QString _possibleRightGestures;
    QString _banked_spell;
    bool _AI;
    bool _player;
    QList<QSpell *> _possibleSpells;
    QString _gestureL;
    QString _gestureR;
    QString _spellL;
    QString _spellR;
    QString _targetL;
    QString _targetR;
    QSpell *_bestSpellL;
    QSpell *_bestSpellR;
    int _forcedHand;
    QString _forcedGesture;
    bool _isParaFDF;
    bool _isParaFC;
    bool _isMaladroit;
    bool _charmMonsterLeft;
    bool _charmMonsterRight;
    QMap<int, QSpell *> _turnToCast;
    int _elemental;
    int _totalFriendlyAttack;
    int _totalEnemyAttack;
    int _totalEnemyHP;
    bool _enemyParalyze;
    int _magicBookLevel;
    QWarlockSpellChecker *_SpellChecker;
};

#endif // QWARLOCK_H
