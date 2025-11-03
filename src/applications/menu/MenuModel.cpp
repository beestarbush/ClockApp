#include "MenuModel.h"
#include "MenuItem.h"

MenuModel::MenuModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int MenuModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : m_items.count();
}

QVariant MenuModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.count())
        return QVariant();

    auto* item = m_items[index.row()];
    switch (role) {
    case LabelRole:
        return item->label();
    case ItemRole:
        return QVariant::fromValue(item);
    }
    return QVariant();
}

QHash<int, QByteArray> MenuModel::roleNames() const
{
    return {{LabelRole, "label"}, {ItemRole, "item"}};
}

MenuItem* MenuModel::get(int index) const
{
    return (index >= 0 && index < m_items.count()) ? m_items[index] : nullptr;
}

void MenuModel::add(MenuItem& item)
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items.append(&item);
    endInsertRows();
    emit countChanged();
}
