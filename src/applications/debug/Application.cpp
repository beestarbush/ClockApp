#include "Application.h"
using namespace Debug;

Application::Application(QObject* parent)
    : QObject(parent),
      m_panelEnabled(false)
{
}

bool Application::panelEnabled() const
{
    return m_panelEnabled;
}

void Application::setPanelEnabled(const bool& panelEnabled)
{
    if (m_panelEnabled != panelEnabled) {
        m_panelEnabled = panelEnabled;
        emit panelEnabledChanged();
    }
}
