#pragma once

#ifndef _HEADERDEFINE_H_
#define  _HEADERDEFINE_H_

#include <boost/function.hpp>
#include <boost/serialization/singleton.hpp>
#include "common.h"
#include "net/header.h"
#include "util/logger.h"

class HeaderClosure
{
public:
	HeaderClosure(){}
	virtual ~HeaderClosure(){}
	virtual Header* MakeHeader()=0;
};

template <typename lambda_fun>
class LambdaHeaderClosure  :  public HeaderClosure
{
public:
	LambdaHeaderClosure(lambda_fun& fun):m_headerMaker(fun){}
	~LambdaHeaderClosure(){}
	Header* MakeHeader() { return m_headerMaker(); }
private:
	lambda_fun	m_headerMaker;
};

template <typename lambda_fun>
HeaderClosure* make_header_closure(lambda_fun &fun)
{
	return new LambdaHeaderClosure<lambda_fun>(fun);
}

class HeaderFactory : private boost::serialization::singleton<HeaderFactory>
{
public :

	static HeaderFactory* GetInstance()
	{
		HeaderFactory* p = 0;
		p = &HeaderFactory::get_mutable_instance();
		return p;
	}

	Header* MakeHeader()
	{
		try
		{
			if(m_pClosure)
			{
				Header* p = m_pClosure->MakeHeader();
				return p;
			}

			return nullptr;
		}
		catch(std::exception& exception)
		{
			ST_LOGGER.Error("new Header : %s", exception.what());
			return nullptr;
		}
	}

	bool isSetHeader() { return m_bSetHeader;}
	bool SetClosure(HeaderClosure* p_closure) {
		if(p_closure)
		{
			m_pClosure=p_closure;
			m_bSetHeader=true;
			return true;
		}

		return false;
	}

protected:
	HeaderFactory() {};
	~HeaderFactory(){};
private :
	bool m_bSetHeader;
	HeaderClosure *m_pClosure;
};

#define HEADER_FACTORY() HeaderFactory::GetInstance()

#endif // _HEADERDEFINE_H_
