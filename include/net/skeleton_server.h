#pragma once

#ifndef NETWORK_SERVERBASE_H_
#define NETWORK_SERVERBASE_H_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include "common.h"

class HandlerManager;
class PacketWorkerManager;
class AsyncSessionImpl;
class AsyncSessionPool;

class SkltServer : private boost::noncopyable
{
public:
	const static size_t ksession_pool_count = 5000;
	typedef boost::shared_ptr<AsyncSessionImpl>			AsyncSessionImplPtr;
	typedef boost::shared_ptr<AsyncSessionPool>			AsyncSessionPoolPtr;
	typedef boost::shared_ptr<PacketWorkerManager>		PacketWorkerManagerPtr;
	
	explicit SkltServer( const int16_t port, HandlerManager *handler_manager);
	~SkltServer(void);

	bool Initialize(HandlerManager * handler_manager);
	void Run();

private:
	bool StartAccept();
	bool OnAccept(AsyncSessionImplPtr pSession);

	boost::asio::io_service			m_ioService;
	boost::asio::ip::tcp::acceptor	m_ioAcceptor;
	AsyncSessionPoolPtr		m_spAsyncSessionPool;
	PacketWorkerManagerPtr	m_spPacketWorkerManager;
};

#endif // NETWORK_SERVERBASE_H_