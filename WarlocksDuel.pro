TEMPLATE = app

QT += qml quick network

HEADERS += \
    cpp/qmonster.h \
    cpp/qwarlock.h \
    cpp/qwarlockdictionary.h \
    cpp/qwarlockspellchecker.h \
    cpp/qwarlockutils.h \
    cpp/qwarloksduelcore.h

SOURCES += main.cpp \
    cpp/qmonster.cpp \
    cpp/qwarlock.cpp \
    cpp/qwarlockdictionary.cpp \
    cpp/qwarlockspellchecker.cpp \
    cpp/qwarlockutils.cpp \
    cpp/qwarloksduelcore.cpp

RESOURCES += qml.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/project.properties

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        C:/Users/Sergii/Dropbox/Work/Qt/WavingHands/../library/openssl/libcrypto.so \
        $$PWD/../library/openssl/libssl.so
}
