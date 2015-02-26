// dummyServer.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <boost/shared_ptr.hpp>
#include "net/header_factory.h"
#include "header_cct.h"
#include "util/memory_manager.h"
#include "net/acceptor.h"
#include "net/async_session_impl.h"
#include "net/network_define.h"
#include "net/hive_server.h"
#include "net/packet_worker_manager.h"

class MySession : public AsyncSessionImpl
{

};
class DummyServer : public HiveServer
{
public:
	DummyServer(const int16_t port, HandlerManager *handler_manager)
		: HiveServer(port,handler_manager)
	{}
private :
	bool OnAccept(AsyncSessionImplPtr pSession)
	{
		HeaderCCT header;
		size_t  index = 0;
		int16_t testValue=1;

		index = header.MakeHeader(102,sizeof(int16_t));

		char*  szResponseBuffer = COMMON_POOL::New(header.GetTotalSize());
		memset(szResponseBuffer,0x0,header.GetTotalSize());

		header.Serialize(szResponseBuffer,header.GetTotalSize());
		MEMORY_MANAGER::WriteToBuffer(szResponseBuffer,header.GetTotalSize(),index,&testValue);

		// Ender 만들고, 붙여넣자
		header.MakeEnder(1);
		MEMORY_MANAGER::WriteToBuffer(szResponseBuffer,header.GetTotalSize(),index,header.GetEnder(),header.GetEnderSize());
		ST_LOGGER.Info("[OnAccept : Send PK_CONNECT to Client][%d]",header.GetTotalSize());

		boost::shared_ptr<PacketInfo> ptr(new PacketInfo(pSession,102,szResponseBuffer,header.GetTotalSize(),0) );
		GetWorkerManager()->Insert(*ptr);

		HeaderCCT header2;
		index=0;
		int32_t tt=1;
		header2.MakeHeader(1010,sizeof(int32_t));
		char*  szBuff= COMMON_POOL::New(header2.GetTotalSize());
		memset(szBuff,0x0,header2.GetTotalSize());

		header2.Serialize(szBuff,header2.GetTotalSize());
		MEMORY_MANAGER::WriteToBuffer(szBuff,header2.GetTotalSize(),index,&tt);

		// Ender 만들고, 붙여넣자
		header2.MakeEnder(2);
		MEMORY_MANAGER::WriteToBuffer(szBuff,header2.GetTotalSize(),index,header2.GetEnder(),header2.GetEnderSize());
		ST_LOGGER.Info("[OnAccept : Send PK_CONNECT_OK to Client][%d]",header2.GetTotalSize());

		boost::shared_ptr<PacketInfo> ptr2(new PacketInfo(pSession,1010,szBuff,header2.GetTotalSize(),0) );
		GetWorkerManager()->Insert(*ptr2);
		
		return true;
	}
};

boost::tuple<int32_t, char*, size_t> TestProtocol(const char* buffer,  size_t buffer_size);
const std::string CONFIG_FILE_NAME("Configure.ini");
int32_t g_serverVersion;

void HandlerSet(HandlerManager *handler_manager);

int _tmain()
{
	ST_LOGGER.Create("log4cxx.xml");
	ST_LOGGER.Info("Dummy Server Start!!!");

	srand((uint32_t)time(NULL));

	ConfigureData* pConf = ST_CONFIG()->GetInstancePtr();
	if(!pConf->LoadCofigureFile(CONFIG_FILE_NAME))
	{
		ST_LOGGER.Error("Load configure file FAIL [Error]");
		return 0;
	}

	ST_LOGGER.Trace("LoadCofigureFile [Success]");

	g_serverVersion = pConf->GetConfigureData<int32_t>("CONFIGURE.VERSION", 143);

	// Header closure 생성
	HeaderClosure* pClosure = make_header_closure( []()->Header* {	return new HeaderCCT();	});
	HEADER_FACTORY()->SetClosure(pClosure);

	HandlerManager *pHandlerManager = new HandlerManager;
	HandlerSet(pHandlerManager);

	std::vector<int32_t> vProtocolList;
	pHandlerManager->ProtocolListGet(vProtocolList);
	ST_LOGGER.Trace("[Loaded protocol list][Start]");
	for(auto protocolNo : vProtocolList)
	{
		ST_LOGGER.Trace("Protocol [%d] loaded",protocolNo);
	}
	ST_LOGGER.Trace("[Loaded protocol list][End]");

	ST_LOGGER.Info("=============== Dummy Server Start ===================");

	DummyServer dummyServer(pConf->GetConfigureData<int16_t>("CONFIGURE.PORT", 12800), pHandlerManager);
	ST_LOGGER.Trace(" Server Run()" );
	dummyServer.Run();

	ST_LOGGER.Trace(" Server Stop()" );
	ST_LOGGER.Info("=============== Dummy Server Terminated ===================");

	return 0;
}

#pragma pack(1)
typedef struct tagCK_KEEPALIVE      // keep alive 검사
{
	char    cRes;
	int     nRes;
}CK_KEEPALIVE;
#pragma pack()

StopWatch test_sw;
std::atomic<int32_t> test_count(0);
boost::tuple<int32_t,char*, size_t> TestProtocol(const char* buffer,  size_t buffer_size)
{
	int32_t nCnt = test_count.fetch_add(1);
	size_t  index = 0;
	int testValue=0;
	HeaderCCT header;
	//	index += header.GetHeaderSize();

	MEMORY_MANAGER::ReadFromBuffer(const_cast<char*>(buffer),buffer_size,index,&testValue,sizeof(testValue));

	if(nCnt == 0)
	{
		test_sw.Start();
	}
	ST_LOGGER.Info("[Packet Handler : TestProtocol] Value[%d] Test nCnt[%d] Elapsemillisec [%d] MilliSec",testValue, nCnt, test_sw.GetElapsedmilliSec().count());

	CK_KEEPALIVE ck;
	ck.cRes = 66;
	ck.nRes = 100;
	index = header.MakeHeader(500,sizeof(ck));

	char*  szResponseBuffer = COMMON_POOL::New(header.GetTotalSize());
	memset(szResponseBuffer,0x0,header.GetTotalSize());

	header.Serialize(szResponseBuffer,header.GetTotalSize());
	MEMORY_MANAGER::WriteToBuffer(szResponseBuffer,header.GetTotalSize(),index,&ck);

	// Ender 만들고, 붙여넣자
	header.MakeEnder(test_count);
	MEMORY_MANAGER::WriteToBuffer(szResponseBuffer,header.GetTotalSize(),index,header.GetEnder(),header.GetEnderSize());

	std::stringstream hexCode;
	for (size_t idx = 0; idx < header.GetTotalSize(); ++idx)
	{
		hexCode << std::hex << static_cast<int16_t>(szResponseBuffer[idx]);
	}
	ST_LOGGER.Info("[Packet Handler : TestProtocol] [HEX CODE : %s][%d]", hexCode.str().c_str(),header.GetTotalSize());

	return boost::tuple<int32_t, char*, size_t>(500, szResponseBuffer, (const size_t&)header.GetTotalSize());
}

boost::tuple<int32_t,char*, size_t> KeepAlive(const char* buffer,  size_t buffer_size)
{
	size_t  index = 0;
	HeaderCCT header;

	CK_KEEPALIVE ck;

	MEMORY_MANAGER::ReadFromBuffer((char*)buffer,buffer_size,index,&ck,sizeof(ck));

	ST_LOGGER.Info("[Packet Handler : KeepAlive] Value[%x] Test nCnt[%d]",ck.cRes, ck.nRes);

	return boost::tuple<int32_t,char*, size_t>(0,nullptr, (const size_t&)0);
}

void HandlerSet(HandlerManager *handler_manager)
{
	handler_manager->ProtocolRegister(101, TestProtocol );
	handler_manager->ProtocolRegister(501, KeepAlive );
}