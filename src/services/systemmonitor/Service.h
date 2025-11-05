#ifndef SERVICES_SYSTEMMONITOR_SERVICE_H
#define SERVICES_SYSTEMMONITOR_SERVICE_H

#include <QObject>
#include <QTimer>

namespace RemoteApi
{
class Service;
}
namespace Version
{
class Service;
}
namespace Notification
{
class Service;
}
namespace Temperature
{
class Driver;
}
namespace System
{
class Driver;
}

namespace SystemMonitor
{
/**
 * Service
 *
 * Periodically monitors system status (temperature, uptime, etc.) and
 * reports it to the remote API server.
 */
class Service : public QObject
{
    Q_OBJECT

  public:
    explicit Service(RemoteApi::Service& remoteApi,
                     Temperature::Driver& temperature,
                     System::Driver& system,
                     Version::Service& version,
                     Notification::Service& notificationManager,
                     QObject* parent = nullptr);

  private:
    void monitor();
    void report();

    RemoteApi::Service& m_remoteApi;
    Temperature::Driver& m_temperature;
    System::Driver& m_system;
    Version::Service& m_version;
    Notification::Service& m_notificationManager;

    QTimer m_monitorTimer;
    QTimer m_reportTimer;
    bool m_isReporting;
};
} // namespace SystemMonitor

#endif // SERVICES_SYSTEMMONITOR_SERVICE_H
