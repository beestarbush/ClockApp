#include "Model.h"
#include "Item.h"
#include <QDateTime>
#include <QTimer>

using namespace Services::Notification;

Model::Model(QObject* parent)
    : QAbstractListModel(parent),
      m_highestPriorityItem(nullptr)
{
    m_highestPriorityItem = new Item(this);
}

int Model::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_notifications.size();
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_notifications.size())
        return QVariant();

    const Item* notification = m_notifications.at(index.row());

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

QHash<int, QByteArray> Model::roleNames() const
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

int Model::getActiveCount() const
{
    int activeCount = 0;
    for (const auto* notification : m_notifications) {
        if (notification->isActive()) {
            activeCount++;
        }
    }
    return activeCount;
}

bool Model::hasNotifications() const
{
    // Check if there are any active notifications
    for (const auto* notification : m_notifications) {
        if (notification->isActive()) {
            return true;
        }
    }
    return false;
}

Item* Model::getHighestPriorityNotification()
{
    return m_highestPriorityItem;
}

Item* Model::get(int index) const
{
    return (index >= 0 && index < m_notifications.count()) ? m_notifications[index] : nullptr;
}

void Model::updateHighestPriorityNotification()
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

void Model::addNotification(const QString& title, const QString& message, Item::Type type, bool active, quint64 duration)
{
    Item* notification = new Item(title, message, type, duration, active, this);

    insertNotificationSorted(notification);

    // Set up auto-removal timer if duration > 0
    if (duration > 0) {
        setupAutoRemove(notification);
    }

    updateHighestPriorityNotification();
    emit highestPriorityChanged();
    emit notificationAdded(notification->id());
}

void Model::removeNotification(const quint64 id)
{
    for (int i = 0; i < m_notifications.size(); ++i) {
        if (m_notifications[i]->id() == id) {
            removeNotificationAt(i);
            break;
        }
    }
}

void Model::removeNotificationAt(int index)
{
    if (index < 0 || index >= m_notifications.size())
        return;

    Item* notification = m_notifications[index];
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
}

void Model::clearInactive()
{
    // Remove all inactive notifications
    for (int i = m_notifications.size() - 1; i >= 0; --i) {
        if (!m_notifications[i]->isActive()) {
            removeNotificationAt(i);
        }
    }
}

void Model::clearAll()
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

    emit countChanged();
    emit activeCountChanged();
    emit hasNotificationsChanged();
    updateHighestPriorityNotification();
    emit highestPriorityChanged();
}

void Model::insertNotificationSorted(Item* notification)
{
    int insertIndex = 0;

    // Find insertion point based on type (higher priority first)
    // For same type: active notifications come before inactive ones
    // For same type and status: newer notifications come first
    for (int i = 0; i < m_notifications.size(); ++i) {
        if (notification->type() > m_notifications[i]->type()) {
            insertIndex = i;
            break;
        }
        else if (notification->type() == m_notifications[i]->type()) {
            // Same type - check active status
            if (notification->isActive() && !m_notifications[i]->isActive()) {
                // New notification is active, existing is inactive - insert here
                insertIndex = i;
                break;
            }
            else if (notification->isActive() == m_notifications[i]->isActive()) {
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

void Model::setupAutoRemove(Item* notification)
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

void Model::setNotificationActive(const quint64 id, bool active)
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

void Model::toggleNotificationActive(const quint64 id)
{
    for (int i = 0; i < m_notifications.size(); ++i) {
        if (m_notifications[i]->id() == id) {
            setNotificationActive(id, !m_notifications[i]->isActive());
            break;
        }
    }
}
