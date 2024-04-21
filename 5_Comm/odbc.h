#ifndef __ODBC_H__
#define __ODBC_H__

#include <sql.h>
#include <sqlext.h>

class OdbcStmt {
public:
    OdbcStmt();
    ~OdbcStmt();

    bool connect(const char* dsn);
    bool exec(const char* sql);
    bool fetch();
    void close();
    void getFieldValueByColName(const char* colName, char* colValue);

private:
    SQLHENV m_henv;
    SQLHDBC m_hdbc;
    SQLHSTMT m_hstmt;
};

#endif // __ODBC_H__