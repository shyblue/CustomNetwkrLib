
#pragma once

#ifndef BUSINESS_PROTOCOL_LEAGUETIMESET_H_
#define BUSINESS_PROTOCOL_LEAGUETIMESET_H_

#include <boost/tuple/tuple.hpp>

class LeagueTimeSet {
public :
    typedef boost::tuple<const char*, size_t> result_object;
    static result_object ProtocolProcess(const char * buffer, const size_t size);
private :
    static result_object ErrorResult(int16_t& result_value);
    static size_t Serialize(char* buffer, size_t buffer_size, uint16_t& result_value);
    static size_t Unserialize(char* buffer, size_t buffer_size, uint64_t& leaguetime);
};

#endif
