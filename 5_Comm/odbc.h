#ifndef __ODBC_H__
#define __ODBC_H__

#include "sql.h"
#include "sqltypes.h"
#include "sqlext.h"
#include <cstring>
#include "define.h"
#include <iostream>
#include "tools.h"

class DLLExport OdbcStmt {
public:
    OdbcStmt();
    ~OdbcStmt();

    int Open(const char* sql);
    int Exec();

    //bool connect(const char* dsn);
    bool exec(const char* sql);
    bool fetch();
    void close();
    void GetValue(const char* colName, char* colValue);
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

    static void InitConnection(const char* dsn) {
        if (m_bConnected) {
            return;
        }
        // 初始化 ODBC 连接
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
        SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
        SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
        SQLConnect(m_hdbc, (SQLCHAR*)dsn, SQL_NTS, NULL, 0, NULL, 0);
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

private:
    static SQLHENV m_henv;
    static SQLHDBC m_hdbc;
    static bool m_bConnected;

    SQLHSTMT m_hstmt;

    std::string m_strSql;
};

#endif // __ODBC_H__