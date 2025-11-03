#ifndef SERVICES_SYSTEM_MONITOR_H
#define SERVICES_SYSTEM_MONITOR_H

#include <QObject>
#include <QTimer>

class RemoteApi;
class Temperature;
class System;
class Version;
class NotificationManager;

/**
 * SystemMonitor
 *
 * Periodically monitors system status (temperature, uptime, etc.) and
 * reports it to the remote API server.
 */
class SystemMonitor : public QObject
{
    Q_OBJECT

  public:
    explicit SystemMonitor(RemoteApi& remoteApi,
                           Temperature& temperature,
                           System& system,
                           Version& version,
                           NotificationManager& notificationManager,
                           QObject* parent = nullptr);

  private:
    void monitor();
    void report();

    RemoteApi& m_remoteApi;
    Temperature& m_temperature;
    System& m_system;
    Version& m_version;
    NotificationManager& m_notificationManager;

    QTimer m_monitorTimer;
    QTimer m_reportTimer;
    bool m_isReporting;
};

#endif // SERVICES_SYSTEM_MONITOR_H
