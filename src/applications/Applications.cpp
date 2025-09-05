#include "Applications.h"
#include "services/Services.h"

Applications::Applications(Services &services, QObject *parent) :
    QObject(parent),
    m_clock(new Clock(*services.m_animationManager, this)),
    m_marriedTimer(new MarriedTimer(*services.m_animationManager, this)),
    m_kuikenTimer(new BirthdayTimer("kuiken-timer", *services.m_animationManager, this))
{
    m_kuikenTimer->setBackgroundAnimation("kuiken.gif");
    m_kuikenTimer->setBackgroundOpacity(0.5);
    m_kuikenTimer->setTimestamp(1738195200);

    m_marriedTimer->setBackgroundAnimation(QStringLiteral("married_bw.gif"));
    m_marriedTimer->setBackgroundOpacity(0.3);
    m_marriedTimer->setTimestamp(1730382722);
}
