#include "Application.h"
#include "services/media/Service.h"
#include <QDebug>
#include <QSettings>
using namespace Applications::Clock;

Application::Application(QString name, Services::Media::Service& media, QObject* parent)
    : QObject(parent),
      m_configuration(new Configuration(name, parent)),
      m_media(media)
{
    m_configuration->load();
    qInfo() << "Clock configuration: \n"
            << *m_configuration;

    // Refresh background when media sync completes
    connect(&m_media, &Services::Media::Service::syncCompleted, this, [this]() {
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
