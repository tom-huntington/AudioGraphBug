#pragma once
#include <Windows.h>

class timer
{
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

public:
	void start_timer();

	void time();
	void time(long long unsigned count);

	void timeCumulative(long long unsigned count);
	void timeCumulativeReseting(long long unsigned count);

	void timeAverage();
	void timeAverageReseting();

};