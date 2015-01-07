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

unix:!macx: LIBS += -L$$PWD/../open-zwave/ -lopenzwave

INCLUDEPATH += $$PWD/../open-zwave/cpp/src
DEPENDPATH += $$PWD/../open-zwave/cpp/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../open-zwave/libopenzwave.so
unix:!macx:QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\',-z,origin'
unix:!macx:QMAKE_POST_LINK += $(COPY_FILE) -a $$PWD/../open-zwave/libopenzwave.so* ./

HEADERS += \
    zwave.h

zwave_libs.path = $$DESTDIR
zwave_libs.files = $$PWD/../open-zwave/libopenzwave.so.*
INSTALLS += zwave_libs

