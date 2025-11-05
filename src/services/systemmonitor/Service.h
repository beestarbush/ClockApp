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
class Temperature;
class System;

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
                     Temperature& temperature,
                     System& system,
                     Version::Service& version,
                     Notification::Service& notification,
                     QObject* parent = nullptr);

  private:
    void monitor();
    void report();

    RemoteApi::Service& m_remoteApi;
    Temperature& m_temperature;
    System& m_system;
    Version::Service& m_version;
    Notification::Service& m_notification;

    QTimer m_monitorTimer;
    QTimer m_reportTimer;
    bool m_isReporting;
};
} // namespace SystemMonitor

#endif // SERVICES_SYSTEMMONITOR_SERVICE_H
