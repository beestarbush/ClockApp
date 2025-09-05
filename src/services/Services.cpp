#include "Services.h"
#include "DateTime.h"

Services::Services(QObject *parent) :
    QObject(parent),
    m_animationManager(new AnimationManager(this)),
    m_dateTime(new DateTime(this)),
    m_qmlInterface(new QmlInterface(this)),
    m_settings(new Settings(this)),
    m_version(new Version(this))
{
}
