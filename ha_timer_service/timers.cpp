#include "timers.h"

#include <QHashIterator>

static QString TIMER("TIMER");

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

void Timers::setState(
	QString service,
	QString id,
	QString value
) {
	bool parseOk = false;
	int seconds = value.toInt(&parseOk);
	if (parseOk && (service == "TIMER")) {
		if (seconds <= 0) active.remove(id);
		else active[id] = seconds;
	}
}

void Timers::updateState(
	QString service,
	QString id
) {
	if ((service == TIMER) && active.contains(id)) {
		emit stateChange(TIMER, id, QString::number(active[id]));
	}
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
        emit stateChange(TIMER, c, "0");
    }
}
