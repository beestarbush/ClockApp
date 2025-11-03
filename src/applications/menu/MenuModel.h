#ifndef APPS_MENU_MENUMODEL_H
#define APPS_MENU_MENUMODEL_H

#include <QAbstractListModel>
#include <QList>

class MenuItem;

class MenuModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

  public:
    enum Roles
    {
        LabelRole = Qt::UserRole + 1,
        ItemRole
    };

    explicit MenuModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE MenuItem* get(int index) const;
    void add(MenuItem& item);

  signals:
    void countChanged();

  private:
    QList<MenuItem*> m_items;
};

#endif // APPS_MENU_MENUMODEL_H
