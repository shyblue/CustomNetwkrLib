#include "net/async_session_impl.h"
#include "net/sync_session_pool.h"
#include "net/async_session_pool.h"
#include "net/packet_worker_manager.h"
#include "net/skeleton_server.h"
#include "util/configure.h"
#include "util/logger.h"
#include <boost/asio/spawn.hpp>

SkltServer::SkltServer( const int16_t port, HandlerManager *handler_manager) :
	m_ioService(), m_ioAcceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	Initialize(handler_manager);
	StartAccept();
}

SkltServer::~SkltServer(void)
{
}
bool SkltServer::Initialize(HandlerManager * p_handler_manager)
{
	m_spPacketWorkerManager.reset(new PacketWorkerManager(ST_CONFIG()->GetConfigureData<size_t>("CONFIGURE.WORKER_COUNT", 4), p_handler_manager));
	m_spAsyncSessionPool.reset(new AsyncSessionPool(&m_ioService, m_spPacketWorkerManager, ST_CONFIG()->GetConfigureData<size_t>("CONFIGURE.SESSION_POOL_COUNT", 10000)));

	ST_LOGGER.Info("[ServerBase][Initiazize] Complete");
	return true;
}

void SkltServer::Run()
{
	m_ioService.run();
}

bool SkltServer::StartAccept()
{

	boost::asio::spawn(m_ioService,[&](boost::asio::yield_context yield)
	{
		for(;;)
		{
			AsyncSessionPool::OptSessionPtr pOptSession = m_spAsyncSessionPool->Acquire();
			if( !pOptSession || !(pOptSession.get()))
			{
				ST_LOGGER.Error("[ServerBase][AsyncSessionPool] Can't get AsyncSession from Pool [%s]");		
				// TODO: Session을 할당받지 못했을때, 할당받지 못한 이유를 찾고, 해결을 해야 한다.
			}

			AsyncSessionImplPtr pSession = pOptSession.get();
			ST_LOGGER.Trace("[ServerBase][StartAccept] Accept ready session id(%d)", pSession->GetId());
			
			boost::system::error_code error;
			m_ioAcceptor.async_accept(pSession->GetSocket(),yield[error]);

			if(!error)
			{
				ST_LOGGER.Trace("[ServerBase][StartAccept] Client connect session id(%d)", pSession->GetId());
				pSession->SetSessionPoolState(AsyncSessionImpl::kAcquire);
				pSession->Recv();
				OnAccept(pSession);
			}
			else
			{
				ST_LOGGER.Error("[ServerBase][AcceptHandler] error[%s]", error.message().c_str());		
			}
		}
	});

	return true;
}

bool SkltServer::OnAccept(AsyncSessionImplPtr pSession)
{
 	return true;
}