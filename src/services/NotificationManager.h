#ifndef SERVICES_NOTIFICATIONMANAGER_H
#define SERVICES_NOTIFICATIONMANAGER_H

#include <QAbstractListModel>
#include "Notification.h"

class NotificationManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int activeCount READ getActiveCount NOTIFY activeCountChanged)
    Q_PROPERTY(bool hasNotifications READ hasNotifications NOTIFY hasNotificationsChanged)
    Q_PROPERTY(bool isVisible READ isVisible NOTIFY isVisibleChanged)
    Q_PROPERTY(Notification* highestPriorityNotification READ getHighestPriorityNotification NOTIFY highestPriorityChanged)

public:
    enum Roles {
        TitleRole = Qt::UserRole + 1,
        MessageRole,
        TypeRole,
        DurationRole,
        TimestampRole,
        IdRole,
        ActiveRole
    };

    NotificationManager(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Property getters
    int getActiveCount() const;
    bool hasNotifications() const;
    bool isVisible() const;
    Notification* getHighestPriorityNotification();

public slots:
    void addNotification(const QString& title, const QString& message, Notification::Type type = Notification::Info, quint64 duration = 10000);
    void showInfo(const QString& title, const QString& message, quint64 duration = 10000);    // 10 seconds
    void showWarning(const QString& title, const QString& message, quint64 duration = 20000); // 20 seconds
    void showError(const QString& title, const QString& message, quint64 duration = 0);       // persistent
    void removeNotification(const quint64 id);
    void removeNotificationAt(int index);
    void deleteNotification(const quint64 id);  // Permanently delete notification
    void clearAll();
    void clearInactive();  // Remove only inactive notifications
    void setNotificationActive(const quint64 id, bool active);
    void toggleNotificationActive(const quint64 id);

signals:
    void countChanged();
    void activeCountChanged();
    void hasNotificationsChanged();
    void isVisibleChanged();
    void highestPriorityChanged();
    void notificationAdded(const quint64 id);
    void notificationRemoved(const quint64 id);

private:
    void insertNotificationSorted(Notification* notification);
    void setupAutoRemove(Notification* notification);
    void updateHighestPriorityNotification();

    QList<Notification*> m_notifications;
    bool m_isVisible;
    Notification* m_highestPriorityItem;
};

#endif // SERVICES_NOTIFICATIONMANAGER_H
