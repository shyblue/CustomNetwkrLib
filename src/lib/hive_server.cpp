#include "net/async_session_impl.h"
#include "net/sync_session_pool.h"
#include "net/async_session_pool.h"
#include "net/packet_worker_manager.h"
#include "net/hive_server.h"
#include "util/configure.h"
#include "util/logger.h"

#include <conio.h>

HiveServer::HiveServer( const int16_t port, HandlerManager *handler_manager)
	: m_ioService()
	, m_ioStrand(m_ioService)
	, m_endPoint(boost::asio::ip::tcp::v4(),port)
	, m_ioAcceptor(m_ioService,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port))
	, m_timerHeartBeat(m_ioService)
	, m_lastTimer(m_ioService)
	, m_nShutdown(0)
{
	Initialize(handler_manager);

	StartAccept();
}

HiveServer::~HiveServer(void)
{
}

bool HiveServer::Initialize(HandlerManager * p_handler_manager)
{
	m_spPacketWorkerManager.reset(new PacketWorkerManager(ST_CONFIG()->GetConfigureData<size_t>("CONFIGURE.WORKER_COUNT", 4), p_handler_manager));
	m_spAsyncSessionPool.reset(new AsyncSessionPool(this, m_spPacketWorkerManager, ST_CONFIG()->GetConfigureData<size_t>("CONFIGURE.SESSION_POOL_COUNT", 10000)));

	ST_LOGGER.Info("[ServerBase][Initiazize] Complete");
	return true;
}

void HiveServer::StartAccept()
{
	AsyncSessionPool::OptSessionPtr pOptSession = m_spAsyncSessionPool->Acquire();
	if( !pOptSession || !(pOptSession.get()))
	{
		ST_LOGGER.Error("[HiveServer][Constructor] Can't get AsyncSession from Pool [%s]");		
		// TODO: Session을 할당받지 못했을때, 할당받지 못한 이유를 찾고, 해결을 해야 한다.
	}

	AsyncSessionImplPtr pSession = pOptSession.get();
	ST_LOGGER.Trace("[HiveServer][Constructor] Ready to accept connection : session id[%d]", pSession->GetId());

	m_ioAcceptor.async_accept(
		pSession->GetSocket()
		,pSession->GetStrand().wrap( [&,pSession](boost::system::error_code error_code){HandleAccept(error_code,pSession);})
	);
	
}

void HiveServer::HandleAccept(boost::system::error_code error, AsyncSessionImplPtr sp_session)
{
	if(error||HasStopped())
	{
		ST_LOGGER.Error("[ServerBase][AcceptHandler] error[%s]", error.message().c_str());
		sp_session->StartError(error);
		StartError(error);
	}
	else
	{
		if(sp_session->GetSocket().is_open())
		{
			if(OnAccept(sp_session))
			{
				sp_session->StartTimer();
				sp_session->OnConnect(m_endPoint);
				sp_session->SetSessionPoolState(AsyncSessionImpl::kAcquire);
				sp_session->Recv();
			}
		}
		else
		{
			StartError(error);
		}

	}

	m_ioStrand.post([&]{StartAccept();});
}

void HiveServer::Run()
{

	boost::thread t( [&]{ m_ioService.run(); });
	//boost::bind(&boost::asio::io_service::run,&m_ioService));

	while( !m_nShutdown )
	{
		HeartBeat();
		Sleep(1);
	}

	m_ioService.stop();
	t.join();
}

bool HiveServer::OnAccept(AsyncSessionImplPtr pSession)
{
	ST_LOGGER.Trace("[HiveServer][OnAccept] Server has accepted client session : Session id [%d]",pSession->GetId());

	
	return true;
}

bool HiveServer::HasStopped()
{
	return ( boost::interprocess::ipcdetail::atomic_cas32( &m_nShutdown, 1, 1 ) == 1 );
}

void HiveServer::HeartBeat()
{

}

void HiveServer::Stop()
{
	if( boost::interprocess::ipcdetail::atomic_cas32( &m_nShutdown, 1, 0 ) == 0 )
	{
		m_ioService.stop();
	}
}

void HiveServer::OnTimer( const boost::posix_time::time_duration & delta )
{

}

void HiveServer::OnError( const boost::system::error_code & error )
{

}

void HiveServer::StartTimer()
{

}

void HiveServer::StartError( const boost::system::error_code & error )
{

}
