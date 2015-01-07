#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T22:53:52
#
#-------------------------------------------------

DBUS_ID = Timers

!include( ../common.pri ) {
    error("Couldn't find the common.pri file!")
}

TARGET = ha_timer_service

SOURCES += main.cpp \
    timers.cpp

HEADERS += \
    timers.h
