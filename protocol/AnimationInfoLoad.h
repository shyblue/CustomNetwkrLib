
#pragma once

#ifndef __ANIMATIONINFOLOAD_H_
#define __ANIMATIONINFOLOAD_H_

#include <boost/tuple/tuple.hpp>

class AnimationInfoLoad {
public :
    typedef boost::tuple<const char*, size_t> result_object;
    static result_object ProtocolProcess(const char * buffer, const size_t size);
private :
    static result_object ErrorResult(int16_t& result_value);
    static size_t Serialize(char* buffer, size_t buffer_size, uint16_t& result_value);
    static size_t Deserialize(char* buffer, size_t buffer_size, uint64_t& leaguetime, uint32_t& league_no, uint32_t& animation_post_path_length, std::string& animation_post_path, uint32_t& animation_regular_path_length, std::string& animation_regular_path);
};

#endif
