// dummyServer.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "net/header_factory.h"
#include "header_cct.h"
#include "util/memory_manager.h"

boost::tuple<const char*, size_t> TestProtocol(const char* buffer,  size_t buffer_size);
const std::string CONFIG_FILE_NAME("Configure.ini");
int32_t g_serverVersion;

void HandlerSet(HandlerManager *handler_manager);

int _tmain()
{
	ST_LOGGER.Create("log4cxx.xml");																			   z
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

	// Header closure »ý¼º
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

	ST_LOGGER.Info("=============== Dummy Server by libSkeleton Start ===================");

	SkltServer dummyServer(pConf->GetConfigureData<int16_t>("CONFIGURE.PORT", 12800), pHandlerManager);
	ST_LOGGER.Trace(" Server Run()" );
	dummyServer.Run();

	return 0;
}

StopWatch test_sw;
std::atomic<int32_t> test_count(0);
boost::tuple<const char*, size_t> TestProtocol(const char* buffer,  size_t buffer_size)
{
	int32_t nCnt = test_count.fetch_add(1);
	size_t  index = 0;
	int testValue=0;
	HeaderCCT header;
//	index += header.GetHeaderSize();

	MEMORY_MANAGER::ReadFromBuffer((char*)buffer,buffer_size,index,&testValue,sizeof(testValue));

	if(nCnt == 0)
	{
		test_sw.Start();
	}
	ST_LOGGER.Info("[Packet Handler : TestProtocol] Value[%d] Test nCnt[%d] Elapsemillisec [%d] MilliSec",testValue, nCnt, test_sw.GetElapsedmilliSec().count());

	int32_t idxResponse = 0;
	idxResponse = 1004 + nCnt;
	header.Set('S','M','C',101,sizeof(idxResponse),(int32_t)1,0x01);

	char*  szResponseBuffer = COMMON_POOL::New(header.GetTotalSize());
	memset(szResponseBuffer,0x0,16);

	header.Serialize(szResponseBuffer,header.GetTotalSize());
	index = header.GetHeaderSize();
	MEMORY_MANAGER::WriteToBuffer(szResponseBuffer,header.GetTotalSize(),index,&idxResponse);

	//std::stringstream hexCode;
	//for (size_t idx = 0; idx < header.GetTotalSize(); ++idx)
	//{
	//	hexCode << std::hex << static_cast<int16_t>(szResponseBuffer[idx]);
	//}
	//ST_LOGGER.Info("[Packet Handler : TestProtocol] [HEX CODE : %s][%d] Response[%d]", hexCode.str().c_str(),header.GetTotalSize(),idxResponse);

	return boost::tuple<const char*, size_t>(const_cast<const char*>(szResponseBuffer), (const size_t&)header.GetTotalSize());
}

void HandlerSet(HandlerManager *handler_manager)
{
	handler_manager->ProtocolRegister(101, TestProtocol );
}