#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <qDebug>
#include "cpp/qwarloksduelcore.h"
#include "cpp/qwarlockdictionary.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<QWarloksDuelCore>("ua.sp.warloksduel", 1, 8, "WarlocksDuelCore");
    qmlRegisterSingletonType<QWarlockDictionary>("ua.sp.warlockdictionary", 1, 0, "WarlockDictionary", warlocksdictionary_qobject_singletontype_provider);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();

    // http://stackoverflow.com/questions/9029040/how-to-run-an-android-app-in-background
}
