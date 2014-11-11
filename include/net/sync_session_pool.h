#ifndef NETWORK_CONNETION_SESSIONPOOL_H_
#define NETWORK_CONNETION_SESSIONPOOL_H_

#include <atomic>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/asio/io_service.hpp>

#include "net/session_pool.h"
#include "net/sync_session.h"

template < typename Tobject>
class Pool;
class StopWatch;

class SyncSessionPool: public SessionPool<boost::shared_ptr<SyncSession>>
{
private:
	static const size_t kmax_poolcount_ = 64;
public:
	typedef boost::shared_ptr<SyncSession>		SyncSessionPtr;
	typedef boost::optional<SyncSessionPtr>		OptSyncSesionPtr;
	typedef boost::shared_ptr<StopWatch>		TimerPtr;
	typedef struct timeval						timeval;

	SyncSessionPool(	boost::asio::io_service* pio_service, 
							const size_t pool_count, 
							const std::string ipaddress, 
							const std::string port, 
							const timeval sendtimeout, 
							const timeval recvtimeout, 
							const int64_t maintenance_millisec = 10000);

	~SyncSessionPool(void);

	virtual OptSyncSesionPtr Acquire();

	size_t	GetConnectionCount();
	void ConnectionCountSub();

	bool PacketProcess(char* sendbuffer, const size_t sendbuffer_size, char* recvbuffer, size_t& recvbuffer_size);

private:
	void initialize();

	void Destroy();
	bool CreateObject();
	bool CreateObject(const size_t count);
	bool Connetion(SyncSessionPtr& object);
	void MaintenanceManager( const int64_t millisec );

	boost::asio::io_service*m_ioService;

	std::string				m_strIP;
	std::string				m_strPort;

	size_t					m_sizePoolCnt;

	std::atomic<size_t>		m_sizeConnectionCnt;
	tbb::spin_mutex			m_mutexConnectionCnt;

	TimerPtr				m_pTimer;

	timeval					m_tvSendTimeOut;
	timeval					m_tvRecvTimeOut;

	boost::thread			m_threadMaintenace;
	int64_t					m_msMaintenanceInterval;
};

#endif // NETWORK_CONNETION_SESSIONPOOL_H_