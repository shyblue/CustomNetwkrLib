#include <iostream>
#include <boost/thread.hpp>
#include <boost/format.hpp>
#include <tbb/spin_mutex.h>

#include "net/header_wserver.h"
#include "net/sync_session.h"
#include "util/byte_order.h"
#include "net/connection_session_pool.h"
#include "util/common_pool.h"
#include "util/memory_manager.h"
#include "util/analysis_process.h"
#include "util/logger.h"
#include "util/analyzer.h"
#include "util/cache_manager.h"
#include "util/cache_factory.h"
#include "util/configure.h"
#include "rand/random_factory.h"
#include "rand/random.h"
#include "util/stop_watch.h"
#include "util/encoder.h"
#include "db/redis_connector.h"
#include "db/db_connector.h"
#include "test_module.h"

TestModule::TestModule(void)
{
	CACHE_FACTORY()->Initialize();

}

TestModule::~TestModule(void)
{
}

void TestModule::CommonPoolTestRun()
{
	boost::thread_group thread_group;
	thread_group.create_thread(boost::bind(&TestModule::CommonPoolTest, this));
	for(int32_t i = 0; i < 8; ++i)
		thread_group.add_thread(new boost::thread(&TestModule::CommonPoolTest, this));
}

void TestModule::CommonPoolTest()
{
	for(int i = 0;i < 50000;++i)
	{
		buffer = COMMON_POOL::_new(10);
		COMMON_POOL::_delete(buffer, 10);
	}
}

void TestModule::ConnectSessionPoolRun(boost::asio::io_service * pIo_service)
{

	boost::thread_group thread_group;

	thread_group.create_thread(boost::bind(&TestModule::ThreadConnectSessionPool, this, pIo_service));
	
	for(int32_t i = 0; i < 100; ++i)
		thread_group.add_thread(new boost::thread(&TestModule::ThreadConnectSessionPool, this, pIo_service));
	
}

void TestModule::ThreadConnectSessionPool(boost::asio::io_service * pIo_service)
{
	/// connection session Test
	timeval tm;
	tm.tv_sec = 1;
	tm.tv_usec = 0;

	ConnectionSessionPool csPool(pIo_service, 128, "jinny20123.iptime.org", "10030", tm, tm, 1000);
	StopWatch s;
	s.Reset();
	for(int i = 0; i < 500; ++i)
	{
		const size_t kbuffersize = 1024;
		char send_buffer[kbuffersize] = {0,};
		size_t index = 0;

		int temp = i;
		HeaderWServer header;



		index += header.GetHeaderSize();

		uint64_t useruid = 201212210000000004;
		memcpy(send_buffer+index, &useruid, sizeof(useruid));
		index += sizeof(useruid);

			
		unsigned int kEndMarker = 0xC8D1;
		memcpy(send_buffer+index, &kEndMarker, sizeof(kEndMarker));
		index += sizeof(kEndMarker);

			
		// header data set
		header.Serialize(static_cast<uint32_t>(index), 101, 0, 0xFFFF, 1, 0, (uint32_t)(index - header.GetHeaderSize() - sizeof(kEndMarker)));
		header.Unserialize(send_buffer, kbuffersize);



		char recv_buffer[kbuffersize] = {0,};
		size_t recv_buffer_size = 16;
	
		//LOG_DEBUG("sendsize = %d", index)

		if(!csPool.PacketProcess(send_buffer, index, recv_buffer, recv_buffer_size))
			LOG_ERROR("CONNECTIONSESSIONPOOL()->PacketProcess Fail");
	}
	LOG_INFO("ThreadConnectSessionPool militime(%d)", s.GetElapsedmilliSec().count());
}

void TestModule::analysisTest()
{
}

void TestModule::analysisTestRun(void* arg)
{
	AnalysisProcess* analysisTest = (AnalysisProcess*)arg;

	while(1)
	{
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}
}

void TestModule::DataBaseTest_Thread()
{
	boost::thread_group thread_group;

	thread_group.create_thread(boost::bind(&TestModule::DataBaseTest, this));
	for(int32_t i = 0; i < 3; ++i)
		thread_group.add_thread(new boost::thread(&TestModule::DataBaseTest, this));

	thread_group.join_all();
}

void TestModule::DataBaseTest()
{
	StopWatch sw;

	DBConnector db_connector;
	if(!db_connector.Connet("jinny20123.iptime.org", "ahn", "wjddnd0"))
	{
		LOG_INFO("No Connection");
		return;
	}

	boost::shared_ptr<Random> general((Random*) ST_RAND_FACTORY()->MakeGeneralRandom(50000));

	int count = 5000;
	sw.Reset();

	for(int i = 0; i < count; ++i)
	{
		uint32_t nRandIdx = general->GetValue();
		std::stringstream str;
		str << boost::format("SELECT * FROM dbtest_long where `key` > %1% and `key` < %2%") % nRandIdx % (nRandIdx + 400);
		
		db_connector.ExcuteSQL(str.str().c_str());

		int	value[10] = {0,};

		for(int bindidx =0; bindidx < 10; bindidx++)
			db_connector.BindCol(bindidx+1, value[bindidx]);

		while(db_connector.Fetch())
		{
			//LOG_INFO("DB buffer(%s)",buffer);
		}
	
	}

	LOG_INFO("DB Connection(%d): %d",count, sw.GetElapsedmilliSec());
}

void TestModule::RandomThread()
{
	boost::thread_group thread_group;

	LOG_INFO("start redisstart");

	boost::shared_ptr<Random> generalrand((Random*) ST_RAND_FACTORY()->MakeGeneralRandom(10000));

	thread_group.create_thread(boost::bind(&TestModule::RandomTest, this, generalrand));
	for(int32_t i = 0; i < 7; ++i)
		thread_group.add_thread(new boost::thread(&TestModule::RandomTest, this, generalrand));

	thread_group.join_all();
}

void TestModule::RandomTest(boost::shared_ptr<Random>& general_rand)
{

	StopWatch s;

	s.Reset();
	for(int i = 0; i < 10000000; ++i)
	{
		general_rand->GetValue();
	}
	LOG_INFO("gtime = %d", s.GetElapsedmilliSec().count());
}

void TestModule::RedisThread(boost::asio::io_service* io_service)
{
	boost::thread_group thread_group;

	LOG_INFO("start redisstart");
	thread_group.create_thread(boost::bind(&TestModule::RedisConnectionTest, this, io_service, 1));
	for(int32_t i = 0; i < 7; ++i)
		thread_group.add_thread(new boost::thread(&TestModule::RedisConnectionTest, this, io_service, i + 2));

	thread_group.join_all();
}

void TestModule::RedisConnectionTest(boost::asio::io_service* io_service, int thereadid)
{
	timeval tm;
	tm.tv_sec = 1;
	tm.tv_usec = 0;

	RedisConnector redis(io_service, "127.0.0.1", "0", tm);

	boost::shared_ptr<Random> general((Random*) ST_RAND_FACTORY()->MakeGuaranteeRandom(40000));

	StopWatch sw;
	int count = 100000 * thereadid;

	char buffer[4096] = {0,};

	//sprintf(query, "GET a\r\n");

	sw.Reset();
	for(int i = count-100000;i < count; ++i)
	{
		std::stringstream str("");

		//str << boost::format("SET %1% %1%,%2%,%3%,%4%,%5%,%6%,%7%,%8%,%9%,%10%\r\n") % i % general->GetValue() % general->GetValue() % general->GetValue() % general->GetValue() % general->GetValue() % general->GetValue() % general->GetValue() % general->GetValue() % general->GetValue();
		//str << boost::format("MGET ");
		/*
		uint32_t randidx = general->GetValue();
		for(int makeidx = 0; makeidx < 400; ++makeidx)
		{	
			char buffer[8] = {0,};
			sprintf(buffer, "%d ", randidx + makeidx);
			str << buffer;
		}
		*/

		str << boost::format("ZADD score %1% %2%") % general->GetValue()% i;
		str << "\r\n";


		if(!redis.Send(str.str().c_str(), str.str().size()))
		{
			LOG_INFO("Redis Send False");
		}
		if(!redis.Recv(buffer, 4096))
		{
			LOG_INFO("Redis Recv False");
		}

		//boost::this_thread::sleep(boost::posix_time::millisec(50));
	}

	LOG_INFO("Redis Connection(%d): %d",count, sw.GetElapsedmilliSec())
}