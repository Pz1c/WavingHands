TEMPLATE = app

QT += core
QT += core-private
QT += qml quick network

#android {
#    QT += androidextras
#}

INCLUDEPATH += ../library/game/

HEADERS += \
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
        D:/Android/sdk/android_openssl/latest/arm/libcrypto_1_1.so \
        D:/Android/sdk/android_openssl/latest/arm/libssl_1_1.so \
        D:/Android/sdk/android_openssl/latest/arm64/libcrypto_1_1.so \
        D:/Android/sdk/android_openssl/latest/arm64/libssl_1_1.so \
        D:/Android/sdk/android_openssl/latest/x86/libcrypto_1_1.so \
        D:/Android/sdk/android_openssl/latest/x86/libssl_1_1.so \
        D:/Android/sdk/android_openssl/latest/x86_64/libcrypto_1_1.so \
        D:/Android/sdk/android_openssl/latest/x86_64/libssl_1_1.so
}

android: include(D:/Android/sdk/android_openssl/openssl.pri)

DISTFILES += \
    platforms/android/AndroidManifest.xml
