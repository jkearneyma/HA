#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T23:03:12
#
#-------------------------------------------------

!include( ../common.pri ) {
    error("Couldn't find the common.pri file!")
}

TARGET = ha_gpio_service

SOURCES += main.cpp

HEADERS += \
    ../ha_common.h
