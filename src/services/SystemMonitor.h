#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QObject>
#include <QTimer>

class RemoteApi;
class Temperature;
class System;
class Version;

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
                           QObject* parent = nullptr);

  private:
    void monitor();
    void report();

    RemoteApi& m_remoteApi;
    Temperature& m_temperature;
    System& m_system;
    Version& m_version;

    QTimer m_monitorTimer;
    QTimer m_reportTimer;
    bool m_isReporting;
};

#endif // SYSTEMMONITOR_H
