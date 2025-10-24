#include "Services.h"
#include "DateTime.h"

Services::Services(QObject *parent) :
    QObject(parent),
    m_mediaManager(new MediaManager(this)),
    m_dateTime(new DateTime(this)),
    m_notificationManager(new NotificationManager(this)),
    m_qmlInterface(new QmlInterface(this)),
    m_version(new Version(this))
{
}
