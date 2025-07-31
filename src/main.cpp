#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Version.h"
#include "RoundAnimatedImage.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	Version version(&app);
	qmlRegisterSingletonInstance("Bee", 1, 0, "Version", &version);
	qmlRegisterType<RoundAnimatedImage>("Bee", 1, 0, "RoundAnimatedImage");

	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

	engine.loadFromModule("ClockApp", "Main");

	return app.exec();
}
