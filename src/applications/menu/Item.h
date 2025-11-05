#ifndef APPS_MENU_ITEM_H
#define APPS_MENU_ITEM_H

#include <QObject>
#include <QString>
#include <functional>

namespace Menu
{
class Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label CONSTANT)

  public:
    Item(QString label, QObject* parent = nullptr);

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
} // namespace Menu

#endif // APPS_MENU_ITEM_H
