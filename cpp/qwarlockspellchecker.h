#ifndef QWARLOCKSPELLCHECKER_H
#define QWARLOCKSPELLCHECKER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPair>
#include <QStringList>

#include "qwarlockutils.h"
#include "qspell.h"

class QWarlock;

class QWarlockSpellChecker : public QObject
{
    Q_OBJECT
public:
    static QWarlockSpellChecker *getInstance();

    QString checkSpells(QString Left, QString Right, bool Enemy);
    QList<QSpell *> getSpellsList(QWarlock *warlock, bool SeparateSpellbook = false);
    QList<QSpell *> getPosibleSpellsList(QString left, QString right, bool Enemy, QString possible_left, QString possible_right, bool IsFDF);
    QList<QSpell *> getStriktSpellsList(QString left, QString right, bool Enemy);
    QList<QSpell *> Spells;
    QString getSpellBook(bool IsFDF, bool Sort = false, bool EnableSurrender = true);
signals:

public slots:

protected:
    bool checkStriktSpell(QString left, QString right, QString spell);
    int checkSpellPosible(QString left, QString right, QString spell, QString possible_left, QString possible_right);
    bool checkSpellChar(QChar left, QChar right, QChar spell);
    void checkHandOnSpell(QList<QSpell *> &Result, QSpell *Spell, QString left, QString right, int Hand, bool Enemy, QString possible_left, QString possible_right);

private:
    explicit QWarlockSpellChecker(QObject *parent = nullptr);
    static QWarlockSpellChecker *self;
};

#endif // QWARLOCKSPELLCHECKER_H
