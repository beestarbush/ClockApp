#ifndef HAL_STORAGE_DRIVER_H
#define HAL_STORAGE_DRIVER_H

#include <QObject>

namespace Storage
{
class Driver : public QObject
{
    Q_OBJECT

  public:
    Driver(QObject* parent = nullptr);
};
} // namespace Storage

#endif // HAL_STORAGE_DRIVER_H
