#ifndef APPS_CLOCK_H
#define APPS_CLOCK_H

#include "Configuration.h"
#include <QObject>

namespace Services::Media
{
class Service;
}

namespace Applications::Clock
{
class Configuration;

class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Configuration* configuration MEMBER m_configuration CONSTANT)

  public:
    Application(QString name, Services::Media::Service& media, QObject* parent = nullptr);

    Configuration* configuration() const;

    void applyConfiguration(const Configuration& configuration);

  private:
    Configuration* m_configuration;
    Services::Media::Service& m_media;
};
} // namespace Applications::Clock

#endif // APPS_CLOCK_H