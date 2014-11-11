#include <exception>
#include <boost/make_shared.hpp>

#include "util/logger.h"
#include "util/pool.h"
#include "net/async_session_impl.h"
#include "net/packet_worker_manager.h"
#include "net/packet_worker.h"

PacketWorkerManager::PacketWorkerManager(const size_t worker_cnt, HandlerManager * p_handler_manager) :
	m_workerCnt(worker_cnt)
{
	BOOST_ASSERT(m_workerCnt > 0);
	Initialize(p_handler_manager);
}

PacketWorkerManager::~PacketWorkerManager(void)
{

}

bool PacketWorkerManager::Initialize(HandlerManager * p_handler_manager)
{
	try
	{
		for(int i = 0; i < m_workerCnt; ++i)
		{
			boost::shared_ptr<PacketWorker> sp = boost::make_shared<PacketWorker>(p_handler_manager,this);
			sp->Run(i+1);
			m_workers.push_back(sp);
		}
		ST_LOGGER.Trace("Worker create [%d] Complete ",m_workerCnt);

	}
	catch(std::exception exception)
	{
		ST_LOGGER.Error("exception [%s]", exception.what());
		return false;
	}

	return true;
}

bool PacketWorkerManager::GetPacketInfo(PacketInfo& packet)
{
	m_packetQueue.pop(packet);
	return true;
}

void PacketWorkerManager::Insert(PacketInfo& packet_info)
{
		m_packetQueue.push(packet_info);
}