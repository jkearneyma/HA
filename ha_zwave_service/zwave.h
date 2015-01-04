#ifndef ZWAVE_H
#define ZWAVE_H

#include <QObject>

class zwave : public QObject
{
    Q_OBJECT
public:
    explicit zwave(QObject *parent = 0);
    ~zwave();

signals:

public slots:
};

#endif // ZWAVE_H
