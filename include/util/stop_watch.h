#pragma once

#ifndef UTILITY_TIMER_H_
#define UTILITY_TIMER_H_

#include <vector>
#include <chrono>

class StopWatch
{
public:
	typedef std::chrono::high_resolution_clock TimeUnit;
	typedef std::vector<TimeUnit::time_point> TimeContainer;

	StopWatch(void);
	~StopWatch(void);

	void Start();
	void Reset();
	void Check();

	const std::chrono::seconds GetElapsedSec();
	const std::chrono::milliseconds GetElapsedmilliSec();
	const std::chrono::microseconds GetElapsedmicroSec();
	const TimeUnit::rep GetElapsedtime();

	
private:
	TimeUnit::time_point m_startTime;
	TimeContainer		 m_checkTimeContainer;
};

#endif // UTILITY_TIMER_H_