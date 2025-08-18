#include "Temperature.h"
#include <QDebug>
#include <QFile>

#ifdef PLATFORM_IS_TARGET
const QString PROCESSOR_TEMPERATURE_INTERFACE_NAME = QStringLiteral("/sys/class/thermal/thermal_zone0/hwmon0/temp1_input");
#else
const QString PROCESSOR_TEMPERATURE_INTERFACE_NAME = QStringLiteral("/tmp/processor_temperature");
#endif
constexpr quint16 REFRESH_INTERVAL_MS = 5 * 1000; // Refresh every 5 seconds

Temperature::Temperature(QObject *parent) :
    QObject(parent),
    m_refreshTimer(this),
    m_processorTemperature(0),
    m_valid(false)
{
    update();

    connect(&m_refreshTimer, &QTimer::timeout, this, &Temperature::update);
    m_refreshTimer.start(REFRESH_INTERVAL_MS);
}

qint32 Temperature::processorTemperature() const
{
    return m_processorTemperature;
}

bool Temperature::valid() const
{
    return m_valid;
}

void Temperature::update()
{
    QFile file(PROCESSOR_TEMPERATURE_INTERFACE_NAME);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        bool ok;
        qint32 value = in.readLine().toInt(&ok);
        file.close();
        if (!ok)
        {
            qWarning() << "Failed to read value from file:" << PROCESSOR_TEMPERATURE_INTERFACE_NAME;
            m_valid = false;
            emit validChanged();
            return;
        }

        m_processorTemperature = value;
        m_valid = true;
        emit validChanged();
        emit processorTemperatureChanged();
        return;
    }
    m_valid = false;
    emit validChanged();
    return;
}