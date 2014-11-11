#pragma  once

#ifndef NETWORK_PACKET_DELIVER_FACTORY_H_
#define NETWORK_PACKET_DELIVER_FACTORY_H_

#include <atomic>
#include <boost/serialization/singleton.hpp>
#include <boost/shared_ptr.hpp>
#include "util/auto_task.h"
#include "network_define.h"

template < typename Tobject>
class Pool;

class HandlerManager;
class PacketWorker;
class AsyncSessionImpl;

class PacketWorkerManager
{
public:
	typedef tbb::concurrent_bounded_queue<PacketInfo>	Queue;
	typedef boost::shared_ptr<PacketWorker>				WorkerPtr;
	typedef std::vector<WorkerPtr>						WorkerContainer;
	explicit PacketWorkerManager(const size_t worker_cnt, HandlerManager * handler_manager);
	~PacketWorkerManager(void);

	bool GetPacketInfo(PacketInfo& packetInfo);
	void Insert(PacketInfo& packetinfo);

private:
	bool Initialize(HandlerManager * handler_manager);

	WorkerContainer	m_workers;
	Queue			m_packetQueue;
	size_t			m_workerCnt;
};
#endif // NETWORK_PACKET_DELIVER_FACTORY_H_

