#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "FunkyClass.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	FunkyClass funkyClass(&app);
	qmlRegisterSingletonInstance("Bee", 1, 0, "FunkyClass", &funkyClass);

	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

	engine.loadFromModule("HelloWorldApp", "Main");

	return app.exec();
}
