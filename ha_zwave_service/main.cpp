#include <QCoreApplication>
#include <QtDBus>

#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!QDBusConnection::sessionBus().isConnected()) {
        qWarning("Cannot connect to the D-Bus session bus.\n"
            "Please check your system settings and try again.\n");
        return EXIT_FAILURE;
    }

    if (!QDBusConnection::sessionBus().registerService("com.jkearney.ha.ZWave")) {
        std::cerr
            << qPrintable(QDBusConnection::sessionBus().lastError().message())
            << "\n";
        return EXIT_FAILURE;
    }

//    std::clog << "Open-ZWave version: " << OpenZWave::Manager::getVersionAsString() << "\n";

    return a.exec();
}
