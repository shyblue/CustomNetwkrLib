#include <exception>
#include <boost/make_shared.hpp>

#include "util/pool.h"
#include "util/logger.h"
#include "net/async_session_pool.h"
#include "net/header_factory.h"
#include "net/hive_server.h"

AsyncSessionPool::AsyncSessionPool(HiveServer* p_hive, const PacketWorkerManagerPtr& packet_worker_manager, const size_t pool_cnt)
{
	Initialize(p_hive,packet_worker_manager, pool_cnt);
}

AsyncSessionPool::~AsyncSessionPool(void)
{
}

void AsyncSessionPool::Initialize(HiveServer* p_hive, const PacketWorkerManagerPtr& packet_worker_manager, const size_t pool_amt)
{
	try
	{	
		m_spSessionContainer.reset(new Pool<AsyncSessionImplPtr>(pool_amt));
		size_t nSessionPoolAmt = pool_amt;	

		for(size_t i(0); i < nSessionPoolAmt; ++i)
		{
			boost::shared_ptr<AsyncSessionImpl> p(new AsyncSessionImpl(p_hive, HEADER_FACTORY()->MakeHeader(), packet_worker_manager) );
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

