#include <QObject>
#include <QColor>

class Settings : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString clockBackgroundAnimation READ clockBackgroundAnimation WRITE setClockBackgroundAnimation NOTIFY clockBackgroundAnimationChanged)
	Q_PROPERTY(QColor clockHourColor READ clockHourColor WRITE setClockHourColor NOTIFY clockHourColorChanged)
	Q_PROPERTY(QColor clockMinuteColor READ clockMinuteColor WRITE setClockMinuteColor NOTIFY clockMinuteColorChanged)
	Q_PROPERTY(QColor clockSecondColor READ clockSecondColor WRITE setClockSecondColor NOTIFY clockSecondColorChanged)
	Q_PROPERTY(QColor clockPendulumColor READ clockPendulumColor WRITE setClockPendulumColor NOTIFY clockPendulumColorChanged)
	Q_PROPERTY(quint64 marriedTimerTimestamp READ marriedTimerTimestamp WRITE setMarriedTimerTimestamp NOTIFY marriedTimerTimestampChanged)
	Q_PROPERTY(bool marriedTimerEnabled READ marriedTimerEnabled WRITE setMarriedTimerEnabled NOTIFY marriedTimerEnabledChanged)
	Q_PROPERTY(qreal marriedTimerBackgroundOpacity READ marriedTimerBackgroundOpacity WRITE setMarriedTimerBackgroundOpacity NOTIFY marriedTimerBackgroundOpacityChanged)
	Q_PROPERTY(QString marriedTimerBackgroundAnimation READ marriedTimerBackgroundAnimation WRITE setMarriedTimerBackgroundAnimation NOTIFY marriedTimerBackgroundAnimationChanged)
	Q_PROPERTY(quint64 kuikenTimerTimestamp READ kuikenTimerTimestamp WRITE setKuikenTimerTimestamp NOTIFY kuikenTimerTimestampChanged)
	Q_PROPERTY(bool kuikenTimerEnabled READ kuikenTimerEnabled WRITE setKuikenTimerEnabled NOTIFY kuikenTimerEnabledChanged)
	Q_PROPERTY(qreal kuikenTimerBackgroundOpacity READ kuikenTimerBackgroundOpacity WRITE setKuikenTimerBackgroundOpacity NOTIFY kuikenTimerBackgroundOpacityChanged)
	Q_PROPERTY(QString kuikenTimerBackgroundAnimation READ kuikenTimerBackgroundAnimation WRITE setKuikenTimerBackgroundAnimation NOTIFY kuikenTimerBackgroundAnimationChanged)

public:
	Settings(QObject *parent = nullptr);

	void setClockBackgroundAnimation(const QString &animation);
	QString clockBackgroundAnimation() const;

	void setClockHourColor(const QColor &color);
	QColor clockHourColor() const;
	void setClockMinuteColor(const QColor &color);
	QColor clockMinuteColor() const;
	void setClockSecondColor(const QColor &color);
	QColor clockSecondColor() const;
	void setClockPendulumColor(const QColor &color);
	QColor clockPendulumColor() const;

	void setMarriedTimerTimestamp(quint64 timestamp);
	quint64 marriedTimerTimestamp() const;
	void setMarriedTimerEnabled(bool enabled);
	bool marriedTimerEnabled() const;
	void setMarriedTimerBackgroundOpacity(qreal opacity);
	qreal marriedTimerBackgroundOpacity() const;
	void setMarriedTimerBackgroundAnimation(const QString &animation);
	QString marriedTimerBackgroundAnimation() const;

	void setKuikenTimerTimestamp(quint64 timestamp);
	quint64 kuikenTimerTimestamp() const;
	void setKuikenTimerEnabled(bool enabled);
	bool kuikenTimerEnabled() const;
	void setKuikenTimerBackgroundOpacity(qreal opacity);
	qreal kuikenTimerBackgroundOpacity() const;
	void setKuikenTimerBackgroundAnimation(const QString &animation);
	QString kuikenTimerBackgroundAnimation() const;

signals:
	void clockBackgroundAnimationChanged();
	void clockHourColorChanged();
	void clockMinuteColorChanged();
	void clockSecondColorChanged();
	void clockPendulumColorChanged();

	void marriedTimerTimestampChanged();
	void marriedTimerEnabledChanged();
	void marriedTimerBackgroundOpacityChanged();
	void marriedTimerBackgroundAnimationChanged();

	void kuikenTimerTimestampChanged();
	void kuikenTimerEnabledChanged();
	void kuikenTimerBackgroundOpacityChanged();
	void kuikenTimerBackgroundAnimationChanged();

private:
	bool load();

	QString m_clockBackgroundAnimation;
	QColor m_clockHourColor;
	QColor m_clockMinuteColor;
	QColor m_clockSecondColor;
	QColor m_clockPendulumColor;

	quint64 m_marriedTimerTimestamp;
	bool m_marriedTimerEnabled;
	qreal m_marriedTimerBackgroundOpacity;
	QString m_marriedTimerBackgroundAnimation;

	quint64 m_kuikenTimerTimestamp;
	bool m_kuikenTimerEnabled;
	qreal m_kuikenTimerBackgroundOpacity;
	QString m_kuikenTimerBackgroundAnimation;
};