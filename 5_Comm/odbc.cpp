#include "odbc.h"

void OdbcStmt::OdbcStmt()
{
    m_henv = SQL_NULL_HENV;
    m_hdbc = SQL_NULL_HDBC;
    m_hstmt = SQL_NULL_HSTMT;
}

void OdbcStmt::~OdbcStmt()
{
    close();
}

bool OdbcStmt::connect(const char* dsn)
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv) != SQL_SUCCESS) {
        return false;
    }

    if (SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS) {
        return false;
    }

    if (SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc) != SQL_SUCCESS) {
        return false;
    }

    if (SQLConnect(m_hdbc, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, 0) != SQL_SUCCESS) {
        return false;
    }

    if (SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt) != SQL_SUCCESS) {
        return false;
    }

    return true;
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

    if (m_hdbc != SQL_NULL_HDBC) {
        SQLDisconnect(m_hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
        m_hdbc = SQL_NULL_HDBC;
    }

    if (m_henv != SQL_NULL_HENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
        m_henv = SQL_NULL_HENV;
    }
}

void OdbcStmt::getFieldValueByColName(const char* colName, char* colValue)
{
    SQLLEN len;
    SQLCHAR value[256];

    SQLGetData(m_hstmt, 1, SQL_C_CHAR, value, sizeof(value), &len);
    strcpy(colValue, (char*)value);
}