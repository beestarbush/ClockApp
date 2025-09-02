#include "Settings.h"
#include <QDebug>
#include <QSettings>

#ifdef PLATFORM_IS_TARGET
const QString SETTINGS_PATH = QStringLiteral("/usr/share/");
#else
const QString SETTINGS_PATH = QStringLiteral("/workdir/build");
#endif

const QString CLOCK_GROUP = QStringLiteral("clock");
const QString CLOCK_BACKGROUND_ANIMATION_KEY = QStringLiteral("background-animation");
const QString CLOCK_BACKGROUND_ANIMATION_DEFAULT = QStringLiteral("test.gif");
const QString CLOCK_HOUR_COLOR_KEY = QStringLiteral("hour-color");
const QColor CLOCK_HOUR_COLOR_DEFAULT = QColor(Qt::red);
const QString CLOCK_MINUTE_COLOR_KEY = QStringLiteral("minute-color");
const QColor CLOCK_MINUTE_COLOR_DEFAULT = QColor(Qt::blue);
const QString CLOCK_SECOND_COLOR_KEY = QStringLiteral("second-color");
const QColor CLOCK_SECOND_COLOR_DEFAULT = QColor(Qt::green);
const QString CLOCK_PENDULUM_COLOR_KEY = QStringLiteral("pendulum-bob-color");
const QColor CLOCK_PENDULUM_COLOR_DEFAULT = QColor(Qt::green);
const QString MARRIED_TIMER_GROUP = QStringLiteral("married-timer");
const QString MARRIED_TIMER_ENABLED_KEY = QStringLiteral("enabled");
const bool MARRIED_TIMER_ENABLED_DEFAULT = false;
const QString MARRIED_TIMER_BACKGROUND_OPACITY_KEY = QStringLiteral("background-opacity");
const qreal MARRIED_TIMER_BACKGROUND_OPACITY_DEFAULT = 0.3;
const QString MARRIED_TIMER_BACKGROUND_ANIMATION_KEY = QStringLiteral("background-animation");
const QString MARRIED_TIMER_BACKGROUND_ANIMATION_DEFAULT = QStringLiteral("married_bw.gif");
const QString KUIKEN_TIMER_GROUP = QStringLiteral("kuiken-timer");
const QString KUIKEN_TIMER_ENABLED_KEY = QStringLiteral("enabled");
const bool KUIKEN_TIMER_ENABLED_DEFAULT = false;

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    QSettings::setDefaultFormat(QSettings::NativeFormat);
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, SETTINGS_PATH);

    if (!load()) {
        qWarning() << "Failed to load settings";
    }
}

bool Settings::load()
{
    static QSettings settings;
    settings.beginGroup(CLOCK_GROUP);
    m_clockBackgroundAnimation = settings.value(CLOCK_BACKGROUND_ANIMATION_KEY, CLOCK_BACKGROUND_ANIMATION_DEFAULT).toString();
    m_clockHourColor = settings.value(CLOCK_HOUR_COLOR_KEY, CLOCK_HOUR_COLOR_DEFAULT).value<QColor>();
    m_clockMinuteColor = settings.value(CLOCK_MINUTE_COLOR_KEY, CLOCK_MINUTE_COLOR_DEFAULT).value<QColor>();
    m_clockSecondColor = settings.value(CLOCK_SECOND_COLOR_KEY, CLOCK_SECOND_COLOR_DEFAULT).value<QColor>();
    m_clockPendulumColor = settings.value(CLOCK_PENDULUM_COLOR_KEY, CLOCK_PENDULUM_COLOR_DEFAULT).value<QColor>();
    settings.endGroup();

    settings.beginGroup(MARRIED_TIMER_GROUP);
    m_marriedTimerEnabled = settings.value(MARRIED_TIMER_ENABLED_KEY, MARRIED_TIMER_ENABLED_DEFAULT).toBool();
    m_marriedTimerBackgroundOpacity = settings.value(MARRIED_TIMER_BACKGROUND_OPACITY_KEY, MARRIED_TIMER_BACKGROUND_OPACITY_DEFAULT).toReal();
    m_marriedTimerBackgroundAnimation = settings.value(MARRIED_TIMER_BACKGROUND_ANIMATION_KEY, MARRIED_TIMER_BACKGROUND_ANIMATION_DEFAULT).toString();
    settings.endGroup();

    settings.beginGroup(KUIKEN_TIMER_GROUP);
    m_kuikenTimerEnabled = settings.value(KUIKEN_TIMER_ENABLED_KEY, KUIKEN_TIMER_ENABLED_DEFAULT).toBool();
    settings.endGroup();

    return true;
}

void Settings::setClockBackgroundAnimation(const QString &animation)
{
    if (m_clockBackgroundAnimation != animation) {
        static QSettings settings;
        settings.beginGroup(CLOCK_GROUP);
        settings.setValue(CLOCK_BACKGROUND_ANIMATION_KEY, animation);
        settings.endGroup();

        m_clockBackgroundAnimation = animation;
        emit clockBackgroundAnimationChanged();
    }
}

QString Settings::clockBackgroundAnimation() const
{
    return m_clockBackgroundAnimation;
}

void Settings::setClockHourColor(const QColor &color)
{
    if (m_clockHourColor != color) {
        static QSettings settings;
        settings.beginGroup(CLOCK_GROUP);
        settings.setValue(CLOCK_HOUR_COLOR_KEY, color);
        settings.endGroup();

        m_clockHourColor = color;
        emit clockHourColorChanged();
    }
}

QColor Settings::clockHourColor() const
{
    return m_clockHourColor;
}

void Settings::setClockMinuteColor(const QColor &color)
{
    if (m_clockMinuteColor != color) {
        static QSettings settings;
        settings.beginGroup(CLOCK_GROUP);
        settings.setValue(CLOCK_MINUTE_COLOR_KEY, color);
        settings.endGroup();

        m_clockMinuteColor = color;
        emit clockMinuteColorChanged();
    }
}

QColor Settings::clockMinuteColor() const
{
    return m_clockMinuteColor;
}

void Settings::setClockSecondColor(const QColor &color)
{
    if (m_clockSecondColor != color) {
        static QSettings settings;
        settings.beginGroup(CLOCK_GROUP);
        settings.setValue(CLOCK_SECOND_COLOR_KEY, color);
        settings.endGroup();

        m_clockSecondColor = color;
        emit clockSecondColorChanged();
    }
}

QColor Settings::clockSecondColor() const
{
    return m_clockSecondColor;
}

void Settings::setClockPendulumColor(const QColor &color)
{
    if (m_clockPendulumColor != color) {
        static QSettings settings;
        settings.beginGroup(CLOCK_GROUP);
        settings.setValue(CLOCK_PENDULUM_COLOR_KEY, color);
        settings.endGroup();

        m_clockPendulumColor = color;
        emit clockPendulumColorChanged();
    }
}

QColor Settings::clockPendulumColor() const
{
    return m_clockPendulumColor;
}

void Settings::setMarriedTimerEnabled(bool enabled)
{
    if (m_marriedTimerEnabled != enabled) {
        static QSettings settings;
        settings.beginGroup(MARRIED_TIMER_GROUP);
        settings.setValue(MARRIED_TIMER_ENABLED_KEY, enabled);
        settings.endGroup();

        m_marriedTimerEnabled = enabled;
        emit marriedTimerEnabledChanged();
    }
}

bool Settings::marriedTimerEnabled() const
{
    return m_marriedTimerEnabled;
}

void Settings::setMarriedTimerBackgroundOpacity(qreal opacity)
{
    if (!qFuzzyCompare(m_marriedTimerBackgroundOpacity, opacity)) {
        static QSettings settings;
        settings.beginGroup(MARRIED_TIMER_GROUP);
        settings.setValue(MARRIED_TIMER_BACKGROUND_OPACITY_KEY, opacity);
        settings.endGroup();

        m_marriedTimerBackgroundOpacity = opacity;
        emit marriedTimerBackgroundOpacityChanged();
    }
}

qreal Settings::marriedTimerBackgroundOpacity() const
{
    return m_marriedTimerBackgroundOpacity;
}

void Settings::setMarriedTimerBackgroundAnimation(const QString &animation)
{
    if (m_marriedTimerBackgroundAnimation != animation) {
        static QSettings settings;
        settings.beginGroup(MARRIED_TIMER_GROUP);
        settings.setValue(MARRIED_TIMER_BACKGROUND_ANIMATION_KEY, animation);
        settings.endGroup();

        m_marriedTimerBackgroundAnimation = animation;
        emit marriedTimerBackgroundAnimationChanged();
    }
}

QString Settings::marriedTimerBackgroundAnimation() const
{
    return m_marriedTimerBackgroundAnimation;
}

void Settings::setKuikenTimerEnabled(bool enabled)
{
    if (m_kuikenTimerEnabled != enabled) {
        static QSettings settings;
        settings.beginGroup(KUIKEN_TIMER_GROUP);
        settings.setValue(KUIKEN_TIMER_ENABLED_KEY, enabled);
        settings.endGroup();

        m_kuikenTimerEnabled = enabled;
        emit kuikenTimerEnabledChanged();
    }
}

bool Settings::kuikenTimerEnabled() const
{
    return m_kuikenTimerEnabled;
}
