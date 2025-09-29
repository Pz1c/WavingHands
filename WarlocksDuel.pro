TEMPLATE = app

QT += core
QT += core-private
QT += qml quick network

INCLUDEPATH += ../library/game/
INCLUDEPATH += ../library/QGoogleAnalytics/

HEADERS += \
    ../library/QGoogleAnalytics/qgaconstant.h \
    ../library/QGoogleAnalytics/qgoogleanalytics.h \
    cpp/qbattleinfo.h \
    cpp/qgameconstant.h \
    cpp/qmonster.h \
    cpp/qwarlock.h \
    cpp/qwarlockdictionary.h \
    cpp/qwarlockspellchecker.h \
    cpp/qwarlockstat.h \
    cpp/qwarlockutils.h \
    cpp/qwarloksduelcore.h \
    cpp/qspell.h\
    ../library/game/qcore.h \
    ../library/game/qgamedictionary.h \
    ../library/game/qgameutils.h \
    ../library/game/qlevelitem.h \
    ../library/game/qlevelmanager.h \

SOURCES += main.cpp \
    ../library/QGoogleAnalytics/qgoogleanalytics.cpp \
    cpp/nativeforjava.cpp \
    cpp/qbattleinfo.cpp \
    cpp/qmonster.cpp \
    cpp/qwarlock.cpp \
    cpp/qwarlockdictionary.cpp \
    cpp/qwarlockspellchecker.cpp \
    cpp/qwarlockstat.cpp \
    cpp/qwarlockutils.cpp \
    cpp/qwarloksduelcore.cpp \
    cpp/qspell.cpp\
    ../library/game/qcore.cpp \
    ../library/game/qgamedictionary.cpp \
    ../library/game/qgameutils.cpp \
    ../library/game/qlevelitem.cpp \
    ../library/game/qlevelmanager.cpp

RESOURCES += qml.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/platforms/android

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
    C:/Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/armeabi-v7a/libcrypto_3.so \
    C:/Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/armeabi-v7a/libssl_3.so
}

DISTFILES += \
    platforms/android/AndroidManifest.xml \
    platforms/android/build.gradle \
    platforms/android/google-services.json \
    platforms/android/gradle.properties \
    platforms/android/gradle.properties \
    platforms/android/gradle/wrapper/gradle-wrapper.jar \
    platforms/android/gradle/wrapper/gradle-wrapper.jar \
    platforms/android/gradle/wrapper/gradle-wrapper.properties \
    platforms/android/gradle/wrapper/gradle-wrapper.properties \
    platforms/android/gradlew \
    platforms/android/gradlew \
    platforms/android/gradlew.bat \
    platforms/android/gradlew.bat \
    platforms/android/res/values/libs.xml \
    platforms/android/res/values/libs.xml \
    platforms/android/src/com/kdab/training/AlarmReceiver.java \
    platforms/android/src/com/kdab/training/CheckStatus.java \
    platforms/android/src/com/kdab/training/MainActivity.java \
    platforms/android/src/com/kdab/training/MyBroadcastReceiver.java \
    platforms/android/src/com/kdab/training/MyService.java \
    platforms/android/src/org/qtproject/example/androidnotifier/CustomBR.java \
    platforms/android/src/org/qtproject/example/androidnotifier/NotificationClient.java

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_EXTRA_LIBS = \
        C:/Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/arm64-v8a/libcrypto_3.so \
        C:/Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/arm64-v8a/libssl_3.so
}
