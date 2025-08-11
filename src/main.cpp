#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DateTime.h"
#include "QmlInterface.h"
#include "Screen.h"
#include "TimeSince.h"
#include "Version.h"
#include "RoundAnimatedImage.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	QmlInterface qmlInterface(&app);
	DateTime dateTime(&app);
	Screen screen(&app);
	TimeSince timeSinceMarried(1730382722, false, &app);
	TimeSince timeSinceKuiken(1738195200, true, &app);
	Version version(&app);

	qmlInterface.registerObject("DateTime", &dateTime);
	qmlInterface.registerObject("Screen", &screen);
	qmlInterface.registerObject("Version", &version);
	qmlInterface.registerObject("TimeSinceMarried", &timeSinceMarried);
	qmlInterface.registerObject("TimeSinceKuiken", &timeSinceKuiken);
	qmlInterface.registerObject("QmlInterface", &qmlInterface);
	qmlInterface.registerType<RoundAnimatedImage>("RoundAnimatedImage");

	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

	engine.loadFromModule("ClockApp", "Main");

	return app.exec();
}
