#ifndef SERVICES_DATETIME_SERVICE_H
#define SERVICES_DATETIME_SERVICE_H

#include <QObject>
#include <QTimeZone>

namespace DateTime
{
class Service : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString localTime READ localTime NOTIFY timeChanged)
    Q_PROPERTY(QString utcTime READ utcTime NOTIFY timeChanged)

  public:
    Service(QObject* parent = nullptr);

    QString localTime() const;
    QString utcTime() const;

  signals:
    void timeChanged();

  private:
    QTimeZone m_timeZone;
};
} // namespace DateTime

#endif // SERVICES_DATETIME_SERVICE_H
