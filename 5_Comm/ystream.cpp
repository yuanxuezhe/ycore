#include "ystream.h"

CYStream::CYStream(CCore* core): m_core(core),m_hstmt(SQL_NULL_HSTMT),m_strSql("")
{
    SQLAllocHandle(SQL_HANDLE_STMT, m_core->m_hdbc, &m_hstmt);
}

CYStream::~CYStream()
{
    Close();
}

int CYStream::Exec()
{
    int retcode = SQLExecDirect(m_hstmt, (SQLCHAR*)m_strSql.c_str(), SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        m_core->logger->error("Sql Exec()"); // err
        return retcode;
    }
    m_core->logger->info("Sql Exec(): 【{}】", m_strSql); // ok
    // 解析列名与序号的对应关系
    DescribeColumns();
    return SQL_SUCCESS;
}

std::string CYStream::GetExecSql()
{
    return m_strSql;
}

bool CYStream::Fetch()
{
    if (SQLFetch(m_hstmt) != SQL_SUCCESS) {
        return false;
    }
    return true;
}

void CYStream::Close()
{
    if (m_hstmt != SQL_NULL_HSTMT) {
        SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
        m_hstmt = SQL_NULL_HSTMT;
    }
}

void CYStream::DescribeColumns() {
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

ODBC_RETURN_CODE CYStream::GetValue(const char* colName, int& colValue)
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
            m_core->logger->info("Sql {:<20}= {}  ", fmt::format("GetValue({})",colName), colValue);
            return ODBC_SUCCESS;
        }
    }
    else
    {
        m_core->logger->error("Sql {:<20}= '{}'  ", fmt::format("GetValue({})",colName), "Column Not Found");
        return ODBC_ERROR_INVALID_COLUMN;
    }

    return ODBC_SUCCESS;
}

ODBC_RETURN_CODE CYStream::GetValue(const char* colName, char& colValue)
{
    if (colName == NULL)
    {
        return ODBC_ERROR_INVALID_PARAMETER;
    }

    auto it = m_mpColumnIndex.find(colName);
    if (it != m_mpColumnIndex.end()) {
        char value[2]; // Assuming column value won't exceed 256 characters
        SQLRETURN retCode = SQLGetData(m_hstmt, it->second, SQL_C_CHAR, value, sizeof(value), NULL);

        // 打印数据或进行其他操作
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
            colValue = value[0];
            m_core->logger->info("Sql {:<20}= '{}'  ", fmt::format("GetValue({})",colName), colValue);
            return ODBC_SUCCESS;
        }
    }
    else
    {
        m_core->logger->error("Sql {:<20}= {}  ", fmt::format("GetValue({})",colName), "Column Not Found");
        return ODBC_ERROR_INVALID_COLUMN;
    }

    return ODBC_SUCCESS;
}

// 根据列名获取列值
ODBC_RETURN_CODE CYStream::GetValue(const char* colName, const char* colValue, size_t colValueLen)
{
    if (colName == NULL || colValue == NULL)
    {
        return ODBC_ERROR_INVALID_PARAMETER;
    }

    auto it = m_mpColumnIndex.find(colName);
    if (it != m_mpColumnIndex.end()) {
        // 跳过第一列，获取第二列数据（Name列）
        SQLRETURN retCode = SQLGetData(m_hstmt, it->second, SQL_C_CHAR, (void*)colValue, colValueLen, NULL);
        // 打印数据或进行其他操作
        if (retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) {
            m_core->logger->info("Sql {:<20}= \"{}\"  ", fmt::format("GetValue({})",colName), colValue);
            return ODBC_SUCCESS;
        }
    }
    else
    {
        m_core->logger->error("Sql {:<20}= '{}'  ", fmt::format("GetValue({})",colName), "Column Not Found");
        return ODBC_ERROR_INVALID_COLUMN;
    }

    return ODBC_ERROR;
}

int CYStream::PrintDatabaseSet() {
    // Fetch and output each row
    // Get column count
    SQLCHAR value[255];
    SQLLEN indicator;
    SQLULEN colSize;
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
void CYStream::getLastError()
{
    SQLCHAR sqlState[6];
    SQLINTEGER nativeError;
    SQLCHAR errMsg[255];
    SQLSMALLINT errMsgLen;
    SQLError(SQL_NULL_HENV, m_core->m_hdbc, m_hstmt, sqlState, &nativeError, errMsg, 255, &errMsgLen);
    std::cout << "sqlState: " << sqlState << ", nativeError: " << nativeError << ", errMsg: " << errMsg << std::endl;
}