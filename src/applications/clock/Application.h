#ifndef APPS_CLOCK_H
#define APPS_CLOCK_H

#include "Configuration.h"
#include <QObject>

class MediaManager;

namespace Clock
{
class Configuration;

class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Configuration* configuration MEMBER m_configuration CONSTANT)

  public:
    Application(QString name, MediaManager& mediaManager, QObject* parent = nullptr);

    Configuration* configuration() const;

    void applyConfiguration(const Configuration& configuration);

  private:
    Configuration* m_configuration;
    MediaManager& m_mediaManager;
};
} // namespace Clock

#endif // APPS_CLOCK_H