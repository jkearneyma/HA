#include <QCoreApplication>
#include <QtDBus>

#include "gpio.h"
#include "../ha_common.h"
#include "automation_adaptor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setAppDefaults(a);

    auto bus = QDBusConnection::sessionBus();

    if (!bus.isConnected()) {
        qFatal(
            "Cannot connect to the D-Bus session bus.\n"
            "Please check your system settings and try again.\n"
        );
    }

    if (!bus.registerService(DBUS_PFX DBUS_ID)) {
        qFatal(
            "Failed to register: %s\n",
            qUtf8Printable(bus.lastError().message())
        );
    }

    auto g = new GPIO(&a);
    new AutomationAdaptor(g);
    bus.registerObject(INTERFACE, g);

    return a.exec();
}
