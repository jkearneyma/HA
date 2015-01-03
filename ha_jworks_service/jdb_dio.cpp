#include "jsb_dio.h"

#include "libusb-1.0/libusb.h"

#include <QStringList>
#include <iostream>

JSB_DIO::JSB_DIO(
	const JSBDesc &board_,
	std::unique_ptr<Device> device_,
	QObject *parent
) :
	QObject(parent),
	board(board_),
	device(std::move(device_))
{
	// blink LED on the board to show it's active
	unsigned char returnData[board.JW_returnSize];
	libusb_control_transfer(
		device->get(),
		LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_ENDPOINT,
		board.JW_flashLedCmd, 0, 0,
		returnData, board.JW_returnSize, 0
	);
	// announce the device
	emit announce(QString(
		"JWORKS_" + device->serialNumber
		+ " inputs=" + QString::number(board.numInputs)
		+ " outputs=" + QString::number(board.numOutputs)
		+ " dio=" + QString::number(board.numDIO)
	));
}

JSB_DIO::~JSB_DIO() {
}

void JSB_DIO::setState(
	QString id,
	bool toState
) {
	uint number = 0;
	bool parseOk = false;
	auto parsed = id.split("_");
	if (
		(parsed.size() == 4)
		&& (parsed[0] == "JWORKS")
		&& (parsed[1] == device->serialNumber)
		&& ((number = parsed[3].toUInt(&parseOk)), parseOk)
	) {
		unsigned char returnData[board.JW_returnSize];
		if ((parsed[2] == "O") && (number < board.numOutputs)) {
			libusb_control_transfer(
				device->get(),
				LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_ENDPOINT,
				toState ? board.JW_setRelayCmd : board.JW_clearRelayCmd,
				uint16_t(1) << number, 0,
				returnData, board.JW_returnSize, 0
			);
		}
//		else if ((parsed[2] == "D") && (board.JW_setOutByBankCmd != 0)) {
//		}
	}
}

void JSB_DIO::update() {
	if (board.numInputs > 0) {
		unsigned char returnData[board.JW_returnSize];
		libusb_control_transfer(
			device->get(),
			LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_ENDPOINT,
			board.JW_ReadInCmd, 0, 0,
			returnData, board.JW_returnSize, 0
		);

		uint8_t value = returnData[0];
		std::swap(value, lastInputs);
		uint8_t changed = value ^ lastInputs;

		for (int n = 0; n < board.numInputs; ++n, changed >>= 1, value >>= 1) {
			if ((changed & 1) != 0) {
				auto id = "JWORKS_" + device->serialNumber + "_I_" + QString::number(n);
				emit stateChange(id, (value & 1) == 0);
			}
		}
	}
}
