#include "Debug.h"

Debug::Debug(QObject* parent)
    : QObject(parent),
      m_panelEnabled(false)
{
}

bool Debug::panelEnabled() const
{
    return m_panelEnabled;
}

void Debug::setPanelEnabled(const bool& panelEnabled)
{
    if (m_panelEnabled != panelEnabled) {
        m_panelEnabled = panelEnabled;
        emit panelEnabledChanged();
    }
}
