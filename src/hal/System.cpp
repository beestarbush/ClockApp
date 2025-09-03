#include "System.h"
#include <QDebug>

#ifdef PLATFORM_IS_TARGET
const QString SHUTDOWN_COMMAND = QStringLiteral("shutdown");
const QStringList SHUTDOWN_ARGUMENTS = {QStringLiteral("-h"), QStringLiteral("now")};
const QString REBOOT_COMMAND = QStringLiteral("reboot");
#else
const QString SHUTDOWN_COMMAND = QStringLiteral("echo");
const QStringList SHUTDOWN_ARGUMENTS = {QStringLiteral("System shutdown command not available on this platform")};
const QString REBOOT_COMMAND = QStringLiteral("reboot");
#endif

System::System(QObject *parent) :
    QObject(parent),
    m_shutdownProcess(this),
    m_rebootProcess(this)
{
}

void System::shutdown()
{
    connect(&m_shutdownProcess, &QProcess::finished, this, &System::onShutdownFinished);
    m_shutdownProcess.start(SHUTDOWN_COMMAND, SHUTDOWN_ARGUMENTS);
    qDebug() << "System shutdown initiated.";
}

void System::reboot()
{
    connect(&m_rebootProcess, &QProcess::finished, this, &System::onRebootFinished);
    m_rebootProcess.start(REBOOT_COMMAND);
    qDebug() << "System reboot initiated.";
}

void System::onShutdownFinished()
{
    disconnect(&m_shutdownProcess, &QProcess::finished, this, &System::onShutdownFinished);
    if (m_shutdownProcess.exitCode() != 0) {
        qWarning() << "Shutdown command failed:" << m_shutdownProcess.errorString();
        return;
    }
    qDebug() << "Shutdown process completed.";
}

void System::onRebootFinished()
{
    disconnect(&m_rebootProcess, &QProcess::finished, this, &System::onRebootFinished);
    if (m_rebootProcess.exitCode() != 0) {
        qWarning() << "Reboot command failed:" << m_rebootProcess.errorString();
        return;
    }
    qDebug() << "Reboot process completed.";
}
