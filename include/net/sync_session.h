#pragma once

#ifndef NETWORK_ASYNCSESSION_H_
#define NETWORK_ASYNCSESSION_H_

#include <boost/shared_ptr.hpp>
#include <boost/asio/streambuf.hpp>

#include "common.h"

namespace boost
{
namespace asio
{
class io_service;
}
}

class SyncSessionImpl;
class Header;

class SyncSession
{
public:
	typedef boost::shared_ptr<SyncSessionImpl>	SyncSessionImplPtr;

	explicit SyncSession(boost::asio::io_service* io_service, Header* pheader);
	~SyncSession(void);

	bool IsConnection();
	bool Connect(std::string ipaddress, std::string port);

	void SetSendTimeout(const timeval& settime);
	void SetRecvTimeout(const timeval& settime);
	void ClearSendTimeout();
	void ClearRecvTimeout();

	bool Send(const char* buffer, size_t buffer_size);

	bool Recv(char* buffer, size_t& buffer_size);

	bool Close();

	const int32_t GetSendPacketNo();
private:
	SyncSessionImplPtr m_pSyncSessionImpl;
	
};
#endif // NETWORK_ASYNCSESSION_H_

