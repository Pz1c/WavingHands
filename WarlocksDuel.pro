TEMPLATE = app

QT += core
QT += core-private
QT += qml quick network

# Release builds must not log. Several call sites print request bodies that carry the
# account password, and the analytics secret. qDebug() is compiled out below; QML's
# console.log() lives in the prebuilt Qt QML library and is silenced at runtime in main().
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

#android {
#    QT += androidextras
#}

# cpp/ too: the generated QML type registration (CONFIG += qmltypes below) includes
# the registered classes' headers by bare name.
INCLUDEPATH += cpp/
INCLUDEPATH += cpp/game/
INCLUDEPATH += cpp/QGoogleAnalytics/
INCLUDEPATH += cpp/net/

HEADERS += \
    cpp/QGoogleAnalytics/qgaconstant.h \
    cpp/QGoogleAnalytics/qgoogleanalytics.h \
    cpp/qbattleinfo.h \
    cpp/qgameconstant.h \
    cpp/net/qrosterlink.h \
    cpp/net/qrosterwire.h \
    cpp/qmonster.h \
    cpp/qwarlock.h \
    cpp/qwarlockdictionary.h \
    cpp/qwarlockspellchecker.h \
    cpp/qwarlockstat.h \
    cpp/qwarlockutils.h \
    cpp/qwarloksduelcore.h \
    cpp/qspell.h\
    cpp/game/qcore.h \
    cpp/game/qgamedictionary.h \
    cpp/game/qgameutils.h \
    cpp/game/qlevelitem.h \
    cpp/game/qlevelmanager.h \

SOURCES += main.cpp \
    cpp/QGoogleAnalytics/qgoogleanalytics.cpp \
    cpp/nativeforjava.cpp \
    cpp/qbattleinfo.cpp \
    cpp/net/qrosterlink.cpp \
    cpp/net/qrosterwire.cpp \
    cpp/qmonster.cpp \
    cpp/qwarlock.cpp \
    cpp/qwarlockdictionary.cpp \
    cpp/qwarlockspellchecker.cpp \
    cpp/qwarlockstat.cpp \
    cpp/qwarlockutils.cpp \
    cpp/qwarloksduelcore.cpp \
    cpp/qspell.cpp\
    cpp/game/qcore.cpp \
    cpp/game/qgamedictionary.cpp \
    cpp/game/qgameutils.cpp \
    cpp/game/qlevelitem.cpp \
    cpp/game/qlevelmanager.cpp

RESOURCES += qml.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/platforms/android

# 16 KB page size support (required by Google Play for apps targeting Android 15+).
# NDK r28+ does this by default; r27 and older need the flag explicitly.
android {
    QMAKE_LFLAGS += -Wl,-z,max-page-size=16384 -Wl,-z,common-page-size=16384
}

# QML types are registered declaratively (QML_NAMED_ELEMENT / QML_SINGLETON in the C++
# headers). The build generates the registration code plus a .qmltypes description of
# the C++ API into qmltypes/, where a hand-written qmldir turns it into an importable
# module for qmllint and Qt Creator's code model.
CONFIG += qmltypes
QML_IMPORT_NAME = ua.sp.warloksduel
QML_IMPORT_MAJOR_VERSION = 2
QMLTYPES_FILENAME = $$PWD/qmltypes/ua/sp/warloksduel/warloksduel.qmltypes

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/qmltypes

# Settings for qmllint and the QML language server (qmlls), written with this checkout's
# absolute paths so a plain `qmllint <file>` and the editor find qml.qrc and the
# ua.sp.warloksduel module. Relative paths would not work: both tools resolve them
# against each linted file's own folder. Both files are git-ignored.
QMLLINT_INI = "[General]" "ResourcePath=$$PWD/qml.qrc" "AdditionalQmlImportPaths=$$PWD/qmltypes"
write_file($$PWD/.qmllint.ini, QMLLINT_INI)
QMLLS_INI = "[General]" "importPaths=$$PWD/qmltypes" "no-cmake-calls=true"
write_file($$PWD/.qmlls.ini, QMLLS_INI)

# Make these modules of QtFirebase
# NOTE QTFIREBASE_SDK_PATH can be symlinked to match $$PWD/firebase_cpp_sdk
#QTFIREBASE_SDK_PATH = d:/Android/firebase_cpp_sdk
#QTFIREBASE_CONFIG += analytics admob remote_config
#QTFIREBASE_CONFIG += analytics admob
# include QtFirebase
#include(cpp/QtFirebase/qtfirebase.pri)

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
    qmltypes/ua/sp/warloksduel/qmldir \
    platforms/android/AndroidManifest.xml \
    platforms/android/build.gradle \
    platforms/android/gradle.properties \
    platforms/android/gradle/wrapper/gradle-wrapper.properties \
    platforms/android/res/values/libs.xml \
    platforms/android/src/com/kdab/training/CheckScheduler.java \
    platforms/android/src/com/kdab/training/CheckStatus.java \
    platforms/android/src/com/kdab/training/CheckStatusWorker.java \
    platforms/android/src/com/kdab/training/MainActivity.java \
    platforms/android/src/org/qtproject/example/androidnotifier/NotificationClient.java
#android: include(C:/Users/steel/AppData/Local/Android/Sdk/android_openssl/openssl.pri)

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_EXTRA_LIBS = \
        C:/Work/WavingHands/../../Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/arm64-v8a/libcrypto_3.so \
        C:/Work/WavingHands/../../Users/steel/AppData/Local/Android/Sdk/android_openssl/ssl_3/arm64-v8a/libssl_3.so
}
