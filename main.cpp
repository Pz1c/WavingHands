#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>
#include <QtGlobal>
#include <QtQml>
#include <QDebug>
#include <QFontDatabase>
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif
#include <cpp/qgameconstant.h>
#include "cpp/qwarloksduelcore.h"
#include "cpp/qwarlockdictionary.h"


int main(int argc, char *argv[])
{
    bool add_cert = QSslConfiguration::defaultConfiguration().addCaCertificates(":/res/certs/isrgrootx1.pem");
    QString s1 = QString("QSslSocket::sslLibraryBuildVersionString() %1 QSslSocket::sslLibraryVersionString() %2").arg(QSslSocket::sslLibraryBuildVersionString(), QSslSocket::sslLibraryVersionString());
    QString s2 = QString("loading embedded \"ISRG Root X1\" CA cert: %1").arg(add_cert);
    qDebug() << s1;
    qDebug() << s2;

    if (argc > 1 && strcmp(argv[1], "-service") == 0) {
        qDebug() << "Service starting with from the same .so file";
        #ifdef Q_OS_ANDROID
        QAndroidService app(argc, argv);
        return app.exec();
        #else
        return 1;
        #endif
    } else {
        QGuiApplication app(argc, argv);
        qint32 fontId = QFontDatabase::addApplicationFont(":/res/AgencyFB.ttf");
        qDebug() << "fontId" << fontId;
        if (fontId != -1) {
            QStringList fontList = QFontDatabase::applicationFontFamilies(fontId);
            if (fontList.size() > 0) {
                QString family = fontList.at(0);
                QGuiApplication::setFont(QFont(family));
            }
        }

        QWarlockDictionary::getInstance();
        qmlRegisterType<QWarloksDuelCore>("ua.sp.warloksduel", 2, 0, "WarlocksDuelCore");
        qmlRegisterSingletonType<QWarlockDictionary>("ua.sp.warlockdictionary", 1, 0, "WarlockDictionary", gamedictionary_qobject_singletontype_provider);

        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
        return app.exec();
    }
}
