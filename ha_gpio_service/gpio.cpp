#include "gpio.h"

#include <QCoreApplication>

GPIO::GPIO(QObject *parent) : QObject(parent)
{

}

GPIO::~GPIO()
{

}

void GPIO::kill()
{
    QCoreApplication::exit();
}

