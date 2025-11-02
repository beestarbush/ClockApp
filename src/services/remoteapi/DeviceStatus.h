#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include "SerializableObject.h"
#include <QDateTime>
#include <QJsonObject>
#include <QString>

/**
 * Device Status domain object
 * Represents the current state of a device
 */
class DeviceStatus : public SerializableObject
{
  public:
    DeviceStatus();

    // Status information
    QString deviceId;
    QString softwareVersion;
    double cpuTemperature;
    int uptimeSeconds;

    // SerializableObject interface
    QJsonObject toJson() const override;
    bool isValid() const override;
    QString typeName() const override
    {
        return "DeviceStatus";
    }

    // Endpoint routing
    QString getEndpoint() const override;
    QString getCreateEndpoint() const override
    {
        return "/api/devices/register";
    }
    QString getUpdateEndpoint() const override;
    QString getFetchEndpoint() const override;

    // Deserialization
    static DeviceStatus fromJson(const QJsonObject& json);
};

#endif // DEVICESTATUS_H
