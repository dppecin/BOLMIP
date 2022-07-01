#include <ctime>

class CPUTimer
{
	double CPUTimerStartedAt;
	double CPUTimerStopedAt;
	double CPUTotalSeconds;
	bool started;

public:

	CPUTimer();
	
	void reset();
	void start();
	void stop();

	double getCPUTotalSecs();
};