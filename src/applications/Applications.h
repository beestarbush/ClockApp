#include <QObject>

#include "clock/Application.h"
#include "countdown/Application.h"
#include "debug/Application.h"
#include "menu/Application.h"
#include "setup/Application.h"
#include "timeelapsed/Application.h"

class Services;

class Applications : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Setup::Application* setup MEMBER m_setup CONSTANT)
    Q_PROPERTY(Clock::Application* clock MEMBER m_clock CONSTANT)
    Q_PROPERTY(TimeElapsed::Application* marriedTimer MEMBER m_marriedTimer CONSTANT)
    Q_PROPERTY(TimeElapsed::Application* kuikenTimer MEMBER m_kuikenTimer CONSTANT)
    Q_PROPERTY(Countdown::Application* countdownTimer MEMBER m_countdownTimer CONSTANT)
    Q_PROPERTY(Debug::Application* debug MEMBER m_debug CONSTANT)
    Q_PROPERTY(Menu::Application* menu MEMBER m_menu CONSTANT)

  public:
    Applications(Services& services, QObject* parent = nullptr);

  private:
    Clock::Application* m_clock;
    TimeElapsed::Application* m_marriedTimer;
    TimeElapsed::Application* m_kuikenTimer;
    Countdown::Application* m_countdownTimer;
    Setup::Application* m_setup;
    Debug::Application* m_debug;
    Menu::Application* m_menu;
};