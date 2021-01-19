#ifndef QSPELL_H
#define QSPELL_H

#include <QDebug>
#include <QString>
#include <QStringList>

#include "qgameconstant.h"

#define SPELL_TYPE_SUMMON_MONSTER 0
#define SPELL_TYPE_POISON 1
#define SPELL_TYPE_CONFUSION 2
#define SPELL_TYPE_DAMAGE 3
#define SPELL_TYPE_SHIELD 4
#define SPELL_TYPE_MAGIC_SHIELD 5
#define SPELL_TYPE_MASSIVE 6
#define SPELL_TYPE_HASTLE 7
#define SPELL_TYPE_CHARM_MONSTER 8
#define SPELL_TYPE_CURE 9
#define SPELL_TYPE_SPEC 10
#define SPELL_TYPE_DEATH 11
#define SPELL_TYPE_RESIST 12
#define SPELL_TYPE_ELEMENTAL 13
#define SPELL_TYPE_REMOVE_ENCHANTMENT 14
#define SPELL_TYPE_STAB 15

#define SPELL_DISPEL_MAGIC 0
#define SPELL_SUMMON_ICE_ELEMENTAL 1
#define SPELL_SUMMON_FIRE_ELEMENTAL 2
#define SPELL_MAGIC_MIRROR 3
#define SPELL_LIGHTNING_BOLT 4
#define SPELL_CURE_HEAVY_WOUNDS 5
#define SPELL_CURE_LIGHT_WOUNDS 6
#define SPELL_BLINDNESS1 7
#define SPELL_AMNESIA 8
#define SPELL_CONFUSION 9
#define SPELL_DISEASE 10
#define SPELL_BLINDNESS2 11
#define SPELL_DELAY_EFFECT 12
#define SPELL_POISON 13
#define SPELL_PARALYSIS 14
#define SPELL_SUMMON_GIANT 15
#define SPELL_SUMMON_TROLL 16
#define SPELL_SUMMON_OGRE 17
#define SPELL_SUMMON_GOBLIN 18
#define SPELL_FIREBALL 19
#define SPELL_SHIELD 20
#define SPELL_REMOVE_ENCHANTMENT 21
#define SPELL_INVISIBILITY 22
#define SPELL_CHARM_MONSTER 23
#define SPELL_CHARM_PERSON 24
#define SPELL_FINGER_OF_DEATH 25
#define SPELL_HASTE 26
#define SPELL_MAGIC_MISSILE 27
#define SPELL_ANTI_SPELL 28
#define SPELL_PERMANENCY 29
#define SPELL_TIME_STOP1 30
#define SPELL_TIME_STOP2 31
#define SPELL_RESIST_COLD 32
#define SPELL_FEAR 33
#define SPELL_FIRE_STORM 34
#define SPELL_CLAP_OF_LIGHTNING 35
#define SPELL_CAUSE_LIGHT_WOUNDS 36
#define SPELL_CAUSE_HEAVY_WOUNDS 37
#define SPELL_COUNTER_SPELL1 38
#define SPELL_ICE_STORM 39
#define SPELL_RESIST_HEAT 40
#define SPELL_PROTECTION 41
#define SPELL_COUNTER_SPELL2 42
#define SPELL_SURRENDER 43
#define SPELL_STAB 44
#define SPELL_DISEASE_FDF 45
#define SPELL_PARALYSIS_FDF 46
#define SPELL_PARALYSIS_FDFD 47

#define SPELL_ID_MAX 999

#define WARLOCK_HAND_NONE 0
#define WARLOCK_HAND_LEFT 1
#define WARLOCK_HAND_RIGHT 2

#define WARLOCK_PLAYER false
#define WARLOCK_ENEMY true

#define SPELL_PRIORITY_ZERO -999
#define SPELL_DEF_TARGER_NOBODY 0
#define SPELL_DEF_TARGER_SELF 1
#define SPELL_DEF_TARGER_ENEMY 2
#define SPELL_DEF_TARGER_ENEMY_MONSTER 3
#define SPELL_DEF_TARGER_SELF_MONSTER 4

#define logSpellList(sl, point_name) foreach(QSpell *s, sl) { qDebug() << point_name << s->json(); }

class QSpell
{
public:
    QSpell(int SpellID, QString Gesture, QString Name, int SpellType, int Priority, int Level, int Danger, int DefTarget = SPELL_DEF_TARGER_NOBODY, int Damage = 0, bool Active = true);
    QSpell(QSpell *Spell, int Hand, int TurnToCast, bool Enemy = false);

    int spellID() const;

    QString gesture() const;

    QString name() const;

    int priority() const;
    void setPriority(int priority);
    void changePriority(int priority);

    int turnToCast() const;
    void setTurnToCast(int turnToCast);

    int hand() const;

    int spellType() const;

    QString json() const;

    QString toString() const;

    static bool sortAsc(const QSpell *s1, const QSpell *s2) ;
    static bool sortDesc(const QSpell *s1, const QSpell *s2);
    int level() const;

    bool possibleCast() const;
    QString nextGesture() const;

    bool active() const;
    void setActive(bool active);

    bool operator < (const QSpell &s) const;

protected:

    int calcPriority(int Priority, int Danger, int TurnToCast, bool Enemy, int FullTurnToCast);

private:
    bool _active;
    int _spellID;
    QString _gesture;
    QString _name;
    int _hand;
    int _turnToCast;
    int _spellType;
    int _priority;
    int _danger;
    int _level;
    int _alreadyCasted;
    int _defTarget;
    int _damage;
};

#endif // QSPELL_H
