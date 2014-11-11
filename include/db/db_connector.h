#pragma once

#ifndef _DBCONNECTOR_H_
#define _DBCONNECTOR_H_	1

#include "common.h"
#include <string>
#include <sql.h>
#include <sqlext.h>

class DBConnector
{
public:

	DBConnector(void);
	explicit DBConnector(const char* odbc_name, const char* odbc_id, const char* odbc_pw);
	~DBConnector(void);

	void Initialize(const char* inifile);

	bool Connet();
	bool Connet(const char* odbc_name, const char* odbc_id, const char* odbc_pw);
	void DisConnect();
	bool ExcuteSQL(const char* query);
	bool Fetch();
	SQLLEN GetrowCount()
	{ 
		SQLLEN len;
		SQLRowCount(m_hSTMT, &len);
		return len;
	}

	SQLRETURN BindColBinary(SQLUSMALLINT col_no, char* bindvalue, size_t size)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_C_BINARY, bindvalue, size, &length);
	}

	SQLRETURN BindCol(SQLUSMALLINT col_no, char* bindvalue, size_t size)
	{
		SQLLEN length = 0;
		return SQLBindCol(m_hSTMT, col_no, SQL_C_CHAR, bindvalue, size, &length);
	}
	
	SQLRETURN BindCol(SQLUSMALLINT col_no, int8_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_TINYINT, &bindvalue, sizeof(bindvalue), &length);
	}
	
	SQLRETURN BindCol(SQLUSMALLINT col_no, int16_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_SMALLINT, &bindvalue, sizeof(bindvalue), &length);
	}
	SQLRETURN BindCol(SQLUSMALLINT col_no, int32_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_INTEGER, &bindvalue, sizeof(bindvalue), &length);
	}
	SQLRETURN BindCol(SQLUSMALLINT col_no, int64_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_C_SBIGINT, &bindvalue, sizeof(bindvalue), &length);
	}
	SQLRETURN BindCol(SQLUSMALLINT col_no, uint8_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_C_UTINYINT, &bindvalue, sizeof(bindvalue), &length);
	}
	SQLRETURN BindCol(SQLUSMALLINT col_no, uint16_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_C_USHORT, &bindvalue, sizeof(bindvalue), &length);
	}
	SQLRETURN BindCol(SQLUSMALLINT col_no, uint32_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_C_ULONG, &bindvalue, sizeof(bindvalue), &length);
	}
	SQLRETURN BindCol(SQLUSMALLINT col_no, uint64_t& bindvalue)
	{
		SQLLEN length;
		return SQLBindCol(m_hSTMT, col_no, SQL_C_UBIGINT, &bindvalue, sizeof(bindvalue), &length);
	}
private:

	bool SQL_RetCheck(SQLRETURN ret);

	SQLHENV m_hEnv;
	SQLHDBC m_hDbc;
	SQLHSTMT m_hSTMT;

	bool m_bConnect;

	std::string m_odbcName;
	std::string m_odbcID;
	std::string m_odbcPwd;
};

#endif // _DBDBCONNECTOR_H_
