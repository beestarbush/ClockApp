#include "Setup.h"
#include "applications/TimeElapsedTimer.h"
#include "applications/CountdownTimer.h"
#include "applications/BirthdayTimer.h"
#include "applications/MarriedTimer.h"
#include <QSettings>
#include <QDebug>

const QString PROPERTIES_GROUP_NAME = QStringLiteral("setup");
const QString PROPERTY_SETUP_COMPLETE_KEY = QStringLiteral("setup-complete");
const bool PROPERTY_SETUP_COMPLETE_DEFAULT = false;

Setup::Setup(MarriedTimer* marriedTimer,
             BirthdayTimer* kuikenTimer,
             CountdownTimer* christmasTimer,
             QObject *parent) :
    QObject(parent),
    m_setupComplete(PROPERTY_SETUP_COMPLETE_DEFAULT),
    m_setupStep(0),
    m_marriedTimer(marriedTimer),
    m_kuikenTimer(kuikenTimer),
    m_christmasTimer(christmasTimer)
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
