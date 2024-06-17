#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

using namespace std;

int main() {
    // Allocate an environment handle
    SQLHANDLE hEnv;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Allocate a connection handle
    SQLHANDLE hDbc;
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Connect to the database
    SQLCHAR* connStr = (SQLCHAR*)"DRIVER={MySQL ODBC 8.4 Unicode Driver};SERVER=74.48.82.137:53306;DATABASE=djb;USER=root;PASSWORD=1;CHARSET=UTF8;";
    SQLRETURN ret = SQLDriverConnect(hDbc, NULL, connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    if (!SQL_SUCCEEDED(ret)) {
        cout << "Failed to connect to the database." << endl;
        return -1;
    }

    // Allocate a statement handle
    SQLHANDLE hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    // Retrieve table names
    SQLCHAR tableName[256];
    // ret = SQLTables(hStmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
    // if (SQL_SUCCEEDED(ret)) {
    //     cout << "Tables in the database:" << endl;
    //     while (SQLFetch(hStmt) != SQL_NO_DATA) {
    //         SQLGetData(hStmt, 3, SQL_C_CHAR, tableName, 256, 0);
    //         cout << tableName << endl;
    //     }
    // } else {
    //     cout << "Failed to retrieve table names." << endl;
    // }

    // 获取表的字段及属性
    ret = SQLColumns(hStmt, NULL, 0, NULL, 0, (SQLCHAR*)"USER", SQL_NTS, NULL, 0);
    if (SQL_SUCCEEDED(ret)) {
        cout << "表字段及属性：" << endl;
        while (SQLFetch(hStmt) != SQL_NO_DATA) {
            SQLCHAR columnName[256];
            SQLCHAR dataType[256];
            SQLLEN columnNameLenOrInd, dataTypeLenOrInd;
            SQLULEN columnSize;
            SQLGetData(hStmt, 4, SQL_C_CHAR, columnName, 256, &columnNameLenOrInd);
            SQLGetData(hStmt, 5, SQL_C_CHAR, dataType, 256, &dataTypeLenOrInd);
            SQLGetData(hStmt, 7, SQL_C_ULONG, &columnSize, 0, NULL);
            cout << "字段名: " << columnName << ", 数据类型: " << dataType << ", 数据长度: " << columnSize << endl;
        }
    } else {
        cout << "获取表字段及属性失败。" << endl;
    }

    // Free resources
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
