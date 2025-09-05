#ifndef APPS_MARRIED_TIMER_H
#define APPS_MARRIED_TIMER_H

#include "TimeElapsedTimer.h"

class MarriedTimer : public TimeElapsedTimer
{
	Q_OBJECT

public:
	MarriedTimer(AnimationManager& animationManager, QObject *parent = nullptr);
};

#endif // APPS_MARRIED_TIMER_H