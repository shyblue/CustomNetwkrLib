#include "stdafx.h"
#include "header_cct.h"
#include "util/byte_order.h"
#include "util/memory_manager.h"
#include "util/logger.h"

HeaderCCT::HeaderCCT(void)
	: m_version(0x01),m_sign1('S'),m_sign2('M'),m_protocolNo(0),m_dataLen(0),m_handle(0),m_findSig('P')
{
	m_sizeOfHeader = sizeof(m_version) + sizeof(m_sign1) + sizeof(m_sign2) + sizeof(m_protocolNo) + sizeof(m_dataLen) + sizeof(m_handle) + sizeof(m_findSig);
}

HeaderCCT::~HeaderCCT(void)
{
}

size_t HeaderCCT::GetHeaderSize()
{
	return m_sizeOfHeader;
}

size_t HeaderCCT::GetTotalSize()
{
	return m_sizeOfHeader + m_dataLen;
}

size_t HeaderCCT::GetBodySize()
{
	return m_dataLen;
}

size_t HeaderCCT::Serialize(char* p_buffer, size_t size_of_buffer)
{
	size_t index = 0;

	MEMORY_MANAGER::WriteToBuffer(p_buffer, size_of_buffer, index, &m_version, sizeof(m_version));
	MEMORY_MANAGER::WriteToBuffer(p_buffer, size_of_buffer, index, &m_sign1, sizeof(m_sign1));
	MEMORY_MANAGER::WriteToBuffer(p_buffer, size_of_buffer, index, &m_sign2, sizeof(m_sign2));
	MEMORY_MANAGER::WriteToBuffer(p_buffer, size_of_buffer, index, &m_protocolNo, sizeof(m_protocolNo));
	MEMORY_MANAGER::WriteToBuffer(p_buffer, size_of_buffer, index, &m_dataLen, sizeof(m_dataLen));
	MEMORY_MANAGER::WriteToBuffer(p_buffer, size_of_buffer, index, &m_handle, sizeof(m_handle));
	MEMORY_MANAGER::WriteToBuffer(p_buffer, size_of_buffer, index, &m_findSig, sizeof(m_findSig));

	return index;	
}

size_t HeaderCCT::Deserialize(char* p_buffer, size_t size_of_buffer)
{
	size_t index = 0;

	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_version, sizeof(m_version));
	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_sign1, sizeof(m_sign1));
	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_sign2, sizeof(m_sign2));
	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_protocolNo, sizeof(m_protocolNo));
	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_dataLen, sizeof(m_dataLen));
	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_handle, sizeof(m_handle));
	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_findSig, sizeof(m_findSig));

	return index;
}

uint16_t HeaderCCT::GetProtocolNo()
{
	return m_protocolNo;
}

bool HeaderCCT::Validate()
{
	return true;
}
bool HeaderCCT::CheckEndmarker(char* buffer,  size_t length)
{
	return true;
}

void HeaderCCT::SetHeaderSize(size_t size)
{
	m_sizeOfHeader = size;
}
void HeaderCCT::SetDataSize(size_t size)
{
	if(size > MAXUINT16)
	{
		ST_LOGGER.Error("[HeaderCCT] Data size overflow [%d:%d]",MAXUINT16,size);
		m_dataLen = MAXUINT16;
	}
	else
	{
		m_dataLen = (uint16_t)size;
	}
}

void HeaderCCT::HostToNetwork()
{
	m_protocolNo= BYTE_ORDER::host_to_network(m_protocolNo);
	m_dataLen	= BYTE_ORDER::host_to_network(m_dataLen);
	m_handle	= BYTE_ORDER::host_to_network(m_handle);
}
void HeaderCCT::NetworkToHost()
{
	m_protocolNo= BYTE_ORDER::network_to_host(m_protocolNo);
	m_dataLen	= BYTE_ORDER::network_to_host(m_dataLen);
	m_handle	= BYTE_ORDER::network_to_host(m_handle);
}

size_t HeaderCCT::Set(char version,char sign1,char sign2,uint16_t protocolNo,uint16_t dataLen,int32_t handle,char find_sig)
{							 
	size_t index = 0;

	m_version = version;
	m_sign1 = sign1;
	m_sign2 = sign2;
	m_protocolNo = protocolNo;
	m_dataLen = dataLen;
	m_handle = handle;
	m_findSig = find_sig;

	return index;
}