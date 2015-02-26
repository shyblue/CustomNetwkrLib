#pragma  once

#ifndef BUSINESS_BUSINESSMANAGER_H_
#define BUSINESS_BUSINESSMANAGER_H_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include "util/container.h"

class HandlerManager
{
public:
	typedef boost::tuple<int32_t,char*, size_t> ResultObject;
	typedef boost::function<ResultObject(const char*, size_t)> FunctionObject;
	typedef boost::shared_ptr<MapContainer<const int32_t, FunctionObject>>	FunctionContainer;

	HandlerManager(void);
	~HandlerManager(void);

	ResultObject Processing(const int32_t protocol_no,const char* buffer, const size_t buffer_size);

	void ProtocolRegister(const int32_t protocol_no,FunctionObject func);
	void ProtocolListGet(std::vector<int32_t>& protocol_list);
private:
	FunctionContainer		m_handler;

};

#endif
