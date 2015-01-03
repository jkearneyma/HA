#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T23:02:31
#
#-------------------------------------------------

QT       += core dbus

QT       -= gui

TARGET = ha_jworks_service
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    usb.cpp \
    jdb_dio.cpp

HEADERS += \
    usb.h \
    jsb_dio.h

unix:!macx: LIBS += -L/usr/lib/ -lusb-1.0

INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include
