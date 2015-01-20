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
    void announce(const QString &service, const QString &info);
    void stateChange(const QString &service, const QString &id, const QString &value);

public slots:
	void kill();
	void setState(const QString &service, const QString &id, const QString &value);
	void updateState(const QString &service, const QString &id);

private slots:
    void tick();
};

#endif // TIMERS_H
