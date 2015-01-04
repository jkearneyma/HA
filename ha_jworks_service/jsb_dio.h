#ifndef JSB_DIO_H
#define JSB_DIO_H

#include <QObject>
#include "usb.h"
#include <memory>

// currently just handles single-byte inputs and outputs...
// this structure would have to be more complicated to handle more
// complex devices.  Alternatively, additional handler classes could be
// made.
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
	void stateChange(QString service, QString id, QString value);
	void announce(QString service, QString info);

public slots:
	//void setDirection(QString id, bool isOutput);
	void setState(QString service, QString id, QString value);
	void update();
};

#endif // JSB_DIO_H
