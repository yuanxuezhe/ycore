#include <iostream>
#include "sql.h"
#include "sqltypes.h"
#include "sqlext.h"
#include <cstring>

// 封装一个c++函数 odbc根据字段名称获取字段值
void getFieldValueByColName(SQLHSTMT hStmt, SQLCHAR* colName, SQLCHAR* colValue) {
    SQLCHAR value[255];
    SQLLEN indicator;
    SQLCHAR colNameTmp[255];
    SQLSMALLINT colNameLen;
    SQLSMALLINT colType;
    SQLULEN colSize;
    SQLSMALLINT colDecimalDigits;
    SQLSMALLINT colNullable;
    SQLNumResultCols(hStmt, &colNameLen);
    for (int i = 1; i <= colNameLen; i++) {
        SQLDescribeCol(hStmt, i, colNameTmp, 255, &colNameLen, &colType, &colSize, &colDecimalDigits, &colNullable);
        if (strcmp((char*)colNameTmp, (char*)colName) == 0) {
            SQLGetData(hStmt, i, SQL_C_CHAR, value, 255, &indicator);
            strcpy((char*)colValue, (char*)value);
        }
    }
}

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;

    // 初始化环境
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // 连接数据库
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    ret = SQLDriverConnect(hDbc, NULL, (SQLCHAR*)"DSN=smj1;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Failed to connect to database" << std::endl;
        return 1;
    }

    // // 分配语句句柄
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	// strm.open(strSelectSQL.c_str());
	// strm.set_param("begdate"  ,m_nBeginBusidate);
	// strm.set_param("enddate"  ,m_nEndBusidate);
	// strm.execute();

    
    // 执行查询
    ret = SQLExecDirect(hStmt, (SQLCHAR*)"SELECT * FROM litemall_ad where id = '1' ", SQL_NTS);

    //odbc通过字段名称获取字段值
    SQLCHAR col1[255], col2[255], col3[255], col4[255];

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        getFieldValueByColName(hStmt, (SQLCHAR*)"id", col1);
        getFieldValueByColName(hStmt, (SQLCHAR*)"name", col2);
        getFieldValueByColName(hStmt, (SQLCHAR*)"link", col3);
        getFieldValueByColName(hStmt, (SQLCHAR*)"url", col4);

        std::cout << col1 <<", " << col2 <<  "," << col3  <<  "," << col4 << std::endl;
    }



    // 释放资源
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
