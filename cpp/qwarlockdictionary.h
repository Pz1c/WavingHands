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
        QWarlockDictionary *instance = qobject_cast<QWarlockDictionary *>(getInstance());
        // Otherwise the engine takes ownership and deletes the singleton on shutdown,
        // leaving QGameDictionary::self dangling.
        QJSEngine::setObjectOwnership(instance, QJSEngine::CppOwnership);
        return instance;
    }
    QString getLang() override;

protected:
    void fillGameDictionary() override;
    void storeLang() override;

private:
    // Must stay private. Qt prefers a public default constructor over create() for a
    // QML_SINGLETON, and a directly constructed instance never runs fillGameDictionary()
    // (only getInstance() does), so every getStringByCode() in QML returned the code.
    QWarlockDictionary() = default;
};
#endif // QWARLOCKDICTIONARY_H
