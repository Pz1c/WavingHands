#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include <QtNetwork/QSslConfiguration>
//#include <QtNetwork/QSslSocket>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QtGlobal>
#include <QtQml>
#include <QDebug>
#include <QFontDatabase>
#include <QScreen>
#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#endif
#include <cpp/qgameconstant.h>
#include "cpp/qwarloksduelcore.h"
#include "cpp/qwarlockdictionary.h"
#include "cpp/nativeforjava.cpp"

#include <qgoogleanalytics.h>

int main(int argc, char *argv[])
{
    bool add_cert = QSslConfiguration::defaultConfiguration().addCaCertificates(":/res/certs/isrgrootx1.pem");
    QString s1 = QString("QSslSocket::sslLibraryBuildVersionString() %1 QSslSocket::sslLibraryVersionString() %2").arg(QSslSocket::sslLibraryBuildVersionString(), QSslSocket::sslLibraryVersionString());
    QString s2 = QString("loading embedded \"ISRG Root X1\" CA cert: %1").arg(add_cert);
    qDebug() << s1;
    qDebug() << s2;
    qDebug() << NATIVE_JAVA_INCLUDED;

    /*if (argc > 1 && qstrcmp(argv[1], "-service") == 0) {
        qDebug() << "Service starting with from the same .so file";
        #ifdef Q_OS_ANDROID
        QAndroidService app(argc, argv);
        //QWarloksDuelCore *wdc = new QWarloksDuelCore(&app, true);
        //wdc->
        return app.exec();
        #endif
        //return 0;
    } else {*/
        //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
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

        int iwidth =0, iheight = 0;
        #ifdef Q_OS_ANDROID
        QJniObject ctx = QNativeInterface::QAndroidApplication::context();
        QJniObject val = QJniObject::fromString("dummy");
        QJniObject string = QJniObject::callStaticObjectMethod("org/qtproject/example/androidnotifier/NotificationClient", "get_screen_size",
                                                           "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;",
                                                           ctx.object<jobject>(), val.object<jstring>());
        QString res = string.toString();
        qDebug() << "get_screen_size" << res;
        QStringList sl = res.split(",");
        if (sl.size() == 2) {
            iwidth = sl.at(0).toInt();
            iheight = sl.at(1).toInt();
        }
        #endif
        if (iwidth <= 0) {
            QRect rect = QGuiApplication::primaryScreen()->geometry();
            iwidth = rect.width();
            iheight = rect.height();
        }

        /*
        qreal refDpi = 216.;
        qreal refHeight = 1776.;
        qreal refWidth = 1080.;
        */
        qreal height = qMax(iheight, iwidth);
        qreal width = qMin(iheight, iwidth);
        qreal dpi = QGuiApplication::primaryScreen()->logicalDotsPerInch();
        qreal refDpi = 96;
        qreal refHeight = 1068;
        qreal refWidth = 600;
        qreal m_ratio = qMin(height/refHeight, width/refWidth);
        qreal m_ratioFont = qMin(height*refDpi/(dpi*refHeight), width*refDpi/(dpi*refWidth));
        qDebug() << "BEFORE QML" << dpi << height << width << m_ratio << m_ratioFont;

        QWarlockDictionary::getInstance();
        //QGoogleAnalytics::getInstance();
        qmlRegisterType<QWarloksDuelCore>("ua.sp.warloksduel", 2, 0, "WarlocksDuelCore");
        qmlRegisterSingletonType<QWarlockDictionary>("ua.sp.warlockdictionary", 1, 0, "WarlockDictionary", gamedictionary_qobject_singletontype_provider);
        //qmlRegisterSingletonType<QGoogleAnalytics>("ua.sp.GoogleAnalytics", 1, 0, "GoogleAnalytics", googleanalytics_qobject_singletontype_provider);

        QQmlApplicationEngine engine;
        engine.setInitialProperties({
            { "realScreenWidth", QVariant::fromValue(width) },
            { "realScreenHeight", QVariant::fromValue(height) }/*,
            { "calculatedRatio", QVariant::fromValue(m_ratio) },
            { "calculatedRatioFont", QVariant::fromValue(m_ratioFont) } */
        });

        engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
        return app.exec();
    //}
}
