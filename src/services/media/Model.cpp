#include "Model.h"
#include <QDir>
#include <QFileInfo>

using namespace Services::Media;

Model::Model(QObject* parent)
    : QAbstractListModel(parent)
{
}

int Model::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

QVariant Model::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    const Item* item = m_items.at(index.row());

    switch (role) {
    case FilenameRole:
        return item->filename();
    case PathRole:
        return item->path();
    case IsValidRole:
        return item->isValid();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> Model::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FilenameRole] = "filename";
    roles[PathRole] = "path";
    roles[IsValidRole] = "isValid";
    return roles;
}

Item* Model::get(int index) const
{
    return (index >= 0 && index < m_items.count()) ? m_items[index] : nullptr;
}

QString Model::getPath(const QString& filename) const
{
    for (const auto* item : m_items) {
        if (item->filename() == filename) {
            return item->path();
        }
    }
    return QString();
}

bool Model::contains(const QString& filename) const
{
    for (const auto* item : m_items) {
        if (item->filename() == filename) {
            return true;
        }
    }
    return false;
}

int Model::indexOf(const QString& filename) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i]->filename() == filename) {
            return i;
        }
    }
    return -1;
}

void Model::setMedia(const QStringList& filenames, const QString& basePath)
{
    // Clear existing items
    if (!m_items.isEmpty()) {
        beginResetModel();
        qDeleteAll(m_items);
        m_items.clear();
        endResetModel();
    }

    if (filenames.isEmpty()) {
        emit countChanged();
        return;
    }

    // Add new items
    beginInsertRows(QModelIndex(), 0, filenames.size() - 1);
    for (const QString& filename : filenames) {
        QString fullPath = QDir(basePath).absoluteFilePath(filename);
        Item* item = new Item(filename, fullPath, this);
        m_items.append(item);
    }
    endInsertRows();

    emit countChanged();
}

void Model::clear()
{
    if (m_items.isEmpty())
        return;

    beginResetModel();
    qDeleteAll(m_items);
    m_items.clear();
    endResetModel();

    emit countChanged();
}
