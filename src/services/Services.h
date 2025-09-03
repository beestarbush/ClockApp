#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "AnimationManager.h"
#include "DateTime.h"
#include "QmlInterface.h"
#include "Settings.h"
#include "TimeSince.h"
#include "Version.h"

class Services : public QObject
{
	Q_OBJECT
    Q_PROPERTY(AnimationManager* animationManager MEMBER m_animationManager CONSTANT)
    Q_PROPERTY(DateTime* dateTime MEMBER m_dateTime CONSTANT)
    Q_PROPERTY(QmlInterface* qmlInterface MEMBER m_qmlInterface CONSTANT)
    Q_PROPERTY(Settings* settings MEMBER m_settings CONSTANT)
    Q_PROPERTY(TimeSince* timeSinceMarried MEMBER m_timeSinceMarried CONSTANT)
    Q_PROPERTY(TimeSince* timeSinceKuiken MEMBER m_timeSinceKuiken CONSTANT)
    Q_PROPERTY(Version* version MEMBER m_version CONSTANT)

public:
	Services(QObject *parent = nullptr);

private:
    AnimationManager* m_animationManager;
    DateTime* m_dateTime;
    QmlInterface* m_qmlInterface;
    Settings* m_settings;
    TimeSince* m_timeSinceMarried;
    TimeSince* m_timeSinceKuiken;
    Version* m_version;
};

#endif // SERVICES_H