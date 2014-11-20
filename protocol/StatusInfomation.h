
#pragma once

#ifndef __STATUSINFOMATION_H_
#define __STATUSINFOMATION_H_

#include <boost/tuple/tuple.hpp>

class StatusInfomation {
public :
    typedef boost::tuple<const char*, size_t> result_object;
    static result_object ProtocolProcess(const char * buffer, const size_t size);
private :
    static result_object ErrorResult(int16_t& result_value);
    static size_t Serialize(char* buffer, size_t buffer_size, uint16_t& result_value, uint32_t& use_memory);
    static size_t Deserialize(char* buffer, size_t buffer_size, int32_t& infomation_no);
};

#endif
