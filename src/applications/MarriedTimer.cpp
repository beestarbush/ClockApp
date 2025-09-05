#include "MarriedTimer.h"

MarriedTimer::MarriedTimer(AnimationManager& animationManager, QObject *parent) :
    TimeElapsedTimer(QStringLiteral("married-timer"), animationManager, parent)
{
}
