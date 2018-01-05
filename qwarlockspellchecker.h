#ifndef QWARLOCKSPELLCHECKER_H
#define QWARLOCKSPELLCHECKER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPair>
#include <QStringList>

#include "qwarlockutils.h"

class QWarlockSpellChecker : public QObject
{
    Q_OBJECT
public:
    explicit QWarlockSpellChecker(QObject *parent = 0);

    QString checkSpells(QString left, QString right, bool strikt, bool for_print = true);
    QStringList getPosibleSpellsList(QString left, QString right, bool for_print = false, bool only_best = false);
    QStringList getStriktSpellsList(QString left, QString right);
    QList<QValueName> Spells;
signals:

public slots:

protected:
    bool checkStriktSpell(QString left, QString right, QString spell);
    int checkSpellPosible(QString left, QString right, QString spell);
    bool checkSpellChar(QChar left, QChar right, QChar spell);

};

#endif // QWARLOCKSPELLCHECKER_H
