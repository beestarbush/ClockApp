#include "Driver.h"
#include <QDebug>
#include <QNetworkInterface>
using namespace Drivers::Network;

#ifdef PLATFORM_IS_TARGET
const QString INTERFACE_NAME = QStringLiteral("wlan0");
#else
const QString INTERFACE_NAME = QStringLiteral("eth0");
#endif
constexpr quint16 REFRESH_INTERVAL_MS = 30 * 1000; // Refresh every 30 seconds

Driver::Driver(QObject* parent)
    : QObject(parent),
      m_refreshTimer(this),
      m_interfaceName(INTERFACE_NAME),
      m_connected(false),
      m_running(false),
      m_ipAddress(""),
      m_subnetMask("")
{
    update();

    connect(&m_refreshTimer, &QTimer::timeout, this, &Driver::update);
    m_refreshTimer.start(REFRESH_INTERVAL_MS); // Refresh every 30 seconds
}

QString Driver::interfaceName() const
{
    return m_interfaceName;
}

bool Driver::connected() const
{
    return m_connected;
}

bool Driver::running() const
{
    return m_running;
}

QString Driver::ipAddress() const
{
    return m_ipAddress;
}

QString Driver::subnetMask() const
{
    return m_subnetMask;
}

void Driver::update()
{
    for (const QNetworkInterface& interface : QNetworkInterface::allInterfaces()) {
        if (interface.name() == m_interfaceName) {
            bool wasConnected = m_connected;
            bool wasRunning = m_running;

            m_connected = interface.flags() & QNetworkInterface::IsUp;
            m_running = interface.flags() & QNetworkInterface::IsRunning;

            if (m_connected != wasConnected) {
                emit connectedChanged();
            }
            if (m_running != wasRunning) {
                emit runningChanged();
            }

            auto entries = interface.addressEntries();
            if (!entries.isEmpty()) {
                QString newIpAddress = entries.first().ip().toString();
                QString newSubnetMask = entries.first().netmask().toString();

                if (m_ipAddress != newIpAddress) {
                    m_ipAddress = newIpAddress;
                    emit ipAddressChanged();
                }
                if (m_subnetMask != newSubnetMask) {
                    m_subnetMask = newSubnetMask;
                    emit subnetMaskChanged();
                }
            }
            else {
                if (!m_ipAddress.isEmpty()) {
                    m_ipAddress.clear();
                    emit ipAddressChanged();
                }
                if (!m_subnetMask.isEmpty()) {
                    m_subnetMask.clear();
                    emit subnetMaskChanged();
                }
            }
            break;
        }
    }
}
