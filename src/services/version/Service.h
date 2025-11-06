#ifndef SERVICES_VERSION_SERVICE_H
#define SERVICES_VERSION_SERVICE_H

#include <QObject>

namespace Services::Version
{
class Service : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ tag CONSTANT)
    Q_PROPERTY(QString commitHash READ commitHash CONSTANT)
    Q_PROPERTY(QString shortCommitHash READ shortCommitHash CONSTANT)
    Q_PROPERTY(QString dirty READ dirty CONSTANT)

  public:
    Service(QObject* parent = nullptr);

    QString tag() const;
    QString commitHash() const;
    QString shortCommitHash() const;
    QString dirty() const;
};
} // namespace Services::Version

#endif // SERVICES_VERSION_SERVICE_H
