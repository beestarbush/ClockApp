#ifndef APPS_SETUP_APPLICATION_H
#define APPS_SETUP_APPLICATION_H

#include <QObject>
#include <QString>

class RemoteApi;
namespace TimeElapsed
{
class Application;
}
namespace Countdown
{
class Application;
}

// Dial wheel parameters as a value type
struct DialWheelParams
{
    Q_GADGET
    Q_PROPERTY(bool visible MEMBER visible)
    Q_PROPERTY(int min MEMBER min)
    Q_PROPERTY(int max MEMBER max)
    Q_PROPERTY(int step MEMBER step)
    Q_PROPERTY(int value MEMBER value)

  public:
    bool visible = false;
    int min = 0;
    int max = 100;
    int step = 1;
    int value = 0;
};

// Media selection parameters as a value type
struct MediaSelectionParams
{
    Q_GADGET
    Q_PROPERTY(bool visible MEMBER visible)
    Q_PROPERTY(int target MEMBER target)

  public:
    bool visible = false;
    int target = -1;
};

namespace Setup
{
class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool setupComplete READ isSetupComplete NOTIFY setupCompleteChanged)
    Q_PROPERTY(PanelType currentPanel READ currentPanel NOTIFY currentPanelChanged)
    Q_PROPERTY(DialWheelParams dialWheel READ dialWheel NOTIFY dialWheelChanged)
    Q_PROPERTY(MediaSelectionParams mediaSelection READ mediaSelection NOTIFY mediaSelectionChanged)

  public:
    // Setup panel types
    enum PanelType
    {
        Welcome = 0,
        DeviceId = 1,
        ServerConnection = 2,
        MarriedTimerEnable = 3,
        MarriedDateTime = 4,
        MarriedBackground = 5,
        KuikenTimerEnable = 6,
        KuikenDateTime = 7,
        KuikenBackground = 8,
        CountdownTimerEnable = 9,
        CountdownDateTime = 10,
        CountdownBackground = 11,
        Finish = 12,
        PanelTypeCount = 13
    };
    Q_ENUM(PanelType)

    // Media selection targets (matching Menu::BackgroundTarget values)
    enum MediaTarget
    {
        NoTarget = -1,
        MarriedTarget = 1,
        KuikenTarget = 2,
        CountdownTarget = 3
    };
    Q_ENUM(MediaTarget)

    Application(TimeElapsed::Application& marriedTimer, TimeElapsed::Application& kuikenTimer, Countdown::Application& countdownTimer, RemoteApi& remoteApi, QObject* parent = nullptr);

    bool isSetupComplete() const;

    PanelType currentPanel() const;

    DialWheelParams dialWheel() const;
    MediaSelectionParams mediaSelection() const;

    Q_INVOKABLE void finish();
    Q_INVOKABLE void next();
    Q_INVOKABLE void reset();

    // Dialog methods
    Q_INVOKABLE void showDialWheel(int min, int max, int step, int value);
    Q_INVOKABLE void updateDialWheelValue(int value);

    Q_INVOKABLE void selectMedia(int target, const QString& mediaName);

    // DateTime picker helper
    Q_INVOKABLE void showDateTimeComponentPicker(int component, int year, int month, int day, int hour, int minute, int second);

  signals:
    void setupCompleteChanged();
    void currentPanelChanged();
    void dialWheelChanged();
    void mediaSelectionChanged();

  private:
    void loadProperties();
    void saveProperty(const QString& key, const QVariant& value);
    void registerDevice();
    PanelType getNextPanel(PanelType current) const;

    bool m_setupComplete;

    // Current panel state
    PanelType m_currentPanel;

    // Dialog state
    DialWheelParams m_dialWheel;
    MediaSelectionParams m_mediaSelection;

    // Application references
    TimeElapsed::Application& m_marriedTimer;
    TimeElapsed::Application& m_kuikenTimer;
    Countdown::Application& m_countdownTimer;
    RemoteApi& m_remoteApi;
};
} // namespace Setup

#endif // APPS_SETUP_APPLICATION_H
