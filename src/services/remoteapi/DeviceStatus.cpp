#include "DeviceStatus.h"

DeviceStatus::DeviceStatus()
    : deviceId(),
      softwareVersion(),
      cpuTemperature(0.0),
      uptimeSeconds(0)
{
}

QJsonObject DeviceStatus::toJson() const
{
    QJsonObject json;
    json["device_id"] = deviceId;
    json["software_version"] = softwareVersion;
    json["cpu_temperature"] = cpuTemperature;
    json["uptime_seconds"] = uptimeSeconds;

    return json;
}

DeviceStatus DeviceStatus::fromJson(const QJsonObject& json)
{
    DeviceStatus status;

    status.deviceId = json["device_id"].toString();
    status.softwareVersion = json["software_version"].toString();
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
