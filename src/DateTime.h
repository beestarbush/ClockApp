#include <QObject>
#include <QTimeZone>

class DateTime : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString localTime READ localTime NOTIFY timeChanged)
	Q_PROPERTY(QString utcTime READ utcTime NOTIFY timeChanged)

public:
	DateTime(QObject *parent = nullptr);

    QString localTime() const;
	QString utcTime() const;

signals:
	void timeChanged();

private:
    QTimeZone m_timeZone;
};