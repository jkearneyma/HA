#ifndef MAP_H
#define MAP_H

#include <QObject>

struct Translation {
    QString match;
    QString replace;
};

class Map : public QObject
{
    Q_OBJECT
public:
    explicit Map(QObject *parent = 0);
    ~Map();

signals:
    void announce(const QString &module, const QString &msg);
    void stateChange(const QString &module, const QString &addr, const QString &value);

    void fwd_kill();
    void fwd_setState(const QString &module, const QString &addr, const QString &value);
    void fwd_updateState(const QString &module, const QString &addr);

public slots:
    void kill();
    void setState(const QString &module, const QString &addr, const QString &value);
    void updateState(const QString &module, const QString &addr);

    void rcv_stateChange(const QString &module, const QString &addr, const QString &value);
};

#endif // MAP_H
