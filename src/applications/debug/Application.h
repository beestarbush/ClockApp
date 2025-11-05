#ifndef APPS_DEBUG_APPLICATION_H
#define APPS_DEBUG_APPLICATION_H

#include <QObject>

namespace Debug
{
class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool panelEnabled READ panelEnabled WRITE setPanelEnabled NOTIFY panelEnabledChanged)

  public:
    explicit Application(QObject* parent = nullptr);

    bool panelEnabled() const;
    void setPanelEnabled(const bool& panelEnabled);

  signals:
    void panelEnabledChanged();

  private:
    bool m_panelEnabled;
};
} // namespace Debug

#endif // APPS_DEBUG_APPLICATION_H
