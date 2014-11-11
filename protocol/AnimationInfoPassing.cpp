
#include <util/memory_manager.h>
#include <util/logger.h>
#include <util/common_pool.h>
#include <util/byte_order.h>
#include "AnimationInfoPassing.h"


AnimationInfoPassing::result_object AnimationInfoPassing::ProtocolProcess(const char * buffer, const size_t size)
{

    // 클라이언트로부터 전달받은 데이터
    uint64_t request_user_uid;
	uint64_t request_leaguetime;
	uint32_t request_league_no;
	

    // 클라이언트로부터 전달받은 데이터를 처리 합니다.
    Unserialize((char*)buffer, size, request_user_uid, request_leaguetime, request_league_no);

    int16_t result_value = 0;
    // 버퍼사이즈 점검
    if(size != sizeof(request_user_uid) + sizeof(request_leaguetime) + sizeof(request_league_no) )
    {
        LOG_ERROR("[AnimationInfoPassing] buffer Size Error");
        return ErrorResult(result_value);
    }

    // 클라이언트에게 전송할 데이터
    uint16_t response_result_value;
	uint32_t response_animation_info_length;
	char* response_animation_info;
	
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
    size_t response_size = sizeof(response_result_value) + sizeof(response_animation_info_length) + response_animation_info_length ;
    char * response_buffer = COMMON_POOL::_new(response_size);
    size_t response_idx = 0;
    response_index = Serialize(response_buffer, response_size, response_result_value, response_animation_info_length, response_animation_info);

    ST_LOGGER.Info("[AnimationInfoPassing] Complete response_index[%d]", response_idx);

    return result_object((const char*)response_buffer, response_idx);
}
    


AnimationInfoPassing::result_object AnimationInfoPassing::ErrorResult(int16_t& result_value)
{
    size_t response_size = sizeof(result_value);
    char * response_buffer = COMMON_POOL::_new(response_size);

    ST_LOGGER.Error("[AnimationInfoPassing][OutputData][ErrorResult] result_value[%d]", result_value);

    size_t buffer_idx = 0;
    result_value = BYTE_ORDER::host_to_network(result_value);
    MEMORY_MANAGER::WriteToBuffer(response_buffer, response_size, buffer_idx, &result_value);

	return result_object((const char*)response_buffer, buffer_idx);
}
    


size_t AnimationInfoPassing::Serialize(char* buffer, size_t buffer_size, uint16_t& result_value, uint32_t& animation_info_length, char*& animation_info)
{
    ST_LOGGER.Info("[AnimationInfoPassing][OutputData] result_value[%d], animation_info_length[%d], animation_info[%s]", result_value, animation_info_length, animation_info);

    size_t buffer_idx = 0;

    result_value = BYTE_ORDER::host_to_network(result_value);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &result_value);
	animation_info_length = BYTE_ORDER::host_to_network(animation_info_length);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &animation_info_length);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, animation_info, BYTE_ORDER::network_to_host(animation_info_length));
	

    return buffer_idx;
}
    


size_t AnimationInfoPassing::Unserialize(char* buffer, size_t buffer_size, uint64_t& user_uid, uint64_t& leaguetime, uint32_t& league_no)
{
    size_t buffer_idx = 0;

    MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &user_uid);
	user_uid = BYTE_ORDER::network_to_host(user_uid);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &leaguetime);
	leaguetime = BYTE_ORDER::network_to_host(leaguetime);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &league_no);
	league_no = BYTE_ORDER::network_to_host(league_no);
	

    ST_LOGGER.Info("[AnimationInfoPassing][InputData] user_uid[%lld], leaguetime[%lld], league_no[%d]", user_uid, leaguetime, league_no);
    return buffer_idx;
}
    

    