#include "stdafx.h"
#include "header_cct.h"

HeaderCCT::HeaderCCT(void)
	:m_header(),m_protocolNo(0),m_dataLen(0)
{
	memset(&m_header,0x0,sizeof(m_header));
	m_sizeOfHeader = sizeof(SD_HEADER);
	m_sizeOfEnder = sizeof(SD_ENDER);
}

HeaderCCT::~HeaderCCT(void)
{
}

size_t HeaderCCT::GetHeaderSize()
{
	return m_sizeOfHeader;
}

size_t HeaderCCT::GetBodySize()
{
	return m_dataLen;
}

size_t HeaderCCT::GetEnderSize()
{
	return m_sizeOfEnder;
}

size_t HeaderCCT::GetTotalSize()
{
	return m_sizeOfHeader + m_dataLen + m_sizeOfEnder;
}

size_t HeaderCCT::Serialize(char* p_buffer, size_t size_of_buffer)
{
	size_t index = 0;


	return index;	
}


size_t HeaderCCT::Deserialize(char* p_buffer, size_t size_of_buffer)
{
	size_t index = 0;

	MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_header, sizeof(SD_HEADER));
	NetworkToHost();
	//MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_header.type, sizeof(m_sign1));
	//MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_sign2, sizeof(m_sign2));
	//MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_protocolNo, sizeof(m_protocolNo));
	//MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_dataLen, sizeof(m_dataLen));
	//MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_handle, sizeof(m_handle));
	//MEMORY_MANAGER::ReadFromBuffer(p_buffer, size_of_buffer, index, &m_findSig, sizeof(m_findSig));

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
bool HeaderCCT::CheckEndmarker(char* buffer,  size_t ender_len)
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
}
void HeaderCCT::NetworkToHost()
{
	m_header.type= BYTE_ORDER::network_to_host(m_header.type);
	m_header.length= BYTE_ORDER::network_to_host(m_header.length);

	m_protocolNo= m_header.type;
	m_dataLen	= m_header.length;
}

size_t HeaderCCT::MakeHeader(uint16_t protocol_no,uint32_t data_len)
{							 
	size_t index = 0;

	m_protocolNo = protocol_no;
	m_dataLen = data_len;

	memcpy( m_header.szSign, "G~E%tXO", 7 ); // m_szSIGN -> "B~LC&pQ"
	m_header.type			= protocol_no;
	m_header.nHandle		= 1;    
	m_header.cFindSig		= 'P'; // FIND_SIG
	m_header.cVer			= uint16_t( 1010 );
	m_header.length			= data_len;
	m_header.wOpt			= uint16_t( rand() % 40 ); // KEY_NUM -> 40 
	m_header.cEncType		= 0; // SDENC_DES

	HostToNetwork();
	return sizeof(m_header);
}

size_t HeaderCCT::MakeEnder(uint32_t packet_count)
{
	m_ender.cVer	= m_header.cVer; // m_nVER
	m_ender.type	= m_header.type;
	m_ender.length	= m_header.length;
	m_ender.wOpt	= m_header.wOpt;

	m_ender.dwSerial= packet_count;

	return sizeof(m_ender);
}