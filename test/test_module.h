#pragma once

#ifndef TESTMODULER_TESTMODULE_H_
#define TESTMODULER_TESTMODULE_H_

#include <boost/asio/io_service.hpp>

class Analyzer;
class Random;

class TestModule
{
public:
	TestModule(void);
	~TestModule(void);

	void ConnectSessionPoolRun(boost::asio::io_service * pIo_service);
	void ThreadConnectSessionPool(boost::asio::io_service * pIo_service);

	void CommonPoolTest();
	void CommonPoolTestRun();

	void analysisTest();
	void analysisTestRun(void* arg);

	void DataBaseTest();
	void DataBaseTest_Thread();

	void RandomThread();
	void RandomTest(boost::shared_ptr<Random>& general_rand);

	void RedisThread(boost::asio::io_service* io_service);
	void RedisConnectionTest(boost::asio::io_service* io_service, int theread_id);

private:
	tbb::spin_mutex mutex_;
};

#endif // TESTMODULER_TESTMODULE_H_