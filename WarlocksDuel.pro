TEMPLATE = app

QT += core
QT += core-private
QT += qml quick network

#android {
#    QT += androidextras
#}

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

# Make these modules of QtFirebase
# NOTE QTFIREBASE_SDK_PATH can be symlinked to match $$PWD/firebase_cpp_sdk
#QTFIREBASE_SDK_PATH = d:/Android/firebase_cpp_sdk
#QTFIREBASE_CONFIG += analytics admob remote_config
#QTFIREBASE_CONFIG += analytics admob
# include QtFirebase
#include(../library/QtFirebase/qtfirebase.pri)

# Default rules for deployment.
#include(deployment.pri)

#OTHER_FILES += \
#    android/AndroidManifest.xml \
#    android/project.properties

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
    C:/Work/WavingHands/../../Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/armeabi-v7a/libcrypto_3.so \
    C:/Work/WavingHands/../../Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/armeabi-v7a/libssl_3.so
}

#android: include(D:/Android/sdk/android_openssl/openssl.pri)

DISTFILES += \
    platforms/android/AndroidManifest.xml \
    platforms/android/build.gradle \
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
#android: include(C:/Users/steel/AppData/Local/Android/Sdk/android_openssl/openssl.pri)

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_EXTRA_LIBS = \
        C:/Work/WavingHands/../../Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/arm64-v8a/libcrypto_3.so \
        C:/Work/WavingHands/../../Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/arm64-v8a/libssl_3.so
}
