#include "TimerConfiguration.h"
#include <QDebug>
#include <QJsonObject>
#include <QSettings>
using namespace Common;

const QString PROPERTY_INITIALIZED_KEY = QStringLiteral("initialized");
const bool PROPERTY_INITIALIZED_DEFAULT = false;
const QString PROPERTY_TIMESTAMP_KEY = QStringLiteral("timestamp");
constexpr quint64 PROPERTY_TIMESTAMP_DEFAULT = 0;

TimerConfiguration::TimerConfiguration(QString name, QObject* parent)
    : Common::Configuration(name, parent),
      m_initialized(PROPERTY_INITIALIZED_DEFAULT),
      m_timestamp(PROPERTY_TIMESTAMP_DEFAULT)
{
}

void TimerConfiguration::load()
{
    Common::Configuration::load();

    static QSettings settings;
    settings.beginGroup(m_name);
    m_initialized = settings.value(PROPERTY_INITIALIZED_KEY, PROPERTY_INITIALIZED_DEFAULT).toBool();
    m_timestamp = settings.value(PROPERTY_TIMESTAMP_KEY, PROPERTY_TIMESTAMP_DEFAULT).toULongLong();
    settings.endGroup();
}

QJsonObject TimerConfiguration::toJson() const
{
    auto json = Common::Configuration::toJson();
    json[PROPERTY_INITIALIZED_KEY] = m_initialized;
    json[PROPERTY_TIMESTAMP_KEY] = static_cast<qint64>(m_timestamp);

    return json;
}

void TimerConfiguration::fromJson(const QJsonObject& json)
{
    Common::Configuration::fromJson(json);

    if (json.contains(PROPERTY_INITIALIZED_KEY)) {
        setInitialized(json[PROPERTY_INITIALIZED_KEY].toBool());
    }
    if (json.contains(PROPERTY_TIMESTAMP_KEY)) {
        setTimestamp(static_cast<quint64>(json[PROPERTY_TIMESTAMP_KEY].toInteger()));
    }
}

bool TimerConfiguration::isInitialized() const
{
    return m_initialized;
}

void TimerConfiguration::setInitialized(const bool& initialized)
{
    if (m_initialized == initialized) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(PROPERTY_INITIALIZED_KEY, initialized);
    settings.endGroup();

    m_initialized = initialized;
    emit initializedChanged();
}

quint64 TimerConfiguration::timestamp() const
{
    return m_timestamp;
}

void TimerConfiguration::setTimestamp(const quint64& timestamp)
{
    if (m_timestamp == timestamp) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(PROPERTY_TIMESTAMP_KEY, timestamp);
    settings.endGroup();

    m_timestamp = timestamp;
    emit timestampChanged();
}

TimerConfiguration& TimerConfiguration::operator=(const TimerConfiguration& other)
{
    if (this != &other) {
        Common::Configuration::operator=(other);
        setInitialized(other.m_initialized);
        setTimestamp(other.m_timestamp);
    }
    return *this;
}

namespace Common
{
QDebug operator<<(QDebug debug, const TimerConfiguration& config)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << static_cast<const Common::Configuration&>(config);
    debug.nospace() << "Common::TimerConfiguration: (\n"
                    << " - initialized=" << config.isInitialized() << "\n"
                    << " - timestamp=" << config.timestamp() << ")\n";
    return debug;
}
} // namespace Common
