#include "Applications.h"
#include "services/Services.h"

Applications::Applications(Services& services, QObject* parent)
    : QObject(parent),
      m_clock(new Clock(*services.m_mediaManager, this)),
      m_marriedTimer(new MarriedTimer(*services.m_mediaManager, this)),
      m_kuikenTimer(new BirthdayTimer("kuiken-timer", *services.m_mediaManager, this)),
      m_countdownTimer(new CountdownTimer("countdown-timer", *services.m_mediaManager, this)),
      m_setup(new Setup(*m_marriedTimer, *m_kuikenTimer, *m_countdownTimer, *services.m_remoteApi, this)),
      m_debug(new Debug(this)),
      m_menu(new Menu(*m_kuikenTimer, *m_clock, *m_countdownTimer, *m_marriedTimer, this))
{
    auto lNotificationManager = services.m_notificationManager;
    lNotificationManager->showInfo("System started", "The system is ready to use.", false);
}
