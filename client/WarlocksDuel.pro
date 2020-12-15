TEMPLATE = app

QT += qml quick network
android {
    QT += androidextras
}

INCLUDEPATH += ../../library/game/

HEADERS += \
    cpp/qgameconstant.h \
    cpp/qmonster.h \
    cpp/qwarlock.h \
    cpp/qwarlockdictionary.h \
    cpp/qwarlockspellchecker.h \
    cpp/qwarlockutils.h \
    cpp/qwarloksduelcore.h \
    cpp/qspell.h\
    ../../library/game/qcore.h \
    ../../library/game/qgamedictionary.h \
    ../../library/game/qgameutils.h \
    ../../library/game/qlevelitem.h \
    ../../library/game/qlevelmanager.h \

SOURCES += main.cpp \
    cpp/qmonster.cpp \
    cpp/qwarlock.cpp \
    cpp/qwarlockdictionary.cpp \
    cpp/qwarlockspellchecker.cpp \
    cpp/qwarlockutils.cpp \
    cpp/qwarloksduelcore.cpp \
    cpp/qspell.cpp\
    ../../library/game/qcore.cpp \
    ../../library/game/qgamedictionary.cpp \
    ../../library/game/qgameutils.cpp \
    ../../library/game/qlevelitem.cpp \
    ../../library/game/qlevelmanager.cpp

RESOURCES += qml.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Make these modules of QtFirebase
# NOTE QTFIREBASE_SDK_PATH can be symlinked to match $$PWD/firebase_cpp_sdk
#QTFIREBASE_SDK_PATH = d:/Android/firebase_cpp_sdk
#QTFIREBASE_CONFIG += analytics admob remote_config
#QTFIREBASE_CONFIG += analytics admob
# include QtFirebase
#include(../../library/QtFirebase/qtfirebase.pri)

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/project.properties

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../library/openssl/arm/libcrypto.so \
        $$PWD/../library/openssl/arm/libssl.so
}

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../library/openssl/arm64/libcrypto.so \
        $$PWD/../library/openssl/arm64/libssl.so
}
