#ifndef QSPELL_H
#define QSPELL_H

#include <QDebug>
#include <QString>
#include <QStringList>

#include "qgameconstant.h"


#define logSpellList(sl, point_name) foreach(QSpell *s, sl) { qDebug() << point_name << s->json(); }

class QSpell
{
public:
    QSpell(int SpellID, QString Gesture, QString Name, int SpellType, int Priority, int Level, int Danger, int DefTarget = SPELL_DEF_TARGER_NOBODY,
           int Damage = 0, bool Active = true, bool Basic = false);
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
    static void setOrderType(int OrderType);
    int level() const;

    bool possibleCast() const;
    QString nextGesture() const;

    bool active() const;
    void setActive(bool active);

    bool operator < (const QSpell &s) const;

    static int orderType();

    int alreadyCasted() const;

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
    bool _basic;

    static int _orderType;
};

#endif // QSPELL_H
