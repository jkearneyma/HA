#include "zwave.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wignored-qualifiers"
#include "Options.h"
#include "Manager.h"
#include "Driver.h"
#include "Node.h"
#include "Group.h"
#include "Notification.h"
#include "value_classes/ValueStore.h"
#include "value_classes/Value.h"
#include "value_classes/ValueBool.h"
#include "platform/Log.h"
#pragma clang diagnostic pop

zwave::zwave(QString device, QObject *parent) : QObject(parent)
{

}

zwave::~zwave()
{

}

void zwave::setState(QString id, int toState)
{

}

void zwave::update()
{

}

