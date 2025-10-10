#include <QObject>

#include "hal/Network.h"
#include "hal/Screen.h"
#include "hal/System.h"
#include "hal/Temperature.h"
#include "hal/Storage.h"

class HAL : public QObject
{
	Q_OBJECT
    Q_PROPERTY(Network* network MEMBER m_network CONSTANT)
    Q_PROPERTY(Screen* screen MEMBER m_screen CONSTANT)
    Q_PROPERTY(System* system MEMBER m_system CONSTANT)
    Q_PROPERTY(Temperature* temperature MEMBER m_temperature CONSTANT)
    Q_PROPERTY(Storage* storage MEMBER m_storage CONSTANT)

public:
	HAL(QObject *parent = nullptr);

private:
    Storage* m_storage;
    Network* m_network;
    Screen* m_screen;
    System* m_system;
    Temperature *m_temperature;
};