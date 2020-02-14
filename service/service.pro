TEMPLATE = lib
TARGET = service
CONFIG += dll
QT += core
QT += remoteobjects
QT += androidextras

SOURCES += \
    main.cpp \
    service-service.cpp \
    service.cpp

REPC_SOURCE += ../service.rep

HEADERS += \
    service-service.h \
    service.h
