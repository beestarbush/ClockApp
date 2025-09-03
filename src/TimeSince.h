#include <QObject>
#include <QTimer>

class TimeSince : public QObject
{
	Q_OBJECT
	Q_PROPERTY(quint64 years READ years NOTIFY timeChanged)
	Q_PROPERTY(quint64 days READ days NOTIFY timeChanged)
	Q_PROPERTY(quint64 daysInWeek READ daysInWeek NOTIFY timeChanged)
	Q_PROPERTY(quint64 weeks READ weeks NOTIFY timeChanged)
	Q_PROPERTY(quint64 hours READ hours NOTIFY timeChanged)
	Q_PROPERTY(quint64 minutes READ minutes NOTIFY timeChanged)
	Q_PROPERTY(quint64 seconds READ seconds NOTIFY timeChanged)

public:
	TimeSince(quint64 epochUtcTimestamp, QObject *parent = nullptr);

	quint64 years() const;
	quint64 days() const;
	quint64 daysInWeek() const;
	quint64 weeks() const;
	quint64 hours() const;
	quint64 minutes() const;
	quint64 seconds() const;

	void setYears(const quint64 years);
	void setDays(const quint64 days);
	void setDaysInWeek(const quint64 daysInWeek);
	void setWeeks(const quint64 weeks);
	void setHours(const quint64 hours);
	void setMinutes(const quint64 minutes);
	void setSeconds(const quint64 seconds);

private:
	quint64 calculate();

	quint64 m_epochUtcTimestamp;
	QTimer m_timer;

	quint64 m_years;
	quint64 m_days;
	quint64 m_daysInWeek;
	quint64 m_weeks;
	quint64 m_hours;
	quint64 m_minutes;
	quint64 m_seconds;

signals:
	void timeChanged();
	void yearsChanged(quint64 years);
	void daysChanged(quint64 days);
	void daysInWeekChanged(quint64 daysInWeek);
	void weeksChanged(quint64 weeks);
	void hoursChanged(quint64 hours);
	void minutesChanged(quint64 minutes);
	void secondsChanged(quint64 seconds);
};