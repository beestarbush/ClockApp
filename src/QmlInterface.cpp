#include "QmlInterface.h"
#include <QQmlApplicationEngine>
#include <QDebug>

QmlInterface::QmlInterface(QObject *parent) :
    QObject(parent)
{
}

void QmlInterface::registerObject(const char *name, QObject *object)
{
    if (!object) {
        qWarning() << "Cannot register null object with name:" << name;
        return;
    }

    object->setObjectName(name);
    if (!m_registeredObjectsNames.contains(name)) {
        qmlRegisterSingletonInstance("Bee", 1, 0, name, object);
        m_registeredObjectsNames.append(name);
        qDebug() << "Registered QML interface:" << name;
    }
}