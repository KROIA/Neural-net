/*
 
Autor 	Alex Krieg
Datum  	27.10.2017
Version 1.1

*/



#ifndef TIMER
#define TIMER
#include <ctime>
#include <stdio.h>

#define STOP 0
#define START 1

class Timer
{
	public:
	Timer();
	~Timer();
	bool start(unsigned long intervallMS);
	unsigned long getRuntime();
	void stop();
	unsigned long stopwatch(int status);
	void update();
	private:
	clock_t startTime;
	clock_t runningTime;
	bool isRunning;
	
	clock_t startTime_Stopwatch;
	clock_t runningTime_Stopwatch;
	bool isRunning_stopwatch;
};
#endif

