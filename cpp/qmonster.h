#ifndef QMONSTER_H
#define QMONSTER_H

#include "qspell.h"
#include "qwarlockutils.h"

class QMonster
{
public:
    QMonster(const QString &Name, const QString &Status, const QString &Owner, const QString &Target);

    QString name();
    QString status();
    QString owner();
    QString target();
    QString html(const QString &user_login);
    QString json(const QString &user_login);
    bool is_under_attack(const QString &user_login);
    bool is_owner(const QString &user_login);
    int getStrength() const;
    int getHp() const;

    const QString &newTarget() const;
    void setNewTarget(const QString &newNewTarget);

    int attackStrength() const;
    void setAttackStrength(int newAttackStrength);

    bool fireElemental() const;

    bool iceElemental() const;

    int underControl() const;
    void setUnderControl(int newUnderControl);

    bool justCreated() const;

    int hand() const;

    void setName(const QString &newName);

    void setStrength(int newStrength);

    void setHp(int newHp);

protected:
    QString getColor(const QString &user_login);

private:
    QString _name;
    QString _status;
    QString _owner;
    QString _target;
    QString _newTarget;
    QString _enchantment;
    int _strength;
    int _hp;
    int _attackStrength;
    bool _fireElemental;
    bool _iceElemental;
    bool _justCreated;
    int _hand;
    int _underControl;
};

#endif // QMONSTER_H
