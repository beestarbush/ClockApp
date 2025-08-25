#include <QObject>
#include <QProcess>

class System : public QObject
{
	Q_OBJECT

public:
	System(QObject *parent = nullptr);

	Q_INVOKABLE void shutdown();
	Q_INVOKABLE void reboot();

private:
	void onShutdownFinished();
	void onRebootFinished();

	QProcess m_shutdownProcess;
	QProcess m_rebootProcess;

};