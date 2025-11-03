#include "Clock.h"
#include "services/MediaManager.h"
#include <QDebug>
#include <QSettings>

const QString PROPERTIES_GROUP_NAME = QStringLiteral("clock");
const QString PROPERTY_BACKGROUND_KEY = QStringLiteral("background");
const QString PROPERTY_BACKGROUND_DEFAULT = QStringLiteral("default.gif");
const QString PROPERTY_BACKGROUND_OPACITY_KEY = QStringLiteral("background-opacity");
const qreal PROPERTY_BACKGROUND_OPACITY_DEFAULT = 0.5;
const QString PROPERTY_HOUR_COLOR_KEY = QStringLiteral("hour-color");
const QColor PROPERTY_HOUR_COLOR_DEFAULT = QColor("#995000");
const QString PROPERTY_MINUTE_COLOR_KEY = QStringLiteral("minute-color");
const QColor PROPERTY_MINUTE_COLOR_DEFAULT = QColor("#005099");
const QString PROPERTY_SECOND_COLOR_KEY = QStringLiteral("second-color");
const QColor PROPERTY_SECOND_COLOR_DEFAULT = QColor("#009950");
const QString PROPERTY_PENDULUM_BOB_COLOR_KEY = QStringLiteral("pendulum-bob-color");
const QColor PROPERTY_PENDULUM_BOB_COLOR_DEFAULT = QColor("#009950");
const QString PROPERTY_PENDULUM_ROD_COLOR_KEY = QStringLiteral("pendulum-rod-color");
const QColor PROPERTY_PENDULUM_ROD_COLOR_DEFAULT = QColor("#333333");

Clock::Clock(MediaManager& mediaManager, QObject* parent)
    : QObject(parent),
      m_enabled(true),
      m_backgroundOpacity(PROPERTY_BACKGROUND_OPACITY_DEFAULT),
      m_background(PROPERTY_BACKGROUND_DEFAULT),
      m_hourColor(PROPERTY_HOUR_COLOR_DEFAULT),
      m_minuteColor(PROPERTY_MINUTE_COLOR_DEFAULT),
      m_secondColor(PROPERTY_SECOND_COLOR_DEFAULT),
      m_pendulumBobColor(PROPERTY_PENDULUM_BOB_COLOR_DEFAULT),
      m_pendulumRodColor(PROPERTY_PENDULUM_ROD_COLOR_DEFAULT),
      m_mediaManager(mediaManager)
{
    loadProperties();

    // Refresh background when media sync completes
    connect(&m_mediaManager, &MediaManager::syncCompleted, this, [this]() {
        emit backgroundChanged();
    });
}

void Clock::loadProperties()
{
    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    m_background = settings.value(PROPERTY_BACKGROUND_KEY, PROPERTY_BACKGROUND_DEFAULT).toString();
    m_backgroundOpacity = settings.value(PROPERTY_BACKGROUND_OPACITY_KEY, PROPERTY_BACKGROUND_OPACITY_DEFAULT).toReal();
    m_hourColor = settings.value(PROPERTY_HOUR_COLOR_KEY, PROPERTY_HOUR_COLOR_DEFAULT).value<QColor>();
    m_minuteColor = settings.value(PROPERTY_MINUTE_COLOR_KEY, PROPERTY_MINUTE_COLOR_DEFAULT).value<QColor>();
    m_secondColor = settings.value(PROPERTY_SECOND_COLOR_KEY, PROPERTY_SECOND_COLOR_DEFAULT).value<QColor>();
    m_pendulumBobColor = settings.value(PROPERTY_PENDULUM_BOB_COLOR_KEY, PROPERTY_PENDULUM_BOB_COLOR_DEFAULT).value<QColor>();
    m_pendulumRodColor = settings.value(PROPERTY_PENDULUM_ROD_COLOR_KEY, PROPERTY_PENDULUM_ROD_COLOR_DEFAULT).value<QColor>();
    settings.endGroup();
}

bool Clock::enabled() const
{
    return m_enabled;
}

void Clock::setEnabled(const bool& enabled)
{
    if (m_enabled == enabled) {
        return;
    }

    m_enabled = enabled;
    emit enabledChanged();
}

qreal Clock::backgroundOpacity() const
{
    return m_backgroundOpacity;
}

void Clock::setBackgroundOpacity(const qreal& backgroundOpacity)
{
    if (m_backgroundOpacity == backgroundOpacity) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(PROPERTY_BACKGROUND_KEY, backgroundOpacity);
    settings.endGroup();

    m_backgroundOpacity = backgroundOpacity;
    emit backgroundOpacityChanged();
}

QString Clock::background() const
{
    return m_mediaManager.getMediaPath(m_background);
}

void Clock::setBackground(const QString& background)
{
    if (m_background == background) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(PROPERTY_BACKGROUND_KEY, background);
    settings.endGroup();

    m_background = background;
    emit backgroundChanged();
}

QColor Clock::hourColor() const
{
    return m_hourColor;
}

void Clock::setHourColor(const QColor& hourColor)
{
    if (m_hourColor == hourColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(PROPERTY_HOUR_COLOR_KEY, hourColor);
    settings.endGroup();

    m_hourColor = hourColor;
    emit hourColorChanged();
}

QColor Clock::minuteColor() const
{
    return m_minuteColor;
}

void Clock::setMinuteColor(const QColor& minuteColor)
{
    if (m_minuteColor == minuteColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(PROPERTY_MINUTE_COLOR_KEY, minuteColor);
    settings.endGroup();

    m_minuteColor = minuteColor;
    emit minuteColorChanged();
}

QColor Clock::secondColor() const
{
    return m_secondColor;
}

void Clock::setSecondColor(const QColor& secondColor)
{
    if (m_secondColor == secondColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(PROPERTY_SECOND_COLOR_KEY, secondColor);
    settings.endGroup();

    m_secondColor = secondColor;
    emit secondColorChanged();
}

QColor Clock::pendulumBobColor() const
{
    return m_pendulumBobColor;
}

void Clock::setPendulumBobColor(const QColor& pendulumBobColor)
{
    if (m_pendulumBobColor == pendulumBobColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(PROPERTY_PENDULUM_BOB_COLOR_KEY, pendulumBobColor);
    settings.endGroup();

    m_pendulumBobColor = pendulumBobColor;
    emit pendulumBobColorChanged();
}

QColor Clock::pendulumRodColor() const
{
    return m_pendulumRodColor;
}

void Clock::setPendulumRodColor(const QColor& pendulumRodColor)
{
    if (m_pendulumRodColor == pendulumRodColor) {
        return;
    }

    static QSettings settings;
    settings.beginGroup(PROPERTIES_GROUP_NAME);
    settings.setValue(PROPERTY_PENDULUM_ROD_COLOR_KEY, pendulumRodColor);
    settings.endGroup();

    m_pendulumRodColor = pendulumRodColor;
    emit pendulumRodColorChanged();
}
