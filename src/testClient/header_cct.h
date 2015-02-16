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
	uint8_t		szSign[10];             // header ����
	uint16_t		type;					// struct type(��Ŷ ����)
	uint32_t		length;                 // ��Ŷ ������
	uint32_t		nHandle;                // ��Ŷ�� ������ ID, nHandle
	uint8_t		cEncType;               // ���ڵ� Ÿ��
	uint8_t		szOpt[3];               // �ɼ� <= ������� 24����Ʈ (������), ��������� ���ڵ�ȴ�. 8�� ������ ��������.
	uint8_t		cFindSig;               // FIND_SIG�� �ִ´�. <<= ���⼭���ʹ� ���ڵ����� �ʴ´�. NO_ENCODE�� ���Ҷ� ������ġ
	uint32_t     clientData;                
	uint16_t		wOpt;                   // Header key ��ȣ�� , �� ���� �ڿ� �д�.
};

struct SD_ENDER
{
	uint16_t		cVer;                   // packet version
	uint16_t		type;					// struct type(��Ŷ ����)
	uint32_t		length;					// ��Ŷ ������
	uint16_t		wOpt;  
	uint32_t		dwSerial;               // ��Ŷ �ø��� ��ȣ 2006-05-18
	uint8_t		szOpt[2];               // option, ������� 8�� ��� �� ����
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