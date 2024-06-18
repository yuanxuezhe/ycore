#include "ystream.h"

// 实现一个类继承CYstream
class CMyStream : public CYStream {
public:
    CMyStream(CCore* core) : CYStream(core) {}
    virtual ~CMyStream() {}

    /*
    
        // 获取所有名称
    void GetAllTables(std::vector<TableInfo>& vcTableInfos);
    // 获取所有列属性
    void GetTableAllAttrs(TableInfo& tableInfo);
    // 获取主键
    void GetPrimaryKeys(char *szTableName);
    // 获取索引
    void GetIndex(TableInfo& tableInfo);
    // 获取索引
    ColumnInfo GetColumnInfo(std::string columnName,TableInfo& tableInfo);

     */

    void GetAllTables(std::vector<TableInfo>& vcTableInfos)
    {
        // Retrieve table names
        SQLCHAR tableName[32 + 1];
        SQLCHAR remarks[255 + 1];
        SQLLEN remarksLen;

        SQLTables(Stmt(), NULL, 0, NULL, 0, NULL, 0, NULL, 0);
        while (SQLFetch(Stmt()) == SQL_SUCCESS) {
            SQLGetData(Stmt(), 3, SQL_C_CHAR, tableName, 256, 0);
            // Get table remarks
            SQLGetData(Stmt(), 5, SQL_C_CHAR, remarks, sizeof(remarks), &remarksLen);

            TableInfo tableInfo;
            memset(&tableInfo, 0, sizeof(tableInfo));
            strncpy(tableInfo.szTableName, tableName, sizeof(tableInfo.szTableName) - 1);
            strncpy(tableInfo.szTableComment, remarks, sizeof(tableInfo.szTableComment) - 1);

            vcTableInfos.push_back(tableInfo);
        }
    }

    /*
    void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT handleType, const std::string &msg) {
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            SQLCHAR sqlState[1024];
            SQLCHAR message[1024];
            if (SQLGetDiagRec(handleType, handle, 1, sqlState, NULL, message, 1024, NULL) == SQL_SUCCESS) {
                std::cerr << "Error: " << msg << ": " << message << std::endl;
            }
        }
    }
    */
    // 获取所有列属性
    void GetTableAllAttrs(TableInfo& tableInfo)
    {
        int ret = 0;
        // 获取表的字段及属性
        SQLColumns(Stmt(), NULL, 0, NULL, 0, (SQLCHAR*)tableInfo.szTableName, SQL_NTS, NULL, 0);

        while (SQLFetch(Stmt()) == SQL_SUCCESS) {
            SQLCHAR colName[256];
            SQLLEN colNameLen;
            SQLSMALLINT dataType;
            SQLULEN columnSize;
            SQLSMALLINT decimalDigits;
            SQLSMALLINT nullable;
            SQLCHAR remarks[256];
            SQLLEN remarksLen;

            ret = SQLGetData(Stmt(), 4, SQL_C_CHAR, colName, sizeof(colName), &colNameLen);
            //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

            ret = SQLGetData(Stmt(), 5, SQL_C_SSHORT, &dataType, 0, NULL);
            //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

            ret = SQLGetData(Stmt(), 7, SQL_C_ULONG, &columnSize, 0, NULL);
            //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

            ret = SQLGetData(Stmt(), 9, SQL_C_SSHORT, &decimalDigits, 0, NULL);
            //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

            ret = SQLGetData(Stmt(), 11, SQL_C_SSHORT, &nullable, 0, NULL);
            //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

            // Get remarks
            ret = SQLGetData(Stmt(), 12, SQL_C_CHAR, remarks, sizeof(remarks), &remarksLen);
            //checkError(ret, hStmt, SQL_HANDLE_STMT, "SQLGetData - REMARKS");


            ColumnInfo columnInfo;
            memset(&columnInfo, 0, sizeof(columnInfo));

            strncpy(columnInfo.szColumnName, colName, sizeof(columnInfo.szColumnName) - 1);
            columnInfo.odbcDataType = dataType;
            columnInfo.nColumnSize = columnSize;
            columnInfo.nNullable = nullable;
            strncpy(columnInfo.szColumnComment, remarks, sizeof(columnInfo.szColumnComment) - 1);
            columnInfo.bIsPrimaryKey = false;

            // 判断在map存在不，不存在则新增
            if (tableInfo.mpIndex.find("ALL_COLUMN") == tableInfo.mpIndex.end())
            {
                std::vector<ColumnInfo> vcColumnInfo;
                vcColumnInfo.push_back(columnInfo);
                tableInfo.mpIndex["ALL_COLUMN"] = vcColumnInfo;
            }
            else
            {
                tableInfo.mpIndex["ALL_COLUMN"].push_back(columnInfo);
            }

            tableInfo.vcColumnInfo.push_back(columnInfo);
        }
    }

    void GetPrimaryKeys(char *tableName) {
        SQLCHAR szPkColumnName[256];
        SQLCHAR szPkColumnNameBuffer[256];
        SQLLEN nPkColumnNameLen;
        SQLCHAR szPkTableQualifier[256];
        SQLCHAR szPkTableOwner[256];
        SQLCHAR szPkTableName[256];
        SQLLEN nPkTableQualifierLen, nPkTableOwnerLen, nPkTableNameLen;
        SQLCHAR szKeySeq[256];
        SQLLEN nKeySeqLen;
        SQLCHAR szPkName[256];
        SQLLEN nPkNameLen;
    
        // Retrieve primary key information
        SQLPrimaryKeys(Stmt(), NULL, 0, NULL, 0, tableName, SQL_NTS);

        // Fetch and print the primary key columns
        while (SQLFetch(Stmt()) == SQL_SUCCESS) {
            SQLGetData(Stmt(), 4, SQL_C_CHAR, szPkColumnNameBuffer, sizeof(szPkColumnNameBuffer), &nPkColumnNameLen);
            SQLGetData(Stmt(), 1, SQL_C_CHAR, szPkTableQualifier, sizeof(szPkTableQualifier), &nPkTableQualifierLen);
            SQLGetData(Stmt(), 2, SQL_C_CHAR, szPkTableOwner, sizeof(szPkTableOwner), &nPkTableOwnerLen);
            SQLGetData(Stmt(), 3, SQL_C_CHAR, szPkTableName, sizeof(szPkTableName), &nPkTableNameLen);
            SQLGetData(Stmt(), 6, SQL_C_CHAR, szKeySeq, sizeof(szKeySeq), &nKeySeqLen);
            SQLGetData(Stmt(), 7, SQL_C_CHAR, szPkName, sizeof(szPkName), &nPkNameLen);
    
            // Use the data as needed
            printf("Primary Key Column: %s\n", szPkColumnNameBuffer);
            printf("Table Qualifier: %s\n", szPkTableQualifier);
            printf("Table Owner: %s\n", szPkTableOwner);
            printf("Table Name: %s\n", szPkTableName);
            printf("Key Sequence: %s\n", szKeySeq);
            printf("Primary Key Name: %s\n", szPkName);
        }
    }

    ColumnInfo GetColumnInfo(std::string columnName,TableInfo& tableInfo)
    {
        // 在tabeInfo中找到列序号，并添加到map中
        for (auto& it : tableInfo.vcColumnInfo)
        {
            if (it.szColumnName == columnName)
            {
                return it;
            }
        }
        return ColumnInfo();
    }

    void GetIndex(TableInfo& tableInfo)
    {
        SQLStatistics(Stmt(), NULL, 0, NULL, SQL_NTS, (SQLCHAR*)tableInfo.szTableName, SQL_NTS, SQL_INDEX_ALL, SQL_ENSURE);
        
        SQLCHAR indexName[256], columnName[256];
        SQLSMALLINT nonUnique, type;
        while (SQLFetch(Stmt()) == SQL_SUCCESS) {
            SQLGetData(Stmt(), 6, SQL_C_CHAR, indexName, sizeof(indexName), NULL);
            SQLGetData(Stmt(), 9, SQL_C_CHAR, columnName, sizeof(columnName), NULL);

            for (int i = 0; i < tableInfo.vcColumnInfo.size(); i++)
            {
                if (std::string(tableInfo.vcColumnInfo[i].szColumnName) == std::string((char*)columnName))
                {
                    tableInfo.vcColumnInfo[i].bIsPrimaryKey = true;
                }
            }
            // 判断在map存在不，不存在则新增
            if (tableInfo.mpIndex.find((char*)indexName) == tableInfo.mpIndex.end())
            {
                std::vector<ColumnInfo> vcColumnInfo;
                vcColumnInfo.push_back(GetColumnInfo((char*)columnName,tableInfo));
                tableInfo.mpIndex[(char*)indexName] = vcColumnInfo;
            }
            else
            {
                tableInfo.mpIndex[(char*)indexName].push_back(GetColumnInfo((char*)columnName,tableInfo));
            }
        }
    }
};