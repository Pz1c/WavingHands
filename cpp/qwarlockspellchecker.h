#ifndef QWARLOCKSPELLCHECKER_H
#define QWARLOCKSPELLCHECKER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPair>
#include <QStringList>

#include "qwarlockutils.h"
#include "qspell.h"
#include "qwarlock.h"

class QWarlockSpellChecker : public QObject
{
    Q_OBJECT
public:
    explicit QWarlockSpellChecker(QObject *parent = 0);

    QString checkSpells(QString Left, QString Right, bool Enemy);
    QList<QSpell *> getSpellsList(QWarlock *warlock);
    QList<QSpell *> getPosibleSpellsList(QString left, QString right, bool Enemy, QString possible_left, QString possible_right);
    QList<QSpell *> getStriktSpellsList(QString left, QString right, bool Enemy);
    QList<QSpell *> Spells;
signals:

public slots:

protected:
    bool checkStriktSpell(QString left, QString right, QString spell);
    int checkSpellPosible(QString left, QString right, QString spell, QString possible_left, QString possible_right);
    bool checkSpellChar(QChar left, QChar right, QChar spell);
    void checkHandOnSpell(QList<QSpell *> &Result, QSpell *Spell, QString left, QString right, int Hand, bool Enemy, QString possible_left, QString possible_right);
};

#endif // QWARLOCKSPELLCHECKER_H