#include "pch.h"
#include "timer.h"
#include "stats.h"
#include <wchar.h>

void timer::start_timer()
{
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
}

void timer::time()
{
	static long long unsigned count = 0;
	count++;
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	wchar_t text_buffer[200] = { 0 }; //temporary buffer
	swprintf(text_buffer, _countof(text_buffer), L"Microseconds: %lld\n", ElapsedMicroseconds.QuadPart, 1.0 / 60.0 * 1000000.0, count); // convert
	OutputDebugString(text_buffer); // print

	StartingTime = EndingTime;

}
void timer::time(long long unsigned count)
{

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	wchar_t text_buffer[200] = { 0 }; //temporary buffer
	swprintf(text_buffer, _countof(text_buffer), L"Microseconds: %lld. Count: %llu. Counts per s: %f\n", ElapsedMicroseconds.QuadPart, count, (double)count / (double)(ElapsedMicroseconds.QuadPart / 1000000.0)); // convert
	OutputDebugString(text_buffer); // print

	StartingTime = EndingTime;

}

void timer::timeCumulative(long long unsigned increment)
{
	static long long unsigned cumulative = 0;
	static long long unsigned count = 0;
	static long long unsigned cumulativeTime = 0;
	cumulative += increment;
	count++;
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	cumulativeTime += ElapsedMicroseconds.QuadPart;

	wchar_t text_buffer[200] = { 0 }; //temporary buffer
	swprintf(text_buffer, _countof(text_buffer), L"Microseconds: %lld. Count: %llu. Counts per second: %f\n", ElapsedMicroseconds.QuadPart, count, (double)cumulative / (double)(cumulativeTime / 1000000.0)); // convert
	OutputDebugString(text_buffer); // print

	StartingTime = EndingTime;

}

void timer::timeCumulativeReseting(long long unsigned increment)
{
	static long long unsigned cumulative = 0;
	static long long unsigned count = 0;
	static long long unsigned cumulativeTime = 0;

	if (cumulativeTime / 1000000.0 > 10)
	{
		cumulative = 0;
		count = 0;
		cumulativeTime = 0;
	}

	cumulative += increment;
	count++;
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	cumulativeTime += ElapsedMicroseconds.QuadPart;

	wchar_t text_buffer[200] = { 0 }; //temporary buffer
	swprintf(text_buffer, _countof(text_buffer), L"Microseconds: %lld. Count: %llu. Counts per second: %f\n", ElapsedMicroseconds.QuadPart, count, (double)cumulative / (double)(cumulativeTime / 1000000.0)); // convert
	OutputDebugString(text_buffer); // print

	StartingTime = EndingTime;

}

void timer::timeAverage()
{
	static RunningStats stat;
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	stat.Push(ElapsedMicroseconds.QuadPart);
	static int count = 0;
	if (count < 3)
	{
		stat.Clear();
		count++;
	}

	wchar_t text_buffer[200] = { 0 }; //temporary buffer
	swprintf(text_buffer, _countof(text_buffer), L"Microseconds: %lld. Average: %.3Lf\n", ElapsedMicroseconds.QuadPart, stat.Mean(), 1.0 / 60.0 * 1000000.0); // convert
	OutputDebugString(text_buffer); // print

	StartingTime = EndingTime;

}

void timer::timeAverageReseting()
{
	static RunningStats stat;
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	stat.Push(ElapsedMicroseconds.QuadPart);
	static int count = 0;
	if (count == 100)
	{
		stat.Clear();
		count = 0;
	}

	count++;

	wchar_t text_buffer[200] = { 0 }; //temporary buffer
	swprintf(text_buffer, _countof(text_buffer), L"Microseconds: %lld. Average: %.3Lf\n", ElapsedMicroseconds.QuadPart, stat.Mean(), 1.0 / 60.0 * 1000000.0); // convert
	OutputDebugString(text_buffer); // print

	StartingTime = EndingTime;

}

