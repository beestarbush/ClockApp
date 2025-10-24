#ifndef APPS_BIRTHDAY_TIMER_H
#define APPS_BIRTHDAY_TIMER_H

#include "TimeElapsedTimer.h"
#include <QColor>

class BirthdayTimer : public TimeElapsedTimer
{
	Q_OBJECT
	Q_PROPERTY(QColor barColor READ barColor WRITE setBarColor NOTIFY barColorChanged)
	Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)

public:
	BirthdayTimer(const QString& name, MediaManager& mediaManager, QObject *parent = nullptr);

	QColor barColor() const;
	void setBarColor(const QColor &barColor);

	QColor textColor() const;
	void setTextColor(const QColor &textColor);

signals:
	void barColorChanged();
	void textColorChanged();

private:
	void loadAdditionalProperties() override;

	QColor m_barColor;
	QColor m_textColor;
};

#endif // APPS_BIRTHDAY_TIMER_H