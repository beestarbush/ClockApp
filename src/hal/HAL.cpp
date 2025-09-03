#include "HAL.h"

HAL::HAL(QObject *parent) :
    QObject(parent),
    m_network(new Network(this)),
    m_screen(new Screen(this)),
    m_system(new System(this)),
    m_temperature(new Temperature(this))
{
}
