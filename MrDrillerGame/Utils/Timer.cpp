#include "Timer.h"

Timer::Timer(float targetTime)
	: elapsedTime(0.0f), targetTime(targetTime)
{

}

void Timer::Tick(float deltaTime)
{
	elapsedTime += deltaTime;

}


void Timer::Reset()
{
	elapsedTime = 0.0f;
}



