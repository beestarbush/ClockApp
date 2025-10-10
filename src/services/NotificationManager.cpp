#include "NotificationManager.h"
#include <QDateTime>

NotificationManager::NotificationManager(QObject *parent)
    : QAbstractListModel(parent)
    , m_isVisible(false)
    , m_highestPriorityItem(nullptr)
{
    m_highestPriorityItem = new Notification(this);
}

int NotificationManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_notifications.size();
}

QVariant NotificationManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_notifications.size())
        return QVariant();

    const Notification* notification = m_notifications.at(index.row());

    switch (role) {
    case TitleRole:
        return notification->title();
    case MessageRole:
        return notification->message();
    case TypeRole:
        return notification->type();
    case DurationRole:
        return notification->duration();
    case TimestampRole:
        return notification->timestamp();
    case IdRole:
        return notification->id();
    case ActiveRole:
        return notification->isActive();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> NotificationManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[MessageRole] = "message";
    roles[TypeRole] = "type";
    roles[DurationRole] = "duration";
    roles[TimestampRole] = "timestamp";
    roles[IdRole] = "notificationId";
    roles[ActiveRole] = "isActive";
    return roles;
}

int NotificationManager::getActiveCount() const
{
    int activeCount = 0;
    for (const auto* notification : m_notifications) {
        if (notification->isActive()) {
            activeCount++;
        }
    }
    return activeCount;
}

bool NotificationManager::hasNotifications() const
{
    // Check if there are any active notifications
    for (const auto* notification : m_notifications) {
        if (notification->isActive()) {
            return true;
        }
    }
    return false;
}

bool NotificationManager::isVisible() const
{
    return m_isVisible && hasNotifications();
}

Notification* NotificationManager::getHighestPriorityNotification()
{
    return m_highestPriorityItem;
}

void NotificationManager::updateHighestPriorityNotification()
{
    // Find the first active notification (since list is sorted by type/priority)
    for (const auto* notification : m_notifications) {
        if (notification->isActive()) {
            *m_highestPriorityItem = *notification;
            return;
        }
    }

    // No active notifications found - clear the item
    m_highestPriorityItem->reset();
}

void NotificationManager::addNotification(const QString& title, const QString& message, Notification::Type type, quint64 duration)
{
    Notification* notification = new Notification(title, message, type, duration, this);

    insertNotificationSorted(notification);

    // Set up auto-removal timer if duration > 0
    if (duration > 0) {
        setupAutoRemove(notification);
    }

    // Update visibility - should be visible when we have active notifications
    bool shouldBeVisible = hasNotifications();
    if (m_isVisible != shouldBeVisible) {
        m_isVisible = shouldBeVisible;
        emit isVisibleChanged();
    }

    updateHighestPriorityNotification();
    emit highestPriorityChanged();
    emit notificationAdded(notification->id());
}

void NotificationManager::showInfo(const QString& title, const QString& message, quint64 duration)
{
    addNotification(title, message, Notification::Type::Info, duration);
}

void NotificationManager::showWarning(const QString& title, const QString& message, quint64 duration)
{
    addNotification(title, message, Notification::Type::Warning, duration);
}

void NotificationManager::showError(const QString& title, const QString& message, quint64 duration)
{
    addNotification(title, message, Notification::Type::Error, duration);
}

void NotificationManager::removeNotification(const quint64 id)
{
    for (int i = 0; i < m_notifications.size(); ++i) {
        if (m_notifications[i]->id() == id) {
            removeNotificationAt(i);
            break;
        }
    }
}

void NotificationManager::removeNotificationAt(int index)
{
    if (index < 0 || index >= m_notifications.size())
        return;

    Notification* notification = m_notifications[index];
    quint64 id = notification->id();

    // Clean up auto-remove timer
    if (notification->autoRemoveTimer()) {
        notification->autoRemoveTimer()->stop();
        notification->autoRemoveTimer()->deleteLater();
        notification->setAutoRemoveTimer(nullptr);
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_notifications.removeAt(index);
    endRemoveRows();

    // Delete the notification item
    notification->deleteLater();

    emit countChanged();
    emit activeCountChanged();
    emit hasNotificationsChanged();
    updateHighestPriorityNotification();
    emit highestPriorityChanged();
    emit notificationRemoved(id);

    // Update visibility
    if (m_notifications.isEmpty() && m_isVisible) {
        m_isVisible = false;
        emit isVisibleChanged();
    }
}

void NotificationManager::deleteNotification(const quint64 id)
{
    // Same as removeNotification but with a different name for clarity
    removeNotification(id);
}

void NotificationManager::clearInactive()
{
    // Remove all inactive notifications
    for (int i = m_notifications.size() - 1; i >= 0; --i) {
        if (!m_notifications[i]->isActive()) {
            removeNotificationAt(i);
        }
    }
}

void NotificationManager::clearAll()
{
    if (m_notifications.isEmpty())
        return;

    // Clean up all timers and delete notification items
    for (auto* notification : m_notifications) {
        if (notification->autoRemoveTimer()) {
            notification->autoRemoveTimer()->stop();
            notification->autoRemoveTimer()->deleteLater();
            notification->setAutoRemoveTimer(nullptr);
        }
        notification->deleteLater();
    }

    beginResetModel();
    m_notifications.clear();
    endResetModel();

    if (m_isVisible) {
        m_isVisible = false;
        emit isVisibleChanged();
    }

    emit countChanged();
    emit activeCountChanged();
    emit hasNotificationsChanged();
    updateHighestPriorityNotification();
    emit highestPriorityChanged();
}

void NotificationManager::insertNotificationSorted(Notification* notification)
{
    int insertIndex = 0;

    // Find insertion point based on type (higher priority first)
    // For same type: active notifications come before inactive ones
    // For same type and status: newer notifications come first
    for (int i = 0; i < m_notifications.size(); ++i) {
        if (notification->type() > m_notifications[i]->type()) {
            insertIndex = i;
            break;
        } else if (notification->type() == m_notifications[i]->type()) {
            // Same type - check active status
            if (notification->isActive() && !m_notifications[i]->isActive()) {
                // New notification is active, existing is inactive - insert here
                insertIndex = i;
                break;
            } else if (notification->isActive() == m_notifications[i]->isActive()) {
                // Same type and same status - newer comes first
                if (notification->timestamp() >= m_notifications[i]->timestamp()) {
                    insertIndex = i;
                    break;
                }
            }
            // If new is inactive and existing is active, continue searching
        }
        insertIndex = i + 1;
    }

    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    m_notifications.insert(insertIndex, notification);
    endInsertRows();

    emit countChanged();
    emit activeCountChanged();
    emit hasNotificationsChanged();
}

void NotificationManager::setupAutoRemove(Notification* notification)
{
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(notification->duration());

    connect(timer, &QTimer::timeout, this, [this, id = notification->id()]() {
        // Instead of removing, just inactivate the notification
        setNotificationActive(id, false);
    });

    notification->setAutoRemoveTimer(timer);
    timer->start();
}

void NotificationManager::setNotificationActive(const quint64 id, bool active)
{
    for (int i = 0; i < m_notifications.size(); ++i) {
        if (m_notifications[i]->id() == id) {
            if (m_notifications[i]->isActive() != active) {
                bool hadActiveNotifications = hasNotifications();

                m_notifications[i]->setIsActive(active);

                // Emit dataChanged signal to update the view
                QModelIndex index = createIndex(i, 0);
                emit dataChanged(index, index, {ActiveRole});

                // Check if the active notifications state changed
                bool hasActiveNotificationsNow = hasNotifications();
                if (hadActiveNotifications != hasActiveNotificationsNow) {
                    emit hasNotificationsChanged();

                    // Update visibility based on active notifications
                    if (m_isVisible != hasActiveNotificationsNow) {
                        m_isVisible = hasActiveNotificationsNow;
                        emit isVisibleChanged();
                    }
                }

                // Always emit these signals when active state changes
                emit activeCountChanged();
                updateHighestPriorityNotification();
                emit highestPriorityChanged();
            }
            break;
        }
    }
}

void NotificationManager::toggleNotificationActive(const quint64 id)
{
    for (int i = 0; i < m_notifications.size(); ++i) {
        if (m_notifications[i]->id() == id) {
            setNotificationActive(id, !m_notifications[i]->isActive());
            break;
        }
    }
}
