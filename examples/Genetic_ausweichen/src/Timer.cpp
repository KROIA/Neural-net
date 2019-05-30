/*
 
Autor 	Alex Krieg
Datum  	27.10.2017
Version 1.1

*/



#include "Timer.h"

Timer::Timer()
{
	isRunning = 0;
	isRunning_stopwatch = 0;
}
Timer::~Timer()
{
	
}
bool Timer::start(unsigned long intervallMS)
{
	if(isRunning)
	{
		runningTime = clock();
		if((runningTime - startTime) > intervallMS)
		{
			isRunning = 0;
			return 1;
		}
		
	}
	else
	{
		isRunning 	= 1;
		startTime 	= clock();
		runningTime = startTime;
	}
	return 0;
}
unsigned long Timer::getRuntime()
{
	if(isRunning)
	{
		return runningTime - startTime;
	}
	return 0;
}
void Timer::stop()
{
	isRunning = 0;
}
unsigned long Timer::stopwatch(int status)
{
	if(isRunning_stopwatch)
	{
		runningTime_Stopwatch = clock();
	}
	if(!isRunning_stopwatch && status)
	{
		isRunning_stopwatch = true;
		startTime_Stopwatch = clock();
	}
	else
	{
		isRunning_stopwatch = false;
		return runningTime_Stopwatch - startTime_Stopwatch;
	}
}
void Timer::update()
{
	if(isRunning_stopwatch)
	{
		runningTime_Stopwatch = clock();
	}
	if(isRunning)
	{
		runningTime = clock();
	}
}







