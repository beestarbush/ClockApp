#include "Menu.h"
#include "Applications.h"
#include "BirthdayTimer.h"
#include "Clock.h"
#include "CountdownTimer.h"
#include "MarriedTimer.h"
#include "Setup.h"

Menu::Menu(BirthdayTimer& birthdayTimer, Clock& clock, CountdownTimer& countdownTimer, MarriedTimer& marriedTimer, QObject* parent)
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
          MenuItem("Menu", this),
          MenuItem("Settings", this),
          MenuItem("Notifications", this),
          MenuItem("Backgrounds", this),
          MenuItem("Colors", this),
          MenuItem("Version", this)},
      m_settingsItems{
          MenuItem("Display brightness", this),
          MenuItem("Background opacity", this),
          MenuItem("Setup wizard", this)},
      m_colorsItems{
          MenuItem("Hours", this),
          MenuItem("Minutes", this),
          MenuItem("Seconds", this),
          MenuItem("Pendulum", this)},
      m_backgroundsItems{
          MenuItem("Clock", this),
          MenuItem("Married", this),
          MenuItem("Kuiken", this),
          MenuItem("Countdown", this)}
{
    buildMenus();
}

MenuModel* Menu::main()
{
    return &m_main;
}

MenuModel* Menu::settings()
{
    return &m_settings;
}

MenuModel* Menu::colors()
{
    return &m_colors;
}

MenuModel* Menu::backgrounds()
{
    return &m_backgrounds;
}

Menu::DialogType Menu::dialog() const
{
    return m_dialog;
}

int Menu::dialogParam() const
{
    return m_dialogParam;
}

void Menu::buildMenus()
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

void Menu::showDialog(DialogType type, int param)
{
    m_dialog = type;
    m_dialogParam = param;
    emit dialogChanged();
    emit dialogParamChanged();
}

void Menu::closeDialog()
{
    m_dialog = None;
    m_dialogParam = -1;
    emit dialogChanged();
    emit dialogParamChanged();
}

void Menu::setBackground(BackgroundTarget target, const QString& mediaName)
{
    switch (target) {
    case ClockBackground:
        m_clock.setBackground(mediaName);
        break;
    case Married:
        m_marriedTimer.setBackground(mediaName);
        break;
    case Kuiken:
        m_birthdayTimer.setBackground(mediaName);
        break;
    case Countdown:
        m_countdownTimer.setBackground(mediaName);
        break;
    default:
        qWarning() << "Unknown background target:" << target;
        break;
    }
}

void Menu::setColor(ColorPointer pointer, const QColor& color)
{
    switch (pointer) {
    case Hours:
        m_clock.setHourColor(color);
        break;
    case Minutes:
        m_clock.setMinuteColor(color);
        break;
    case Seconds:
        m_clock.setSecondColor(color);
        break;
    case Pendulum:
        m_clock.setPendulumBobColor(color);
        break;
    default:
        qWarning() << "Unknown color pointer:" << pointer;
        break;
    }
}

void Menu::dialWheelValueChanged(int value)
{
    qDebug() << "Dial wheel value changed:" << value;
    // Handle dial wheel value change based on context
}
