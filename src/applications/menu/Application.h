#ifndef APPS_MENU_APPLICATION_H
#define APPS_MENU_APPLICATION_H

#include <QColor>
#include <QObject>
#include <QString>
#include <QVariant>

#include "Item.h"
#include "Model.h"
namespace Applications::Clock
{
class Application;
}
namespace Applications::TimeElapsed
{
class Application;
}
namespace Applications::Countdown
{
class Application;
}

namespace Applications::Menu
{
class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Menu::Model* main READ main CONSTANT)
    Q_PROPERTY(Menu::Model* settings READ settings CONSTANT)
    Q_PROPERTY(Menu::Model* colors READ colors CONSTANT)
    Q_PROPERTY(Menu::Model* backgrounds READ backgrounds CONSTANT)
    Q_PROPERTY(DialogType dialog READ dialog NOTIFY dialogChanged)
    Q_PROPERTY(int dialogParam READ dialogParam NOTIFY dialogParamChanged)

  public:
    // Main menu items
    enum MainMenuItem
    {
        MainMenu = 0,
        MainSettings = 1,
        MainNotifications = 2,
        MainBackgrounds = 3,
        MainColors = 4,
        MainVersion = 5,
        MainMenuCount = 6
    };
    Q_ENUM(MainMenuItem)

    // Settings menu items
    enum SettingsMenuItem
    {
        SettingsBrightness = 0,
        SettingsOpacity = 1,
        SettingsWizard = 2,
        SettingsMenuCount = 3
    };
    Q_ENUM(SettingsMenuItem)

    // Dialog types
    enum DialogType
    {
        None,
        Version,
        SetupWizard,
        ScreenBrightness,
        BackgroundOpacity,
        MediaSelection,
        ColorSelection,
        Notifications,
        DialWheel
    };
    Q_ENUM(DialogType)

    // Color pointer indices
    enum ColorPointer
    {
        Hours = 0,
        Minutes = 1,
        Seconds = 2,
        Pendulum = 3,
        ColorPointerCount = 4
    };
    Q_ENUM(ColorPointer)

    // Background selection indices
    enum BackgroundTarget
    {
        ClockBackground = 0,
        Married = 1,
        Kuiken = 2,
        Countdown = 3,
        BackgroundTargetCount = 4
    };
    Q_ENUM(BackgroundTarget)

    Application(TimeElapsed::Application& birthdayTimer, Clock::Application& clock, Countdown::Application& countdownTimer, TimeElapsed::Application& marriedTimer, QObject* parent = nullptr);

    Menu::Model* main();
    Menu::Model* settings();
    Menu::Model* colors();
    Menu::Model* backgrounds();

    DialogType dialog() const;
    int dialogParam() const;

    Q_INVOKABLE void showDialog(DialogType type, int param = -1);
    Q_INVOKABLE void closeDialog();
    Q_INVOKABLE void setBackground(BackgroundTarget target, const QString& mediaName);
    Q_INVOKABLE void setColor(ColorPointer pointer, const QColor& color);
    Q_INVOKABLE void dialWheelValueChanged(int value);

  signals:
    void dialogChanged();
    void dialogParamChanged();

  private:
    void buildMenus();

    TimeElapsed::Application& m_birthdayTimer;
    Clock::Application& m_clock;
    Countdown::Application& m_countdownTimer;
    TimeElapsed::Application& m_marriedTimer;

    Menu::Model m_main;
    Menu::Model m_settings;
    Menu::Model m_colors;
    Menu::Model m_backgrounds;

    DialogType m_dialog;
    int m_dialogParam;

    // Store menu items to manage lifetime - sized using enum counts
    Menu::Item m_mainItems[MainMenuCount];
    Menu::Item m_settingsItems[SettingsMenuCount];
    Menu::Item m_colorsItems[ColorPointerCount];
    Menu::Item m_backgroundsItems[BackgroundTargetCount];
};
} // namespace Applications::Menu

#endif // APPS_MENU_APPLICATION_H
