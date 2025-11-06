#ifndef DRIVERS_TEMPERATURE_DRIVER_H
#define DRIVERS_TEMPERATURE_DRIVER_H

#include <QObject>
#include <QTimer>

namespace Drivers::Temperature
{
class Driver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint32 processorTemperature READ processorTemperature NOTIFY processorTemperatureChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

  public:
    Driver(QObject* parent = nullptr);

    qint32 processorTemperature() const;
    bool valid() const;

  signals:
    void processorTemperatureChanged();
    void validChanged();

  private:
    void update();

    QTimer m_refreshTimer;
    qint32 m_processorTemperature;
    bool m_valid;
};
} // namespace Drivers::Temperature

#endif // DRIVERS_TEMPERATURE_DRIVER_H
