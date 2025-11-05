#include "Application.h"
#include "applications/countdown/Application.h"
#include "applications/timeelapsed/Application.h"
#include "services/remoteapi/DeviceStatus.h"
#include "services/remoteapi/Service.h"
#include <QDate>
#include <QDebug>
#include <QSettings>
using namespace Setup;

const QString PROPERTIES_GROUP_NAME = QStringLiteral("setup");
const QString PROPERTY_SETUP_COMPLETE_KEY = QStringLiteral("setup-complete");
const bool PROPERTY_SETUP_COMPLETE_DEFAULT = false;

Application::Application(TimeElapsed::Application& marriedTimer,
                         TimeElapsed::Application& kuikenTimer,
                         Countdown::Application& countdownTimer,
                         RemoteApi::Service& remoteApi,
                         QObject* parent)
    : QObject(parent),
      m_setupComplete(PROPERTY_SETUP_COMPLETE_DEFAULT),
      m_currentPanel(Welcome),
      m_dialWheel(),
      m_mediaSelection(),
      m_marriedTimer(marriedTimer),
      m_kuikenTimer(kuikenTimer),
      m_countdownTimer(countdownTimer),
      m_remoteApi(remoteApi)
{
    loadProperties();
}

bool Application::isSetupComplete() const
{
    return m_setupComplete;
}

Application::PanelType Application::currentPanel() const
{
    return m_currentPanel;
}

DialWheelParams Application::dialWheel() const
{
    return m_dialWheel;
}

MediaSelectionParams Application::mediaSelection() const
{
    return m_mediaSelection;
}

void Application::finish()
{
    m_setupComplete = true;
    m_currentPanel = Welcome;

    // Hide any open dialogs
    m_dialWheel.visible = false;
    m_mediaSelection.visible = false;

    saveProperty(PROPERTY_SETUP_COMPLETE_KEY, true);

    // Register device with server when setup is complete
    registerDevice();

    emit dialWheelChanged();
    emit mediaSelectionChanged();
    emit currentPanelChanged();
    emit setupCompleteChanged();
}

void Application::next()
{
    // Hide current dialogs
    if (m_dialWheel.visible) {
        m_dialWheel.visible = false;
        emit dialWheelChanged();
    }
    if (m_mediaSelection.visible) {
        m_mediaSelection.visible = false;
        emit mediaSelectionChanged();
    }

    // State machine: determine next panel based on current panel
    PanelType nextPanel = getNextPanel(m_currentPanel);

    if (nextPanel != m_currentPanel) {
        m_currentPanel = nextPanel;

        // Auto-show media selection dialogs for background panels
        if (m_currentPanel == MarriedBackground) {
            m_mediaSelection.target = MarriedTarget;
            m_mediaSelection.visible = true;
            emit mediaSelectionChanged();
        }
        else if (m_currentPanel == KuikenBackground) {
            m_mediaSelection.target = KuikenTarget;
            m_mediaSelection.visible = true;
            emit mediaSelectionChanged();
        }
        else if (m_currentPanel == CountdownBackground) {
            m_mediaSelection.target = CountdownTarget;
            m_mediaSelection.visible = true;
            emit mediaSelectionChanged();
        }

        emit currentPanelChanged();
    }
}

void Application::reset()
{
    m_setupComplete = false;

    saveProperty(PROPERTY_SETUP_COMPLETE_KEY, false);

    emit setupCompleteChanged();
}

void Application::loadProperties()
{
    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    m_setupComplete = settings.value(PROPERTY_SETUP_COMPLETE_KEY, PROPERTY_SETUP_COMPLETE_DEFAULT).toBool();
    settings.endGroup();
}

void Application::saveProperty(const QString& key, const QVariant& value)
{
    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(key, value);
    settings.endGroup();
    settings.sync();
}

void Application::registerDevice()
{
    if (!m_remoteApi.enabled()) {
        return;
    }

    DeviceStatus status;
    status.deviceId = m_remoteApi.deviceId();
    m_remoteApi.createObject(status, [](bool success, const QString& error) {
        if (success) {
            qDebug() << "Device registered successfully";
        }
        else {
            qWarning() << "Failed to register device:" << error;
        }
    });
}

Application::PanelType Application::getNextPanel(PanelType current) const
{
    // State machine transitions
    switch (current) {
    case Welcome:
        return DeviceId;

    case DeviceId:
        return ServerConnection;

    case ServerConnection:
        return MarriedTimerEnable;

    case MarriedTimerEnable:
        // If married timer is enabled, go to date/time, otherwise skip to kuiken
        if (m_marriedTimer.configuration()->enabled()) {
            return MarriedDateTime;
        }
        return KuikenTimerEnable;

    case MarriedDateTime:
        return MarriedBackground;

    case MarriedBackground:
        return KuikenTimerEnable;

    case KuikenTimerEnable:
        // If kuiken timer is enabled, go to date/time, otherwise skip to countdown
        if (m_kuikenTimer.configuration()->enabled()) {
            return KuikenDateTime;
        }
        return CountdownTimerEnable;

    case KuikenDateTime:
        return KuikenBackground;

    case KuikenBackground:
        return CountdownTimerEnable;

    case CountdownTimerEnable:
        // If countdown timer is enabled, go to date/time, otherwise skip to finish
        if (m_countdownTimer.configuration()->enabled()) {
            return CountdownDateTime;
        }
        return Finish;

    case CountdownDateTime:
        return CountdownBackground;

    case CountdownBackground:
        return Finish;

    case Finish:
    default:
        return Finish; // Stay at finish
    }
}

void Application::showDialWheel(int min, int max, int step, int value)
{
    m_dialWheel.min = min;
    m_dialWheel.max = max;
    m_dialWheel.step = step;
    m_dialWheel.value = value;
    m_dialWheel.visible = true;

    emit dialWheelChanged();
}

void Application::updateDialWheelValue(int value)
{
    if (m_dialWheel.value == value) {
        return;
    }

    m_dialWheel.value = value;
    emit dialWheelChanged();
}

void Application::selectMedia(int target, const QString& mediaName)
{
    switch (target) {
    case MarriedTarget:
        m_marriedTimer.configuration()->setBackground(mediaName);
        break;
    case KuikenTarget:
        m_kuikenTimer.configuration()->setBackground(mediaName);
        break;
    case CountdownTarget:
        m_countdownTimer.configuration()->setBackground(mediaName);

        break;
    default:
        break;
    }
}

void Application::showDateTimeComponentPicker(int component, int year, int month, int day, int hour, int minute, int second)
{
    int min = 0, max = 100, step = 1, value = 0;

    switch (component) {
    case 0: // Day
        min = 1;
        max = QDate(year, month, 1).daysInMonth();
        step = 1;
        value = day;
        break;
    case 1: // Month
        min = 1;
        max = 12;
        step = 1;
        value = month;
        break;
    case 2: // Year
        min = 1970;
        max = 2100;
        step = 1;
        value = year;
        break;
    case 3: // Hours
        min = 0;
        max = 23;
        step = 1;
        value = hour;
        break;
    case 4: // Minutes
        min = 0;
        max = 59;
        step = 1;
        value = minute;
        break;
    case 5: // Seconds
        min = 0;
        max = 59;
        step = 1;
        value = second;
        break;
    }

    showDialWheel(min, max, step, value);
}
