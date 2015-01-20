#include <QCoreApplication>
#include <QtDBus>

#include "map.h"
#include "../ha_common.h"
#include "automation_interface.h"
#include "automation_adaptor.h"

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
            qUtf8Printable(bus.lastError().message())
        );
    }

    QDBusInterface dbus_iface(
        "org.freedesktop.DBus",
        "/org/freedesktop/DBus",
        "org.freedesktop.DBus",
        bus
    );

    QStringList names(
        dbus_iface.call("ListActivatableNames").arguments().at(0)
            .toStringList()
            .filter(
                QRegExp("^com\\.jkearney\\.HA\\.")
            )
    );

    auto m = new Map(&a);
    new AutomationAdaptor(m);
    bus.registerObject(INTERFACE, m);

    for (int i = 0; i < names.size(); ++i) {
        QString name = names[i];
        if (name == DBUS_PFX DBUS_ID) continue;
        dbus_iface.call("StartServiceByName", name, 0u);

        QString iname("/" + name);
        iname.replace('.', '/');
        auto iface = new com::jkearney::HA::automation(name, iname, bus, &a);
        if (!iface->isValid()) {
            qDebug() << "didn't get interface to " << name << " " << iname;
        }
        else {
            // signals
            a.connect(iface, &com::jkearney::HA::automation::announce, m, &Map::announce);
            a.connect(iface, &com::jkearney::HA::automation::stateChange, m, &Map::rcv_stateChange);

            // methods
            a.connect(m, &Map::fwd_kill, iface, &com::jkearney::HA::automation::kill);
            a.connect(m, &Map::fwd_setState, iface, &com::jkearney::HA::automation::setState);
            a.connect(m, &Map::fwd_updateState, iface, &com::jkearney::HA::automation::updateState);
        }
    }

    return a.exec();
}
