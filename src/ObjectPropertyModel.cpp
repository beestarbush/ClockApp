#include "ObjectPropertyModel.h"
#include <QDebug>

ObjectPropertyModel::ObjectPropertyModel(QObject *rootObject, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootNode = new Node{Node::Type::Object, rootObject, QMetaProperty(), nullptr};
    buildTree(m_rootNode, rootObject);
}

void ObjectPropertyModel::buildTree(Node *parentNode, QObject *object)
{
    if (!object) return;

    const QMetaObject *meta = object->metaObject();

    // Map property object pointers to property names
    QHash<QObject*, QString> propertyObjectNames;
    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);
        if (!prop.isReadable())
            continue;
        QVariant value = object->property(prop.name());
        if (value.canConvert<QObject*>()) {
            QObject *propObj = value.value<QObject*>();
            if (propObj)
                propertyObjectNames[propObj] = QString::fromLatin1(prop.name());
        }
    }

    // Add property children (skip if matches a child object)
    QSet<QObject*> childObjects;
    for (QObject *child : object->children()) {
        childObjects.insert(child);
    }
    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);
        if (!prop.isReadable())
            continue;
        QVariant value = object->property(prop.name());
        if (value.canConvert<QObject*>()) {
            QObject *propObj = value.value<QObject*>();
            if (propObj && childObjects.contains(propObj)) {
                continue;
            }
        }
        Node *propNode = new Node{Node::Type::Property, object, prop, parentNode};
        parentNode->children.append(propNode);
    }

    // Add object children recursively, set propertyName if matched
    for (QObject *child : object->children()) {
        Node *childNode = new Node{Node::Type::Object, child, QMetaProperty(), parentNode};
        if (propertyObjectNames.contains(child))
            childNode->propertyName = propertyObjectNames.value(child);
        parentNode->children.append(childNode);
        buildTree(childNode, child);
    }
}

QModelIndex ObjectPropertyModel::index(int row, int col, const QModelIndex &parent) const {
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : m_rootNode;
    if (row < 0 || row >= parentNode->children.size()) return {};
    return createIndex(row, col, parentNode->children[row]);
}

QModelIndex ObjectPropertyModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) return {};
    Node *node = static_cast<Node*>(child.internalPointer());
    if (!node || !node->parent || node->parent == m_rootNode) return {};
    Node *parent = node->parent;
    Node *grand = parent->parent;
    int row = grand ? grand->children.indexOf(parent) : 0;
    return createIndex(row, 0, parent);
}

int ObjectPropertyModel::rowCount(const QModelIndex &parent) const {
    Node *parentNode = parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : m_rootNode;
    return parentNode->children.size();
}

QVariant ObjectPropertyModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};
    Node *node = static_cast<Node*>(index.internalPointer());

    if (node->type == Node::Type::Object) {
        if (role == NameRole)
            return !node->propertyName.isEmpty() ? node->propertyName
                                                 : (node->object ? node->object->objectName() : "<null>");
        if (role == TypeRole)
            return node->object ? node->object->metaObject()->className() : "";
        if (role == ValueRole)
            return "<object>";
        if (role == IsPropertyRole)
            return false;
        if (role == EditableRole)
            return false;
    } else if (node->type == Node::Type::Property) {
        if (role == NameRole)
            return QString::fromLatin1(node->property.name());
        if (role == TypeRole)
            return QString::fromLatin1(node->property.typeName());
        if (role == ValueRole && node->object)
            return node->object->property(node->property.name());
        if (role == IsPropertyRole)
            return true;
        if (role == EditableRole)
            return node->property.isWritable();
    }
    return {};
}

bool ObjectPropertyModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;
    Node *node = static_cast<Node*>(index.internalPointer());

    if (node->type == Node::Type::Property && role == ValueRole) {
        if (node->object && node->property.isWritable()) {
            bool ok = node->object->setProperty(node->property.name(), value);
            if (ok) {
                emit dataChanged(index, index, {ValueRole});
                return true;
            }
        }
    }
    return false;
}

QHash<int, QByteArray> ObjectPropertyModel::roleNames() const {
    return {
        {NameRole, "display"},
        {TypeRole, "type"},
        {ValueRole, "value"},
        {IsPropertyRole, "isProperty"},
        {EditableRole, "editable"}
    };
}

Qt::ItemFlags ObjectPropertyModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
