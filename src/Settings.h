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
	Q_PROPERTY(bool marriedTimerEnabled READ marriedTimerEnabled WRITE setMarriedTimerEnabled NOTIFY marriedTimerEnabledChanged)
	Q_PROPERTY(qreal marriedTimerBackgroundOpacity READ marriedTimerBackgroundOpacity WRITE setMarriedTimerBackgroundOpacity NOTIFY marriedTimerBackgroundOpacityChanged)
	Q_PROPERTY(QString marriedTimerBackgroundAnimation READ marriedTimerBackgroundAnimation WRITE setMarriedTimerBackgroundAnimation NOTIFY marriedTimerBackgroundAnimationChanged)
	Q_PROPERTY(bool kuikenTimerEnabled READ kuikenTimerEnabled WRITE setKuikenTimerEnabled NOTIFY kuikenTimerEnabledChanged)

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

	void setMarriedTimerEnabled(bool enabled);
	bool marriedTimerEnabled() const;
	void setMarriedTimerBackgroundOpacity(qreal opacity);
	qreal marriedTimerBackgroundOpacity() const;
	void setMarriedTimerBackgroundAnimation(const QString &animation);
	QString marriedTimerBackgroundAnimation() const;

	void setKuikenTimerEnabled(bool enabled);
	bool kuikenTimerEnabled() const;

signals:
	void clockBackgroundAnimationChanged();
	void clockHourColorChanged();
	void clockMinuteColorChanged();
	void clockSecondColorChanged();
	void clockPendulumColorChanged();

	void marriedTimerEnabledChanged();
	void marriedTimerBackgroundOpacityChanged();
	void marriedTimerBackgroundAnimationChanged();

	void kuikenTimerEnabledChanged();

private:
	bool load();

	QString m_clockBackgroundAnimation;
	QColor m_clockHourColor;
	QColor m_clockMinuteColor;
	QColor m_clockSecondColor;
	QColor m_clockPendulumColor;

	bool m_marriedTimerEnabled;
	qreal m_marriedTimerBackgroundOpacity;
	QString m_marriedTimerBackgroundAnimation;

	bool m_kuikenTimerEnabled;
};