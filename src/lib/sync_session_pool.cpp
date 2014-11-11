#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/optional.hpp>

#include "common.h"
#include "util/pool.h"
#include "util/logger.h"
#include "util/stop_watch.h"
#include "net/sync_session.h"
#include "net/header_factory.h"
#include "net/sync_session_pool.h"


SyncSessionPool::SyncSessionPool(boost::asio::io_service* pio_service,
											 const size_t pool_count, 
											 const std::string ipaddress, 
											 const std::string port, 
											 const timeval sendtimeout, 
											 const timeval recvtimeout,
											 const int64_t maintenance_millisec
											 ) :
	SessionPool<boost::shared_ptr<SyncSession>>(),
	m_ioService(pio_service),
	m_sizePoolCnt(pool_count), 
	m_strIP(ipaddress), 
	m_strPort(port),
	m_sizeConnectionCnt(0),
	m_tvSendTimeOut(sendtimeout),
	m_tvRecvTimeOut(recvtimeout),
	m_msMaintenanceInterval(maintenance_millisec)
{
	initialize();

}


SyncSessionPool::~SyncSessionPool(void)
{
	m_threadMaintenace.interrupt();
	m_threadMaintenace.join();

	//Destroy();
}

size_t	SyncSessionPool::GetConnectionCount()
{
	size_t size = 0;
	size = m_sizeConnectionCnt.load();
	return size;
}


void SyncSessionPool::ConnectionCountSub()
{
	m_sizeConnectionCnt.fetch_sub(1);
}

bool SyncSessionPool::CreateObject()
{
	SyncSessionPtr object = boost::make_shared<SyncSession>(m_ioService, HEADER_FACTORY()->MakeHeader());

	if (!object || !object.get()) return false;

	if (!Connetion(object)) return false;

	return true;
}

bool SyncSessionPool::CreateObject(const size_t count)
{
	for(size_t i = 0; i < count; ++i)
	{
		if(!CreateObject())
			return false;
	}

	return true;
}

bool SyncSessionPool::Connetion(SyncSessionPtr& object)
{
	object->SetSendTimeout(m_tvSendTimeOut);
	object->SetRecvTimeout(m_tvRecvTimeOut);

	if(object->Connect(m_strIP, m_strPort))
	{
		if(Restore(object))
		{
			m_sizeConnectionCnt.fetch_add(1);
			return true;
		}
	}

	return false;
}

void SyncSessionPool::initialize()
{
	try
	{
		m_pTimer.reset(new StopWatch());
		m_pTimer->Start();
		
		CreateObject(m_sizePoolCnt);

		size_t	load_connection_count = 0;
		load_connection_count = m_sizeConnectionCnt.load();
		ST_LOGGER.Trace(L"[ConnectionSessionPoolImpl][initialize] connection_count[%d]", load_connection_count );
	}
	catch(std::exception& /*exception*/)
	{
		ST_LOGGER.Error(L"Error");
	}
}


SyncSessionPool::OptSyncSesionPtr SyncSessionPool::Acquire()
{
	MaintenanceManager(0);
	return m_spSessionContainer->Acquire();
}


void SyncSessionPool::MaintenanceManager( boost::int64_t millisec )
{
	if( m_pTimer->GetElapsedmilliSec().count() > m_msMaintenanceInterval)
	{
		size_t	load_connection_count = 0;
		load_connection_count = m_sizeConnectionCnt.load();

		if ( load_connection_count < m_sizePoolCnt )
		{
			size_t remain_createcount = m_sizePoolCnt - m_sizeConnectionCnt.load();
			CreateObject(remain_createcount);
		}

		m_pTimer->Reset();
	}
}

void SyncSessionPool::Destroy()
{
	for(;;)
	{
		if( GetConnectionCount() == 0 ) return;

		OptSyncSesionPtr opt_session = Acquire();

		if(!opt_session) continue;

		SyncSessionPtr session = opt_session.get();

		if (session && session.get() )
		{
			session->Close();
		}
		continue;
	}
}

bool SyncSessionPool::PacketProcess(char* sendbuffer, const size_t sendbuffer_size, char* recvbuffer, size_t& recvbuffer_size)
{

	for(;;)
	{
		OptSyncSesionPtr opt_session = Acquire();

		if( GetConnectionCount() == 0 ) return false;

		if(!opt_session) continue;

		SyncSessionPtr session = opt_session.get();

		if (!session || 
			!session.get() || 
			!session->Send(sendbuffer, sendbuffer_size) ||
			!session->Recv(recvbuffer, recvbuffer_size) ) 
		{
			if( session && session.get() )  
				session->Close();

			ConnectionCountSub();
			continue;
		}

		Restore(session);
		break;
	}

	return true;
}