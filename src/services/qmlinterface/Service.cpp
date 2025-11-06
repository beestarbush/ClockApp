#include "Service.h"
#include <QDebug>
#include <QQmlApplicationEngine>
using namespace Services::QmlInterface;

Service::Service(QObject* parent)
    : QObject(parent)
{
}

void Service::registerObject(const char* name, QObject* object)
{
    if (!object) {
        qWarning() << "Cannot register null object with name:" << name;
        return;
    }

    QElapsedTimer lTimer;
    lTimer.start();
    object->setObjectName(name);
    if (!m_registeredObjectsNames.contains(name)) {
        qmlRegisterSingletonInstance("Bee", 1, 0, name, object);
        m_registeredObjectsNames.append(name);
        qDebug() << "Registered QML interface:" << name << "in" << lTimer.elapsed() << "ms";
    }
}
