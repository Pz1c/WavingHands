TEMPLATE = app

QT += qml quick network

HEADERS += \
    qwarloksduelcore.h \
    qwarlockdictionary.h \
    qwarlockutils.h \
    qwarlock.h \
    qmonster.h \
    qwarlockspellchecker.h

SOURCES += main.cpp \
    qwarloksduelcore.cpp \
    qwarlockdictionary.cpp \
    qwarlockutils.cpp \
    qwarlock.cpp \
    qmonster.cpp \
    qwarlockspellchecker.cpp

RESOURCES += qml.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/project.properties
