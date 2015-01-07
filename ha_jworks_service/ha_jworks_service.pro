#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T23:02:31
#
#-------------------------------------------------

DBUS_ID = JWorks

!include( ../common.pri ) {
    error("Couldn't find the common.pri file!")
}

TARGET = ha_jworks_service

SOURCES += main.cpp \
    usb.cpp \
    jdb_dio.cpp

HEADERS += \
    usb.h \
    jsb_dio.h

unix:!macx: LIBS += -L/usr/lib/ -lusb-1.0

INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include
