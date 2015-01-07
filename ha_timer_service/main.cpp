#include <QCoreApplication>
#include <QtDBus>

#include <iostream>

#include "../ha_common.h"
#include "timers.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setAppDefaults(a);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
        return EXIT_FAILURE;
    }

    if (!QDBusConnection::sessionBus().registerService(DBUS_PFX DBUS_ID)) {
        std::cerr
            << qPrintable(QDBusConnection::sessionBus().lastError().message())
            << "\n";
        return EXIT_FAILURE;
    }

    Timers timers;
    QDBusConnection::sessionBus().registerObject(
        "/",
        &timers,
        QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals
    );

    return a.exec();
}
