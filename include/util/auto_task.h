#pragma once

#ifndef _UTILITY_AUTOTASK_IPML_H_
#define _UTILITY_AUTOTASK_IPML_H_

/// @class AutoTask
/// @brief 실시간으로 등록된 작업을 처리하는 처리기 클래스
#include <boost/thread.hpp>
#include <tbb/concurrent_queue.h>

template <typename T >
class AutoTask
{
public:
    typedef tbb::concurrent_bounded_queue<T> TaskContainer;

    AutoTask(void);
    virtual ~AutoTask(void);

    // Processing을 실행할 객체 데이터를 넣는다.
    void				Insert(T& object);

    // Task 클래스를 실행시킨다.
    void				Run();
    // Task 클래스를 종료한다.
    void				Stop();
protected:
    // 상속받은 객체에서 해당 오브젝트에 실행할 작업 내용을 정의한다.
    virtual void		Processing(T& object) = 0;
    virtual void		AutoTaskRun();
private:

    TaskContainer  m_taskContainer;
    boost::thread* m_thread;
};

template <typename T >
AutoTask<T>::AutoTask(void)
{
    Run();
}

template <typename T >
AutoTask<T>::~AutoTask(void)
{
    Stop();
}

template <typename T >
void AutoTask<T>::Stop()
{
    m_thread->interrupt();
    m_thread->join();
}

template <typename T >
void AutoTask<T>::Run()
{
    m_thread = new boost::thread( boost::bind( &AutoTask<T>::AutoTaskRun, this ) );
}

template <typename T >
void AutoTask<T>::AutoTaskRun()
{
    try
    {
        T object;
        if(m_taskContainer.try_pop(object))
            Processing(object);
    }
    catch(boost::thread_interrupted &)
    {
    }
}

template <typename T >
void AutoTask<T>::Insert(T& object)
{
    m_taskContainer.push(object);
}

#endif // _UTILITY_AUTOTASK_IPML_H_