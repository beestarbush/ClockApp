#include <QObject>

#include "network/Driver.h"
#include "screen/Driver.h"
#include "storage/Driver.h"
#include "system/Driver.h"
#include "temperature/Driver.h"

class HAL : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Network::Driver* network MEMBER m_network CONSTANT)
    Q_PROPERTY(Screen::Driver* screen MEMBER m_screen CONSTANT)
    Q_PROPERTY(System::Driver* system MEMBER m_system CONSTANT)
    Q_PROPERTY(Temperature::Driver* temperature MEMBER m_temperature CONSTANT)
    Q_PROPERTY(Storage::Driver* storage MEMBER m_storage CONSTANT)

  public:
    friend class Services;

    HAL(QObject* parent = nullptr);

  private:
    Storage::Driver* m_storage;
    Network::Driver* m_network;
    Screen::Driver* m_screen;
    System::Driver* m_system;
    Temperature::Driver* m_temperature;
};