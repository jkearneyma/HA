#ifndef TIMERS_H
#define TIMERS_H

#include <QObject>
#include <QtDBus>
#include <QTimer>
#include <QHash>

class Timers : public QObject
{
    Q_OBJECT
    QTimer *timer;
    QHash<QString, uint> active;
public:
    explicit Timers(QObject *parent = 0);
    ~Timers();

signals:
    void announce(QString service, QString info);
    void stateChange(QString service, QString id, QString value);

public slots:
    void setState(QString service, QString id, QString value);
	void updateState(QString service, QString id);

private slots:
    void tick();
};

#endif // TIMERS_H
