#pragma once

#ifndef NETWORK_POOL_ASYNSESSIONPOOL_H_
#define NETWORK_POOL_ASYNSESSIONPOOL_H_

#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>

#include "session_pool.h"
#include "async_session_impl.h"

class HiveServer;

class AsyncSessionPool : public SessionPool<boost::shared_ptr<AsyncSessionImpl>>
{
public:
	typedef boost::shared_ptr<PacketWorkerManager>		PacketWorkerManagerPtr;
	typedef boost::shared_ptr<AsyncSessionImpl>			AsyncSessionImplPtr;

	AsyncSessionPool(HiveServer* p_hive,const PacketWorkerManagerPtr& packet_worker_manager, const size_t count);
	~AsyncSessionPool(void);
private:
	void Initialize(HiveServer* p_hive, const PacketWorkerManagerPtr& packet_worker_manager, const size_t count);
};

#endif // NETWORK_POOL_ASYNSESSIONPOOL_H_


