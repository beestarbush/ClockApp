#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "DateTime.h"
#include "MediaManager.h"
#include "NotificationManager.h"
#include "QmlInterface.h"
#include "RemoteApi.h"
#include "Version.h"

class HAL;

class Services : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MediaManager* mediaManager MEMBER m_mediaManager CONSTANT)
    Q_PROPERTY(DateTime* dateTime MEMBER m_dateTime CONSTANT)
    Q_PROPERTY(NotificationManager* notificationManager MEMBER m_notificationManager CONSTANT)
    Q_PROPERTY(QmlInterface* qmlInterface MEMBER m_qmlInterface CONSTANT)
    Q_PROPERTY(Version* version MEMBER m_version CONSTANT)
    Q_PROPERTY(RemoteApi* remoteApi MEMBER m_remoteApi CONSTANT)

  public:
    friend class Applications;

    Services(HAL& hal, QObject* parent = nullptr);

  private:
    RemoteApi* m_remoteApi;
    MediaManager* m_mediaManager;
    DateTime* m_dateTime;
    NotificationManager* m_notificationManager;
    QmlInterface* m_qmlInterface;
    Version* m_version;
};

#endif // SERVICES_H