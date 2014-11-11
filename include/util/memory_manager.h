#pragma once

#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_
#include <cstring>
#include <boost/serialization/singleton.hpp>

class MemoryManager
{
public:
	template <typename _T>
	static bool ReadFromBuffer(void * buffer, size_t buffer_size, size_t& buffer_pos, _T* target)
	{
		if(buffer_size < buffer_pos + sizeof(_T)) return false;
		memcpy(target, (char*)buffer+buffer_pos, sizeof(_T) );
		buffer_pos += sizeof(_T);
		return true;
	}

	template <typename _T>
	static bool ReadFromBuffer(void * buffer, size_t buffer_size, size_t& buffer_pos, _T* target, size_t target_size)
	{
		if(buffer_size < buffer_pos + target_size) return false;
		memcpy(target, (char*)buffer+buffer_pos, target_size );
		buffer_pos += target_size;
		return true;
	}

	template <typename _T>
	static bool WriteToBuffer(void * buffer, size_t buffer_size, size_t& buffer_pos, _T* target)
	{
		if(buffer_size < buffer_pos + sizeof(_T)) return false;
		memcpy((char*)buffer+buffer_pos, target, sizeof(_T) );
		buffer_pos += sizeof(_T);
		return true;
	}

	template <typename _T>
	static bool WriteToBuffer(void * buffer, size_t buffer_size, size_t& buffer_pos, _T* target, size_t target_size)
	{
		if(buffer_size < buffer_pos + target_size) return false;
		memcpy((char*)buffer+buffer_pos, target, target_size );
		buffer_pos += target_size;
		return true;
	}
};

#define MEMORY_MANAGER MemoryManager

#endif // MEMORY_MANAGER_H_
