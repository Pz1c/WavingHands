#ifndef QWARLOCKSPELLCHECKER_H
#define QWARLOCKSPELLCHECKER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPair>
#include <QStringList>

#include "qwarlockutils.h"
#include "qspell.h"

class QWarlockSpellChecker : public QObject
{
    Q_OBJECT
public:
    explicit QWarlockSpellChecker(QObject *parent = 0);

    QString checkSpells(QString Left, QString Right, bool strikt, bool Enemy);
    QList<QSpell *> getSpellsList(QString Left, QString Right, bool strikt, bool Enemy);
    QList<QSpell *> getPosibleSpellsList(QString left, QString right, bool Enemy = true);
    QList<QSpell *> getStriktSpellsList(QString left, QString right, bool Enemy);
    QList<QSpell *> Spells;
signals:

public slots:

protected:
    bool checkStriktSpell(QString left, QString right, QString spell);
    int checkSpellPosible(QString left, QString right, QString spell);
    bool checkSpellChar(QChar left, QChar right, QChar spell);
    void checkHandOnSpell(QList<QSpell *> &Result, QSpell *Spell, QString left, QString right, int Hand, bool Enemy);
};

#endif // QWARLOCKSPELLCHECKER_H
