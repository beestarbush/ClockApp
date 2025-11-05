#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "applications/Applications.h"
#include "applications/menu/Application.h"
#include "applications/setup/Application.h"
#include "hal/HAL.h"
#include "qmlcomponents/QmlUtils.h"
#include "qmlcomponents/RoundAnimatedImage.h"
#include "services/Services.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("bee");
    app.setApplicationName("clockapp");

    QQmlApplicationEngine engine;
    QElapsedTimer lTimer;
    lTimer.start();

    HAL hal(&app);
    qDebug() << "Initialized HAL in" << lTimer.restart() << "ms";
    Services services(hal, &app);
    qDebug() << "Initialized Services in" << lTimer.restart() << "ms";
    Applications applications(services, &app);
    qDebug() << "Initialized Applications in" << lTimer.elapsed() << "ms";

    auto qmlInterface = services.qmlInterface();
    qmlInterface->registerObject("QmlInterface", qmlInterface);
    qmlInterface->registerObject("HAL", &hal);
    qmlInterface->registerObject("Services", &services);
    qmlInterface->registerObject("Applications", &applications);
    qmlInterface->registerType<RoundAnimatedImage>("RoundAnimatedImage");
    qmlInterface->registerType<QmlUtils>("QmlUtils");
    qmlInterface->registerUncreatableType<Menu::Application>("MenuEnums");
    qmlInterface->registerUncreatableType<Setup::Application>("SetupEnums");

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
