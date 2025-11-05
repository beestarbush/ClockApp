#include "Services.h"
#include "datetime/Service.h"
#include "hal/HAL.h"

Services::Services(HAL& hal, QObject* parent)
    : QObject(parent),
      m_version(new Version::Service(this)),
      m_remoteApi(new RemoteApi::Service(*hal.m_network, this)),
      m_notification(new Notification::Service(this)),
      m_media(new Media::Service(*m_remoteApi, this)),
      m_systemMonitor(new SystemMonitor::Service(*m_remoteApi, *hal.m_temperature, *hal.m_system, *m_version, *m_notification, this)),
      m_dateTime(new DateTime::Service(this)),
      m_qmlInterface(new QmlInterface::Service(this))
{
}

QmlInterface::Service* Services::qmlInterface() const
{
    return m_qmlInterface;
}