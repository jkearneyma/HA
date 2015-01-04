#ifndef JSB_DIO_H
#define JSB_DIO_H

#include <QObject>
#include "usb.h"
#include <memory>

struct JSBDesc {
	const char *name;
	uint16_t productId;
	uint8_t numInputs, numOutputs, numDIO;
	size_t JW_returnSize;
	uint8_t JW_flashLedCmd;
	uint8_t JW_setRelaysCmd;
	uint8_t JW_relayStateCmd;
	uint8_t JW_ReadInCmd;
	// TODO:
	// JW_setDirByBankCmd
	// JW_setOutByBankCmd
};

// ID format is:
//  JWORKS_<serial>_<I|O|D>_<number>
//  'number' is 0-based

class JSB_DIO : public QObject
{
	Q_OBJECT
	const JSBDesc board;
	std::unique_ptr<Device> device;
	uint8_t lastInputs, lastOutputs;
public:
	JSB_DIO(
		const JSBDesc &board,
		std::unique_ptr<Device> device,
		QObject *parent = 0
	);
	~JSB_DIO();

signals:
	void stateChange(QString id, bool newState);
	void announce(QString info);

public slots:
	//void setDirection(QString id, bool isOutput);
	void setState(QString id, bool toState);
	void update();
};

#endif // JSB_DIO_H
