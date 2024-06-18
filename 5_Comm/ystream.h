#ifndef __YSTREAM_H__
#define __YSTREAM_H__

// #include "sql.h"
// #include "sqltypes.h"
// #include "sqlext.h"
#include "CommStruct.h"
#include "define.h"
#include "tools.h"
#include "core.h"

// 定义连接信息
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1024
#define P CYStream::Paramter

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

// enum ODBC_DATA_TYPE {
//     ODBC_DATA_TYPE_UNKNOWN = 0,
//     ODBC_DATA_TYPE_CHAR = 1,
//     ODBC_DATA_TYPE_NUMERIC = 2,
//     ODBC_DATA_TYPE_DECIMAL = 3,
//     ODBC_DATA_TYPE_INTEGER = 4,
//     ODBC_DATA_TYPE_SMALLINT = 5,
//     ODBC_DATA_TYPE_FLOAT = 6,
//     ODBC_DATA_TYPE_REAL = 7,
//     ODBC_DATA_TYPE_DOUBLE = 8,
//     ODBC_DATA_TYPE_DATE = 9,
//     ODBC_DATA_TYPE_TIME = 10,
//     ODBC_DATA_TYPE_TIMESTAMP = 11,
//     ODBC_DATA_TYPE_VARCHAR = 12,
//     ODBC_DATA_TYPE_LONGVARCHAR = -1,
//     ODBC_DATA_TYPE_BINARY = -2,
//     ODBC_DATA_TYPE_VARBINARY = -3,
//     ODBC_DATA_TYPE_LONGVARBINARY = -4,
//     ODBC_DATA_TYPE_BIGINT = -5,
//     ODBC_DATA_TYPE_TINYINT = -6,
//     ODBC_DATA_TYPE_BIT = -7,
//     ODBC_DATA_TYPE_WCHAR = -8,
//     ODBC_DATA_TYPE_WVARCHAR = -9,
//     ODBC_DATA_TYPE_WLONGVARCHAR = -10,
//     ODBC_DATA_TYPE_GUID = -11
// };

//class CCore;

class DLLExport CYStream {
public:
    CYStream(CCore* core);
    ~CYStream();

    template <typename... Args>
    void Open(const std::string& format, Args&&... args) {
        m_strSql = fmt::format(format, std::forward<Args>(args)...);
    }

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
    static std::string  Paramter(const input_type& in)
    {
        std::stringstream ss;
        ss << "'" << in << "'";
        return ss.str();
    }
    
    SQLHSTMT Stmt() { return m_hstmt; }

private:
    void DescribeColumns();

private:
    CCore*                          m_core;
    SQLHSTMT                        m_hstmt;
    std::map<std::string, int>      m_mpColumnIndex;
    std::string                     m_strSql;
};

#endif // __YSTREAM_H__

