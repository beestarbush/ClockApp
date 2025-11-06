#ifndef SERVICES_MEDIA_MODEL_H
#define SERVICES_MEDIA_MODEL_H

#include "Item.h"
#include <QAbstractListModel>
#include <QList>

namespace Services::Media
{
class Model : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

  public:
    enum Roles
    {
        FilenameRole = Qt::UserRole + 1,
        PathRole,
        IsValidRole
    };

    explicit Model(QObject* parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Model access
    Q_INVOKABLE Item* get(int index) const;
    Q_INVOKABLE QString getPath(const QString& filename) const;
    Q_INVOKABLE bool contains(const QString& filename) const;
    Q_INVOKABLE int indexOf(const QString& filename) const;
    Q_INVOKABLE int length() const
    {
        return rowCount();
    }

    // Model manipulation
    void setMedia(const QStringList& filenames, const QString& basePath);
    void clear();

  signals:
    void countChanged();

  private:
    QList<Item*> m_items;
};
} // namespace Services::Media

#endif // SERVICES_MEDIA_MODEL_H
