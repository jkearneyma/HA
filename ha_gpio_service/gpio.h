#ifndef GPIO_H
#define GPIO_H

#include <QObject>

class GPIO : public QObject
{
    Q_OBJECT
public:
    explicit GPIO(QObject *parent = 0);
    ~GPIO();

signals:

public slots:
    void kill();
};

#endif // GPIO_H
