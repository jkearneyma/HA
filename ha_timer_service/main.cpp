#include <QCoreApplication>
#include <QtDBus>

#include <iostream>

#include "../ha_common.h"
#include "automation_adaptor.h"
#include "timers.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setAppDefaults(a);

    auto bus = QDBusConnection::sessionBus();
    if (!bus.isConnected()) {
        qFatal("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
    }

    if (!bus.registerService(DBUS_PFX DBUS_ID)) {
        qFatal(
            "Failed to register: %s\n",
            qUtf8Printable(QDBusConnection::sessionBus().lastError().message())
        );
    }

    auto timers = new Timers(&a);
    new AutomationAdaptor(timers);
    bus.registerObject(INTERFACE, timers);

    return a.exec();
}
