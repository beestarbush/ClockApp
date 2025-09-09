#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "RoundAnimatedImage.h"
#include "ObjectPropertyModel.h"
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

	QObject rootObj(&app);
	rootObj.setObjectName("RootObject");

	HAL hal(&rootObj);
	Services services(&rootObj);
	Applications applications(services, &rootObj);

	qmlInterface.registerObject("QmlInterface", &qmlInterface);
	qmlInterface.registerObject("HAL", &hal);
	qmlInterface.registerObject("Services", &services);
	qmlInterface.registerObject("Applications", &applications);
	qmlInterface.registerType<RoundAnimatedImage>("RoundAnimatedImage");
	qmlInterface.registerType<ObjectPropertyModel>("ObjectPropertyModel");

	ObjectPropertyModel propertyModel(&rootObj, &app);
	engine.rootContext()->setContextProperty("objectModel", &propertyModel);

	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

	engine.loadFromModule("ClockApp", "Main");

	return app.exec();
}
