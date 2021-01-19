#ifndef QWARLOCKDICTIONARY_H
#define QWARLOCKDICTIONARY_H

#include <qgamedictionary.h>
#include "qwarlockutils.h"

class QWarlockDictionary : public QGameDictionary
{
    Q_OBJECT

public:
    static QGameDictionary *getInstance();
    QString getLang() override;

protected:
    void fillGameDictionary() override;
    void storeLang() override;
};
#endif // QWARLOCKDICTIONARY_H
