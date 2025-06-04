#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "FunkyClass.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	FunkyClass funkyClass(&app);
#ifdef PLATFORM_IS_TARGET
	funkyClass.setRotationDegrees(270);
#endif
	qmlRegisterSingletonInstance("Bee", 1, 0, "FunkyClass", &funkyClass);

	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

	engine.loadFromModule("ClockApp", "Main");

	return app.exec();
}
