#include "Item.h"
#include <QDateTime>
using namespace Notification;

// Initialize static member
quint64 Item::s_nextId = 1;

Item::Item(QObject* parent)
    : QObject(parent),
      m_autoRemoveTimer(nullptr)
{
}

Item::Item(const QString& title, const QString& message, Type type, quint64 duration, bool active, QObject* parent)
    : QObject(parent),
      m_autoRemoveTimer(nullptr)
{
    m_id = s_nextId++;
    m_title = title;
    m_message = message;
    m_type = type;
    m_duration = duration;
    m_timestamp = QDateTime::currentMSecsSinceEpoch();
    m_isActive = active;
}

Item::~Item()
{
    if (m_autoRemoveTimer) {
        m_autoRemoveTimer->deleteLater();
        m_autoRemoveTimer = nullptr;
    }
}

void Item::setIsActive(bool active)
{
    if (m_isActive != active) {
        m_isActive = active;
        emit isActiveChanged();
    }
}

Item& Item::operator=(const Item& other)
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

void Item::reset()
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
