
#pragma once

#ifndef BUSINESS_PROTOCOL_ANIMATIONINFOPASSING_H_
#define BUSINESS_PROTOCOL_ANIMATIONINFOPASSING_H_

#include <boost/tuple/tuple.hpp>

class AnimationInfoPassing {
public :
    typedef boost::tuple<const char*, size_t> result_object;
    static result_object ProtocolProcess(const char * buffer, const size_t size);
private :
    static result_object ErrorResult(int16_t& result_value);
    static size_t Serialize(char* buffer, size_t buffer_size, uint16_t& result_value, uint32_t& animation_info_length, char*& animation_info);
    static size_t Unserialize(char* buffer, size_t buffer_size, uint64_t& user_uid, uint64_t& leaguetime, uint32_t& league_no);
};

#endif
