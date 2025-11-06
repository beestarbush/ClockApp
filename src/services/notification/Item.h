#ifndef SERVICES_NOTIFICATION_ITEM_H
#define SERVICES_NOTIFICATION_ITEM_H

#include <QObject>
#include <QTimer>

namespace Services::Notification
{
class Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 id READ id CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString message READ message CONSTANT)
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(quint64 duration READ duration CONSTANT)
    Q_PROPERTY(quint64 timestamp READ timestamp CONSTANT)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)

  public:
    enum Type
    {
        Info = 1,    // Lowest priority - 10 seconds
        Warning = 2, // Medium priority - 20 seconds
        Error = 3    // Highest priority - persistent
    };
    Q_ENUM(Type)

    Item(QObject* parent = nullptr);

    // Constructor for creating new notifications
    Item(const QString& title, const QString& message, Type type, quint64 duration, bool active = true, QObject* parent = nullptr);

    ~Item();

    quint64 id() const
    {
        return m_id;
    }
    QString title() const
    {
        return m_title;
    }
    QString message() const
    {
        return m_message;
    }
    Type type() const
    {
        return m_type;
    }
    quint64 duration() const
    {
        return m_duration;
    }
    quint64 timestamp() const
    {
        return m_timestamp;
    }
    bool isActive() const
    {
        return m_isActive;
    }

    // Timer access
    QTimer* autoRemoveTimer() const
    {
        return m_autoRemoveTimer;
    }
    void setAutoRemoveTimer(QTimer* timer)
    {
        m_autoRemoveTimer = timer;
    }

    void setId(const quint64 id)
    {
        m_id = id;
    }
    void setTitle(const QString& title)
    {
        m_title = title;
    }
    void setMessage(const QString& message)
    {
        m_message = message;
    }
    void setType(Type type)
    {
        m_type = type;
    }
    void setDuration(quint64 duration)
    {
        m_duration = duration;
    }
    void setTimestamp(quint64 timestamp)
    {
        m_timestamp = timestamp;
    }
    void setIsActive(bool active);

    // Assignment operator
    Item& operator=(const Item& other);

    // Reset to default/empty state
    void reset();

  signals:
    void isActiveChanged();

  private:
    quint64 m_id;
    QString m_title;
    QString m_message;
    Type m_type = Type::Info;
    quint64 m_duration = 10000;
    quint64 m_timestamp = 0;
    bool m_isActive = true;
    QTimer* m_autoRemoveTimer; // Timer for automatic removal

    static quint64 s_nextId; // Static member for generating unique IDs
};
} // namespace Services::Notification

#endif // SERVICES_NOTIFICATION_ITEM_H
