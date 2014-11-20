
#include <util/memory_manager.h>
#include <util/logger.h>
#include <util/common_pool.h>
#include <util/byte_order.h>
#include "PlayerDetailInfoPassing.h"


PlayerDetailInfoPassing::result_object PlayerDetailInfoPassing::ProtocolProcess(const char * buffer, const size_t size)
{

    // 클라이언트로부터 전달받은 데이터
    uint64_t request_user_uid;
	uint8_t request_player_type;
	

    // 클라이언트로부터 전달받은 데이터를 처리 합니다.
    Deserialize((char*)buffer, size, request_user_uid, request_player_type);

    int16_t result_value = 0;
    // 버퍼사이즈 점검
    if(size != sizeof(request_user_uid) + sizeof(request_player_type) )
    {
        LOG_ERROR("[PlayerDetailInfoPassing] buffer Size Error");
        return ErrorResult(result_value);
    }

    // 클라이언트에게 전송할 데이터
    uint16_t response_result_value;
	uint32_t response_detailinfo_length;
	char* response_detailinfo;
	
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
    size_t response_size = sizeof(response_result_value) + sizeof(response_detailinfo_length) + response_detailinfo_length ;
    char * response_buffer = COMMON_POOL::_new(response_size);
    size_t response_idx = 0;
    response_index = Serialize(response_buffer, response_size, response_result_value, response_detailinfo_length, response_detailinfo);

    ST_LOGGER.Info("[PlayerDetailInfoPassing] Complete response_index[%d]", response_idx);

    return result_object((const char*)response_buffer, response_idx);
}
    


PlayerDetailInfoPassing::result_object PlayerDetailInfoPassing::ErrorResult(int16_t& result_value)
{
    size_t response_size = sizeof(result_value);
    char * response_buffer = COMMON_POOL::_new(response_size);

    ST_LOGGER.Error("[PlayerDetailInfoPassing][OutputData][ErrorResult] result_value[%d]", result_value);

    size_t buffer_idx = 0;
    result_value = BYTE_ORDER::host_to_network(result_value);
    MEMORY_MANAGER::WriteToBuffer(response_buffer, response_size, buffer_idx, &result_value);

	return result_object((const char*)response_buffer, buffer_idx);
}
    


size_t PlayerDetailInfoPassing::Serialize(char* buffer, size_t buffer_size, uint16_t& result_value, uint32_t& detailinfo_length, char*& detailinfo)
{
    ST_LOGGER.Info("[PlayerDetailInfoPassing][OutputData] result_value[%d], detailinfo_length[%d], detailinfo[%s]", result_value, detailinfo_length, detailinfo);

    size_t buffer_idx = 0;

    result_value = BYTE_ORDER::host_to_network(result_value);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &result_value);
	detailinfo_length = BYTE_ORDER::host_to_network(detailinfo_length);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &detailinfo_length);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, detailinfo, BYTE_ORDER::network_to_host(detailinfo_length));
	

    return buffer_idx;
}
    


size_t PlayerDetailInfoPassing::Deserialize(char* buffer, size_t buffer_size, uint64_t& user_uid, uint8_t& player_type)
{
    size_t buffer_idx = 0;

    MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &user_uid);
	user_uid = BYTE_ORDER::network_to_host(user_uid);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &player_type);
	

    ST_LOGGER.Info("[PlayerDetailInfoPassing][InputData] user_uid[%lld], player_type[%c]", user_uid, player_type);
    return buffer_idx;
}
    

    