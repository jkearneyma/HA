#include "map.h"

#include <QCoreApplication>

Map::Map(QObject *parent) : QObject(parent)
{

}

Map::~Map()
{

}

void Map::kill()
{
    emit fwd_kill();
    QCoreApplication::exit();
}

void Map::setState(const QString &module, const QString &addr, const QString &value)
{
    emit fwd_setState(module, addr, value);
}

void Map::updateState(const QString &module, const QString &addr)
{
    emit fwd_updateState(module, addr);
}

void Map::rcv_stateChange(const QString &module, const QString &addr, const QString &value)
{
    emit stateChange(module + "!", addr, value);
}

