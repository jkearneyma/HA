#include <QCoreApplication>
#include <QtDBus>

#include <iostream>
#include <memory>

#include "usb.h"
#include "jsb_dio.h"

// if USB devices cannot be accessed, make sure that there is a rule
// in /etc/udev/rules.d/some-file.rule with
//   SUBSYSTEMS=="usb", ATTRS{idVendor}=="07c3", GROUP:="users"
// and that the current user is in group users.

namespace {
JSBDesc boards[] = {
	{ "JSB252", 0x0252, 0, 2, 0, 8, 0xb1, 0xb5, 0xb7,    0 },
	{ "JSB280", 0x1280, 0, 4, 0, 8, 0x01, 0x02, 0x04,    0 },
	{ "JSB281", 0x1281, 0, 1, 0, 8, 0xb1, 0x02, 0x04,    0 },
	{ "JSB283", 0x1283, 0, 1, 0, 8, 0xb1, 0xc4, 0xb8,    0 },
	{ "JSB342", 0x1400, 4, 4, 0, 8, 0xb9, 0xb1, 0xb5, 0xb2 },
	{ "JSB394", 0x8383, 8, 0, 0, 8, 0xb9,    0,    0, 0xb2 }, // also JSB383
	{ "JSB2944",0x2280, 0, 4, 0, 8, 0xb1, 0xb5, 0xb7,    0 }
};
}

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
	QTimer *timer = new QTimer(&a);
	for (auto &board : boards) {
		USB::DeviceList list(usb, 0x07c3, board.productId);
		for (auto device : list.devices) {
			auto controller = new JSB_DIO(board, std::unique_ptr<Device>(new Device(device)), &a);
			a.connect(timer, SIGNAL(timeout()), controller, SLOT(update()));
		}
	}
	timer->start(200); // 5x / second

	return a.exec();
}
