
#include <util/memory_manager.h>
#include <util/logger.h>
#include <util/common_pool.h>
#include <util/byte_order.h>
#include "PlayerDetailInfoLoad.h"


PlayerDetailInfoLoad::result_object PlayerDetailInfoLoad::ProtocolProcess(const char * buffer, const size_t size)
{

    // 클라이언트로부터 전달받은 데이터
    uint64_t request_leaguetime;
	int32_t request_pitcher_path_length;
	std::string request_pitcher_path;
	uint32_t request_fielder_path_length;
	std::string request_fielder_path;
	

    // 클라이언트로부터 전달받은 데이터를 처리 합니다.
    Unserialize((char*)buffer, size, request_leaguetime, request_pitcher_path_length, request_pitcher_path, request_fielder_path_length, request_fielder_path);

    int16_t result_value = 0;
    // 버퍼사이즈 점검
    if(size != sizeof(request_leaguetime) + sizeof(request_pitcher_path_length) + request_pitcher_path_length + sizeof(request_fielder_path_length) + request_fielder_path_length )
    {
        LOG_ERROR("[PlayerDetailInfoLoad] buffer Size Error");
        return ErrorResult(result_value);
    }

    // 클라이언트에게 전송할 데이터
    uint16_t response_result_value;
	
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
    size_t response_size = sizeof(response_result_value) ;
    char * response_buffer = COMMON_POOL::_new(response_size);
    size_t response_idx = 0;
    response_index = Serialize(response_buffer, response_size, response_result_value);

    ST_LOGGER.Info("[PlayerDetailInfoLoad] Complete response_index[%d]", response_idx);

    return result_object((const char*)response_buffer, response_idx);
}
    


PlayerDetailInfoLoad::result_object PlayerDetailInfoLoad::ErrorResult(int16_t& result_value)
{
    size_t response_size = sizeof(result_value);
    char * response_buffer = COMMON_POOL::_new(response_size);

    ST_LOGGER.Error("[PlayerDetailInfoLoad][OutputData][ErrorResult] result_value[%d]", result_value);

    size_t buffer_idx = 0;
    result_value = BYTE_ORDER::host_to_network(result_value);
    MEMORY_MANAGER::WriteToBuffer(response_buffer, response_size, buffer_idx, &result_value);

	return result_object((const char*)response_buffer, buffer_idx);
}
    


size_t PlayerDetailInfoLoad::Serialize(char* buffer, size_t buffer_size, uint16_t& result_value)
{
    ST_LOGGER.Info("[PlayerDetailInfoLoad][OutputData] result_value[%d]", result_value);

    size_t buffer_idx = 0;

    result_value = BYTE_ORDER::host_to_network(result_value);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &result_value);
	

    return buffer_idx;
}
    


size_t PlayerDetailInfoLoad::Unserialize(char* buffer, size_t buffer_size, uint64_t& leaguetime, int32_t& pitcher_path_length, std::string& pitcher_path, uint32_t& fielder_path_length, std::string& fielder_path)
{
    size_t buffer_idx = 0;

    MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &leaguetime);
	leaguetime = BYTE_ORDER::network_to_host(leaguetime);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &pitcher_path_length);
	pitcher_path_length = BYTE_ORDER::network_to_host(pitcher_path_length);
	pitcher_path.resize(pitcher_path_length);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &pitcher_path[0], pitcher_path_length);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &fielder_path_length);
	fielder_path_length = BYTE_ORDER::network_to_host(fielder_path_length);
	fielder_path.resize(fielder_path_length);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &fielder_path[0], fielder_path_length);
	

    ST_LOGGER.Info("[PlayerDetailInfoLoad][InputData] leaguetime[%lld], pitcher_path_length[%d], pitcher_path[%s], fielder_path_length[%d], fielder_path[%s]", leaguetime, pitcher_path_length, pitcher_path.c_str(), fielder_path_length, fielder_path.c_str());
    return buffer_idx;
}
    

    