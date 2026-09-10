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

// Flaws a finished decision can carry, reported as a bitmask by checkDecision(). Kept as a
// free predicate over a plain value so a turn can be judged in a test without a battle, a
// network or a QWarlock. Restores the job isSpellsNormal() did in js/ai_utils.js before
// commit 4ef4b8a: validate the chosen PAIR once more before it is sent.
//
// Note what is NOT a flaw: a hand making no gesture is legal - QWarloksDuelCore::sendOrders
// splits "LH$" into ("LH", "") and posts &LH=, exactly as the human client does. An idle
// hand is a wasted hand, not an illegal one, so it is reported separately from the rest.
enum QDecisionFlaw {
    DF_NONE                 = 0,
    DF_MALADROIT_SPLIT      = 1 << 0, // maladroitness forces both hands to the same gesture
    DF_ILLEGAL_GESTURE_L    = 1 << 1, // gesture outside what the hand may make this turn
    DF_ILLEGAL_GESTURE_R    = 1 << 2,
    DF_UNMIRRORED_BOTH_HAND = 1 << 3, // a both-hands (lowercase) gesture the other hand did not match
    DF_SURRENDER            = 1 << 4, // P with both hands
    DF_PAIR_CONFLICT        = 1 << 5, // the two chosen spells cannot share this turn
    DF_IDLE_HAND_L          = 1 << 6, // legal, but the hand contributes nothing
    DF_IDLE_HAND_R          = 1 << 7
};

// Everything checkDecision() needs. Spell pointers are borrowed, never owned.
struct QDecision {
    QString gestureL, gestureR;
    const QSpell *bestL;
    const QSpell *bestR;
    QString allowedL, allowedR;
    int maladroit;

    QDecision() : bestL(nullptr), bestR(nullptr), maladroit(0) {}
};

// Pure: reads only its argument, writes nothing, allocates nothing.
int checkDecision(const QDecision &d);
QString decisionFlawsToString(int flaws);

// One probe of the anti-spell table: which of our own spells could answer a given
// enemy spell, and the window of turns in which the answer has to land. Ported from
// getAntispellFilter() in js/ai_utils.js, which was removed in 4ef4b8a.
// Empty searchTypes / searchIDs mean "no restriction on this axis".
struct QAntiSpellRule {
    QList<int> searchTypes;
    QList<int> searchIDs;
    QList<int> notIDs;
    int fromTurn;
    int toTurn;
};

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

    // The hand that is ALREADY paralysed on this warlock, carried over from a
    // previous turn. WARLOCK_HAND_NONE when it is not currently paralysed.
    int lockedParalyzedHand() const;
    void setLockedParalyzedHand(int Hand);

    void setMagicBookLevel(int newMagicBookLevel);

    int magicBookLevel() const;

    bool AI() const;

    // Snapshot of the decision this warlock has just made, and its flaw bitmask. Public so a
    // test harness can score a turn without reparsing printOrders().
    QDecision decision() const;
    int decisionViolations() const;

protected:
    void analyzeMonster(QList<QMonster *> &monsters, QWarlock *enemy);
    void processMonster(QList<QMonster *> &monsters, QWarlock *enemy);
    void analyzeEnemy(QWarlock *enemy, const QString &paralyzed, const QString &charmed);
    void breakEnemy(QWarlock *enemy);
    void processMaladroit();
    void attackEnemy(QWarlock *enemy);
    bool spellsConflict(const QSpell *left, const QSpell *right) const;
    bool gestureAllowed(const QString &gesture, int hand) const;
    QString firstAllowedGesture(int hand, const QString &avoid) const;
    void validateSpellForTurn();
    void parseStatus();
    void checkPossibleGesture();
    void fillTurnToCast();
    void setSpellPriority(QWarlock *enemy, const QList<QMonster *> &monsters);
    void checkSpells();
    void targetSpell(const QWarlock *enemy, const QList<QMonster *> &monsters);
    QSpell *getSpellByFilter(const QList<QSpell *> &sl, int CastFrom, int CastTo, const QList<int> &spellTypes, const QList<int> &notID, const QList<int> &byID, int Hand = -1, bool OwnHands = false) const;
    QSpell *getSpellByFilter(const QList<QSpell *> &sl, int CastFrom, int CastTo, int SpellType, const QList<int> &notID, const QList<int> &byID, int Hand = -1, bool OwnHands = false) const;
    bool buildAntiSpellRules(const QSpell *s, int minTurnToCast, QList<QAntiSpellRule> &rules) const;
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
    int _lockedParalyzedHand;
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
