#include "ystream.h"

CYStream::CYStream(CCore* core): m_core(core),m_hstmt(SQL_NULL_HSTMT),m_strSql("")
{
    SQLAllocHandle(SQL_HANDLE_STMT, m_core->m_hdbc, &m_hstmt);
}

CYStream::~CYStream()
{
    Close();
}

int CYStream::Open(const char* sql)
{
    if (sql == NULL)
    {
        return SQL_ERROR;
    }
    
    m_strSql = sql;

    return SQL_SUCCESS;
}

int CYStream::Exec()
{
    int retcode = SQLExecDirect(m_hstmt, (SQLCHAR*)m_strSql.c_str(), SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        m_core->logger->error("Sql Exec()"); // err
        return retcode;
    }
    m_core->logger->info("Sql Exec(): 【{}】", GetExecSql()); // ok
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

void CYStream::GetAllTables(std::vector<TableInfo>& vcTableInfos)
{
    // Retrieve table names
    SQLCHAR tableName[32 + 1];
    SQLCHAR remarks[255 + 1];
    SQLLEN remarksLen;

    SQLTables(m_hstmt, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
    while (SQLFetch(m_hstmt) == SQL_SUCCESS) {
        SQLGetData(m_hstmt, 3, SQL_C_CHAR, tableName, 256, 0);
        // Get table remarks
        SQLGetData(m_hstmt, 5, SQL_C_CHAR, remarks, sizeof(remarks), &remarksLen);

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
void CYStream::GetTableAllAttrs(TableInfo& tableInfo)
{
    int ret = 0;
    // 获取表的字段及属性
    SQLColumns(m_hstmt, NULL, 0, NULL, 0, (SQLCHAR*)tableInfo.szTableName, SQL_NTS, NULL, 0);

    while (SQLFetch(m_hstmt) == SQL_SUCCESS) {
        SQLCHAR colName[256];
        SQLLEN colNameLen;
        SQLSMALLINT dataType;
        SQLULEN columnSize;
        SQLSMALLINT decimalDigits;
        SQLSMALLINT nullable;
        SQLCHAR remarks[256];
        SQLLEN remarksLen;

        ret = SQLGetData(m_hstmt, 4, SQL_C_CHAR, colName, sizeof(colName), &colNameLen);
        //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

        ret = SQLGetData(m_hstmt, 5, SQL_C_SSHORT, &dataType, 0, NULL);
        //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

        ret = SQLGetData(m_hstmt, 7, SQL_C_ULONG, &columnSize, 0, NULL);
        //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

        ret = SQLGetData(m_hstmt, 9, SQL_C_SSHORT, &decimalDigits, 0, NULL);
        //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

        ret = SQLGetData(m_hstmt, 11, SQL_C_SSHORT, &nullable, 0, NULL);
        //checkError(ret, m_hstmt, SQL_HANDLE_STMT, "SQLGetData");

        // Get remarks
        ret = SQLGetData(m_hstmt, 12, SQL_C_CHAR, remarks, sizeof(remarks), &remarksLen);
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

void CYStream::GetPrimaryKeys(char *tableName) {
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
    SQLPrimaryKeys(m_hstmt, NULL, 0, NULL, 0, tableName, SQL_NTS);

    // Fetch and print the primary key columns
    while (SQLFetch(m_hstmt) == SQL_SUCCESS) {
        SQLGetData(m_hstmt, 4, SQL_C_CHAR, szPkColumnNameBuffer, sizeof(szPkColumnNameBuffer), &nPkColumnNameLen);
        SQLGetData(m_hstmt, 1, SQL_C_CHAR, szPkTableQualifier, sizeof(szPkTableQualifier), &nPkTableQualifierLen);
        SQLGetData(m_hstmt, 2, SQL_C_CHAR, szPkTableOwner, sizeof(szPkTableOwner), &nPkTableOwnerLen);
        SQLGetData(m_hstmt, 3, SQL_C_CHAR, szPkTableName, sizeof(szPkTableName), &nPkTableNameLen);
        SQLGetData(m_hstmt, 6, SQL_C_CHAR, szKeySeq, sizeof(szKeySeq), &nKeySeqLen);
        SQLGetData(m_hstmt, 7, SQL_C_CHAR, szPkName, sizeof(szPkName), &nPkNameLen);
 
        // Use the data as needed
        printf("Primary Key Column: %s\n", szPkColumnNameBuffer);
        printf("Table Qualifier: %s\n", szPkTableQualifier);
        printf("Table Owner: %s\n", szPkTableOwner);
        printf("Table Name: %s\n", szPkTableName);
        printf("Key Sequence: %s\n", szKeySeq);
        printf("Primary Key Name: %s\n", szPkName);
    }
}

ColumnInfo CYStream::GetColumnInfo(std::string columnName,TableInfo& tableInfo)
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

void CYStream::GetIndex(TableInfo& tableInfo)
{
    SQLStatistics(m_hstmt, NULL, 0, NULL, SQL_NTS, (SQLCHAR*)tableInfo.szTableName, SQL_NTS, SQL_INDEX_ALL, SQL_ENSURE);
    
    SQLCHAR indexName[256], columnName[256];
    SQLSMALLINT nonUnique, type;
    while (SQLFetch(m_hstmt) == SQL_SUCCESS) {
        SQLGetData(m_hstmt, 6, SQL_C_CHAR, indexName, sizeof(indexName), NULL);
        SQLGetData(m_hstmt, 9, SQL_C_CHAR, columnName, sizeof(columnName), NULL);

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