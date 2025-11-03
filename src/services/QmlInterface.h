#ifndef SERVICES_QMLINTERFACE_H
#define SERVICES_QMLINTERFACE_H

#include <QElapsedTimer>
#include <QObject>
#include <QQmlApplicationEngine>

class QmlInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList registeredObjectsNames MEMBER m_registeredObjectsNames CONSTANT)

  public:
    QmlInterface(QObject* parent = nullptr);

    void registerObject(const char* name, QObject* object);
    template <typename T>
    void registerType(const char* name)
    {
        QElapsedTimer lTimer;
        lTimer.start();
        qmlRegisterType<T>("Bee", 1, 0, name);
        qDebug() << "Registered QML type:" << name << "in" << lTimer.elapsed() << "ms";
    }
    template <typename T>
    void registerUncreatableType(const char* name)
    {
        QElapsedTimer lTimer;
        lTimer.start();
        qmlRegisterUncreatableMetaObject(
            T::staticMetaObject,
            "Bee",
            1,
            0,
            name,
            QString("Cannot create %1").arg(name));
        qDebug() << "Registered QML type:" << name << "in" << lTimer.elapsed() << "ms";
    }

  private:
    QStringList m_registeredObjectsNames;
};

#endif // SERVICES_QMLINTERFACE_H