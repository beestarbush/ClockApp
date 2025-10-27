#include "MarriedTimer.h"

MarriedTimer::MarriedTimer(MediaManager& mediaManager, QObject* parent)
    : TimeElapsedTimer(QStringLiteral("married-timer"), mediaManager, parent)
{
}
