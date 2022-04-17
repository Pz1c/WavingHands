#ifndef QSPELL_H
#define QSPELL_H

#include <QDebug>
#include <QString>
#include <QStringList>

#include "qgameconstant.h"


#define logSpellList(sl, point_name) foreach(QSpell *s, sl) { qDebug() << point_name << s->json(true); }
#define logSpellItem(s) (s ? s->json() : "NULL")

class QSpell
{
public:
    QSpell(int SpellID, QString Gesture, QString Name, int SpellType, int Priority, int Level, int Danger,
           int DefTarget = SPELL_DEF_TARGER_NOBODY, int Damage = 0, bool Active = true, bool Basic = false,
           int SpellGroup = SPELL_GROUP_UNDEFINED, int SpellBookLevel = 0);
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

    QString json(bool Short = false) const;

    QString toString() const;

    static bool sortAsc(const QSpell *s1, const QSpell *s2);
    static bool sortDesc(const QSpell *s1, const QSpell *s2);
    static void setOrderType(int OrderType);
    static void sort(QList<QSpell *> &list, int order_type = 0);
    int level() const;

    bool possibleCast() const;
    QString nextGesture() const;

    bool active() const;
    void setActive(bool active);

    bool operator < (const QSpell &s) const;

    static int orderType();

    int alreadyCasted() const;

    int length() const;

    int damage() const;

    qreal realPriority() const;
    void setRealPriority(qreal newRealPriority);

    int danger() const;
    bool checkValidSequence(const QSpell &s);

    int spellBookLevel() const;

protected:

    int calcPriority(bool Enemy);

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
    qreal _realPriority;
    int _spellGroup;
    int _spellBookLevel;

    static int _orderType;
};

#endif // QSPELL_H
