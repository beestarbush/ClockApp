#ifndef APPS_MENU_H
#define APPS_MENU_H

#include <QColor>
#include <QObject>
#include <QString>
#include <QVariant>

#include "menu/MenuItem.h"
#include "menu/MenuModel.h"

class BirthdayTimer;
class Clock;
class CountdownTimer;
class MarriedTimer;

class Menu : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MenuModel* main READ main CONSTANT)
    Q_PROPERTY(MenuModel* settings READ settings CONSTANT)
    Q_PROPERTY(MenuModel* colors READ colors CONSTANT)
    Q_PROPERTY(MenuModel* backgrounds READ backgrounds CONSTANT)
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

    Menu(BirthdayTimer& birthdayTimer, Clock& clock, CountdownTimer& countdownTimer, MarriedTimer& marriedTimer, QObject* parent = nullptr);

    MenuModel* main();
    MenuModel* settings();
    MenuModel* colors();
    MenuModel* backgrounds();

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

    BirthdayTimer& m_birthdayTimer;
    Clock& m_clock;
    CountdownTimer& m_countdownTimer;
    MarriedTimer& m_marriedTimer;

    MenuModel m_main;
    MenuModel m_settings;
    MenuModel m_colors;
    MenuModel m_backgrounds;

    DialogType m_dialog;
    int m_dialogParam;

    // Store menu items to manage lifetime - sized using enum counts
    MenuItem m_mainItems[MainMenuCount];
    MenuItem m_settingsItems[SettingsMenuCount];
    MenuItem m_colorsItems[ColorPointerCount];
    MenuItem m_backgroundsItems[BackgroundTargetCount];
};

#endif // APPS_MENU_H
