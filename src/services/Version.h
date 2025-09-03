#ifndef SERVICES_VERSION_H
#define SERVICES_VERSION_H

#include <QObject>

class Version : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString tag READ tag CONSTANT)
	Q_PROPERTY(QString commitHash READ commitHash CONSTANT)
	Q_PROPERTY(QString shortCommitHash READ shortCommitHash CONSTANT)
	Q_PROPERTY(QString dirty READ dirty CONSTANT)

public:
	Version(QObject *parent = nullptr);

	QString tag() const;
	QString commitHash() const;
	QString shortCommitHash() const;
	QString dirty() const;
};

#endif // SERVICES_VERSION_H