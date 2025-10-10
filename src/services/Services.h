#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "AnimationManager.h"
#include "DateTime.h"
#include "QmlInterface.h"
#include "Version.h"

class Services : public QObject
{
	Q_OBJECT
    Q_PROPERTY(AnimationManager* animationManager MEMBER m_animationManager CONSTANT)
    Q_PROPERTY(DateTime* dateTime MEMBER m_dateTime CONSTANT)
    Q_PROPERTY(QmlInterface* qmlInterface MEMBER m_qmlInterface CONSTANT)
    Q_PROPERTY(Version* version MEMBER m_version CONSTANT)

public:
    friend class Applications;

	Services(QObject *parent = nullptr);

private:
    AnimationManager* m_animationManager;
    DateTime* m_dateTime;
    QmlInterface* m_qmlInterface;
    Version* m_version;
};

#endif // SERVICES_H