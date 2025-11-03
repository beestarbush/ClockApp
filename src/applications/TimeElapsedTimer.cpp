#include "TimeElapsedTimer.h"
#include "services/MediaManager.h"
#include <QDateTime>
#include <QDebug>
#include <QSettings>

const QString PROPERTY_ENABLED_KEY = QStringLiteral("enabled");
const bool PROPERTY_ENABLED_DEFAULT = false;
const QString PROPERTY_INITIALIZED_KEY = QStringLiteral("initialized");
const bool PROPERTY_INITIALIZED_DEFAULT = false;
const QString PROPERTY_BACKGROUND_KEY = QStringLiteral("background");
const QString PROPERTY_BACKGROUND_DEFAULT = QStringLiteral("default.gif");
const QString PROPERTY_BACKGROUND_OPACITY_KEY = QStringLiteral("background-opacity");
const qreal PROPERTY_BACKGROUND_OPACITY_DEFAULT = 0.5;
const QString PROPERTY_TIMESTAMP_KEY = QStringLiteral("timestamp");
constexpr quint64 PROPERTY_TIMESTAMP_DEFAULT = 0;

constexpr quint64 SECONDS_IN_MINUTE = 60;
constexpr quint64 SECONDS_IN_HOUR = SECONDS_IN_MINUTE * 60;
constexpr quint64 SECONDS_IN_A_DAY = SECONDS_IN_HOUR * 24;
constexpr quint64 DAYS_IN_A_WEEK = 7;
constexpr quint64 DAYS_IN_YEAR = 365;

TimeElapsedTimer::TimeElapsedTimer(const QString& name, MediaManager& mediaManager, QObject* parent)
    : QObject(parent),
      m_name(name),
      m_enabled(PROPERTY_ENABLED_DEFAULT),
      m_initialized(PROPERTY_INITIALIZED_DEFAULT),
      m_backgroundOpacity(PROPERTY_BACKGROUND_OPACITY_DEFAULT),
      m_background(PROPERTY_BACKGROUND_DEFAULT),
      m_timestamp(PROPERTY_TIMESTAMP_DEFAULT),
      m_years(0),
      m_days(0),
      m_daysInWeek(0),
      m_weeks(0),
      m_hours(0),
      m_minutes(0),
      m_seconds(0),
      m_mediaManager(mediaManager),
      m_timer(this)
{
    loadProperties();
    startTimer();

    // Refresh background when media sync completes
    connect(&m_mediaManager, &MediaManager::syncCompleted, this, [this]() {
        emit backgroundChanged();
    });
}

QString TimeElapsedTimer::name() const
{
    return m_name;
}

void TimeElapsedTimer::setName(const QString& name)
{
    if (m_name == name) {
        return;
    }

    m_name = name;
    emit nameChanged();
}

bool TimeElapsedTimer::enabled() const
{
    return m_enabled;
}

void TimeElapsedTimer::setEnabled(const bool& enabled)
{
    if (m_enabled == enabled) {
        return;
    }

    saveProperty(PROPERTY_ENABLED_KEY, enabled);
    m_enabled = enabled;
    emit enabledChanged();
}

bool TimeElapsedTimer::isInitialized() const
{
    return m_initialized;
}

void TimeElapsedTimer::setInitialized(const bool& initialized)
{
    if (m_initialized == initialized) {
        return;
    }

    saveProperty(PROPERTY_INITIALIZED_KEY, initialized);
    m_initialized = initialized;
    emit initializedChanged();
}

qreal TimeElapsedTimer::backgroundOpacity() const
{
    return m_backgroundOpacity;
}

void TimeElapsedTimer::setBackgroundOpacity(const qreal& backgroundOpacity)
{
    if (m_backgroundOpacity == backgroundOpacity) {
        return;
    }

    saveProperty(PROPERTY_BACKGROUND_OPACITY_KEY, backgroundOpacity);

    m_backgroundOpacity = backgroundOpacity;
    emit backgroundOpacityChanged();
}

QString TimeElapsedTimer::background() const
{
    return m_mediaManager.getMediaPath(m_background);
}

void TimeElapsedTimer::setBackground(const QString& background)
{
    if (m_background == background) {
        return;
    }

    saveProperty(PROPERTY_BACKGROUND_KEY, background);
    m_background = background;
    emit backgroundChanged();
}

quint64 TimeElapsedTimer::timestamp() const
{
    return m_timestamp;
}

void TimeElapsedTimer::setTimestamp(const quint64& timestamp)
{
    if (m_timestamp == timestamp) {
        return;
    }

    saveProperty(PROPERTY_TIMESTAMP_KEY, timestamp);
    m_timestamp = timestamp;
    emit timestampChanged();

    // Recalculate time immediately when timestamp changes
    calculateTimeElapsed();
}

void TimeElapsedTimer::startTimer()
{
    calculateTimeElapsed();

    if (!m_timer.isActive()) {
        connect(&m_timer, &QTimer::timeout, this, &TimeElapsedTimer::calculateTimeElapsed);
        m_timer.start(1000); // Update every second
    }
}

void TimeElapsedTimer::stopTimer()
{
    if (m_timer.isActive()) {
        disconnect(&m_timer, &QTimer::timeout, this, &TimeElapsedTimer::calculateTimeElapsed);
        m_timer.stop();
    }
}

void TimeElapsedTimer::loadProperties()
{
    static QSettings settings;
    settings.beginGroup(m_name);

    m_enabled = settings.value(PROPERTY_ENABLED_KEY, PROPERTY_ENABLED_DEFAULT).toBool();
    m_initialized = settings.value(PROPERTY_INITIALIZED_KEY, PROPERTY_INITIALIZED_DEFAULT).toBool();
    m_background = settings.value(PROPERTY_BACKGROUND_KEY, PROPERTY_BACKGROUND_DEFAULT).toString();
    m_backgroundOpacity = settings.value(PROPERTY_BACKGROUND_OPACITY_KEY, PROPERTY_BACKGROUND_OPACITY_DEFAULT).toReal();
    m_timestamp = settings.value(PROPERTY_TIMESTAMP_KEY, PROPERTY_TIMESTAMP_DEFAULT).toULongLong();

    loadAdditionalProperties();

    settings.endGroup();
}

void TimeElapsedTimer::loadAdditionalProperties()
{
    // Default implementation does nothing.
    // Derived classes can override this to load their specific properties.
}

void TimeElapsedTimer::saveProperty(const QString& key, const QVariant& value)
{
    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(key, value);
    settings.endGroup();
}

void TimeElapsedTimer::calculateTimeElapsed()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    QDateTime referenceDate = QDateTime::fromSecsSinceEpoch(m_timestamp);

    // Calculate the difference in seconds
    qint64 diffSeconds = now.toSecsSinceEpoch() - referenceDate.toSecsSinceEpoch();

    quint64 years = diffSeconds / (SECONDS_IN_A_DAY * DAYS_IN_YEAR);
    quint64 days = diffSeconds / (SECONDS_IN_A_DAY);
    quint64 daysInWeek = days % DAYS_IN_A_WEEK;
    quint64 weeks = days / DAYS_IN_A_WEEK;
    quint64 hours = (diffSeconds % (SECONDS_IN_A_DAY)) / (SECONDS_IN_HOUR);
    quint64 minutes = (diffSeconds % (SECONDS_IN_HOUR)) / SECONDS_IN_MINUTE;
    quint64 seconds = diffSeconds % SECONDS_IN_MINUTE;

    setYears(years);
    setDays(days);
    setDaysInWeek(daysInWeek);
    setWeeks(weeks);
    setHours(hours);
    setMinutes(minutes);
    setSeconds(seconds);

    emit timeChanged();
}

quint64 TimeElapsedTimer::years() const
{
    return m_years;
}

void TimeElapsedTimer::setYears(const quint64 years)
{
    if (m_years != years) {
        m_years = years;
        emit yearsChanged(m_years);
    }
}

quint64 TimeElapsedTimer::days() const
{
    return m_days;
}

void TimeElapsedTimer::setDays(const quint64 days)
{
    if (m_days != days) {
        m_days = days;
        emit daysChanged(m_days);
    }
}

quint64 TimeElapsedTimer::daysInWeek() const
{
    return m_daysInWeek;
}

void TimeElapsedTimer::setDaysInWeek(const quint64 daysInWeek)
{
    if (m_daysInWeek != daysInWeek) {
        m_daysInWeek = daysInWeek;
        emit daysInWeekChanged(m_daysInWeek);
    }
}

quint64 TimeElapsedTimer::weeks() const
{
    return m_weeks;
}

void TimeElapsedTimer::setWeeks(const quint64 weeks)
{
    if (m_weeks != weeks) {
        m_weeks = weeks;
        emit weeksChanged(m_weeks);
    }
}

quint64 TimeElapsedTimer::hours() const
{
    return m_hours;
}

void TimeElapsedTimer::setHours(const quint64 hours)
{
    if (m_hours != hours) {
        m_hours = hours;
        emit hoursChanged(m_hours);
    }
}

quint64 TimeElapsedTimer::minutes() const
{
    return m_minutes;
}

void TimeElapsedTimer::setMinutes(const quint64 minutes)
{
    if (m_minutes != minutes) {
        m_minutes = minutes;
        emit minutesChanged(m_minutes);
    }
}

quint64 TimeElapsedTimer::seconds() const
{
    return m_seconds;
}

void TimeElapsedTimer::setSeconds(const quint64 seconds)
{
    if (m_seconds != seconds) {
        m_seconds = seconds;
        emit secondsChanged(m_seconds);
    }
}
