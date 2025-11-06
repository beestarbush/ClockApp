#include "Service.h"
#include "drivers/system/Driver.h"
#include "drivers/temperature/Driver.h"
#include "git_version.h"
#include "services/notification/Service.h"
#include "services/remoteapi/DeviceStatus.h"
#include "services/remoteapi/Service.h"
#include "services/version/Service.h"

#include <QDebug>
using namespace Services::SystemMonitor;

constexpr int MONITOR_INTERVAL = 10 * 1000;    // 10 seconds
constexpr int REPORT_INTERVAL = 5 * 60 * 1000; // 5 minutes

Service::Service(RemoteApi::Service& remoteApi,
                 Drivers::Temperature::Driver& temperature,
                 Drivers::System::Driver& system,
                 Version::Service& version,
                 Notification::Service& notificationManager,
                 QObject* parent)
    : QObject(parent),
      m_remoteApi(remoteApi),
      m_temperature(temperature),
      m_system(system),
      m_version(version),
      m_notificationManager(notificationManager),
      m_isReporting(false)
{
    // Configure timer
    m_monitorTimer.setSingleShot(false);
    m_monitorTimer.setInterval(MONITOR_INTERVAL);
    connect(&m_monitorTimer, &QTimer::timeout, this, &Service::monitor);
    monitor();

    m_reportTimer.setSingleShot(false);
    m_reportTimer.setInterval(REPORT_INTERVAL);
    connect(&m_reportTimer, &QTimer::timeout, this, &Service::report);
    report();

    // Connect to remote API connected changes, since we then want to start a report, and start the report timer. Also
    // when we disconnect the remote API, we want to stop the report timer.
    connect(&m_remoteApi, &RemoteApi::Service::connectedChanged, this, [this]() {
        if (m_remoteApi.connected()) {
            report();
        }
        else {
            m_reportTimer.stop();
        }
    });
}

void Service::monitor()
{
    if (m_temperature.valid() && m_temperature.processorTemperature() > 85000) { // 85.0 °C
        m_notificationManager.showWarning(
            QStringLiteral("High CPU temperature"),
            QStringLiteral("The CPU temperature is too high. Please ensure proper cooling."));
    }

    m_monitorTimer.start();
}

void Service::report()
{
    if (!m_remoteApi.enabled()) {
        qDebug() << "Remote API not enabled, skipping status report";
        return;
    }
    if (m_isReporting) {
        qDebug() << "Status report already in progress, skipping...";
        return;
    }

    m_isReporting = true;

    // Create DeviceStatus object
    DeviceStatus status;
    status.deviceId = m_remoteApi.deviceId();
    status.softwareVersion = m_version.tag();
    status.cpuTemperature = m_temperature.processorTemperature();
    status.uptimeSeconds = m_system.uptimeSeconds();

    // Send update to remote API
    m_remoteApi.updateObject(status, [this](bool success, const QString& error) {
        if (success) {
            qDebug() << "Device status reported successfully";
            m_isReporting = false;
            if (m_remoteApi.enabled()) {
                m_reportTimer.start();
            }
        }
        else {
            qWarning() << "Failed to report device status:" << error;
        }
    });
}
