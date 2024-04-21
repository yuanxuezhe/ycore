#include "odbc.h"

SQLHENV OdbcStmt::m_henv = SQL_NULL_HENV;
SQLHDBC OdbcStmt::m_hdbc = SQL_NULL_HDBC;
bool OdbcStmt::m_bConnected = false;

OdbcStmt::OdbcStmt()
{
    m_hstmt = SQL_NULL_HSTMT;
    SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
}

OdbcStmt::~OdbcStmt()
{
    close();
}

int OdbcStmt::Open(const char* sql)
{
    if (sql == NULL)
    {
        return SQL_ERROR;
    }
    
    m_strSql = sql;

    return SQL_SUCCESS;
}

int OdbcStmt::Exec()
{
    std::cout << "sql: " << m_strSql << std::endl;
    int retcode = SQLExecDirect(m_hstmt, (SQLCHAR*)m_strSql.c_str(), SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        return retcode;
    }

    return SQL_SUCCESS;
}

bool OdbcStmt::exec(const char* sql)
{
    if (SQLExecDirect(m_hstmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS) {
        return false;
    }

    return true;
}

bool OdbcStmt::fetch()
{
    if (SQLFetch(m_hstmt) != SQL_SUCCESS) {
        return false;
    }

    return true;
}

void OdbcStmt::close()
{
    if (m_hstmt != SQL_NULL_HSTMT) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
        m_hstmt = SQL_NULL_HSTMT;
    }
}

// 根据列名获取列值
void OdbcStmt::GetValue(const char* colName, char* colValue)
{
    SQLCHAR value[255];
    SQLLEN indicator;
    SQLCHAR colNameTmp[255];
    SQLSMALLINT colNameLen;
    SQLSMALLINT colType;
    SQLULEN colSize;
    SQLSMALLINT colDecimalDigits;
    SQLSMALLINT colNullable;
    SQLNumResultCols(m_hstmt, &colNameLen);
    for (int i = 1; i <= colNameLen; i++) {
        SQLDescribeCol(m_hstmt, i, colNameTmp, 255, &colNameLen, &colType, &colSize, &colDecimalDigits, &colNullable);
        //std::cout << i <<": " << colNameTmp <<  "," << colType << std::endl;
        if (strcmp((char*)colNameTmp, (char*)colName) == 0) {
            SQLGetData(m_hstmt, i, SQL_C_CHAR, value, 255, &indicator);
            strcpy((char*)colValue, (char*)value);
        }
    }
}

// 获取并输出最后一次的错误信息
void OdbcStmt::getLastError()
{
    SQLCHAR sqlState[6];
    SQLINTEGER nativeError;
    SQLCHAR errMsg[255];
    SQLSMALLINT errMsgLen;
    SQLError(SQL_NULL_HENV, m_hdbc, m_hstmt, sqlState, &nativeError, errMsg, 255, &errMsgLen);
    std::cout << "sqlState: " << sqlState << ", nativeError: " << nativeError << ", errMsg: " << errMsg << std::endl;
}