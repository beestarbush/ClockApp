#include "DeviceStatus.h"

DeviceStatus::DeviceStatus()
    : notificationCount(0)
    , cpuTemperature(0.0)
    , uptimeSeconds(0)
{
}

QJsonObject DeviceStatus::toJson() const
{
    QJsonObject json;
    
    // Core fields
    if (!deviceId.isEmpty()) {
        json["device_id"] = deviceId;
    }
    if (!deviceName.isEmpty()) {
        json["name"] = deviceName;
    }
    if (!softwareVersion.isEmpty()) {
        json["software_version"] = softwareVersion;
    }
    
    json["notification_count"] = notificationCount;
    
    if (lastSeen.isValid()) {
        json["last_seen"] = lastSeen.toString(Qt::ISODate);
    }
    
    // Optional fields
    if (!currentApplication.isEmpty()) {
        json["current_application"] = currentApplication;
    }
    if (cpuTemperature > 0.0) {
        json["cpu_temperature"] = cpuTemperature;
    }
    if (uptimeSeconds > 0) {
        json["uptime_seconds"] = uptimeSeconds;
    }
    
    return json;
}

DeviceStatus DeviceStatus::fromJson(const QJsonObject &json)
{
    DeviceStatus status;
    
    status.deviceId = json["device_id"].toString();
    status.deviceName = json["name"].toString();
    status.softwareVersion = json["software_version"].toString();
    status.notificationCount = json["notification_count"].toInt();
    
    QString lastSeenStr = json["last_seen"].toString();
    if (!lastSeenStr.isEmpty()) {
        status.lastSeen = QDateTime::fromString(lastSeenStr, Qt::ISODate);
    }
    
    status.currentApplication = json["current_application"].toString();
    status.cpuTemperature = json["cpu_temperature"].toDouble();
    status.uptimeSeconds = json["uptime_seconds"].toInt();
    
    return status;
}

bool DeviceStatus::isValid() const
{
    return !deviceId.isEmpty();
}

QString DeviceStatus::getEndpoint() const
{
    return QString("/api/devices");
}

QString DeviceStatus::getUpdateEndpoint() const
{
    if (deviceId.isEmpty()) {
        return getEndpoint();
    }
    return QString("/api/devices/update/%1").arg(deviceId);
}

QString DeviceStatus::getFetchEndpoint() const
{
    if (deviceId.isEmpty()) {
        return "/api/devices";
    }
    return QString("/api/devices/%1").arg(deviceId);
}
