#include "Applications.h"
#include "services/Services.h"

const QString CLOCK_NAME = QStringLiteral("clock");
const QString MARRIED_TIMER_NAME = QStringLiteral("married-timer");
const QString KUIKEN_TIMER_NAME = QStringLiteral("kuiken-timer");
const QString COUNTDOWN_TIMER_NAME = QStringLiteral("countdown-timer");

Applications::Applications(Services& services, QObject* parent)
    : QObject(parent),
      m_clock(new Clock::Application(CLOCK_NAME, *services.m_mediaManager, this)),
      m_marriedTimer(new TimeElapsed::Application(MARRIED_TIMER_NAME, *services.m_mediaManager, this)),
      m_kuikenTimer(new TimeElapsed::Application(KUIKEN_TIMER_NAME, *services.m_mediaManager, this)),
      m_countdownTimer(new Countdown::Application(COUNTDOWN_TIMER_NAME, *services.m_mediaManager, this)),
      m_setup(new Setup::Application(*m_marriedTimer, *m_kuikenTimer, *m_countdownTimer, *services.m_remoteApi, this)),
      m_debug(new Debug::Application(this)),
      m_menu(new Menu::Application(*m_kuikenTimer, *m_clock, *m_countdownTimer, *m_marriedTimer, this))
{
    auto lNotificationManager = services.m_notificationManager;
    lNotificationManager->showInfo("System started", "The system is ready to use.", false);
}
