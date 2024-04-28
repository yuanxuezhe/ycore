#include "odbc.h"

SQLHENV OdbcStmt::m_henv = SQL_NULL_HENV;
SQLHDBC OdbcStmt::m_hdbc = SQL_NULL_HDBC;
bool OdbcStmt::m_bConnected = false;

OdbcStmt::OdbcStmt(): m_hstmt(SQL_NULL_HSTMT),m_strSql("")
{
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

    // 解析列名与序号的对应关系
    DescribeColumns();
    return SQL_SUCCESS;
}

bool OdbcStmt::Fetch()
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

void OdbcStmt::DescribeColumns() {
    SQLSMALLINT colCount;
    SQLNumResultCols(m_hstmt, &colCount);

    SQLCHAR colNameTmp[255];
    SQLSMALLINT colNameLen;
    SQLSMALLINT colType;
    SQLULEN colSize;
    SQLSMALLINT colDecimalDigits;
    SQLSMALLINT colNullable;

    for (SQLSMALLINT i = 1; i <= colCount; ++i) {
        SQLCHAR colNameTmp[255];
        SQLDescribeCol(m_hstmt, i, colNameTmp, 255, &colNameLen, &colType, &colSize, &colDecimalDigits, &colNullable);

        // Store column index by name 
        m_mpColumnIndex[std::string((char*)colNameTmp)] = i;
    }
}

ODBC_RETURN_CODE OdbcStmt::GetValue(const char* colName, int& colValue)
{
    if (colName == NULL)
    {
        return ODBC_ERROR_INVALID_PARAMETER;
    }

    auto it = m_mpColumnIndex.find(colName);
    if (it != m_mpColumnIndex.end()) {
        // 跳过第一列，获取第二列数据（Name列）
        SQLRETURN retCode = SQLGetData(m_hstmt, it->second, SQL_INTEGER, &colValue, 0, NULL);

        // 打印数据或进行其他操作
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
            return ODBC_SUCCESS;
        }
    }
    else
    {
        return ODBC_ERROR_INVALID_COLUMN;
    }

    return ODBC_SUCCESS;
}
// 根据列名获取列值
ODBC_RETURN_CODE OdbcStmt::GetValue(const char* colName, const char* colValue, size_t colValueLen)
{
    if (colName == NULL || colValue == NULL)
    {
        return ODBC_ERROR_INVALID_PARAMETER;
    }

    auto it = m_mpColumnIndex.find(colName);
    if (it != m_mpColumnIndex.end()) {
        // 跳过第一列，获取第二列数据（Name列）
        SQLRETURN retCode = SQLGetData(m_hstmt, it->second, SQL_CHAR, (void*)colValue, colValueLen, NULL);
        // 打印数据或进行其他操作
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
            return ODBC_SUCCESS;
        }
    }
    else
    {
        return ODBC_ERROR_INVALID_COLUMN;
    }

    return ODBC_ERROR;
}

int OdbcStmt::PrintDatabaseSet() {
    // Fetch and output each row
    // Get column count
        SQLCHAR value[255];
    SQLLEN indicator;
    SQLCHAR colNameTmp[255];
    SQLSMALLINT colNameLen;
    SQLSMALLINT colNums;
    SQLSMALLINT colType;
    SQLULEN colSize;
    SQLSMALLINT colDecimalDigits;
    SQLSMALLINT colNullable;
    //SQLDescribeCol(m_hstmt, 0, colNameTmp, 255, &colNameLen, &colType, &colSize, &colDecimalDigits, &colNullable);
    DescribeColumns() ;
    SQLSMALLINT colCount;
    SQLNumResultCols(m_hstmt, &colCount);
    while (SQLFetch(m_hstmt) == SQL_SUCCESS) {
        for (SQLSMALLINT i = 1; i <= colCount; ++i) {
            SQLLEN indicator;
            SQLCHAR value[256]; // Assuming column value won't exceed 256 characters
            SQLGetData(m_hstmt, i, SQL_C_CHAR, value, sizeof(value), &indicator);

            if (indicator != SQL_NULL_DATA) {
                std::cout << "Column " << i << ": " << value << std::endl;
            } else {
                std::cout << "Column " << i << ": NULL" << std::endl;
            }
        }
    }

    return 0;
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