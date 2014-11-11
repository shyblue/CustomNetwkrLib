#pragma once

#ifndef __POLL_H_	
#define __POLL_H_	

#include <atomic>
#include <tbb/concurrent_queue.h>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include "common.h"

template < typename T>
class Pool : private boost::noncopyable
{
public:			
	
	typedef boost::optional<T> OptPtr;

	enum ERROR_CODE {
		ALL_OK = 0, 
		RESTORE_EMPTY, 
		RESTORE_FREE, 
	};

	Pool(size_t count) : m_isRunning(false), m_sizeFlexibleMaxCnt(count)
	{
		m_sizeUseCnt.store(0);
	}
	~Pool(void)
	{
		Release();
	}

	OptPtr Acquire()
	{
		T object;	

		if (m_queue.empty())
			return OptPtr();
		
		if(m_queue.try_pop(object))
		{
			m_sizeUseCnt.fetch_add(1);
		}
		else
		{
			return OptPtr();
		}

		return OptPtr(object);
	}

	OptPtr WaitAcquire()
	{
		T object;
		m_queue.pop(object);

		return OptPtr(object);
	}

	bool Create(T& object)
	{
		return Restore(object, true);
	}

	bool Restore(T& object, bool is_create = false)
	{
		if (object == NULLPTR ) return false; 

		if ( (int32_t)m_queue.size() < m_sizeFlexibleMaxCnt.load() )
		{
			m_queue.push(object);

			if(!is_create)
			{
				m_sizeUseCnt.fetch_sub(1);
			}

		}
		else
		{
			return false; 
		}

		return true;
	}

	const size_t GetUseCount() 
	{ 
		size_t size = 0;;
		size = m_sizeUseCnt.load();
		return size;
	}
	const size_t GetFlexibleMaxCount() 
	{ 
		size_t size = 0;;
		size = m_sizeFlexibleMaxCnt.load();
		return size;
	}

	void SetRunning(bool set) { m_isRunning = set; }
	bool GetRunning() {return m_isRunning; }

private:
	void Release()
	{
		for ( size_t i = 0; (size_t)m_queue.size(); ++i) 
		{
			T object;	
			m_queue.pop(object);
		}
	}

	tbb::concurrent_bounded_queue<T>				m_queue;
	std::atomic<size_t>								m_sizeUseCnt;
	std::atomic<size_t>								m_sizeFlexibleMaxCnt;
	bool											m_isRunning;
};

#endif // __POLL_H_