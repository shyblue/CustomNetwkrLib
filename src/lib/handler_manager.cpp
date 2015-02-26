#include <boost/function.hpp>
#include <boost/tuple/tuple.hpp>

#include "util/logger.h"
#include "handler/handler_manager.h"

HandlerManager::HandlerManager(void) :
	m_handler(new MapContainer<const int32_t, FunctionObject>())
{
}

HandlerManager::~HandlerManager(void)
{
}


HandlerManager::ResultObject HandlerManager::Processing(const int32_t protocol_no,const char* buffer, const size_t size_of_buffer)
{
	FunctionObject* objFunc = m_handler->Getvalue(protocol_no);

	if(objFunc)
	{
		return objFunc->operator()(buffer, size_of_buffer);
		ST_LOGGER.Trace("find protocol_no(%d), buffersize(%d)", protocol_no, size_of_buffer);
	}

	ST_LOGGER.Error("NOT find protocol_no(%d)", protocol_no);
	return ResultObject();
}

void HandlerManager::ProtocolRegister(const int32_t protocol_no, FunctionObject func_object)
{
	if(m_handler->Insert(protocol_no, func_object))
	{
		ST_LOGGER.Trace("ProtocolRegister(%d)", protocol_no);	
	}
	else
	{
		ST_LOGGER.Error("FAIL ProtocolRegister(%d)", protocol_no);	
	}
}

void HandlerManager::ProtocolListGet(std::vector<int32_t>& protocol_list)
{	
	auto container = m_handler->GetContainer();

    for(auto key : container)
	{
		protocol_list.push_back(key.first);
	}
}