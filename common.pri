DESTDIR = /opt/HA

QT += core dbus
QT -= gui

CONFIG += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += DBUS_ID=\\\"$${DBUS_ID}\\\"
DBUS_NAME = com.jkearney.HA.$${DBUS_ID}
DBUS_SERVICE = $${DBUS_NAME}.service

QMAKE_POST_LINK += echo '[D-BUS Service]' > $$OUT_PWD/$${DBUS_SERVICE} $$escape_expand(\\n\\t)
QMAKE_POST_LINK += echo Name=$${DBUS_NAME} >> $$OUT_PWD/$${DBUS_SERVICE} $$escape_expand(\\n\\t)
QMAKE_POST_LINK += echo Exec=\${TARGET} >> $$OUT_PWD/$${DBUS_SERVICE} $$escape_expand(\\n\\t)
QMAKE_POST_LINK += echo User=ha >> $$OUT_PWD/$${DBUS_SERVICE} $$escape_expand(\\n\\t)

service_reg.path = /usr/share/dbus-1/services/
service_reg.files = $$OUT_PWD/$${DBUS_SERVICE}
INSTALLS += service_reg

