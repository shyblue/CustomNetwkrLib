#ifndef NETWORK_POOL_SESSIONPOOL_H_
#define NETWORK_POOL_SESSIONPOOL_H_

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include "util/pool.h"

template < typename Tobject>
class Pool;


class Header;
class PacketWorkerManager;
class AsyncSession;
class AsyncSessionImpl;

template<typename T>
class SessionPool
{
public :
	typedef boost::shared_ptr<Pool<T>>			PoolPtr;
	typedef boost::optional<T>					OptSessionPtr;

	SessionPool();
	virtual ~SessionPool();

	virtual OptSessionPtr Acquire();
	virtual bool Restore(T& session_ptr);

	virtual void CreateContainer();

	const size_t GetUseCount();
	const size_t GetFlexibleMaxCount();

protected:
	bool Create(T& session_ptr);
	PoolPtr m_spSessionContainer;
};

template<typename T>
SessionPool<T>::SessionPool()
{
	CreateContainer();
}

template<typename T>
SessionPool<T>::~SessionPool()
{
}

template<typename T>
typename SessionPool<T>::OptSessionPtr SessionPool<T>::Acquire()
{
	return m_spSessionContainer->Acquire();
}

template<typename T>
bool SessionPool<T>::Restore(T& session_ptr)
{
	return m_spSessionContainer->Restore(session_ptr);
}

template<typename T>
void SessionPool<T>::CreateContainer()
{
	m_spSessionContainer.reset(new Pool<T>(5000));
}


template<typename T>
const size_t SessionPool<T>::GetUseCount()
{
	return m_spSessionContainer->GetUseCount();
}

template<typename T>
const size_t SessionPool<T>::GetFlexibleMaxCount()
{
	return m_spSessionContainer->GetFlexibleMaxCount();
}

template<typename T>
bool SessionPool<T>::Create(T& session_ptr)
{
	return m_spSessionContainer->Create(session_ptr);
}

#endif // NETWORK_POOL_SESSIONPOOL_H_

