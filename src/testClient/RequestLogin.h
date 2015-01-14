#pragma once

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/stream.hpp>

#include <boost/tuple/tuple.hpp>
#include <string>

class RequestLogin {
public :
	typedef boost::tuple<const char*, size_t> result_object;
	static result_object ProtocolProcess(const char * buffer, const size_t size);

private :
	static result_object ErrorResult(int16_t& result_value);
	static size_t Deserialize(char* buffer, size_t buffer_size,uint16_t &sizeOfUserID,std::string &strUserID,uint16_t &sizeOfPasswd,std::string &strPasswd, uint32_t &loginFlag);
	static size_t Serialize(char* buffer, size_t buffer_size, uint16_t& result_value, uint32_t& use_memory);

	uint32_t nLoginFlag;

	std::string user_id;
	std::string passwd;
	int32_t loginFlag;
};
