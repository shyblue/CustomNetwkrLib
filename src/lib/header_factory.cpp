#include <stdio.h>

#include "net/header_factory.h"
#include "util/logger.h"

HeaderFactory::HeaderFactory()
{
}

HeaderFactory::~HeaderFactory()
{

}

HeaderFactory* HeaderFactory::GetInstance()
{
	HeaderFactory* p = 0;
	p = &HeaderFactory::get_mutable_instance();
	return p;
}