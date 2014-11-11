#include "util/stop_watch.h"

StopWatch::StopWatch(void)
    : m_startTime(), m_checkTimeContainer()
{
}

StopWatch::~StopWatch(void)
{
}

void StopWatch::Start()
{ 
	Reset();
}

void StopWatch::Reset()
{
	m_startTime = TimeUnit::now();
	m_checkTimeContainer.clear();
}

void StopWatch::Check()
{
	m_checkTimeContainer.push_back(TimeUnit::now());
}

const std::chrono::seconds StopWatch::GetElapsedSec()
{
	return std::chrono::duration_cast< std::chrono::seconds >(TimeUnit::now() - m_startTime);
}
const std::chrono::milliseconds StopWatch::GetElapsedmilliSec()
{
	return std::chrono::duration_cast< std::chrono::milliseconds >(TimeUnit::now() - m_startTime);
}
const std::chrono::microseconds StopWatch::GetElapsedmicroSec()
{
	return std::chrono::duration_cast< std::chrono::microseconds >(TimeUnit::now() - m_startTime);
}

const StopWatch::TimeUnit::rep StopWatch::GetElapsedtime()
{
	return (TimeUnit::now() - m_startTime).count();
}