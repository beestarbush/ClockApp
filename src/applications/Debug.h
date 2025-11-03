#ifndef APPS_DEBUG_H
#define APPS_DEBUG_H

#include <QObject>

class Debug : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool panelEnabled READ panelEnabled WRITE setPanelEnabled NOTIFY panelEnabledChanged)

  public:
    explicit Debug(QObject* parent = nullptr);

    bool panelEnabled() const;
    void setPanelEnabled(const bool& panelEnabled);

  signals:
    void panelEnabledChanged();

  private:
    bool m_panelEnabled;
};

#endif // APPS_DEBUG_H
