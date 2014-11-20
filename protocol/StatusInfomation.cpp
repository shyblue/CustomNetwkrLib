
#include <util/memory_manager.h>
#include <util/logger.h>
#include <util/common_pool.h>
#include <util/byte_order.h>
#include "StatusInfomation.h"


StatusInfomation::result_object StatusInfomation::ProtocolProcess(const char * buffer, const size_t size)
{

    // 클라이언트로부터 전달받은 데이터
    int32_t request_infomation_no;
	

    // 클라이언트로부터 전달받은 데이터를 처리 합니다.
    Deserialize((char*)buffer, size, request_infomation_no);

    int16_t result_value = 0;
    // 버퍼사이즈 점검
    if(size != sizeof(request_infomation_no) )
    {
        LOG_ERROR("[StatusInfomation] buffer Size Error");
        return ErrorResult(result_value);
    }

    // 클라이언트에게 전송할 데이터
    uint16_t response_result_value;
	uint32_t response_use_memory;
	
    // ----------------- [데이터 처리 시작] -----------------
    // TODO: Packet Handler body 작성

    // ----------------- [데이터 처리 완료] -----------------

    // 프로토콜이 정상적으로 처리 안됬을경우
    if(result_value != 1)
    {
        return ErrorResult(result_value);
    }

    response_result_value = static_cast<uint16_t>( result_value );

    // 클라이언트에게 전송할 데이터를 처리 합니다.
    size_t response_size = sizeof(response_result_value) + sizeof(response_use_memory) ;
    char * response_buffer = COMMON_POOL::_new(response_size);
    size_t response_idx = 0;
    response_index = Serialize(response_buffer, response_size, response_result_value, response_use_memory);

    ST_LOGGER.Info("[StatusInfomation] Complete response_index[%d]", response_idx);

    return result_object((const char*)response_buffer, response_idx);
}
    


StatusInfomation::result_object StatusInfomation::ErrorResult(int16_t& result_value)
{
    size_t response_size = sizeof(result_value);
    char * response_buffer = COMMON_POOL::_new(response_size);

    ST_LOGGER.Error("[StatusInfomation][OutputData][ErrorResult] result_value[%d]", result_value);

    size_t buffer_idx = 0;
    result_value = BYTE_ORDER::host_to_network(result_value);
    MEMORY_MANAGER::WriteToBuffer(response_buffer, response_size, buffer_idx, &result_value);

	return result_object((const char*)response_buffer, buffer_idx);
}
    


size_t StatusInfomation::Serialize(char* buffer, size_t buffer_size, uint16_t& result_value, uint32_t& use_memory)
{
    ST_LOGGER.Info("[StatusInfomation][OutputData] result_value[%d], use_memory[%d]", result_value, use_memory);

    size_t buffer_idx = 0;

    result_value = BYTE_ORDER::host_to_network(result_value);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &result_value);
	use_memory = BYTE_ORDER::host_to_network(use_memory);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &use_memory);
	

    return buffer_idx;
}
    


size_t StatusInfomation::Deserialize(char* buffer, size_t buffer_size, int32_t& infomation_no)
{
    size_t buffer_idx = 0;

    MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &infomation_no);
	infomation_no = BYTE_ORDER::network_to_host(infomation_no);
	

    ST_LOGGER.Info("[StatusInfomation][InputData] infomation_no[%d]", infomation_no);
    return buffer_idx;
}
    

    