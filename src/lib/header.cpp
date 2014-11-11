#include <stdint.h>
#include <stddef.h>

#include "util/byte_order.h"
#include "net/header.h"

size_t Header::GetEndMakerSize()
{
	return 0;
}

uint32_t Header::GetNetworkEndMarker()
{
	return 0;
}
