#pragma once

#ifndef _UTILITY_ENCODER_H_
#define _UTILITY_ENCODER_H_
#include <string.h>

#include "common.h"
class Encoder
{
public:
	Encoder(void) {};
	~Encoder(void) {};

	static void XorConvert(unsigned char key, size_t size, char* org_str, char* dest_str)
	{
		int nPos = 0;
		while( nPos < size) 
		{
			*dest_str++ = *org_str++ ^ key;
			++nPos;
		}
		dest_str = '\0';
	}

	static void StrtoBinary(char* src, int src_len, char* dst)
	{
		int nCnt = 1;		
		char charTmp[3] = {0,};

		for(nCnt=0;nCnt<src_len;nCnt++)
		{	
			memset(charTmp,0,sizeof(charTmp));
			SNPRINTF(charTmp,"%02x",src[nCnt]);
			strcat(dst, charTmp);
		}
	}
};

#define ENCODER Encoder

#endif // _UTILITY_ENCODER_H_
