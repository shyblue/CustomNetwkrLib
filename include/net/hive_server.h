#pragma once

#ifndef NETWORK_SERVERBASE_H_
#define NETWORK_SERVERBASE_H_

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <tbb/concurrent_queue.h>

#include "common.h"

class HandlerManager;
class PacketWorkerManager;
class AsyncSessionImpl;
class AsyncSessionPool;

class HiveServer : public boost::enable_shared_from_this<HiveServer>, private boost::noncopyable
{
public:
	const static size_t ksession_pool_count = 5000;
	typedef boost::shared_ptr<AsyncSessionImpl>			AsyncSessionImplPtr;
	typedef boost::shared_ptr<AsyncSessionPool>			AsyncSessionPoolPtr;
	typedef boost::shared_ptr<PacketWorkerManager>		PacketWorkerManagerPtr;

	bool Initialize(HandlerManager * handler_manager);

	boost::asio::io_service& GetService() { return m_ioService; }
	boost::asio::ip::tcp::endpoint& GetEndPoint() {return m_endPoint; }
	PacketWorkerManagerPtr GetWorkerManager() { return m_spPacketWorkerManager; }

	void Run();
	void Post();
	void Start()
	void Stop();
	bool HasStopped();

	void HeartBeat();

protected:
	explicit HiveServer(const int16_t port, HandlerManager *handler_manager);
	virtual ~HiveServer(void);

private:

	void StartAccept();
	void HandleAccept(const boost::system::error_code error_code, AsyncSessionImplPtr sp_session);

	virtual bool OnStart()=0;
	virtual bool OnAccept(AsyncSessionImplPtr pSession)=0;
	virtual bool OnError( const boost::system::error_code & error )=0;
	virtual bool OnTimer( const boost::posix_time::time_duration & delta )=0;
	virtual bool OnStop()=0;
	virtual bool OnHeartBeat()=0;

	void StartTimer();
	void StartError( const boost::system::error_code & error );

	boost::asio::io_service	m_ioService;
	boost::asio::strand m_ioStrand;
	boost::asio::ip::tcp::acceptor m_ioAcceptor;

	boost::asio::deadline_timer m_lastTimer;
	boost::posix_time::ptime m_lastTime;

	boost::asio::ip::tcp::endpoint m_endPoint;

	boost::asio::deadline_timer m_timerHeartBeat;
	uint32_t				m_nHeartBeatInterval;

	AsyncSessionPoolPtr		m_spAsyncSessionPool;
	PacketWorkerManagerPtr	m_spPacketWorkerManager;

	volatile uint32_t m_nShutdown;
};
#endif // NETWORK_SERVERBASE_H_