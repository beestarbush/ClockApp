#include "Application.h"
#include "services/MediaManager.h"
#include <QDebug>
#include <QSettings>
using namespace Clock;

Application::Application(QString name, MediaManager& mediaManager, QObject* parent)
    : QObject(parent),
      m_configuration(new Configuration(name, parent)),
      m_mediaManager(mediaManager)
{
    m_configuration->load();
    qInfo() << "Clock configuration: \n"
            << *m_configuration;

    // Refresh background when media sync completes
    connect(&m_mediaManager, &MediaManager::syncCompleted, this, [this]() {
        emit m_configuration->backgroundChanged();
    });
}

Configuration* Application::configuration() const
{
    return m_configuration;
}

void Application::applyConfiguration(const Configuration& configuration)
{
    qDebug() << "Applying configuration to Clock";

    *m_configuration = configuration;

    qDebug() << "Clock configuration applied successfully";
}
