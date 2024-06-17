#include "CommStruct.h"
#include "tools.h"
#include "define.h"

struct Atom
{   
    std::string atom0;               // 原始字段  USER_ID
    std::string atom1;               // 类型type 0 int/ char/double等等    1 int/ char* /double等等
    std::string atom2;               // 0|驼峰命名 1|全大写 2|全小写 3|n+驼峰命名 4|m_n+驼峰命名
    std::string atom3;               // 后缀1  char: [lenth + 1]    其他 ""
    std::string atom4;               // 后缀2  char数组   “,  sizeof(2)” 其他  ""
    std::string atom5;               // 备注  用户ID 
    std::string atom6;               // 字符串用P()将atom2包起来
};

class Auto2Code
{
public:
    /*
    *   解析配置文件

    * 0  原始字段  USER_ID
    * 1  类型type 0 int/ char/double等等    1 int/ char* /double等等
    * 2  参数类型 0|驼峰命名 1|全大写 2|全小写 3|n+驼峰命名 4|m_n+驼峰命名

    * 3  后缀1  char: [lenth + 1]    其他 ""
    * 4  后缀2  char数组   “,  sizeof(2)” 其他  ""
    * 5  备注  用户ID  
    * 6  P(2)字符串用P()将2包起来
    * 
    *   格式  %FIX1|FIX2|FIX3|FIX4%       %%COLUME|0|0|串|多个记录分隔符%%
    *   FIX1表示操作的对象   TABLE|表  COLUMN|列  ALL_COLUMN|所有列  PK_COLUMN|主键列

    *   FIX2 类型type
    *   FIX3 参数类型 0|驼峰命名 1|全大写 2|全小写 3|n+驼峰命名 4|m_n+驼峰命名
    *   FIX4 格式化字符串 fmt::format
    *   FIX5 分隔符
    */
    static std::string LoadCodeTemplate(const std::string &strPath);

    static void IntoFile(const std::string &text, const std::string &strPath);

    static Atom Convert2Atom(const TableInfo &tableInfo, std::string & paramfmt);

    static Atom Convert2Atom(const ColumnInfo &columnInfo, std::string & paramtype, std::string & paramfmt);

    static std::string RuleTransfor(const Atom &atom, const std::vector<std::string>& vcRule);

    static std::string RuleTransfor(const TableInfo &tableInfo, const std::string &strRule);
};