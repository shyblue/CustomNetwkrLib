#pragma once

#ifndef _COMMONPOOL_H_
#define _COMMONPOOL_H_	1 

#include <boost/noncopyable.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <tbb/spin_mutex.h>
#include <atomic>


template <
	typename type_of_buffer, size_t size_default = 256, size_t size_next = 32,
	typename allocator = boost::default_user_allocator_new_delete,
	typename mutex_pool = boost::details::pool::default_mutex >
class Allocator : private boost::noncopyable
{
public:
	struct pooltag {};
	struct pooltag_4 {};
	struct pooltag_8 {};
	struct pooltag_12 {};
	struct pooltag_16 {};
	struct pooltag_24 {};
	struct pooltag_32 {};
	struct pooltag_128 {};

	typedef boost::singleton_pool<pooltag, size_default, allocator, mutex_pool, size_next>				SINGLETOP_POOL;
	typedef boost::singleton_pool<pooltag_4, size_default * 4, allocator, mutex_pool, size_next>		SINGLETOP_POOL_4;
	typedef boost::singleton_pool<pooltag_8, size_default * 8, allocator, mutex_pool, size_next>		SINGLETOP_POOL_8;
	typedef boost::singleton_pool<pooltag_12, size_default * 12, allocator, mutex_pool, size_next>		SINGLETOP_POOL_12;
	typedef boost::singleton_pool<pooltag_16, size_default * 16, allocator, mutex_pool, size_next>		SINGLETOP_POOL_16;
	typedef boost::singleton_pool<pooltag_24, size_default * 24, allocator, mutex_pool, size_next>		SINGLETOP_POOL_24;
	typedef boost::singleton_pool<pooltag_32, size_default * 32, allocator, mutex_pool, size_next>		SINGLETOP_POOL_32;
	typedef boost::singleton_pool<pooltag_128, size_default * 128, allocator, mutex_pool, size_next>	SINGLETOP_POOL_128;

	static type_of_buffer* New( size_t size_of_request = size_default)
	{
		if(size_of_request <= size_default)
		{
			return (type_of_buffer*)SINGLETOP_POOL::malloc();
		}
		else if(size_of_request <= size_default * 4)
		{
			return (type_of_buffer*)SINGLETOP_POOL_4::malloc();
		}
		else if(size_of_request <= size_default * 8)
		{
			return (type_of_buffer*)SINGLETOP_POOL_8::malloc();
		}
		else if(size_of_request <= size_default * 12)
		{
			return (type_of_buffer*)SINGLETOP_POOL_12::malloc();
		}
		else if(size_of_request <= size_default * 16)
		{
			return (type_of_buffer*)SINGLETOP_POOL_16::malloc();
		}
		else if(size_of_request <= size_default * 24)
		{
			return (type_of_buffer*)SINGLETOP_POOL_24::malloc();
		}
		else if(size_of_request <= size_default * 32)
		{
			return (type_of_buffer*)SINGLETOP_POOL_32::malloc();
		}
		else if(size_of_request <= size_default * 128)
		{
			return (type_of_buffer*)SINGLETOP_POOL_128::malloc();
		}
		else
		{
			return (type_of_buffer*)SINGLETOP_POOL_128::ordered_malloc( (size_of_request/(size_default * 128))+1);
		}

		return 0;
	}

	static bool Delete(type_of_buffer* buffer, size_t responsesize = size_default)
	{
		if(responsesize <= size_default)
		{
			SINGLETOP_POOL::free(buffer);
		}
		else if(responsesize <= size_default * 4)
		{
			SINGLETOP_POOL_4::free(buffer);
		}
		else if(responsesize <= size_default * 8)
		{
			SINGLETOP_POOL_8::free(buffer);
		}
		else if(responsesize <= size_default * 12)
		{
			SINGLETOP_POOL_12::free(buffer);
		}
		else if(responsesize <= size_default * 16)
		{
			SINGLETOP_POOL_16::free(buffer);
		}
		else if(responsesize <= size_default * 24)
		{
			SINGLETOP_POOL_24::free(buffer);
		}
		else if(responsesize <= size_default * 32)
		{
			SINGLETOP_POOL_32::free(buffer);
		}
		else if(responsesize <= size_default * 128)
		{
			SINGLETOP_POOL_128::free(buffer);
		}
		else
		{
			SINGLETOP_POOL_128::ordered_free(buffer, (responsesize/size_default * 128)+1);
		}
				
		return true;
	}

	static bool PurgeMemory()
	{
		SINGLETOP_POOL::purge_memory();
		SINGLETOP_POOL_4::purge_memory();
		SINGLETOP_POOL_8::purge_memory();
		SINGLETOP_POOL_12::purge_memory();
		SINGLETOP_POOL_16::purge_memory();
		SINGLETOP_POOL_24::purge_memory();
		SINGLETOP_POOL_32::purge_memory();
		SINGLETOP_POOL_128::purge_memory();

		return true;
	}

private :
	Allocator(void);
	~Allocator(void);

	
};

#define COMMON_POOL Allocator<char>

#endif // _COMMONPOOL_H_

