#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "RoundAnimatedImage.h"
#include "hal/HAL.h"
#include "services/Services.h"
#include "applications/Applications.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	app.setOrganizationName("bee");
    app.setApplicationName("clockapp");

	QQmlApplicationEngine engine;
	QmlInterface qmlInterface(&app);

	HAL hal(&app);
	Services services(&app);
	Applications applications(services, &app);

	qmlInterface.registerObject("QmlInterface", &qmlInterface);
	qmlInterface.registerObject("HAL", &hal);
	qmlInterface.registerObject("Services", &services);
	qmlInterface.registerObject("Applications", &applications);
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
