#pragma once
#ifndef _UTILITY_REPEATER_TASK_H_
#define _UTILITY_REPEATER_TASK_H_

#include <boost/thread.hpp>
#include "logger.h"

template<typename T>
class RepeateTask
{
public:
	RepeateTask();
	RepeateTask(T& object, const int32_t millisec);
	virtual ~RepeateTask();

	void Run();
	void Stop();
protected:

	virtual void	Processing() = 0;
	virtual void	Reset_Timetick(int32_t repeate_timetick) { m_tickRepeate = repeate_timetick; }
	T*				m_pObject;
	int32_t			m_tickRepeate;
private:
	void RepeateTaskRun();
	boost::thread*	m_pThread;
};

template<typename T>
RepeateTask<T>::RepeateTask()
{
}

template<typename T>
RepeateTask<T>::RepeateTask(T& object, const int32_t millisec) :
	m_pObject(&object),
	m_tickRepeate(millisec),
	m_pThread(0)
{
}

template<typename T>
RepeateTask<T>::~RepeateTask()
{
}

template <typename T >
void RepeateTask<T>::Run()
{
	m_pThread = new boost::thread( boost::bind( &RepeateTask<T>::RepeateTaskRun, this ) );
}

template <typename T >
void RepeateTask<T>::Stop()
{
	m_pThread->interrupt();
	m_pThread->join();
}

template <typename T >
void RepeateTask<T>::RepeateTaskRun()
{
	try
	{
		for(;;)
		{
			boost::this_thread::sleep(boost::posix_time::millisec(m_tickRepeate));
			Processing();
		}
	}
	catch(boost::thread_interrupted &)
	{
		ST_LOGGER.Info(L"[RepeateTask][RepeateTaskRun] Interrupt");
	}
}

#endif // _UTILITY_REPEATER_TASK_H_