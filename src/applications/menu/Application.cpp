#include "Application.h"
#include "applications/clock/Application.h"
#include "applications/countdown/Application.h"
#include "applications/timeelapsed/Application.h"
using namespace Menu;

Application::Application(TimeElapsed::Application& birthdayTimer, Clock::Application& clock, Countdown::Application& countdownTimer, TimeElapsed::Application& marriedTimer, QObject* parent)
    : QObject(parent),
      m_birthdayTimer(birthdayTimer),
      m_clock(clock),
      m_countdownTimer(countdownTimer),
      m_marriedTimer(marriedTimer),
      m_main(this),
      m_settings(this),
      m_colors(this),
      m_backgrounds(this),
      m_dialog(None),
      m_dialogParam(-1),
      m_mainItems{
          Item("Menu", this),
          Item("Settings", this),
          Item("Notifications", this),
          Item("Backgrounds", this),
          Item("Colors", this),
          Item("Version", this)},
      m_settingsItems{
          Item("Display brightness", this),
          Item("Background opacity", this),
          Item("Setup wizard", this)},
      m_colorsItems{
          Item("Hours", this),
          Item("Minutes", this),
          Item("Seconds", this),
          Item("Pendulum", this)},
      m_backgroundsItems{
          Item("Clock", this),
          Item("Married", this),
          Item("Kuiken", this),
          Item("Countdown", this)}
{
    buildMenus();
}

Menu::Model* Application::main()
{
    return &m_main;
}

Menu::Model* Application::settings()
{
    return &m_settings;
}

Menu::Model* Application::colors()
{
    return &m_colors;
}

Menu::Model* Application::backgrounds()
{
    return &m_backgrounds;
}

Application::DialogType Application::dialog() const
{
    return m_dialog;
}

int Application::dialogParam() const
{
    return m_dialogParam;
}

void Application::buildMenus()
{
    // Build main menu using enum indices - no magic numbers!
    m_mainItems[MainMenu].setAction([this]() {
        closeDialog();
    });
    m_mainItems[MainSettings].setAction([this]() {
        // Settings is a submenu - no direct action
    });
    m_mainItems[MainNotifications].setAction([this]() {
        showDialog(Notifications);
    });
    m_mainItems[MainBackgrounds].setAction([this]() {
        // Backgrounds is a submenu - no direct action
    });
    m_mainItems[MainColors].setAction([this]() {
        // Colors is a submenu - no direct action
    });
    m_mainItems[MainVersion].setAction([this]() {
        showDialog(Version);
    });

    for (auto& item : m_mainItems)
        m_main.add(item);

    // Build settings submenu using enum indices
    m_settingsItems[SettingsBrightness].setAction([this]() {
        showDialog(ScreenBrightness);
    });
    m_settingsItems[SettingsOpacity].setAction([this]() {
        showDialog(BackgroundOpacity);
    });
    m_settingsItems[SettingsWizard].setAction([this]() {
        showDialog(SetupWizard);
    });

    for (auto& item : m_settingsItems)
        m_settings.add(item);

    // Build colors submenu using enum indices
    m_colorsItems[Hours].setAction([this]() {
        showDialog(ColorSelection, Hours);
    });
    m_colorsItems[Minutes].setAction([this]() {
        showDialog(ColorSelection, Minutes);
    });
    m_colorsItems[Seconds].setAction([this]() {
        showDialog(ColorSelection, Seconds);
    });
    m_colorsItems[Pendulum].setAction([this]() {
        showDialog(ColorSelection, Pendulum);
    });

    for (auto& item : m_colorsItems)
        m_colors.add(item);

    // Build backgrounds submenu using enum indices
    m_backgroundsItems[ClockBackground].setAction([this]() {
        showDialog(MediaSelection, ClockBackground);
    });
    m_backgroundsItems[Married].setAction([this]() {
        showDialog(MediaSelection, Married);
    });
    m_backgroundsItems[Kuiken].setAction([this]() {
        showDialog(MediaSelection, Kuiken);
    });
    m_backgroundsItems[Countdown].setAction([this]() {
        showDialog(MediaSelection, Countdown);
    });

    for (auto& item : m_backgroundsItems)
        m_backgrounds.add(item);
}

void Application::showDialog(DialogType type, int param)
{
    m_dialog = type;
    m_dialogParam = param;
    emit dialogChanged();
    emit dialogParamChanged();
}

void Application::closeDialog()
{
    m_dialog = None;
    m_dialogParam = -1;
    emit dialogChanged();
    emit dialogParamChanged();
}

void Application::setBackground(BackgroundTarget target, const QString& mediaName)
{
    switch (target) {
    case ClockBackground:
        m_clock.configuration()->setBackground(mediaName);
        break;
    case Married:
        m_marriedTimer.configuration()->setBackground(mediaName);
        break;
    case Kuiken:
        m_birthdayTimer.configuration()->setBackground(mediaName);
        break;
    case Countdown:
        m_countdownTimer.configuration()->setBackground(mediaName);
        break;
    default:
        qWarning() << "Unknown background target:" << target;
        break;
    }
}

void Application::setColor(ColorPointer pointer, const QColor& color)
{
    switch (pointer) {
    case Hours:
        m_clock.configuration()->setHourColor(color);
        break;
    case Minutes:
        m_clock.configuration()->setMinuteColor(color);
        break;
    case Seconds:
        m_clock.configuration()->setSecondColor(color);
        break;
    case Pendulum:
        m_clock.configuration()->setPendulumBobColor(color);
        break;
    default:
        qWarning() << "Unknown color pointer:" << pointer;
        break;
    }
}

void Application::dialWheelValueChanged(int value)
{
    qDebug() << "Dial wheel value changed:" << value;
    // Handle dial wheel value change based on context
}
