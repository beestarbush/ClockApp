#pragma once

#include <QAbstractItemModel>
#include <QPointer>
#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QVector>

class ObjectPropertyModel : public QAbstractItemModel {
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        ValueRole,
        IsPropertyRole,
        EditableRole
    };
    Q_ENUM(Roles)

    explicit ObjectPropertyModel(QObject *rootObject, QObject *parent = nullptr);

    QModelIndex index(int row, int col, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override { return 1; }
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    struct Node {
        enum class Type { Object, Property };
        Type type;
        QPointer<QObject> object;
        QMetaProperty property;
        Node *parent = nullptr;
        QVector<Node*> children;
        QString propertyName;
    };

    Node *m_rootNode;
    void buildTree(Node *parentNode, QObject *object);
};
