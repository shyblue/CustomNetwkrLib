#include "net/sync_session_impl.h"
#include "net/sync_session.h"

SyncSession::SyncSession(boost::asio::io_service* io_service, Header* pheader) :
	m_pSyncSessionImpl(new SyncSessionImpl(io_service, pheader))
{
}

SyncSession::~SyncSession(void)
{
}

bool SyncSession::IsConnection()
{
	return m_pSyncSessionImpl->IsConnection();
}

bool SyncSession::Connect(std::string ipaddress, std::string port)
{
	return m_pSyncSessionImpl->Connect(ipaddress, port);
}

void SyncSession::SetSendTimeout(const timeval& settime )
{
	m_pSyncSessionImpl->SetSendTimeout(settime);
}

void SyncSession::SetRecvTimeout(const timeval& settime )
{
	m_pSyncSessionImpl->SetRecvTimeout(settime);
}

void SyncSession::ClearSendTimeout()
{
	m_pSyncSessionImpl->ClearSendTimeout();
}

void SyncSession::ClearRecvTimeout()
{
	m_pSyncSessionImpl->ClearRecvTimeout();
}

bool SyncSession::Send(const char* buffer, size_t buffer_size)
{
	return m_pSyncSessionImpl->Send(buffer, buffer_size);
}
bool SyncSession::Recv(char* buffer, size_t& buffer_size)
{
	return m_pSyncSessionImpl->Recv(buffer, buffer_size);
}

bool SyncSession::Close()
{
	return m_pSyncSessionImpl->Close();
}

const int32_t SyncSession::GetSendPacketNo()
{
	return m_pSyncSessionImpl->GetSendPacketNo();
}