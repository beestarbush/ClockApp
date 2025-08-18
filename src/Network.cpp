#include "Network.h"
#include <QDebug>
#include <QNetworkInterface>

#ifdef PLATFORM_IS_TARGET
const QString INTERFACE_NAME = QStringLiteral("wlan0");
#else
const QString INTERFACE_NAME = QStringLiteral("eth0");
#endif
constexpr quint16 REFRESH_INTERVAL_MS = 30 * 1000; // Refresh every 30 seconds

Network::Network(QObject *parent) :
    QObject(parent),
    m_refreshTimer(this),
    m_interfaceName(INTERFACE_NAME),
    m_connected(false),
    m_running(false),
    m_ipAddress(""),
    m_subnetMask("")
{
    update();

    connect(&m_refreshTimer, &QTimer::timeout, this, &Network::update);
    m_refreshTimer.start(REFRESH_INTERVAL_MS); // Refresh every 30 seconds
}

QString Network::interfaceName() const
{
    return m_interfaceName;
}

bool Network::connected() const
{
    return m_connected;
}

bool Network::running() const
{
    return m_running;
}

QString Network::ipAddress() const
{
    return m_ipAddress;
}

QString Network::subnetMask() const
{
    return m_subnetMask;
}

void Network::update()
{
    for (const QNetworkInterface &interface : QNetworkInterface::allInterfaces()) {
        if (interface.name() == m_interfaceName) {
            m_connected = interface.flags() & QNetworkInterface::IsUp;
            m_running = interface.flags() & QNetworkInterface::IsRunning;
            emit connectedChanged();
            emit runningChanged();
            
            auto entries = interface.addressEntries();
            if (!entries.isEmpty()) {
                m_ipAddress = entries.first().ip().toString();
                m_subnetMask = entries.first().netmask().toString();
                emit ipAddressChanged();
                emit subnetMaskChanged();
            }
            else {
                m_ipAddress.clear();
                m_subnetMask.clear();
                emit ipAddressChanged();
                emit subnetMaskChanged();
            }
            break;
        }
    }
}