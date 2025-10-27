#ifndef DEVICESTATUS_H
#define DEVICESTATUS_H

#include "SerializableObject.h"
#include <QString>
#include <QDateTime>
#include <QJsonObject>

/**
 * Device Status domain object
 * Represents the current state of a device
 */
class DeviceStatus : public SerializableObject
{
public:
    DeviceStatus();
    
    // Device identification
    QString deviceName;
    
    // Status information
    QString softwareVersion;
    int notificationCount;
    QDateTime lastSeen;
    
    // Additional metadata (optional)
    QString currentApplication;
    double cpuTemperature;
    int uptimeSeconds;
    
    // SerializableObject interface
    QJsonObject toJson() const override;
    bool isValid() const override;
    QString typeName() const override { return "DeviceStatus"; }
    
    // Endpoint routing
    QString getEndpoint() const override;
    QString getCreateEndpoint() const override { return "/api/devices/register"; }
    QString getUpdateEndpoint() const override;
    QString getFetchEndpoint() const override;
    
    // Deserialization
    static DeviceStatus fromJson(const QJsonObject &json);
};

#endif // DEVICESTATUS_H
