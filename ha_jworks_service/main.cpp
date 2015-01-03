#include <QCoreApplication>
#include <QtDBus>

#include "usb.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
                 "Please check your system settings and try again.\n");
        return EXIT_FAILURE;
    }

    if (!QDBusConnection::sessionBus().registerService("com.jkearney.ha.JWorks")) {
        std::cerr
            << qPrintable(QDBusConnection::sessionBus().lastError().message())
            << "\n";
        return EXIT_FAILURE;
    }

    USB usb;
    USB::DeviceList dl1(usb, 1, 1);

    return a.exec();
}
