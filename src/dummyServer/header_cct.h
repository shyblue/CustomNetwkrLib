#pragma once

#ifndef _HEADER_CCT_H_
#define _HEADER_CCT_H_

#include <stddef.h>
#include "common.h"
#include "net/header.h"

class HeaderCCT : public Header
{
public:
	HeaderCCT(void);
	~HeaderCCT(void);


	virtual size_t GetHeaderSize();
	virtual size_t GetTotalSize();
	virtual size_t GetBodySize();
	virtual size_t Serialize(char* buffer, size_t buffersize);

	virtual size_t Deserialize(char* buffer, size_t buffersize);
	virtual uint16_t GetProtocolNo();

	virtual bool Validate();
	virtual bool CheckEndmarker(char* buffer, size_t length);

	virtual void SetHeaderSize(size_t size);
	virtual void SetDataSize(size_t size);

	virtual void HostToNetwork();
	virtual void NetworkToHost();

	size_t Set(char version,char sign1,char sign2,uint16_t protocolNo,uint16_t dataLen,int32_t handle,char find_sig);

	/*
	PKHEADER_MIN -- from SDSockClient.cs
	public byte cVer;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
	public byte[] szSign = new byte[2];	    // data부분
	public ushort wType;
	public ushort dwSize;
	public int nHandle;
	public byte cFindSig;        
	 */

	char m_version;
	char m_sign1;
	char m_sign2;
	uint16_t m_protocolNo;
	uint16_t m_dataLen;
	int32_t m_handle;
	char m_findSig;
};
#endif // _HEADER_CCT_H_