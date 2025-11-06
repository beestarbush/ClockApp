#include "Service.h"
using namespace Services::Notification;

Service::Service(QObject* parent)
    : QObject(parent),
      m_model(this),
      m_isVisible(false)
{
    // Connect model signals to update visibility
    connect(&m_model, &Model::hasNotificationsChanged, this, [this]() {
        bool shouldBeVisible = m_model.hasNotifications();
        if (m_isVisible != shouldBeVisible) {
            m_isVisible = shouldBeVisible;
            emit isVisibleChanged();
        }
    });
}

Model* Service::model()
{
    return &m_model;
}

bool Service::isVisible() const
{
    return m_isVisible && m_model.hasNotifications();
}

void Service::showInfo(const QString& title, const QString& message, bool active, quint64 duration)
{
    m_model.addNotification(title, message, Item::Type::Info, active, duration);
}

void Service::showWarning(const QString& title, const QString& message, quint64 duration)
{
    m_model.addNotification(title, message, Item::Type::Warning, true, duration);
}

void Service::showError(const QString& title, const QString& message, quint64 duration)
{
    m_model.addNotification(title, message, Item::Type::Error, true, duration);
}
