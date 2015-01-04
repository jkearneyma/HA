#include "jsb_dio.h"

#include "libusb-1.0/libusb.h"

#include <QStringList>
#include <QtDBus>
#include <iostream>

JSB_DIO::JSB_DIO(
	const JSBDesc &board_,
	std::unique_ptr<Device> device_,
	QObject *parent
) :
	QObject(parent),
	board(board_),
	device(std::move(device_)),
	lastInputs(0),
	lastOutputs(0)
{
	QDBusConnection::sessionBus().registerObject(
		"/",
		this,
		QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals
	);
	// blink LED on the board to show it's active
	unsigned char returnData[board.JW_returnSize];
	libusb_control_transfer(
		device->get(),
		LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_ENDPOINT,
		board.JW_flashLedCmd, 0, 0,
		returnData, board.JW_returnSize, 0
	);
	// announce the device
	emit announce(
		"JWORKS",
		device->serialNumber
		+ " inputs=" + QString::number(board.numInputs)
		+ " outputs=" + QString::number(board.numOutputs)
		+ " dio=" + QString::number(board.numDIO)
	);
}

JSB_DIO::~JSB_DIO() {
}

void JSB_DIO::setState(
	QString service,
	QString id,
	QString toState
) {
	uint number = 0, value = 0;
	bool parseOk = false;
	auto parsed = id.split("_");
	if (
		(parsed.size() == 3)
		&& (service == "JWORKS")
		&& (parsed[0] == device->serialNumber)
		&& ((number = parsed[2].toUInt(&parseOk)), parseOk)
		&& ((value = toState.toUInt(&parseOk)), parseOk)
	) {
		uint8_t relay = uint16_t(1) << number;
		uint8_t newOutputs = (value != 0)
			? (lastOutputs | relay)
			: (lastOutputs & ~relay);
		unsigned char returnData[board.JW_returnSize];
		if ((parsed[1] == "O") && (number < board.numOutputs)) {
			libusb_control_transfer(
				device->get(),
				LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_ENDPOINT,
				board.JW_setRelaysCmd, newOutputs, 0,
				returnData, board.JW_returnSize, 0
			);
		}
//		else if ((parsed[1] == "D") && (board.JW_setOutByBankCmd != 0)) {
//		}
	}
}

void JSB_DIO::update() {
	auto report = [&](uint8_t cmd, QString type, uint8_t &prev, uint8_t count) {
		if (count < 1) return;
		unsigned char returnData[board.JW_returnSize];
		libusb_control_transfer(
			device->get(),
			LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_ENDPOINT,
			cmd, 0, 0,
			returnData, board.JW_returnSize, 0
		);

		uint8_t value = returnData[0];
		std::swap(value, prev);
		uint8_t changed = value ^ prev;

		QString baseId( device->serialNumber + "_" + type + "_");
		for (int n = 0; n < count; ++n, changed >>= 1, value >>= 1) {
			if ((changed & 1) != 0) {
				auto id = baseId + QString::number(n);
				emit stateChange("JWORKS", id, (value & 1) ? "1" : "0");
			}
		}
	};

	report(board.JW_ReadInCmd, "I", lastInputs, board.numInputs);
	report(board.JW_relayStateCmd, "O", lastOutputs, board.numOutputs);
}
