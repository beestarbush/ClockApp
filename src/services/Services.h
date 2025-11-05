#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "datetime/Service.h"
#include "media/Service.h"
#include "notification/Service.h"
#include "qmlinterface/Service.h"
#include "remoteapi/Service.h"
#include "systemmonitor/Service.h"
#include "version/Service.h"

class HAL;

class Services : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Media::Service* media MEMBER m_media CONSTANT)
    Q_PROPERTY(DateTime::Service* dateTime MEMBER m_dateTime CONSTANT)
    Q_PROPERTY(Notification::Service* notification MEMBER m_notification CONSTANT)
    Q_PROPERTY(QmlInterface::Service* qmlInterface MEMBER m_qmlInterface CONSTANT)
    Q_PROPERTY(Version::Service* version MEMBER m_version CONSTANT)
    Q_PROPERTY(RemoteApi::Service* remoteApi MEMBER m_remoteApi CONSTANT)
    Q_PROPERTY(SystemMonitor::Service* systemMonitor MEMBER m_systemMonitor CONSTANT)

  public:
    friend class Applications;

    Services(HAL& hal, QObject* parent = nullptr);

    QmlInterface::Service* qmlInterface() const;

  private:
    Version::Service* m_version;
    RemoteApi::Service* m_remoteApi;
    Notification::Service* m_notification;
    Media::Service* m_media;
    SystemMonitor::Service* m_systemMonitor;
    DateTime::Service* m_dateTime;
    QmlInterface::Service* m_qmlInterface;
};

#endif // SERVICES_H