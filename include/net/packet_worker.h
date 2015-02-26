#pragma once

#ifndef NETWORK_PACKET_DELIVER_H_
#define NETWORK_PACKET_DELIVER_H_

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/io_service.hpp>
#include "util/auto_task.h"
#include "network_define.h"

class HandlerManager;

class PacketWorkerManager;

class PacketWorker :
	public boost::enable_shared_from_this<PacketWorker>
{
public:
	typedef boost::shared_ptr<HandlerManager> HandlerManagerPtr;
	typedef boost::tuple<int32_t,char*,size_t> ResultObject;

	explicit PacketWorker(HandlerManager* p_handler_manager, PacketWorkerManager* p_packet_worker_manager);
	virtual ~PacketWorker();

	void Run(int worker_id);

	void Processing();
	void Processing(PacketInfo& object);
private:
	void SetHandlerManager(HandlerManager* handler_manager);
	void SetFactory(PacketWorkerManager* factory);

	void Task(int worker_id);

	HandlerManagerPtr		m_spHandlerManager;
	PacketWorkerManager*	m_pPacketWorkerManager;
	boost::thread*			m_pThread;
	int						m_nWorkerId;
};

#endif // NETWORK_PACKET_DELIVER_H_

