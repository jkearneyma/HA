#-------------------------------------------------
#
# Project created by QtCreator 2015-01-07T13:09:05
#
#-------------------------------------------------

DBUS_ID = Map

!include( ../common.pri ) {
    error("Couldn't find the common.pri file!")
}

TARGET = ha_map_service

DBUS_INTERFACES += ../automation.xml

SOURCES += main.cpp \
    map.cpp

HEADERS += \
    map.h
