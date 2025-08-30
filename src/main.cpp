#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DateTime.h"
#include "Network.h"
#include "QmlInterface.h"
#include "Screen.h"
#include "System.h"
#include "Temperature.h"
#include "TimeSince.h"
#include "Version.h"
#include "RoundAnimatedImage.h"
#include "AnimationManager.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	QmlInterface qmlInterface(&app);
	DateTime dateTime(&app);
	Network network(&app);
	Screen screen(&app);
	System system(&app);
	Temperature temperature(&app);
	TimeSince timeSinceMarried(1730382722, false, &app);
	TimeSince timeSinceKuiken(1738195200, true, &app);
	Version version(&app);
	AnimationManager animationManager(&app);

	qmlInterface.registerObject("DateTime", &dateTime);
	qmlInterface.registerObject("Network", &network);
	qmlInterface.registerObject("Screen", &screen);
	qmlInterface.registerObject("System", &system);
	qmlInterface.registerObject("Temperature", &temperature);
	qmlInterface.registerObject("Version", &version);
	qmlInterface.registerObject("TimeSinceMarried", &timeSinceMarried);
	qmlInterface.registerObject("TimeSinceKuiken", &timeSinceKuiken);
	qmlInterface.registerObject("QmlInterface", &qmlInterface);
	qmlInterface.registerObject("AnimationManager", &animationManager);
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
