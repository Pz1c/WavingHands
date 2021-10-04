#include <QGuiApplication>
#include <QQmlApplicationEngine>
#ifdef Q_OS_ANDROID
//#include <QAndroidJniObject>
#endif
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>
#include <QtGlobal>
#include <QtQml>
#include <QDebug>
#include <QFontDatabase>
#include "cpp/qwarloksduelcore.h"
#include "cpp/qwarlockdictionary.h"


int main(int argc, char *argv[])
{
    //QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qDebug() << "loading embedded \"ISRG Root X1\" CA cert:"
          << QSslConfiguration::defaultConfiguration().addCaCertificates(":/res/certs/isrgrootx1.pem");

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

    // http://stackoverflow.com/questions/9029040/how-to-run-an-android-app-in-background
}
