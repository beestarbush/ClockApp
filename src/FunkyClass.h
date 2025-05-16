#include <QObject>

class FunkyClass : public QObject
{
	Q_OBJECT
public:
	FunkyClass(QObject *parent = nullptr);
    ~FunkyClass() override = default;

	Q_INVOKABLE QString getGreeting() const;
};