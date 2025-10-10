#include "Notification.h"
#include <QDateTime>

// Initialize static member
quint64 Notification::s_nextId = 1;

Notification::Notification(QObject* parent)
    : QObject(parent), m_autoRemoveTimer(nullptr)
{
}

Notification::Notification(const QString& title, const QString& message, Notification::Type type, quint64 duration, QObject* parent)
    : QObject(parent), m_autoRemoveTimer(nullptr)
{
    m_id = s_nextId++;
    m_title = title;
    m_message = message;
    m_type = type;
    m_duration = duration;
    m_timestamp = QDateTime::currentMSecsSinceEpoch();
    m_isActive = true;
}

Notification::~Notification()
{
    if (m_autoRemoveTimer) {
        m_autoRemoveTimer->deleteLater();
        m_autoRemoveTimer = nullptr;
    }
}

void Notification::setIsActive(bool active)
{
    if (m_isActive != active) {
        m_isActive = active;
        emit isActiveChanged();
    }
}

Notification& Notification::operator=(const Notification& other)
{
    if (this == &other) {
        return *this; // Self-assignment check
    }
    
    bool activeChanged = (m_isActive != other.m_isActive);
    
    m_id = other.m_id;
    m_title = other.m_title;
    m_message = other.m_message;
    m_type = other.m_type;
    m_duration = other.m_duration;
    m_timestamp = other.m_timestamp;
    m_isActive = other.m_isActive;
    
    if (activeChanged) {
        emit isActiveChanged();
    }
    
    return *this;
}

void Notification::reset()
{
    bool activeChanged = m_isActive;
    
    m_id = 0;
    m_title = "";
    m_message = "";
    m_type = Type::Info;
    m_duration = 0;
    m_timestamp = 0;
    m_isActive = false;
    
    if (activeChanged) {
        emit isActiveChanged();
    }
}
