#ifndef ZWAVE_H
#define ZWAVE_H

#include <QObject>

class zwave : public QObject
{
    Q_OBJECT
    QString device;
public:
    explicit zwave(
        QString exePath,
        QString device,
        int loglevel,
        QObject *parent = 0
    );
    ~zwave();

signals:
	void stateChange(const QString &service, const QString &id, const QString &newState);
	void announce(const QString &service, const QString &info);

public slots:
	void updateState(const QString &service, const QString &id);
	void setState(const QString &service, const QString &id, const QString &toState);
	void healNetwork(bool doRR);
	void writeConfigs();
	void kill();

private slots:
	// internal use
	void queueAnnounce(const QString &);
	void queueStateChange(const QString &, const QString &);
};

#endif // ZWAVE_H
