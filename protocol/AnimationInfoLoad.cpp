
#include <util/memory_manager.h>
#include <util/logger.h>
#include <util/common_pool.h>
#include <util/byte_order.h>
#include "AnimationInfoLoad.h"


AnimationInfoLoad::result_object AnimationInfoLoad::ProtocolProcess(const char * buffer, const size_t size)
{

    // 클라이언트로부터 전달받은 데이터
    uint64_t request_leaguetime;
	uint32_t request_league_no;
	uint32_t request_animation_post_path_length;
	std::string request_animation_post_path;
	uint32_t request_animation_regular_path_length;
	std::string request_animation_regular_path;
	

    // 클라이언트로부터 전달받은 데이터를 처리 합니다.
    Deserialize((char*)buffer, size, request_leaguetime, request_league_no, request_animation_post_path_length, request_animation_post_path, request_animation_regular_path_length, request_animation_regular_path);

    int16_t result_value = 0;
    // 버퍼사이즈 점검
    if(size != sizeof(request_leaguetime) + sizeof(request_league_no) + sizeof(request_animation_post_path_length) + request_animation_post_path_length + sizeof(request_animation_regular_path_length) + request_animation_regular_path_length )
    {
        LOG_ERROR("[AnimationInfoLoad] buffer Size Error");
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

    ST_LOGGER.Info("[AnimationInfoLoad] Complete response_index[%d]", response_idx);

    return result_object((const char*)response_buffer, response_idx);
}
    


AnimationInfoLoad::result_object AnimationInfoLoad::ErrorResult(int16_t& result_value)
{
    size_t response_size = sizeof(result_value);
    char * response_buffer = COMMON_POOL::_new(response_size);

    ST_LOGGER.Error("[AnimationInfoLoad][OutputData][ErrorResult] result_value[%d]", result_value);

    size_t buffer_idx = 0;
    result_value = BYTE_ORDER::host_to_network(result_value);
    MEMORY_MANAGER::WriteToBuffer(response_buffer, response_size, buffer_idx, &result_value);

	return result_object((const char*)response_buffer, buffer_idx);
}
    


size_t AnimationInfoLoad::Serialize(char* buffer, size_t buffer_size, uint16_t& result_value)
{
    ST_LOGGER.Info("[AnimationInfoLoad][OutputData] result_value[%d]", result_value);

    size_t buffer_idx = 0;

    result_value = BYTE_ORDER::host_to_network(result_value);
	MEMORY_MANAGER::WriteToBuffer(buffer, buffer_size, buffer_idx, &result_value);
	

    return buffer_idx;
}
    


size_t AnimationInfoLoad::Deserialize(char* buffer, size_t buffer_size, uint64_t& leaguetime, uint32_t& league_no, uint32_t& animation_post_path_length, std::string& animation_post_path, uint32_t& animation_regular_path_length, std::string& animation_regular_path)
{
    size_t buffer_idx = 0;

    MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &leaguetime);
	leaguetime = BYTE_ORDER::network_to_host(leaguetime);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &league_no);
	league_no = BYTE_ORDER::network_to_host(league_no);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &animation_post_path_length);
	animation_post_path_length = BYTE_ORDER::network_to_host(animation_post_path_length);
	animation_post_path.resize(animation_post_path_length);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &animation_post_path[0], animation_post_path_length);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &animation_regular_path_length);
	animation_regular_path_length = BYTE_ORDER::network_to_host(animation_regular_path_length);
	animation_regular_path.resize(animation_regular_path_length);
	MEMORY_MANAGER::ReadFromBuffer(buffer, buffer_size, buffer_idx, &animation_regular_path[0], animation_regular_path_length);
	

    ST_LOGGER.Info("[AnimationInfoLoad][InputData] leaguetime[%lld], league_no[%d], animation_post_path_length[%d], animation_post_path[%s], animation_regular_path_length[%d], animation_regular_path[%s]", leaguetime, league_no, animation_post_path_length, animation_post_path.c_str(), animation_regular_path_length, animation_regular_path.c_str());
    return buffer_idx;
}
    

    