#include "timers.h"

#include <QHashIterator>

Timers::Timers(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(1000);
}

Timers::~Timers() {
    timer->stop();
}

void Timers::setTimer(QString name, uint seconds) {
    active[name] = seconds;
}

void Timers::CancelTimer(QString name) {
    active.remove(name);
    emit timerWasCancelled(name);
}

void Timers::tick() {
    QStringList expired;
    for (auto c = active.begin(); c != active.end(); ++c) {
        if ((*c)-- == 0) {
            expired.append(c.key());
        }
    }
    for (auto &c : expired) {
        active.remove(c);
        emit timerExpired(c);
    }
}
