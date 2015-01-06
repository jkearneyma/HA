#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T18:04:24
#
#-------------------------------------------------

QT       += core dbus

QT       -= gui

TARGET = ha_zwave_service
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    zwave.cpp

unix:!macx: LIBS += -L$$PWD/../open-zwave/ -lopenzwave

INCLUDEPATH += $$PWD/../open-zwave/cpp/src
DEPENDPATH += $$PWD/../open-zwave/cpp/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../open-zwave/libopenzwave.so
unix:!macx:QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\',-z,origin'
unix:!macx:QMAKE_POST_LINK += $(COPY_FILE) -a $$PWD/../open-zwave/libopenzwave.so* ./

HEADERS += \
    zwave.h
