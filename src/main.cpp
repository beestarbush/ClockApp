#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "applications/Container.h"
#include "applications/menu/Application.h"
#include "applications/setup/Application.h"
#include "drivers/Container.h"
#include "qmlcomponents/QmlUtils.h"
#include "qmlcomponents/RoundAnimatedImage.h"
#include "services/Container.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("bee");
    app.setApplicationName("clockapp");

    QQmlApplicationEngine engine;
    QElapsedTimer lTimer;
    lTimer.start();

    Drivers::Container drivers(&app);
    qDebug() << "Initialized Drivers in" << lTimer.restart() << "ms";
    Services::Container services(drivers, &app);
    qDebug() << "Initialized Services in" << lTimer.restart() << "ms";
    Applications::Container applications(services, &app);
    qDebug() << "Initialized Applications in" << lTimer.elapsed() << "ms";

    auto qmlInterface = services.qmlInterface();
    qmlInterface->registerObject("QmlInterface", qmlInterface);
    qmlInterface->registerObject("Drivers", &drivers);
    qmlInterface->registerObject("Services", &services);
    qmlInterface->registerObject("Applications", &applications);
    qmlInterface->registerType<RoundAnimatedImage>("RoundAnimatedImage");
    qmlInterface->registerType<QmlUtils>("QmlUtils");
    qmlInterface->registerUncreatableType<Applications::Menu::Application>("MenuEnums");
    qmlInterface->registerUncreatableType<Applications::Setup::Application>("SetupEnums");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.loadFromModule("Main", "Main");

    return app.exec();
}
