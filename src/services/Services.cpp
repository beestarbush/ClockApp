#include "Services.h"
#include "DateTime.h"

Services::Services(QObject *parent) :
    QObject(parent),
    m_animationManager(new AnimationManager(this)),
    m_dateTime(new DateTime(this)),
    m_qmlInterface(new QmlInterface(this)),
    m_settings(new Settings(this)),
    m_timeSinceMarried(new TimeSince(m_settings->marriedTimerTimestamp(), this)),
    m_timeSinceKuiken(new TimeSince(m_settings->kuikenTimerTimestamp(), this)),
    m_version(new Version(this))
{
}
