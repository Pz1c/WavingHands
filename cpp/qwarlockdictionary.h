#ifndef QWARLOCKDICTIONARY_H
#define QWARLOCKDICTIONARY_H

#include <QtQml/qqmlregistration.h>
#include <qgamedictionary.h>
#include "qwarlockutils.h"

class QWarlockDictionary : public QGameDictionary
{
    Q_OBJECT
    // The "WarlockDictionary" QML singleton, registered at build time (CONFIG += qmltypes).
    QML_NAMED_ELEMENT(WarlockDictionary)
    QML_SINGLETON

public:
    static QGameDictionary *getInstance();
    // Gives QML the one C++ instance, exactly as the old qmlRegisterSingletonType
    // provider did. main() creates it as a QWarlockDictionary before QML loads.
    static QWarlockDictionary *create(QQmlEngine *, QJSEngine *) {
        return qobject_cast<QWarlockDictionary *>(getInstance());
    }
    QString getLang() override;

protected:
    void fillGameDictionary() override;
    void storeLang() override;
};
#endif // QWARLOCKDICTIONARY_H
