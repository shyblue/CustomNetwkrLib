#pragma once

#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_	1

#include <cstring>
#include <boost/serialization/singleton.hpp>

template <typename T>
class BufferManager{
public:
	static size_t GetBuffer(uint8_t* buffer, T& value)
	{
		const size_t size = sizeof(value);
		memcpy(reinterpret_cast<void *>(&value), buffer, size);
		buffer += size;
		return size;
	}
	static size_t GetBufferPtr(uint8_t* buffer, T value, const size_t value_size)
	{
		memcpy(reinterpret_cast<void *>(value), buffer, value_size);
		buffer += value_size;
		return value_size;
	}

	static size_t SetBuffer(uint8_t* buffer, T& value)
	{
		const size_t size = sizeof(value);
		memcpy(buffer, reinterpret_cast<void *>(&value), size);
		buffer += size;
		return size;
	}
	static size_t SetBufferPtr(uint8_t* buffer, T value, const size_t value_size)
	{
		memcpy(buffer, reinterpret_cast<void *>(value), value_size);
		buffer += value_size;
		return value_size;
	}

protected:
	BufferManager();
	~BufferManager();
private:
};

template < size_t size_of_buffer, typename type_of_buffer = int8_t >
class CBuffer 
{
public:
	CBuffer();
	~CBuffer();

	enum CBufferResult
	{
		brSuccessful = 0,
		brWrite,
		brRead
	};

	type_of_buffer* Getbuffer(){ return m_pBuffer; }
	void reset(const bool bmemset = false);

	template<typename T>
	int32_t Write(T& data);
	int32_t Write(void* data, const size_t size);

	template  < typename T >
	size_t Read(T& data);
	size_t Read(void* data, const size_t size);
	void ReadSet(const size_t size){ m_sizeRead += size; }

	type_of_buffer* GetReadPos(){
		ReadsetRangeSet(0);
		return (m_pBuffer +m_sizeRead);
	}
	type_of_buffer* GetWritePos(){ 
		WritesetRangeSet(0);
		return (m_pBuffer + m_sizeWrite);
	}

	void Set(void * ptr){
		m_pBuffer = reinterpret_cast<type_of_buffer*>(ptr);
	}
private:
	size_t ReadsetRangeCheck(){
		if( m_sizeRead < size_of_buffer ) 
			return 0;  

		return (m_sizeRead - size_of_buffer); 
	}
	size_t WritesetRangeCheck(){ 
		if( m_sizeWrite < size_of_buffer ) 
			return 0;
	 	return (m_sizeRead - size_of_buffer); 
	}

	size_t ReadsetRangeCheck(size_t add){ 
		if( (m_sizeRead + add) < size_of_buffer ) 
			return 0;
		else
			return (m_sizeRead + add - size_of_buffer); 
	}
	size_t WritesetRangeCheck(size_t add){ 
		if( (m_sizeWrite + add) < size_of_buffer ) 
		return 0;
	else
		return (m_sizeWrite + add - size_of_buffer); 
	}	

	void ReadsetRangeSet(size_t size)
	{ 
		if( m_sizeRead < size_of_buffer )
			m_sizeRead += size;
		else
			m_sizeRead = (m_sizeRead + size - size_of_buffer);
	}
	void WritesetRangeSet(size_t size)
	{ 
		if( m_sizeWrite < size_of_buffer )	
			m_sizeWrite += size;
		else
			m_sizeWrite = (m_sizeWrite + size - size_of_buffer);
	}
	type_of_buffer * m_pBuffer;

	size_t m_sizeRead;
	size_t m_sizeWrite;
};

template < size_t size_of_buffer, typename type_of_buffer>
CBuffer<size_of_buffer, type_of_buffer>::CBuffer() : m_pBuffer(0), m_sizeRead(0), m_sizeWrite(0)
{
}

template < size_t size_of_buffer, typename type_of_buffer >
CBuffer<size_of_buffer, type_of_buffer>::~CBuffer() 
{
}

template < size_t size_of_buffer, typename type_of_buffer>
void CBuffer<size_of_buffer, type_of_buffer>::reset(const bool b_mem_set) 
{
	m_sizeRead = 0;	
	m_sizeWrite = 0;

	if(b_mem_set)
		memset(&m_pBuffer[0], 0, size_of_buffer);
}
template < size_t size_of_buffer, typename type_of_buffer >
template  < typename T >
int32_t CBuffer<size_of_buffer, type_of_buffer>::Write(T& data)
{
	size_t size = sizeof(data);
	size_t sizeRemain = WritesetRangeCheck(size);

	if(sizeRemain == 0)
	{
		if(!memcpy_s(GetWritePos(), m_pBuffer.size(), &data, sizeof(data) ) )
		{
			WritesetRangeSet(size * sizeof(type_of_buffer)) ;
		}
	}
	else
	{
		size_t sizeTail = size - sizeRemain;
		if(size)
		{
			if(!memcpy_s(GetWritePos(), m_pBuffer.size(), &data, sizeTail ))
			{
				 WritesetRangeSet(sizeTail * sizeof(type_of_buffer));
			}
		}

		if(!memcpy_s(GetWritePos(), m_pBuffer.size(), &data, sizeRemain ))
		{
				WritesetRangeSet(sizeRemain * sizeof(type_of_buffer));
		}
	}

	return 0;
}
template < size_t size_of_buffer, typename type_of_buffer>
int32_t CBuffer<size_of_buffer, type_of_buffer>::Write(void* data, const size_t size) 
{
	size_t sizeRemain = WritesetRangeCheck(size);

	if(sizeRemain == 0)
	{
		if(!memcpy_s(GetWritePos(), m_pBuffer.size(), data, size ))
		{
			WritesetRangeSet(size * sizeof(type_of_buffer)) ;
		}
	}
	else
	{
		size_t sizeTail = size - sizeRemain;
		if(sizeTail)
		{
			if(!memcpy_s(GetWritePos(), m_pBuffer.size(), data, sizeTail ))
			{
				 WritesetRangeSet(sizeTail * sizeof(type_of_buffer));
			}
		}
		if(!memcpy_s(GetWritePos(), m_pBuffer.size(), reinterpret_cast<type_of_buffer *>(data) + sizeTail , sizeRemain ))
		{
				WritesetRangeSet(sizeRemain * sizeof(type_of_buffer));
		}
	}

	return 0;
}

template < size_t size_of_buffer, typename type_of_buffer >
template  < typename T >
size_t CBuffer<size_of_buffer, type_of_buffer>::Read(T& data)
{
	size_t size = sizeof(data);
	size_t sizeRemain = ReadsetRangeCheck(size);

	if(sizeRemain == 0)
	{
		if(!memcpy_s(&data, sizeof(data), GetReadPos(), sizeof(data)))
		{
			 ReadsetRangeSet(size * sizeof(type_of_buffer)) ;
		}
	}
	else
	{
		size_t sizeTail = size - sizeRemain;
		if(sizeTail)
		{
			if(!memcpy_s(&data, sizeof(data), GetReadPos(), sizeTail ))
			{
				 ReadsetRangeSet(sizeTail * sizeof(type_of_buffer));
			}
		}

		if(!memcpy_s(&data, sizeof(data), GetReadPos(), sizeRemain ))
		{
				ReadsetRangeSet(sizeRemain * sizeof(type_of_buffer));
		}
	}

	return size * sizeof(type_of_buffer);
}

template < size_t size_of_buffer, typename type_of_buffer >
size_t CBuffer<size_of_buffer, type_of_buffer>::Read(void * data, const size_t size)
{
	size_t sizeRemain = ReadsetRangeCheck(size);

	if(sizeRemain == 0)
	{
		if(!memcpy_s(data, size, GetReadPos(), size ))
		{
			 ReadsetRangeSet(size * sizeof(type_of_buffer)) ;
		}
	}
	else
	{
		size_t sizeTail = size - sizeRemain;
		if(sizeTail)
		{
			if(!memcpy_s(data, sizeTail, GetReadPos(), sizeTail ))
			{
				 ReadsetRangeSet(sizeTail * sizeof(type_of_buffer));
			}
		}
		if(!memcpy_s(reinterpret_cast<type_of_buffer *>(data) + sizeTail, sizeRemain, GetReadPos(), sizeRemain ))
		{
				ReadsetRangeSet(sizeRemain * sizeof(type_of_buffer));
		}
	}

	return size * sizeof(type_of_buffer);
}

#endif // _BUFFER_HPP_
