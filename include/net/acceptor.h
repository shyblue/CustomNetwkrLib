#pragma  once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <tbb/concurrent_queue.h>

class HiveServer;
class AsyncSessionImpl;

class Acceptor : public boost::enable_shared_from_this< Acceptor >, private boost::noncopyable
{
	friend class HiveServer;

	typedef boost::shared_ptr<AsyncSessionImpl>			AsyncSessionImplPtr;
	typedef HiveServer& HiveRef;
public:
	const HiveRef GetHive() const { return m_rHive; }
	boost::asio::ip::tcp::acceptor& GetAcceptor() { return m_ioAcceptor; }
	boost::asio::strand& GetStrand() { return m_ioStrand; }

	void SetTimerInterval( int32_t timer_interval_ms ) {m_timer_interval = timer_interval_ms;}
	int32_t GetTimerInterval() const { return m_timer_interval; }

	bool HasError() {return ( boost::interprocess::ipcdetail::atomic_cas32( &m_nErrorState, 1, 1 ) == 1 ); }

	void Accept( AsyncSessionImplPtr connection );
	void Stop();

	Acceptor::Acceptor( HiveRef hive );
	virtual ~Acceptor();

private:
	void StartTimer();
	void HandleTimer( const boost::system::error_code & error );

	void StartError( const boost::system::error_code & error );

	void DispatchAccept( AsyncSessionImplPtr connection );
	void HandleAccept( const boost::system::error_code & error, AsyncSessionImplPtr connection );

	virtual bool OnAccept( AsyncSessionImplPtr connection );
	virtual void OnTimer( const boost::posix_time::time_duration & delta );
	virtual void OnError( const boost::system::error_code & error );

	HiveRef m_rHive;

	boost::asio::ip::tcp::acceptor m_ioAcceptor;
	boost::asio::strand m_ioStrand;
	boost::asio::deadline_timer m_timer;
	boost::posix_time::ptime m_last_time;

	int32_t m_timer_interval;
	volatile uint32_t m_nErrorState;
};