#ifndef SERVICES_NOTIFICATION_MODEL_H
#define SERVICES_NOTIFICATION_MODEL_H

#include "Item.h"
#include <QAbstractListModel>
#include <QList>

namespace Services::Notification
{
class Model : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int activeCount READ getActiveCount NOTIFY activeCountChanged)
    Q_PROPERTY(bool hasNotifications READ hasNotifications NOTIFY hasNotificationsChanged)
    Q_PROPERTY(Notification::Item* highestPriorityNotification READ getHighestPriorityNotification NOTIFY highestPriorityChanged)

  public:
    enum Roles
    {
        TitleRole = Qt::UserRole + 1,
        MessageRole,
        TypeRole,
        DurationRole,
        TimestampRole,
        IdRole,
        ActiveRole
    };

    explicit Model(QObject* parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Property getters
    int getActiveCount() const;
    bool hasNotifications() const;
    Item* getHighestPriorityNotification();

    // Model manipulation
    Q_INVOKABLE Item* get(int index) const;
    Q_INVOKABLE void addNotification(const QString& title, const QString& message, Item::Type type, bool active, quint64 duration);
    Q_INVOKABLE void removeNotification(const quint64 id);
    Q_INVOKABLE void removeNotificationAt(int index);
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE void clearInactive();
    Q_INVOKABLE void setNotificationActive(const quint64 id, bool active);
    Q_INVOKABLE void toggleNotificationActive(const quint64 id);

  signals:
    void countChanged();
    void activeCountChanged();
    void hasNotificationsChanged();
    void highestPriorityChanged();
    void notificationAdded(const quint64 id);
    void notificationRemoved(const quint64 id);

  private:
    void insertNotificationSorted(Item* notification);
    void setupAutoRemove(Item* notification);
    void updateHighestPriorityNotification();

    QList<Item*> m_notifications;
    Item* m_highestPriorityItem;
};
} // namespace Services::Notification

#endif // SERVICES_NOTIFICATION_MODEL_H
