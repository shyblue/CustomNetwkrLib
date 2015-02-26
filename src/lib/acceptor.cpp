#include "net/acceptor.h"
#include "net/hive_server.h"
#include "net/async_session_impl.h"

Acceptor::Acceptor( HiveRef hive )
	: m_rHive( hive )
	, m_ioAcceptor( hive.GetService(), hive.GetEndPoint() )
	, m_ioStrand( hive.GetService() )
	, m_timer( hive.GetService() )
	, m_timer_interval( 1000 )
	, m_nErrorState( 0 )
{
}

Acceptor::~Acceptor() {}

void Acceptor::Accept( AsyncSessionImplPtr connection )
{
	auto self(shared_from_this());
	m_ioStrand.post( [&,connection]{DispatchAccept(connection);} );
}

void Acceptor::Stop()
{
	auto self(shared_from_this());
	m_ioStrand.post( [&]{HandleTimer(boost::asio::error::connection_reset);} );
}

void Acceptor::StartTimer()
{
	m_last_time = boost::posix_time::microsec_clock::local_time();
	m_timer.expires_from_now( boost::posix_time::milliseconds( m_timer_interval ) );

	auto self(shared_from_this());
	m_timer.async_wait( m_ioStrand.wrap([&](boost::system::error_code ec){HandleTimer(ec);}) );
}

void Acceptor::HandleTimer( const boost::system::error_code & error )
{
	if( error || HasError() || m_rHive.HasStopped() )
	{
		StartError( error );
	}
	else
	{
		OnTimer( boost::posix_time::microsec_clock::local_time() - m_last_time );
		StartTimer();
	}
}

void Acceptor::StartError( const boost::system::error_code & error )
{
	if( boost::interprocess::ipcdetail::atomic_cas32( &m_nErrorState, 1, 0 ) == 0 )
	{
		boost::system::error_code ec;
		m_ioAcceptor.cancel( ec );
		m_ioAcceptor.close( ec );
		m_timer.cancel( ec );
		OnError( error );
	}
}

void Acceptor::DispatchAccept( AsyncSessionImplPtr connection )
{
	auto self(shared_from_this());

	m_ioAcceptor.async_accept(
		connection->GetSocket(),
		connection->GetStrand().wrap([&,connection](boost::system::error_code ec){HandleAccept(ec,connection);} ));
}
void Acceptor::HandleAccept( const boost::system::error_code & error, AsyncSessionImplPtr connection )
{
	if( error || HasError() || m_rHive.HasStopped() )
	{
		connection->StartError( error );
		return;
	}

	if( connection->GetSocket().is_open() )
	{
		if( OnAccept( connection ) )
		{
			connection->StartTimer();
			connection->OnAccept( m_ioAcceptor.local_endpoint() );
			connection->SetSessionPoolState(AsyncSessionImpl::kAcquire);
		}
	}
	else
	{
		StartError( error );
	}

	m_rHive.OnAccept(connection);
}

bool Acceptor::OnAccept( AsyncSessionImplPtr connection )
{
	return true;;
}

void Acceptor::OnTimer( const boost::posix_time::time_duration & delta )
{
}

void Acceptor::OnError( const boost::system::error_code & error )
{
}
