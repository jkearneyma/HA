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
    void timerExpired(QString name);
    void timerWasCancelled(QString name);

public slots:
    Q_NOREPLY void setTimer(QString name, uint seconds);
    Q_NOREPLY void CancelTimer(QString name);

private slots:
    void tick();
};

#endif // TIMERS_H
