#ifndef ZWAVE_H
#define ZWAVE_H

#include <QObject>

class zwave : public QObject
{
    Q_OBJECT
    QString device;
public:
    explicit zwave(QString device, int loglevel, QObject *parent = 0);
    ~zwave();

signals:
	void stateChange(QString service, QString id, QString newState);
	void announce(QString service, QString info);

public slots:
	void updateState(QString service, QString id);
	void setState(QString service, QString id, QString toState);
	void healNetwork(bool doRR);
	void writeConfigs();
	void kill();

private slots:
	// internal use
	void queueAnnounce(QString);
	void queueStateChange(QString, QString);
};

#endif // ZWAVE_H
