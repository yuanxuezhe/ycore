#ifndef __ODBC_H__
#define __ODBC_H__

#include "sql.h"
#include "sqltypes.h"
#include "sqlext.h"
#include <cstring>
#include "define.h"
#include <iostream>
#include "tools.h"
#include "map"

// 定义连接信息
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1024

// 定义枚举变量
enum ODBC_RETURN_CODE {
    ODBC_ERROR = -1,
    ODBC_SUCCESS = 0,
    ODBC_ERROR_NO_DATA = 100,
    ODBC_ERROR_INVALID_PARAMETER = 22000,
    ODBC_ERROR_INVALID_COLUMN = 22001,
    ODBC_ERROR_INVALID_CURSOR = 22002,
    ODBC_ERROR_INVALID_DESCRIPTOR = 22003,
    ODBC_ERROR_UNKNOWN = 99999
};

class DLLExport OdbcStmt {
public:
    OdbcStmt();
    ~OdbcStmt();

    int Open(const char* sql);
    int Exec();
    std::string GetExecSql();
    bool Fetch();
    void Close();
    ODBC_RETURN_CODE GetValue(const char* colName, int& colValue);
    ODBC_RETURN_CODE GetValue(const char* colName, const char* colValue, size_t colValueLen);
    int PrintDatabaseSet();
    // 获取并输出最后一次的错误信息
    void getLastError();

    template <typename input_type>
    void SetParam(int index, const input_type& in)
    {
        std::stringstream ss;
        if (std::is_same<input_type, char>::value || std::is_same<input_type, std::string>::value) {
            ss << "'" << in << "'";
        } else {
            ss << in;
        } 
        
        std::string key = "{@" + std::to_string(index) + "}";


        CTools::ReplaceAll(m_strSql, key, ss.str());
    }

    static void InitConnection(const char* connectionString) {
        if (m_bConnected) {
            return;
        }
        // 初始化 ODBC 连接
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
        SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

        SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);

        // // Set the connection charset to UTF-8
        // SQLINTEGER charset = SQL_UTF8;
        // SQLSetConnectAttr(m_hdbc, SQL_ATTR_CONNECTION_CHARSET, (SQLPOINTER)charset, SQL_IS_INTEGER);



        SQLDriverConnect(m_hdbc, NULL, (SQLCHAR*)connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

        m_bConnected = true;
    }

    static void CloseConnection() {
        if (!m_bConnected) {
            return;
        }
        // 关闭 ODBC 连接
        SQLDisconnect(m_hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
    }
//private:
public:
    void DescribeColumns();
private:
    static SQLHENV m_henv;
    static SQLHDBC m_hdbc;
    static bool m_bConnected;

    SQLHSTMT m_hstmt;

    std::map<std::string, int> m_mpColumnIndex;
    std::string m_strSql;
};

#endif // __ODBC_H__

