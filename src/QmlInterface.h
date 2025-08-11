#include <QObject>
#include <QQmlApplicationEngine>

class QmlInterface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QStringList registeredObjectsNames MEMBER m_registeredObjectsNames CONSTANT)

public:
	QmlInterface(QObject *parent = nullptr);

    void registerObject(const char *name, QObject *object);
	template<typename T>
	void registerType(const char *name) {
		qmlRegisterType<T>("Bee", 1, 0, name);
		qDebug() << "Registered QML type:" << name;
	}

private:
    QStringList m_registeredObjectsNames;
};