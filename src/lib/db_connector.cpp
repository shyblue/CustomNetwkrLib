#include "db/db_connector.h"
#include "util/logger.h"

DBConnector::DBConnector(void)  :
	m_bConnect(false)
{
}
DBConnector::DBConnector(const char* odbc_name, const char* odbc_id, const char* odbc_pw) :
	m_odbcName(odbc_name),
	m_odbcID(odbc_id) ,
	m_odbcPwd(odbc_pw) ,
	m_bConnect(false)
{
}

DBConnector::~DBConnector(void)
{
	if (m_bConnect)
		DisConnect();
}

bool DBConnector::Connet()
{
	if ( !SQL_RetCheck(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv)) )
	{		
		return false;
	}

	if ( !SQL_RetCheck(SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER)) )
		return false;

	if ( !SQL_RetCheck(SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc)))
		return false;

	
	if ( !SQL_RetCheck(SQLConnect(m_hDbc, (SQLTCHAR*)m_odbcName.c_str(), SQL_NTS, (SQLTCHAR*)m_odbcID.c_str(), SQL_NTS, (SQLTCHAR*)m_odbcPwd.c_str(), SQL_NTS)) )
	{
		return false;
	}

	if ( !SQL_RetCheck(SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hSTMT)) )
		return false;

	m_bConnect = true;

	return true;
}

bool DBConnector::SQL_RetCheck(SQLRETURN ret)
{
    return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

bool DBConnector::Connet(const char* odbc_name, const char* odbc_id, const char* odbc_pw)
{

	m_odbcName = odbc_name;
	m_odbcID = odbc_id;
	m_odbcPwd = odbc_pw;

    return Connet();
}

void DBConnector::DisConnect()
{
	if(m_hSTMT) SQLFreeHandle(SQL_HANDLE_STMT, m_hSTMT);
	if(m_hDbc) SQLDisconnect(m_hDbc);
	if(m_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	if(m_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);

	m_bConnect = false;
}

bool DBConnector::ExcuteSQL(const char* query)
{
	SQLSMALLINT ret = SQL_SUCCESS;
	ret = SQLExecDirect(m_hSTMT, (SQLTCHAR*)query, SQL_NTS);

    return SQL_SUCCEEDED(ret);

}

bool DBConnector::Fetch()
{

	if( SQLFetch(m_hSTMT) != SQL_NO_DATA)
	{
		return true;
	}
	else
	{
		if(m_hSTMT) SQLCloseCursor(m_hSTMT);
	}

	return false;
}