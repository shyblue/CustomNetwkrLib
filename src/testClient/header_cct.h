#pragma once

#ifndef _HEADER_CCT_H_
#define _HEADER_CCT_H_

#include <stddef.h>
#include "common.h"
#include "net/header.h"

#pragma pack( 1 )
struct SD_HEADER
{
	uint16_t		cVer;                   // packet version
	uint8_t		szSign[10];             // header 구분
	uint16_t		type;					// struct type(팩킷 구분)
	uint32_t		length;                 // 팩킷 사이즈
	uint32_t		nHandle;                // 팩킷의 소유자 ID, nHandle
	uint8_t		cEncType;               // 인코딩 타입
	uint8_t		szOpt[3];               // 옵션 <= 여기까지 24바이트 (꼭지켜), 여기까지만 인코드된다. 8로 나누어 떨어진다.
	uint8_t		cFindSig;               // FIND_SIG를 넣는다. <<= 여기서부터는 인코딩하지 않는다. NO_ENCODE을 정할때 시작위치
	uint32_t     clientData;                
	uint16_t		wOpt;                   // Header key 번호값 , 꼭 제일 뒤에 둔다.
};

struct SD_ENDER
{
	uint16_t		cVer;                   // packet version
	uint16_t		type;					// struct type(팩킷 구분)
	uint32_t		length;					// 팩킷 사이즈
	uint16_t		wOpt;  
	uint32_t		dwSerial;               // 팩킷 시리얼 번호 2006-05-18
	uint8_t		szOpt[2];               // option, 여기까지 8의 배수 꼭 지켜
};
#pragma pack()

class HeaderCCT : public Header
{
public:
	HeaderCCT(void);
	~HeaderCCT(void);


	virtual size_t GetHeaderSize();
	virtual size_t GetTotalSize();
	virtual size_t GetBodySize();
	virtual size_t GetEnderSize();
	virtual size_t Serialize(char* buffer, size_t buffer_size);

	virtual size_t Deserialize(char* buffer, size_t buffer_size);
	virtual uint16_t GetProtocolNo();

	virtual bool Validate();
	virtual bool CheckEndmarker(char* buffer, size_t end_len);

	virtual void SetHeaderSize(size_t size);
	virtual void SetDataSize(size_t size);

	virtual void HostToNetwork();
	virtual void NetworkToHost();

	size_t MakeHeader(uint16_t protocol_no, uint32_t data_len);
	size_t MakeEnder(uint32_t packet_count);
	SD_ENDER* GetEnder() { return &m_ender; }

	SD_HEADER m_header;
	SD_ENDER  m_ender;

	uint16_t m_protocolNo;
	uint32_t m_dataLen;

};
#endif // _HEADER_CCT_H_