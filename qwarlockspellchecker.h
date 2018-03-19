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

    QString checkSpells(QString Left, QString Right, bool strikt);
    QStringList getPosibleSpellsList(QString left, QString right);
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
