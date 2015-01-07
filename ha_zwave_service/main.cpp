#include <QCoreApplication>
#include <QtDBus>
#include <QSettings>

#include <iostream>

#include "../ha_common.h"
#include "zwave.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setAppDefaults(a);
    QSettings settings; // for port

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

    /*auto z = */new zwave(
        a.applicationDirPath(),
        settings.value("zwave/device", "/dev/ttyUSB0").toString(),
        settings.value("zwave/loglevel", "3").toUInt(),
        &a
    );

    return a.exec();
}
