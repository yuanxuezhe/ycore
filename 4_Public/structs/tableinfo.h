#ifndef __STRUCT_TABLEINFO_H__
#define __STRUCT_TABLEINFO_H__

#include "../define.h"

struct ColumnInfo {
    char szColumnName[32 + 1];
    int odbcDataType;
    int nColumnSize;
    int nNullable;
    char szColumnComment[255 + 1];
    bool bIsPrimaryKey;
};

struct TableInfo {
    char szTableName[32 + 1];
    char szTableComment[255 + 1];
    std::vector<ColumnInfo> vcColumnInfo;
    std::map<std::string, std::vector<ColumnInfo>> mpIndex;
};

#endif // __STRUCT_TABLEINFO_H__ 