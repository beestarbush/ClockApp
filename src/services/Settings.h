#ifndef SERVICES_SETTINGS_H
#define SERVICES_SETTINGS_H

#include <QObject>

class Settings : public QObject
{
	Q_OBJECT

public:
	Settings(QObject *parent = nullptr);
	
};

#endif // SERVICES_SETTINGS_H