#include "CPUTimer.h"

#include <stdio.h>
#include <stdlib.h>

CPUTimer::CPUTimer()
{
	started = false;
	CPUTotalSeconds = 0;
} 

void CPUTimer::reset()
{
	started = false;
	CPUTotalSeconds = 0;
}

void CPUTimer::start()
{
	started = true;
	CPUTimerStartedAt = clock();
}

void CPUTimer::stop()
{
	if (started)
	{
		CPUTimerStopedAt = clock();
		CPUTotalSeconds += CPUTimerStopedAt - CPUTimerStartedAt;
	}
	else
	{
		printf("CPUTimer::stop() must be called after calling CPUTimer::start()!\n");
		exit(0);
	}
	started = false;
}

double CPUTimer::getCPUTotalSecs()
{
	return CPUTotalSeconds / CLOCKS_PER_SEC;
}
