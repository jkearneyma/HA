#include <QCoreApplication>
#include <QtDBus>

#include <iostream>
#include <memory>

#include "usb.h"
#include "jsb_dio.h"

namespace {
JSBDesc boards[] = {
	{ "JSB342", 0x1400, 4, 4, 0, 8, 0xb9, 0xc1, 0xc0, 0xb5, 0xb2 },
	{ "JSB394", 0x8383, 8, 0, 0, 8, 0xb9,    0,    0,    0, 0xb2 }
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
			QDBusConnection::sessionBus().registerObject(
				"/",
				controller,
				QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals
			);
		}
	}
	timer->start(200); // 5x / second

	return a.exec();
}
