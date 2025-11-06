#ifndef SERVICES_NOTIFICATION_SERVICE_H
#define SERVICES_NOTIFICATION_SERVICE_H

#include "Item.h"
#include "Model.h"
#include <QObject>

namespace Services::Notification
{
class Service : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Notification::Model* model READ model CONSTANT)
    Q_PROPERTY(bool isVisible READ isVisible NOTIFY isVisibleChanged)

  public:
    Service(QObject* parent = nullptr);

    Model* model();
    bool isVisible() const;

  public slots:
    void showInfo(const QString& title, const QString& message, bool active = true, quint64 duration = 10000); // 10 seconds
    void showWarning(const QString& title, const QString& message, quint64 duration = 20000);                  // 20 seconds
    void showError(const QString& title, const QString& message, quint64 duration = 0);                        // persistent

  signals:
    void isVisibleChanged();

  private:
    Model m_model;
    bool m_isVisible;
};
} // namespace Services::Notification

#endif // SERVICES_NOTIFICATION_SERVICE_H
