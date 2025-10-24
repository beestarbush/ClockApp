#include "CountdownTimer.h"
#include "services/MediaManager.h"
#include <QSettings>
#include <QDateTime>
#include <QDebug>

const QString PROPERTY_ENABLED_KEY = QStringLiteral("enabled");
const bool PROPERTY_ENABLED_DEFAULT = false;
const QString PROPERTY_INITIALIZED_KEY = QStringLiteral("initialized");
const bool PROPERTY_INITIALIZED_DEFAULT = false;
const QString PROPERTY_BACKGROUND_KEY = QStringLiteral("background");
const QString PROPERTY_BACKGROUND_DEFAULT = QStringLiteral("test.gif");
const QString PROPERTY_BACKGROUND_OPACITY_KEY = QStringLiteral("background-opacity");
const qreal PROPERTY_BACKGROUND_OPACITY_DEFAULT = 0.5;
const QString PROPERTY_TARGET_TIMESTAMP_KEY = QStringLiteral("target-timestamp");
constexpr quint64 PROPERTY_TARGET_TIMESTAMP_DEFAULT = 0;

constexpr quint64 SECONDS_IN_MINUTE = 60;
constexpr quint64 SECONDS_IN_HOUR = SECONDS_IN_MINUTE * 60;
constexpr quint64 SECONDS_IN_A_DAY = SECONDS_IN_HOUR * 24;
constexpr quint64 DAYS_IN_A_WEEK = 7;
constexpr quint64 DAYS_IN_YEAR = 365;

CountdownTimer::CountdownTimer(const QString &name, MediaManager& mediaManager, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_enabled(PROPERTY_ENABLED_DEFAULT),
    m_initialized(PROPERTY_INITIALIZED_DEFAULT),
    m_backgroundOpacity(PROPERTY_BACKGROUND_OPACITY_DEFAULT),
    m_background(PROPERTY_BACKGROUND_DEFAULT),
    m_targetTimestamp(PROPERTY_TARGET_TIMESTAMP_DEFAULT),
    m_years(0),
    m_days(0),
    m_daysInWeek(0),
    m_weeks(0),
    m_hours(0),
    m_minutes(0),
    m_seconds(0),
    m_mediaManager(mediaManager),
    m_timer(this),
    m_finished(false)
{
    loadProperties();
    startTimer();
}

QString CountdownTimer::name() const {
    return m_name;
}

void CountdownTimer::setName(const QString &name){
    if (m_name == name) {
        return;
    }
    m_name = name;
    emit nameChanged();
}

bool CountdownTimer::enabled() const {
    return m_enabled;
}

void CountdownTimer::setEnabled(const bool &enabled) {
    if (m_enabled == enabled) {
        return;
    }
    saveProperty(PROPERTY_ENABLED_KEY, enabled);
    m_enabled = enabled;
    emit enabledChanged();
}

bool CountdownTimer::isInitialized() const {
    return m_initialized;
}

void CountdownTimer::setInitialized(const bool &initialized) {
    if (m_initialized == initialized) {
        return;
    }
    saveProperty(PROPERTY_INITIALIZED_KEY, initialized);
    m_initialized = initialized;
    emit initializedChanged();
}

qreal CountdownTimer::backgroundOpacity() const {
    return m_backgroundOpacity;
}

void CountdownTimer::setBackgroundOpacity(const qreal &backgroundOpacity) {
    if (m_backgroundOpacity == backgroundOpacity) {
        return;
    }
    saveProperty(PROPERTY_BACKGROUND_OPACITY_KEY, backgroundOpacity);
    m_backgroundOpacity = backgroundOpacity;
    emit backgroundOpacityChanged();
}

QString CountdownTimer::background() const {
    return m_mediaManager.getMediaPath(m_background);
}
void CountdownTimer::setBackground(const QString &background) {
    if (m_background == background) {
        return;
    }
    saveProperty(PROPERTY_BACKGROUND_KEY, background);
    m_background = background;
    emit backgroundChanged();
}

quint64 CountdownTimer::targetTimestamp() const {
    return m_targetTimestamp;
}

void CountdownTimer::setTargetTimestamp(const quint64 &timestamp) {
    if (m_targetTimestamp == timestamp) {
        return;
    }
    saveProperty(PROPERTY_TARGET_TIMESTAMP_KEY, timestamp);
    m_targetTimestamp = timestamp;
    emit targetTimestampChanged();
    calculateTimeRemaining();
}

void CountdownTimer::startTimer() {
    calculateTimeRemaining();
    if (!m_timer.isActive()) {
        connect(&m_timer, &QTimer::timeout, this, &CountdownTimer::calculateTimeRemaining);
        m_timer.start(1000); // Update every second
    }
}

void CountdownTimer::stopTimer() {
    if (m_timer.isActive()) {
        disconnect(&m_timer, &QTimer::timeout, this, &CountdownTimer::calculateTimeRemaining);
        m_timer.stop();
    }
}

void CountdownTimer::loadProperties() {
    static QSettings settings;
    settings.beginGroup(m_name);
    m_enabled = settings.value(PROPERTY_ENABLED_KEY, PROPERTY_ENABLED_DEFAULT).toBool();
    m_initialized = settings.value(PROPERTY_INITIALIZED_KEY, PROPERTY_INITIALIZED_DEFAULT).toBool();
    m_background = settings.value(PROPERTY_BACKGROUND_KEY, PROPERTY_BACKGROUND_DEFAULT).toString();
    m_backgroundOpacity = settings.value(PROPERTY_BACKGROUND_OPACITY_KEY, PROPERTY_BACKGROUND_OPACITY_DEFAULT).toReal();
    m_targetTimestamp = settings.value(PROPERTY_TARGET_TIMESTAMP_KEY, PROPERTY_TARGET_TIMESTAMP_DEFAULT).toULongLong();
    loadAdditionalProperties();
    settings.endGroup();
}

void CountdownTimer::loadAdditionalProperties() {
    // For subclasses to implement.
}

void CountdownTimer::saveProperty(const QString& key, const QVariant& value) {
    static QSettings settings;
    settings.beginGroup(m_name);
    settings.setValue(key, value);
    settings.endGroup();
}

void CountdownTimer::calculateTimeRemaining() {
    QDateTime now = QDateTime::currentDateTimeUtc();
    QDateTime targetDate = QDateTime::fromSecsSinceEpoch(m_targetTimestamp);
    qint64 diffSeconds = targetDate.toSecsSinceEpoch() - now.toSecsSinceEpoch();
    if (diffSeconds <= 0) {
        if (!m_finished) {
            setYears(0); setDays(0); setDaysInWeek(0); setWeeks(0); setHours(0); setMinutes(0); setSeconds(0);
            emit countdownFinished();
            m_finished = true;
        }
        emit timeChanged();
        return;
    }
    m_finished = false;
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

quint64 CountdownTimer::years() const {
    return m_years;
}

void CountdownTimer::setYears(const quint64 years) {
    if (m_years != years) {
        m_years = years;
        emit yearsChanged(m_years);
    }
}

quint64 CountdownTimer::days() const {
    return m_days;
}

void CountdownTimer::setDays(const quint64 days) {
    if (m_days != days) {
        m_days = days;
        emit daysChanged(m_days);
    }
}

quint64 CountdownTimer::daysInWeek() const {
    return m_daysInWeek;
}

void CountdownTimer::setDaysInWeek(const quint64 daysInWeek) {
    if (m_daysInWeek != daysInWeek) {
        m_daysInWeek = daysInWeek;
        emit daysInWeekChanged(m_daysInWeek);
    }
}

quint64 CountdownTimer::weeks() const {
    return m_weeks;
}

void CountdownTimer::setWeeks(const quint64 weeks) {
    if (m_weeks != weeks) {
        m_weeks = weeks;
        emit weeksChanged(m_weeks);
    }
}

quint64 CountdownTimer::hours() const {
    return m_hours;
}

void CountdownTimer::setHours(const quint64 hours) {
    if (m_hours != hours) {
        m_hours = hours;
        emit hoursChanged(m_hours);
    }
}

quint64 CountdownTimer::minutes() const {
    return m_minutes;
}

void CountdownTimer::setMinutes(const quint64 minutes) {
    if (m_minutes != minutes) {
        m_minutes = minutes;
        emit minutesChanged(m_minutes);
    }
}

quint64 CountdownTimer::seconds() const {
    return m_seconds;
}

void CountdownTimer::setSeconds(const quint64 seconds) {
    if (m_seconds != seconds) {
        m_seconds = seconds;
        emit secondsChanged(m_seconds);
    }
}
