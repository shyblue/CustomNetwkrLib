#pragma once

#ifndef UTILITY_CALCULATION_H_
#define UTILITY_CALCULATION_H_

#include <tbb/spin_mutex.h>

template<typename T >
class ReinterationCalculation
{
public :
	// thread safe
	ReinterationCalculation(bool threadsafe = true);
	~ReinterationCalculation();

	const T GetMin();
	const T GetMax();
	const double GetMean();
	const size_t GetCount();

	void Insert(T value);
	void Clear();
private:
	void Calculation_Mean(T& insertvalue);
	void Calculation_MinMax(T& insertvalue);

	T	m_min;
	T	m_max;
	double	m_mean;
	size_t	m_count;
	bool	m_bThread;

	static const size_t m_constMaxCnt;
		
	tbb::spin_mutex m_mutexCalculation;
};

template<typename T>
const size_t ReinterationCalculation<T>::m_constMaxCnt = 1 << 17;  // 131072 = 2^17

template<typename T>
ReinterationCalculation<T>::ReinterationCalculation(bool thread_safe) : 
	m_count(0),
	m_mean(0.0f),
	m_bThread(thread_safe)

{

}

template<typename T>
ReinterationCalculation<T>::~ReinterationCalculation(void)
{

}

template<typename T>
const T ReinterationCalculation<T>::GetMin()
{
	return m_min;
}

template<typename T>
const T ReinterationCalculation<T>::GetMax()
{
	return m_max;
}

template<typename T>
const double ReinterationCalculation<T>::GetMean()
{
	return m_mean;
}

template<typename T>
const size_t ReinterationCalculation<T>::GetCount()
{
	return m_count;
}

template<typename T>
void ReinterationCalculation<T>::Insert(T value)
{	
	{
		if(m_bThread)
			tbb::spin_mutex::scoped_lock scope_lock(m_mutexCalculation);

		if( m_count <  m_constMaxCnt )
			++m_count;

		Calculation_Mean(value);
		Calculation_MinMax(value);
	}
}

template<typename T>
void ReinterationCalculation<T>::Clear()
{
	{
		if(m_bThread)
			tbb::spin_mutex::scoped_lock scope_lock(m_mutexCalculation);

		m_count = 0;
	}
}


template<typename T>
void ReinterationCalculation<T>::Calculation_Mean(T& insert_value)
{
	double dinsertvalue = (double)insert_value;
	if(m_count == 1)
	{
		m_mean = dinsertvalue;
	}
	else
	{
		double newmean =  m_mean + ( (dinsertvalue - m_mean) / (double)m_count );
		m_mean = newmean;
	}
}

template<typename T>
void ReinterationCalculation<T>::Calculation_MinMax(T& insert_value)
{
	if(m_count == 1)
	{
		m_max = insert_value;
		m_min = insert_value;
	}
	else
	{
		if( insert_value  > m_max )
			m_max = insert_value;

		if ( insert_value < m_min)
			m_min = insert_value;
	}
}

#endif
