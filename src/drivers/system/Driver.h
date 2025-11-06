#ifndef DRIVERS_SYSTEM_DRIVER_H
#define DRIVERS_SYSTEM_DRIVER_H

#include <QObject>
#include <QProcess>
#include <QTimer>

namespace Drivers::System
{
class Driver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint64_t uptimeSeconds READ uptimeSeconds NOTIFY uptimeSecondsChanged)

  public:
    Driver(QObject* parent = nullptr);

    Q_INVOKABLE void shutdown();
    Q_INVOKABLE void reboot();

    uint64_t uptimeSeconds() const;

  signals:
    void uptimeSecondsChanged();

  private:
    void onShutdownFinished();
    void onRebootFinished();

    QProcess m_shutdownProcess;
    QProcess m_rebootProcess;
    QTimer m_uptimeTimer;
    uint64_t m_uptimeSeconds;
};
} // namespace Drivers::System

#endif // DRIVERS_SYSTEM_DRIVER_H
