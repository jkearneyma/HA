#ifndef ZWAVE_H
#define ZWAVE_H

#include <QObject>

class zwave : public QObject
{
    Q_OBJECT
public:
    explicit zwave(QString device, QObject *parent = 0);
    ~zwave();

signals:
	void stateChange(QString id, int newState);
	void announce(QString info);

public slots:
	void setState(QString id, int toState);
	void update();
};

#endif // ZWAVE_H
