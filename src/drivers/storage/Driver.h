#ifndef DRIVERS_STORAGE_DRIVER_H
#define DRIVERS_STORAGE_DRIVER_H

#include <QObject>

namespace Drivers::Storage
{
class Driver : public QObject
{
    Q_OBJECT

  public:
    Driver(QObject* parent = nullptr);
};
} // namespace Drivers::Storage

#endif // DRIVERS_STORAGE_DRIVER_H
