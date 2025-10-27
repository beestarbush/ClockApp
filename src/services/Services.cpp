#include "Services.h"
#include "DateTime.h"
#include "hal/HAL.h"

Services::Services(HAL& hal, QObject* parent)
    : QObject(parent),
      m_remoteApi(new RemoteApi(*hal.m_network, this)),
      m_mediaManager(new MediaManager(*m_remoteApi, this)),
      m_dateTime(new DateTime(this)),
      m_notificationManager(new NotificationManager(this)),
      m_qmlInterface(new QmlInterface(this)),
      m_version(new Version(this))
{
    // m_remoteApi->setEnabled(true);
    // m_remoteApi->setServerUrl("https://bijsterbosch.org/clock");
    // m_remoteApi->setDeviceId("clock-0000");
}
