#ifndef __YSTREAM_H__
#define __YSTREAM_H__

#include "sql.h"
#include "sqltypes.h"
#include "sqlext.h"
#include <cstring>
#include "define.h"
#include <iostream>
#include "tools.h"
#include "core.h"
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

class CCore;

class DLLExport CYStream {
public:
    CYStream(CCore* core);
    ~CYStream();

    int Open(const char* sql);
    int Exec();
    std::string GetExecSql();
    bool Fetch();
    void Close();
    ODBC_RETURN_CODE GetValue(const char* colName, int& colValue);
    ODBC_RETURN_CODE GetValue(const char* colName, char& colValue);
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
//private:
public:
    void DescribeColumns();
private:
    CCore* m_core;
    //SQLHENV m_henv;
    //SQLHDBC m_hdbc;

    SQLHSTMT m_hstmt;

    std::map<std::string, int> m_mpColumnIndex;
    std::string m_strSql;
};

#endif // __YSTREAM_H__

