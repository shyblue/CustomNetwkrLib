#include <exception>
#include <boost/make_shared.hpp>

#include "util/pool.h"
#include "util/logger.h"
#include "net/asyn_session_pool.h"
#include "net/header_factory.h"

AsyncSessionPool::AsyncSessionPool(boost::asio::io_service* pio_service, const PacketWorkerManagerPtr& packet_worker_manager, const size_t pool_cnt) : 
	m_ioService(pio_service)
{
	Initialize(packet_worker_manager, pool_cnt);
}

AsyncSessionPool::~AsyncSessionPool(void)
{
}

void AsyncSessionPool::Initialize(const PacketWorkerManagerPtr& packet_worker_manager, const size_t pool_amt)
{
	try
	{	
		m_spSessionContainer.reset(new Pool<AsyncSessionImplPtr>(pool_amt));
		size_t nSessionPoolAmt = pool_amt;	

		for(size_t i(0); i < nSessionPoolAmt; ++i)
		{
			boost::shared_ptr<AsyncSessionImpl> p =  boost::make_shared<AsyncSessionImpl>(m_ioService, HEADER_FACTORY()->MakeHeader(), packet_worker_manager);
			p->SetId(i);
			p->SetSessionPool(this);
			Create(p);
		}

		ST_LOGGER.Info(L"Session Pool Amount [%d] : Create complete", nSessionPoolAmt);
	}
	catch(std::exception& exception)
	{
		ST_LOGGER.Error(L"exception = %s", exception.what())	;
	}
}

