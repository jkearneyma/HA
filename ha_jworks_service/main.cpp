#include <QCoreApplication>

#include "usb.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    USB usb;
    USB::DeviceList dl1(usb, 1, 1);

    return a.exec();
}
