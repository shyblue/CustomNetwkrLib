#include "stdafx.h"

#include <util/memory_manager.h>
#include <util/logger.h>
#include <util/common_pool.h>
#include <util/byte_order.h>
#include <util/logger.h>

#include "RequestLogin.h"

RequestLogin::result_object RequestLogin::ProtocolProcess(const char * buffer, const size_t size)
{

	int16_t resultValue=0;
	// Data from network transferred
	uint16_t sizeOfUserID;
	std::string strUserID;
	uint16_t sizeOfPasswd;
	std::string strPasswd;
	uint32_t nLoginFlag=0;

	// Deserialize from network buffer to local data variable
	Deserialize((char*)buffer, size, sizeOfUserID, strUserID, sizeOfPasswd, strPasswd, nLoginFlag);
	size_t sizeOfProtocol = sizeof(uint16_t) + sizeOfUserID + sizeof(uint16_t) + sizeOfPasswd + sizeof(nLoginFlag);

	// check size between buffer and variable
	if(size != sizeOfProtocol )
	{
		ST_LOGGER.Error("[RequestLogin] buffer Size Error [%d:%d]", size, sizeof(resultValue) );
		return ErrorResult(resultValue);
	}

	// Response protocol data
	uint16_t response_result_value;
	uint32_t response_use_memory;
	
	// ----------------- [Protocol processing start] -----------------

	// ----------------- [Protocol processing end] -----------------

	// Protocol processing error
	if(resultValue != 1)
	{
		return ErrorResult(resultValue);
	}

	response_result_value = resultValue;

	// Make response protocol
	size_t response_size = sizeof(response_result_value) + sizeof(response_use_memory) ;
	char * response_buffer = COMMON_POOL::New(response_size);
	size_t response_index = 0;
	response_index = Serialize(response_buffer, response_size, response_result_value, response_use_memory);

	ST_LOGGER.Info("[RequestLogin] Complete response_index[%lu]", response_index);

	return result_object((const char*)response_buffer, response_index);
}

RequestLogin::result_object RequestLogin::ErrorResult(int16_t& result_value)
{
	size_t response_size = sizeof(result_value);
	char * response_buffer = COMMON_POOL::New(response_size);

	ST_LOGGER.Error("[RequestLogin][OutputData][ErrorResult] result_value[%d]", result_value);

	size_t bufferindex = 0;
	result_value = BYTE_ORDER::host_to_network(result_value);
	MEMORY_MANAGER::WriteToBuffer(response_buffer, response_size, bufferindex, &result_value);

	return result_object((const char*)response_buffer, bufferindex);	
}

size_t RequestLogin::Serialize(char* buffer, size_t buffer_size, uint16_t& result_value, uint32_t& use_memory)
{
	ST_LOGGER.Info("[RequestLogin][OutputData] result_value[%d], use_memory[%d]", result_value, use_memory);

	size_t bufferindex = 0;

	result_value = BYTE_ORDER::host_to_network(result_value);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, bufferindex, &result_value);
	use_memory = BYTE_ORDER::host_to_network(use_memory);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, bufferindex, &use_memory);
	

	return bufferindex;
}

size_t RequestLogin::Deserialize(char* buffer, size_t buffer_size,uint16_t &sizeOfUserID,std::string &strUserID,uint16_t &sizeOfPasswd,std::string &strPasswd, uint32_t &login_flag)
{
	size_t bufferindex = 0;

	return bufferindex;
}