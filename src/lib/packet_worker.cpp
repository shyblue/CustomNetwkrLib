#include <boost/tuple/tuple.hpp>

#include "util/logger.h"
#include "util/common_Pool.h"
#include "net/network_define.h"
#include "net/packet_worker.h"
#include "net/async_session_impl.h"
#include "net/packet_worker_manager.h"
#include "handler/handler_manager.h"

PacketWorker::PacketWorker(HandlerManager* p_handler_manager, PacketWorkerManager* p_packet_worker_manager)
{
	SetHandlerManager(p_handler_manager);
	SetFactory(p_packet_worker_manager);
}

PacketWorker::~PacketWorker()
{
}

void PacketWorker::SetHandlerManager(HandlerManager* handler_manager)
{
	m_spHandlerManager.reset(handler_manager);
}
void PacketWorker::SetFactory(PacketWorkerManager* factory)
{
	m_pPacketWorkerManager = factory;
}

void PacketWorker::Task(int worker_id)
{
	m_nWorkerId = worker_id;
	ST_LOGGER.Trace("[PacketWorker][Ready] Worker [%d] Thread [%d]",worker_id, m_pThread->get_id());
	for(;;)
	{
		Processing();
	}
}
void PacketWorker::Run(int worker_id)
{
	m_pThread = new boost::thread( [this,worker_id](){ Task( worker_id ); } );
}

void PacketWorker::Processing()
{
	PacketInfo packet;
	m_pPacketWorkerManager->GetPacketInfo(packet);
	Processing(packet);
}

void PacketWorker::Processing(PacketInfo& packet)
{
	ST_LOGGER.Trace("[Worker:%d][%d] Pacekt.protocol_no [%d]", m_nWorkerId,m_pThread->get_id(), packet.m_protocolNo);

	ResultObject resultObject = PacketWorker::m_spHandlerManager->Processing(packet.m_protocolNo, packet.m_pBuffer, packet.m_sizeOfBuffer);

	if(packet.m_pBuffer && packet.m_sizeOfBuffer > 0)
	{
		packet.m_pAsyncSessionImp->BufferRelease(packet.m_pBuffer, static_cast<uint32_t>(packet.m_sizeOfBuffer));
	}

	if(resultObject.get<0>() && resultObject.get<1>() > 0)
	{
		packet.m_pAsyncSessionImp->Send(resultObject.get<0>(), resultObject.get<1>());
		COMMON_POOL::Delete((char*)resultObject.get<0>(), resultObject.get<1>() );
	}
}