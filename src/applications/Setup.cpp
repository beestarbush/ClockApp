#include "Setup.h"
#include "applications/BirthdayTimer.h"
#include "applications/CountdownTimer.h"
#include "applications/MarriedTimer.h"
#include "applications/TimeElapsedTimer.h"
#include "services/RemoteApi.h"
#include "services/remoteapi/DeviceStatus.h"
#include <QDebug>
#include <QSettings>

const QString PROPERTIES_GROUP_NAME = QStringLiteral("setup");
const QString PROPERTY_SETUP_COMPLETE_KEY = QStringLiteral("setup-complete");
const bool PROPERTY_SETUP_COMPLETE_DEFAULT = false;

Setup::Setup(MarriedTimer* marriedTimer,
             BirthdayTimer* kuikenTimer,
             CountdownTimer* christmasTimer,
             RemoteApi& remoteApi,
             QObject* parent)
    : QObject(parent),
      m_setupComplete(PROPERTY_SETUP_COMPLETE_DEFAULT),
      m_setupStep(0),
      m_marriedTimer(marriedTimer),
      m_kuikenTimer(kuikenTimer),
      m_christmasTimer(christmasTimer),
      m_remoteApi(remoteApi)
{
    loadProperties();
}

bool Setup::isSetupComplete() const
{
    return m_setupComplete;
}

int Setup::setupStep() const
{
    return m_setupStep;
}

void Setup::setSetupStep(int step)
{
    if (m_setupStep == step) {
        return;
    }

    m_setupStep = step;
    emit setupStepChanged();
}

void Setup::markSetupComplete()
{
    m_setupComplete = true;
    saveProperty(PROPERTY_SETUP_COMPLETE_KEY, true);

    // Register device with server when setup is complete
    registerDevice();

    emit setupCompleteChanged();
}

void Setup::nextSetupStep()
{
    m_setupStep++;
    emit setupStepChanged();
}

void Setup::resetSetup()
{
    m_setupComplete = false;
    m_setupStep = 0;

    saveProperty(PROPERTY_SETUP_COMPLETE_KEY, false);

    emit setupCompleteChanged();
    emit setupStepChanged();
}

void Setup::loadProperties()
{
    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    m_setupComplete = settings.value(PROPERTY_SETUP_COMPLETE_KEY, PROPERTY_SETUP_COMPLETE_DEFAULT).toBool();
    settings.endGroup();
}

void Setup::saveProperty(const QString& key, const QVariant& value)
{
    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(key, value);
    settings.endGroup();
    settings.sync();
}

void Setup::registerDevice()
{
    if (!m_remoteApi.enabled()) {
        qDebug() << "RemoteApi not enabled, skipping device registration";
        return;
    }

    DeviceStatus status;
    status.deviceId = m_remoteApi.deviceId();
    status.deviceName = "Clock device";

    m_remoteApi.createObject(status, [](bool success, const QString& error) {
        if (success) {
            qDebug() << "Device registered successfully";
        }
        else {
            qWarning() << "Failed to register device:" << error;
        }
    });
}
