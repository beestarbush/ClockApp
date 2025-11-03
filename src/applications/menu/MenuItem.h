#ifndef APPS_MENU_MENUITEM_H
#define APPS_MENU_MENUITEM_H

#include <QObject>
#include <QString>
#include <functional>

class MenuItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label CONSTANT)

  public:
    MenuItem(QString label, QObject* parent = nullptr);

    QString label() const
    {
        return m_label;
    }

    void setAction(std::function<void()> action)
    {
        m_action = std::move(action);
    }
    Q_INVOKABLE void trigger()
    {
        if (m_action)
            m_action();
    }

  private:
    QString m_label;
    std::function<void()> m_action;
};

#endif // APPS_MENU_MENUITEM_H
