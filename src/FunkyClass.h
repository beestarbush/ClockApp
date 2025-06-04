#include <QObject>

class FunkyClass : public QObject
{
	Q_OBJECT
	Q_PROPERTY(quint32 rotationDegrees READ rotationDegrees WRITE setRotationDegrees NOTIFY rotationDegreesChanged)

public:
	FunkyClass(QObject *parent = nullptr);
	~FunkyClass() override = default;

	Q_INVOKABLE QString getGreeting() const;

	quint32 rotationDegrees() const;
	void setRotationDegrees(quint32 degrees);

signals:
	void rotationDegreesChanged(quint32 degrees);

private:
	quint32 m_rotationDegrees = 0;
};