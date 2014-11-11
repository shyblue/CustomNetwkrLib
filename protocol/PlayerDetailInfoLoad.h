
#pragma once

#ifndef BUSINESS_PROTOCOL_PLAYERDETAILINFOLOAD_H_
#define BUSINESS_PROTOCOL_PLAYERDETAILINFOLOAD_H_

#include <boost/tuple/tuple.hpp>

class PlayerDetailInfoLoad {
public :
    typedef boost::tuple<const char*, size_t> result_object;
    static result_object ProtocolProcess(const char * buffer, const size_t size);
private :
    static result_object ErrorResult(int16_t& result_value);
    static size_t Serialize(char* buffer, size_t buffer_size, uint16_t& result_value);
    static size_t Unserialize(char* buffer, size_t buffer_size, uint64_t& leaguetime, int32_t& pitcher_path_length, std::string& pitcher_path, uint32_t& fielder_path_length, std::string& fielder_path);
};

#endif
