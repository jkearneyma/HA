#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T18:04:24
#
#-------------------------------------------------

DBUS_ID = ZWave
TARGET = ha_zwave_service

!include( ../common.pri ) {
    error("Couldn't find the common.pri file!")
}

SOURCES += main.cpp \
    zwave.cpp

unix:!macx: LIBS += -lopenzwave

INCLUDEPATH += /usr/local/include/openzwave
DEPENDPATH += /usr/local/include/openzwave

HEADERS += \
    zwave.h

config_dir.target = config
config_dir.commands = cp -ur ../open-zwave/config $$DESTDIR
QMAKE_EXTRA_TARGETS += config_dir
PRE_TARGETDEPS += $${config_dir.target}
