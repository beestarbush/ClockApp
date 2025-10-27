#include <QObject>

#include "applications/BirthdayTimer.h"
#include "applications/Clock.h"
#include "applications/CountdownTimer.h"
#include "applications/MarriedTimer.h"
#include "applications/Setup.h"

class Services;

class Applications : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Setup* setup MEMBER m_setup CONSTANT)
    Q_PROPERTY(Clock* clock MEMBER m_clock CONSTANT)
    Q_PROPERTY(MarriedTimer* marriedTimer MEMBER m_marriedTimer CONSTANT)
    Q_PROPERTY(BirthdayTimer* kuikenTimer MEMBER m_kuikenTimer CONSTANT)
    Q_PROPERTY(CountdownTimer* christmasTimer MEMBER m_christmasTimer CONSTANT)

  public:
    Applications(Services& services, QObject* parent = nullptr);

  private:
    Clock* m_clock;
    MarriedTimer* m_marriedTimer;
    BirthdayTimer* m_kuikenTimer;
    CountdownTimer* m_christmasTimer;
    Setup* m_setup;
};