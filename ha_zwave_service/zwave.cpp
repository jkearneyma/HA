#include "zwave.h"

#include <set>

#include <QtDBus>
#include <QSemaphore>
#include <QMetaObject>

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

using namespace OpenZWave;

namespace {

std::set<uint32_t> homeIds;
QSemaphore exited;

template <typename T>
QString toString(Manager *manager, ValueID valueId, bool (Manager::*get)(ValueID const &, T*)) {
	T value;
	(manager->*get)(valueId, &value);
	return QString::number(value);
}

template <>
QString toString<std::string>(Manager *manager, ValueID valueId, bool (Manager::*get)(ValueID const &, std::string*)) {
	std::string value;
	(manager->*get)(valueId, &value);
	return QString(value.c_str());
}

QString toString(ValueID &v) {
	return QString::number(v.GetHomeId(), 16)
		+ "_" + QString::number(v.GetNodeId())
		+ "_" + QString::number(v.GetGenre())
		+ "_" + QString::number(v.GetCommandClassId())
		+ "_" + QString::number(v.GetInstance())
		+ "_" + QString::number(v.GetIndex())
		+ "_" + QString::number(v.GetType());
}

ValueID fromString(QString id) {
	auto parts = id.split("_");
	uint uints[7];
	bool parseOk = false;
	if ((parts.size() != 7)
		|| ((uints[0] = parts[0].toUInt(&parseOk, 16)), !parseOk)
	) throw std::runtime_error("bad ZWAVE address");
	for (size_t i = 1; i < 7; ++i) {
		if ((uints[i] = parts[i].toUInt(&parseOk)), !parseOk)
			throw std::runtime_error("bad ZWAVE address");
	}
	return ValueID(uints[0], uints[1], ValueID::ValueGenre(uints[2]), uints[3],
		uints[4], uints[5], ValueID::ValueType(uints[6]));
}

void OnNotification(Notification const* notification, void* context) {
	auto obj = reinterpret_cast<zwave *>(context);
	auto manager = Manager::Get();

	auto notify = [&](const QString &msg) {
		QMetaObject::invokeMethod(obj, "queueAnnounce", Qt::QueuedConnection,
			Q_ARG(QString, msg));
	};

	switch (notification->GetType()) {
	case Notification::Type_DriverReady:
		notify("driver=1");
		break;

	case Notification::Type_DriverFailed:
		notify("driver=0");
		break;

	case Notification::Type_AwakeNodesQueried:
	case Notification::Type_AllNodesQueried:
	case Notification::Type_AllNodesQueriedSomeDead:
//		numNodes = notification-> ????
		notify("ready=1");
		homeIds.insert(notification->GetHomeId());
		break;

	case Notification::Type_DriverRemoved:
		homeIds.erase(notification->GetHomeId());
		if (homeIds.empty()) {
			exited.release();
		}
		break;

	case Notification::Type_ValueAdded:
	case Notification::Type_ValueChanged:
	case Notification::Type_ValueRefreshed:
		{
			auto valueId = notification->GetValueID();
			auto label = manager->GetValueLabel(valueId);
			QString value("UNKNOWN");
			switch (valueId.GetType()) {
				case ValueID::ValueType_Bool:
					value = toString(manager, valueId, &Manager::GetValueAsBool);
					break;
				case ValueID::ValueType_Byte:
					value = toString(manager, valueId, &Manager::GetValueAsByte);
					break;
				case ValueID::ValueType_Decimal:
					value = toString(manager, valueId, &Manager::GetValueAsFloat);
					break;
				case ValueID::ValueType_Int:
					value = toString(manager, valueId, &Manager::GetValueAsInt);
					break;
				case ValueID::ValueType_Short:
					value = toString(manager, valueId, &Manager::GetValueAsShort);
					break;
				case ValueID::ValueType_String:
					value = toString(manager, valueId, &Manager::GetValueAsString);
					break;
				case ValueID::ValueType_List:
					value = toString<int>(manager, valueId, &Manager::GetValueListSelection);
					break;
			case ValueID::ValueType_Button:
				// supposedly write-only
				value = toString(manager, valueId, &Manager::GetValueAsString);
				break;
			case ValueID::ValueType_Schedule: // complex
			case ValueID::ValueType_Raw: // byte array
				// ???
				break;
			}
			QMetaObject::invokeMethod(obj, "queueStateChange", Qt::QueuedConnection,
				Q_ARG(QString, toString(valueId)), Q_ARG(QString, value));
		}
		break;

	default:
		break;
	}
}

} // namespace

zwave::zwave(
    QString exePath,
    QString device_,
    int logLevel,
    QObject *parent
) :
  QObject(parent),
  device(device_)
{
	QDBusConnection::sessionBus().registerObject(
		"/",
		this,
		QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals
	);

	auto configDir = exePath + "/config/";
	auto zwcfgDir = exePath + "/";
	auto options = Options::Create(
		configDir.toUtf8().data(),
		zwcfgDir.toUtf8().data(),
		""
	);
	options->AddOptionInt("SaveLogLevel", LogLevel(logLevel));
	options->AddOptionInt("QueueLogLevel", LogLevel_Alert);
	options->AddOptionInt("DumpTriggerLevel", LogLevel_Error);
	options->AddOptionInt("PollInterval", 100);
	options->AddOptionBool("IntervalBetweenPolls", true);
	options->AddOptionBool("SaveConfiguration", false);
	options->AddOptionString("LogFileName", "zwave.log", false);
	options->AddOptionBool("ConsoleOutput", false);
	options->AddOptionBool("ValidateValueChanges", true);
	options->Lock();

	auto manager = Manager::Create();
	manager->AddWatcher(OnNotification, this);
	// TODO: should be an array
	manager->AddDriver(device.toUtf8().data());

	queueAnnounce(
		QString("version=") + manager->getVersionAsString().c_str()
	);
}

zwave::~zwave() {
	Manager *manager = Manager::Get();
	if (manager!= nullptr) {
		// TODO: should be an array
		Manager::Get()->RemoveDriver(device.toUtf8().data());
		exited.acquire();
		Manager::Get()->RemoveWatcher(OnNotification, manager);
		Manager::Destroy();
		Options::Destroy();
	}
}

void zwave::updateState(QString service, QString id) {
	try {
		auto manager = Manager::Get();
		if ((manager != nullptr) && (service == "ZWAVE")) {
			ValueID valueId = fromString(id);
			manager->RefreshValue(valueId);
		}
	}
	catch (...) {
	}
}

void zwave::setState(QString service, QString id, QString value)
{
	try {
		auto manager = Manager::Get();
		if ((manager != nullptr) && (service == "ZWAVE")) {
			ValueID valueId = fromString(id);
			bool parseOk = false;
			switch (valueId.GetType()) {
			case ValueID::ValueType_Bool: {
					uint v = value.toUInt(&parseOk);
					parseOk && manager->SetValue(valueId, bool(v));
				}
				break;
			case ValueID::ValueType_Byte: {
					uint v = value.toUInt(&parseOk);
					parseOk && manager->SetValue(valueId, uint8(v));
				}
				break;
			case ValueID::ValueType_Decimal: {
					float v = value.toFloat(&parseOk);
					parseOk && manager->SetValue(valueId, v);
				}
				break;
			case ValueID::ValueType_List:
			case ValueID::ValueType_Int: {
					int v = value.toInt(&parseOk);
					parseOk && manager->SetValue(valueId, v);
				}
				break;
			case ValueID::ValueType_Short: {
					short v = value.toShort(&parseOk);
					parseOk && manager->SetValue(valueId, v);
				}
				break;
			case ValueID::ValueType_String: {
					parseOk && manager->SetValue(valueId, value.toUtf8().data());
				}
				break;
			default:
				break;
			}
		}
	}
	catch (...) {
	}
}

void zwave::healNetwork(bool doRR) {
	for (auto homeId : homeIds) {
		Manager::Get()->HealNetwork(homeId, doRR);
	}
}

void zwave::writeConfigs() {
	for (auto homeId : homeIds) {
		Manager::Get()->WriteConfig(homeId);
	}
}

void zwave::kill()
{
	QCoreApplication::exit();
}

void zwave::queueAnnounce(QString msg) {
	emit announce("ZWAVE", msg);
}

void zwave::queueStateChange(QString id, QString value) {
	emit stateChange("ZWAVE", id, value);
}

