#include <exception>
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>
#include <boost/detail/endian.hpp>
#include <boost/asio/spawn.hpp>

#include "common.h"

#include "net/network_define.h"
#include "net/async_session_impl.h"
#include "net/async_session_pool.h"
#include "net/packet_worker_manager.h"
#include "net/header.h"
#include "util/common_pool.h"
#include "util/memory_manager.h"
#include "util/logger.h"
#include "util/configure.h"


AsyncSessionImpl::AsyncSessionImpl(boost::asio::io_service* io_service, Header* p_header,const PacketWorkerManagerPtr& packet_worker_manager) :
	SessionBase(io_service)
	,m_spWorkerManager(packet_worker_manager)
	,m_szHeaderBuffer(0)
	,m_szBodyBuffer(0)
	,m_sizeOfBodyBuffer(0)
	,m_nSessionPoolState(SessionPoolState::kNoneRegister)
	,m_id(0)
	,m_sizeOfMaxBuffer(ST_CONFIG()->GetConfigureData<size_t>("CONFIGURE.SESSION_BUFFER_MAX", 32768))
	,m_strand(*io_service)
{
	m_spHeader.reset(p_header);
}

AsyncSessionImpl::~AsyncSessionImpl()
{
}

bool AsyncSessionImpl::Recv(char* buffer, size_t buffer_size)
{
	try
	{
		if(!m_szHeaderBuffer)
		{
			m_szHeaderBuffer = COMMON_POOL::New(m_spHeader->GetHeaderSize());
		}

		RecvProcess();
	}
	catch(std::exception& error)
	{
		ST_LOGGER.Error("error[%s]", error.what());
		return false;
	}
	return true;
}

void AsyncSessionImpl::RecvProcess()
{
	auto self(shared_from_this());
	boost::asio::spawn( m_strand ,
		[this,self](boost::asio::yield_context yield)
		{
			boost::system::error_code error;
			size_t bytes_transferred;

			bytes_transferred = m_tcpSocket.async_receive(boost::asio::buffer(m_szHeaderBuffer,m_spHeader->GetHeaderSize()),yield[error]);
			if(!ReadHeader(error,bytes_transferred)) return;

			bytes_transferred = m_tcpSocket.async_receive(boost::asio::buffer(m_szBodyBuffer, m_sizeOfBodyBuffer),yield[error]);
			if(!ReadBody(error,bytes_transferred)) return;

			RecvProcess();
		}
	);
}

bool AsyncSessionImpl::ReadHeader( const boost::system::error_code & error, size_t bytes_transferred )
{
	ST_LOGGER.Trace("[AsyncSessionImpl][ReadHeader] bytes_transferred = %d", bytes_transferred);
	if( error )
	{
		ST_LOGGER.Error("[AsyncSessionImpl][ReadHeader] Error occurred [%s : %d] %s", error.category().name(),error.value(),error.message().c_str());
		Close();
		return false;
	}

	if ( bytes_transferred != m_spHeader->GetHeaderSize() )
	{
		ST_LOGGER.Error("bytes_transferred[%d] != sizeof(Header)", bytes_transferred);
		Close();
		return false;
	}

	m_spHeader->Deserialize(m_szHeaderBuffer, m_spHeader->GetHeaderSize());

	if ( !m_spHeader->Validate() )
	{
		ST_LOGGER.Error("Header data Invalid.");
		Close();
		return false;
	}

	m_sizeOfBodyBuffer =  m_spHeader->GetBodySize() + m_spHeader->GetEnderSize();

	if( AsyncSessionImpl::m_sizeOfMaxBuffer < m_sizeOfBodyBuffer ) 
	{
		ST_LOGGER.Error("Session buffer size overflow or invalid : bodybuffersize_(%llu)", m_sizeOfBodyBuffer);
		Close();
		return false;
	}
		
	if(m_sizeOfBodyBuffer > 0)
	{
		m_szBodyBuffer = COMMON_POOL::New(m_sizeOfBodyBuffer);
		m_sizeOfPrevBodyBuffer = m_sizeOfBodyBuffer;
	}

	return true;
}


bool AsyncSessionImpl::ReadBody(const boost::system::error_code & error, size_t byte_transferred )
{
	ST_LOGGER.Trace("[AsyncSessionImpl][ReadBody]bytes_transferred = %d", byte_transferred);

	MakeHexCode();

	if (m_spHeader->CheckEndmarker(m_szBodyBuffer, byte_transferred) && !error && (m_sizeOfBodyBuffer == byte_transferred) )
	{	
		auto self(shared_from_this());
		boost::shared_ptr<PacketInfo> ptr = boost::make_shared<PacketInfo>(self, m_spHeader->GetProtocolNo(), m_szBodyBuffer, m_sizeOfBodyBuffer - m_spHeader->GetEnderSize());
		m_spWorkerManager->Insert(*ptr);
		m_szBodyBuffer = nullptr;
	}
	else
	{
		ST_LOGGER.Error("error[%s]", error.message().c_str());
		Close();
		return false;
	}

	return true;
}


bool AsyncSessionImpl::Send(const char* data, size_t data_length)
{
	try
	{
		// Call from Packet worker thread ( Processing() )
		SendProcess(data, data_length);
	}
	catch(std::exception& error)
	{
		ST_LOGGER.Error("exception.what[%s]", error.what());
		return false;
	}
	return true;
}

void AsyncSessionImpl::SendProcess(const char* data, const size_t data_length)
{
	try
	{
		ST_LOGGER.Trace("[AsyncSessionImpl][SendProcess] Send packet request [%d]", data_length);
		auto self(shared_from_this());
		
		boost::asio::async_write(m_tcpSocket,boost::asio::buffer(data,data_length),
		[this,self,data,&data_length](boost::system::error_code error,size_t bytes_transferred)
		{
			COMMON_POOL::Delete((char*)data, data_length);
			if(error)
			{
				ST_LOGGER.Error("[Packet Send error [%s : %d] %s",error.category().name(),error.value(),error.message().c_str());
				Close();
				return;
			}
			ST_LOGGER.Trace("[AsyncSessionImpl][SendProcess] Send packet complete [%d]", bytes_transferred);
		});
	}
	catch(std::exception exception)
	{
		ST_LOGGER.Error("exception.what[%s]", exception.what());
	}
}


void AsyncSessionImpl::SetSessionPool(AsyncSessionPool* p_session_pool)
{
	m_pSessionPool = p_session_pool;
}

bool AsyncSessionImpl::Close()
{
	try
	{
		if(m_tcpSocket.is_open())
		{
			boost::system::error_code IgnoreError;
			m_tcpSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, IgnoreError);
			m_tcpSocket.close();
		}

		if(m_szBodyBuffer && m_sizeOfPrevBodyBuffer)
		{
			BufferRelease(m_szBodyBuffer, static_cast<uint32_t>(m_sizeOfPrevBodyBuffer));
		}
	
		if (m_nSessionPoolState == AsyncSessionImpl::kAcquire)
		{
			boost::shared_ptr<AsyncSessionImpl> psession(shared_from_this());	
			m_pSessionPool->Restore(psession);
			SetSessionPoolState(AsyncSessionImpl::kRestore);
		}
		ST_LOGGER.Trace("[AsyncSessionImpl][Close] session id[%d]", GetId());

	}
	catch(std::exception& error)
	{
		ST_LOGGER.Error("exception.what[%s]", error.what());
		return false;
	}
	return true;
}

bool AsyncSessionImpl::BufferRelease(char* buffer, uint32_t buffer_size)
{
	if(COMMON_POOL::Delete(buffer, buffer_size))
	{
		buffer = nullptr;
		buffer_size = 0;

		return true;
	}

	return false;
}

void AsyncSessionImpl::PacketSend(const boost::system::error_code & error)
{

}
void AsyncSessionImpl::MakeHexCode()
{
	/// hex code를 출력한다.
	ConfigureData* pConfigure = ST_CONFIG()->GetInstancePtr();
	std::string HEX_CODE = pConfigure->GetConfigureData<std::string>("CONFIGURE.HEX_CODE", "N");

	if (HEX_CODE.compare("Y") == 0)
	{
		std::stringstream hexCode;

		for(size_t idx=0;idx<m_spHeader->GetHeaderSize();++idx)
		{
			hexCode << std::hex << std::setfill('0') << std::setw(2) << static_cast<int16_t>((BYTE)m_szHeaderBuffer[idx]);
			if(idx < m_spHeader->GetHeaderSize()-1) hexCode << " ";
		}
		hexCode << "|";
		for(size_t idx=0;idx<m_spHeader->GetBodySize();++idx)
		{
			hexCode << std::hex << std::setfill('0') << std::setw(2) << static_cast<int16_t>((BYTE)m_szBodyBuffer[idx]);
			if(idx < m_spHeader->GetBodySize()-1) hexCode << " ";
		}

		ST_LOGGER.Trace("[PACKET HEX CODE] [%s]", hexCode.str().c_str());
	}
}