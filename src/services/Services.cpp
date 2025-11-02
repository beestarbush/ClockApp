#include "Services.h"
#include "DateTime.h"
#include "hal/HAL.h"

Services::Services(HAL& hal, QObject* parent)
    : QObject(parent),
      m_version(new Version(this)),
      m_remoteApi(new RemoteApi(*hal.m_network, this)),
      m_mediaManager(new MediaManager(*m_remoteApi, this)),
      m_systemMonitor(new SystemMonitor(*m_remoteApi, *hal.m_temperature, *hal.m_system, *m_version, this)),
      m_dateTime(new DateTime(this)),
      m_notificationManager(new NotificationManager(this)),
      m_qmlInterface(new QmlInterface(this))
{
}
