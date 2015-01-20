DESTDIR = /home/ha/bin

QT += core dbus
QT -= gui

CONFIG += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

DBUS_ADAPTORS += ../automation.xml

HEADERS += \
    ../ha_common.h

DEFINES += DBUS_ID=\\\"$${DBUS_ID}\\\"
DBUS_NAME = com.jkearney.HA.$${DBUS_ID}
DBUS_SERVICE = $${DBUS_NAME}.service
SVC_FILE = $$DESTDIR/$$DBUS_SERVICE

service.target = $${SVC_FILE}
service.commands = \
 mkdir -p $$DESTDIR $$escape_expand(\\n\\t) \
 echo '[D-BUS Service]' > $${SVC_FILE} $$escape_expand(\\n\\t) \
 echo Name=$${DBUS_NAME} >> $${SVC_FILE} $$escape_expand(\\n\\t) \
 echo Exec=$$DESTDIR/$$TARGET >> $${SVC_FILE} $$escape_expand(\\n\\t) \
 echo User=ha >> $${SVC_FILE} $$escape_expand(\\n\\t)
QMAKE_EXTRA_TARGETS += service
PRE_TARGETDEPS += $${SVC_FILE}

!isEmpty( DBUS_ID ) {
service_reg.path = /usr/share/dbus-1/services/
service_reg.files = $${SVC_FILE}
service_reg.CONFIG += no_check_exist
INSTALLS += service_reg
}

