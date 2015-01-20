#include <QCoreApplication>
#include <QtDBus>
#include <QSettings>

#include <iostream>

#include "../ha_common.h"
#include "automation_adaptor.h"
#include "zwave.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setAppDefaults(a);
    QSettings settings; // for port

    auto bus = QDBusConnection::sessionBus();

    if (!bus.isConnected()) {
        qFatal("Cannot connect to the D-Bus session bus.\n"
            "Please check your system settings and try again.\n");
    }

    if (!bus.registerService(DBUS_PFX DBUS_ID)) {
        qFatal(
            "Failed to register: %s\n",
            qUtf8Printable(bus.lastError().message())
        );
    }

    auto z = new zwave(
        a.applicationDirPath(),
        settings.value("zwave/device", "/dev/ttyUSB0").toString(),
        settings.value("zwave/loglevel", "3").toUInt(),
        &a
    );
    new AutomationAdaptor(z);
    bus.registerObject(INTERFACE, z);

    return a.exec();
}
