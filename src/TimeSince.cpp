#include "TimeSince.h"
#include <QDateTime>
#include <QDebug>

constexpr quint64 SECONDS_IN_MINUTE = 60;
constexpr quint64 SECONDS_IN_HOUR = SECONDS_IN_MINUTE * 60;
constexpr quint64 SECONDS_IN_A_DAY = SECONDS_IN_HOUR * 24;
constexpr quint64 DAYS_IN_A_WEEK = 7;
constexpr quint64 DAYS_IN_YEAR = 365;

TimeSince::TimeSince(quint64 epochUtcTimestamp, QObject *parent) :
    QObject(parent),
    m_epochUtcTimestamp(epochUtcTimestamp),
    m_timer(this),
    m_years(0),
    m_days(0),
    m_daysInWeek(0),
    m_weeks(0),
    m_hours(0),
    m_minutes(0),
    m_seconds(0)
{
    connect(&m_timer, &QTimer::timeout, this, &TimeSince::calculate);
    m_timer.start(1000);
}

quint64 TimeSince::years() const
{
    return m_years;
}

quint64 TimeSince::days() const
{
    return m_days;
}

quint64 TimeSince::daysInWeek() const
{
    return m_daysInWeek;
}

quint64 TimeSince::weeks() const
{
    return m_weeks;
}

quint64 TimeSince::hours() const
{
    return m_hours;
}

quint64 TimeSince::minutes() const
{
    return m_minutes;
}

quint64 TimeSince::seconds() const
{
    return m_seconds;
}

void TimeSince::setYears(const quint64 years)
{
    if (m_years != years) {
        m_years = years;
        emit yearsChanged(m_years);
    }
}

void TimeSince::setDays(const quint64 days)
{
    if (m_days != days) {
        m_days = days;
        emit daysChanged(m_days);
    }
}

void TimeSince::setDaysInWeek(const quint64 daysInWeek)
{
    if (m_daysInWeek != daysInWeek) {
        m_daysInWeek = daysInWeek;
        emit daysInWeekChanged(m_daysInWeek);
    }
}

void TimeSince::setWeeks(const quint64 weeks)
{
    if (m_weeks != weeks) {
        m_weeks = weeks;
        emit weeksChanged(m_weeks);
    }
}

void TimeSince::setHours(const quint64 hours)
{
    if (m_hours != hours) {
        m_hours = hours;
        emit hoursChanged(m_hours);
    }
}

void TimeSince::setMinutes(const quint64 minutes)
{
    if (m_minutes != minutes) {
        m_minutes = minutes;
        emit minutesChanged(m_minutes);
    }
}

void TimeSince::setSeconds(const quint64 seconds)
{
    if (m_seconds != seconds) {
        m_seconds = seconds;
        emit secondsChanged(m_seconds);
    }
}

quint64 TimeSince::calculate()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    QDateTime referenceDate = QDateTime::fromSecsSinceEpoch(m_epochUtcTimestamp);

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

    /*qDebug() << "Years:" << years
             << "Weeks:" << weeks
             << "Days:" << days
             << "Hours:" << hours
             << "Minutes:" << minutes
             << "Seconds:" << seconds;*/

    return 0; // Placeholder implementation
}
