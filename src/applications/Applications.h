#include <QObject>

#include "applications/Clock.h"
#include "applications/MarriedTimer.h"
#include "applications/BirthdayTimer.h"

class Services;

class Applications : public QObject
{
	Q_OBJECT
    Q_PROPERTY(Clock* clock MEMBER m_clock CONSTANT)
    Q_PROPERTY(MarriedTimer* marriedTimer MEMBER m_marriedTimer CONSTANT)
    Q_PROPERTY(BirthdayTimer* kuikenTimer MEMBER m_kuikenTimer CONSTANT)

public:
	Applications(Services& services, QObject *parent = nullptr);

private:
    Clock* m_clock;
    MarriedTimer* m_marriedTimer;
    BirthdayTimer* m_kuikenTimer;
};