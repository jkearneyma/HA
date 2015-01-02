#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T22:53:52
#
#-------------------------------------------------

QT       += core dbus
QT       -= gui

TARGET = ha_timer_service
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    timers.cpp

HEADERS += \
    timers.h
