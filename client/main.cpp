#include <QGuiApplication>
#include <QQmlApplicationEngine>
#ifdef Q_OS_ANDROID
//#include <QAndroidJniObject>
#endif
#include <QtQml>
#include <QDebug>
#include "cpp/qwarloksduelcore.h"
#include "cpp/qwarlockdictionary.h"

int main(int argc, char *argv[])
{
    //QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QWarlockDictionary::getInstance();
    qmlRegisterType<QWarloksDuelCore>("ua.sp.warloksduel", 1, 9, "WarlocksDuelCore");
    qmlRegisterSingletonType<QWarlockDictionary>("ua.sp.warlockdictionary", 1, 0, "WarlockDictionary", gamedictionary_qobject_singletontype_provider);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
#ifdef Q_OS_ANDROID
    /*QAndroidJniObject::callStaticMethod<void>("net/is/games/MyService",
                                                  "startMyService",
                                                  "(Landroid/content/Context;)V",
                                                  QtAndroid::androidActivity().object());*/
#endif
    return app.exec();

    // http://stackoverflow.com/questions/9029040/how-to-run-an-android-app-in-background
}
