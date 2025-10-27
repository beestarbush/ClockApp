#ifndef APPS_TIME_ELAPSED_TIMER_H
#define APPS_TIME_ELAPSED_TIMER_H

#include <QColor>
#include <QObject>
#include <QTimer>

class MediaManager;

class TimeElapsedTimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool initialized READ isInitialized WRITE setInitialized NOTIFY initializedChanged)
    Q_PROPERTY(qreal backgroundOpacity READ backgroundOpacity WRITE setBackgroundOpacity NOTIFY backgroundOpacityChanged)
    Q_PROPERTY(QString background READ background WRITE setBackground NOTIFY backgroundChanged)
    Q_PROPERTY(quint64 timestamp READ timestamp WRITE setTimestamp NOTIFY timestampChanged)
    Q_PROPERTY(quint64 years READ years NOTIFY timeChanged)
    Q_PROPERTY(quint64 days READ days NOTIFY timeChanged)
    Q_PROPERTY(quint64 daysInWeek READ daysInWeek NOTIFY timeChanged)
    Q_PROPERTY(quint64 weeks READ weeks NOTIFY timeChanged)
    Q_PROPERTY(quint64 hours READ hours NOTIFY timeChanged)
    Q_PROPERTY(quint64 minutes READ minutes NOTIFY timeChanged)
    Q_PROPERTY(quint64 seconds READ seconds NOTIFY timeChanged)

  public:
    TimeElapsedTimer(const QString& name, MediaManager& mediaManager, QObject* parent = nullptr);
    virtual ~TimeElapsedTimer() = default;

    QString name() const;
    void setName(const QString& name);

    bool enabled() const;
    void setEnabled(const bool& enabled);

    bool isInitialized() const;
    void setInitialized(const bool& initialized);

    qreal backgroundOpacity() const;
    void setBackgroundOpacity(const qreal& backgroundOpacity);

    QString background() const;
    void setBackground(const QString& background);

    quint64 timestamp() const;
    void setTimestamp(const quint64& timestamp);

    quint64 years() const;
    quint64 days() const;
    quint64 daysInWeek() const;
    quint64 weeks() const;
    quint64 hours() const;
    quint64 minutes() const;
    quint64 seconds() const;

    void startTimer();
    void stopTimer();

  signals:
    void nameChanged();
    void enabledChanged();
    void initializedChanged();
    void backgroundOpacityChanged();
    void backgroundChanged();
    void timestampChanged();

    void timeChanged();
    void yearsChanged(quint64 years);
    void daysChanged(quint64 days);
    void daysInWeekChanged(quint64 daysInWeek);
    void weeksChanged(quint64 weeks);
    void hoursChanged(quint64 hours);
    void minutesChanged(quint64 minutes);
    void secondsChanged(quint64 seconds);

  protected:
    void loadProperties();
    virtual void loadAdditionalProperties();
    void saveProperty(const QString& key, const QVariant& value);

    void calculateTimeElapsed();
    void setYears(const quint64 years);
    void setDays(const quint64 days);
    void setDaysInWeek(const quint64 daysInWeek);
    void setWeeks(const quint64 weeks);
    void setHours(const quint64 hours);
    void setMinutes(const quint64 minutes);
    void setSeconds(const quint64 seconds);

    // Common properties
    QString m_name;
    bool m_enabled;
    bool m_initialized;
    qreal m_backgroundOpacity;
    QString m_background;
    quint64 m_timestamp;

    // Properties for indicating the elapsed time
    quint64 m_years;
    quint64 m_days;
    quint64 m_daysInWeek;
    quint64 m_weeks;
    quint64 m_hours;
    quint64 m_minutes;
    quint64 m_seconds;

    // Dependencies and internal objects
    MediaManager& m_mediaManager;
    QTimer m_timer;
};

#endif // APPS_TIME_ELAPSED_TIMER_H