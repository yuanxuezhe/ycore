#include "core.h"
#include "CommStruct.h"
#include "praseconfig.h"
#include "ystream.h"
#include "tinyxml2.h"
#include "auto2code.h"

// 设置参数类型及参数长度
//std::string 
int main() {
    // 配置结构体
    CConfig config;
    if(!PraseConfig::PraseXmlFile(config)) {
        return -1;
    }

    CCore* core = new CCore();
    // 初始化核心模块
    core->Init(config);
    core->logger->info("**********************************************************");
    core->logger->info("*                    yserver   autosql                   *");
    core->logger->info("*                        v:1.0.0.1                       *");
    core->logger->info("**********************************************************");
    core->logger->info("system start successfully");
    core->logger->info("");
    core->logger->info("");


    std::vector<TableInfo> vcTables;

    // 取odbc所有表信息
    CYStream odbc1(core);
    odbc1.GetAllTables(vcTables);
    odbc1.Close();
    
    // 取odbc所有字段信息
    for (auto& it : vcTables) {
        CYStream odbc2(core);
        odbc2.GetTableAllAttrs(it);
        odbc2.Close();
    }

    // 取odbc主键索引信息
    for (auto& it : vcTables) {
        CYStream odbc3(core);
        odbc3.GetIndex(it);
        odbc3.Close();
    }
    
    // 取模板信息
    std::string strTemplate = Auto2Code::LoadCodeTemplate("test.tmp");

    // 正则表达式获取参数
    std::vector<std::string> vcRegMatch;
    CTools::RegexMatch(strTemplate, "(%%[^%]+%%)", vcRegMatch);
    /* 打印vcRegMatch*/
    for (auto& table : vcTables)
    {
        std::string tmp(strTemplate);
        for (auto& regmatch : vcRegMatch)
        {
            std::string res = Auto2Code::RuleTransfor(table, regmatch);
            //core->logger->info("{:<50}  ====>           {}", regmatch, Auto2Code::RuleTransfor(table, res));

            CTools::ReplaceAll(tmp, regmatch, res);
        }

        Auto2Code::IntoFile(tmp, fmt::format("tables/{}.h",CTools::UpperLowerSwitch(table.szTableName, '0')));
    }


    // for (auto& it : vcTables) {
    //     std::cout << "class " << it.first << " {" << std::endl;
    //     //core->logger->info("table_name:{}", it.first);
    //     for (auto& it1 : it.second) {
    //         std::cout << "    " << CTools::CamelName(it1.szColumnName) << ";        // " << it1.szColumnComment << std::endl;
    //         //core->logger->info("column_name:{} column_comment:{} column_size:{} column_type:{} nullable:{}, paramname:{}"
    //         //    , it1.szColumnName, it1.szColumnComment, it1.nColumnSize, (int)it1.odbcDataType, it1.nNullable, CTools::CamelName(it1.szColumnName));
    //     }
    //     std::cout << "};" << std::endl;
    // }

    core->logger->info("over {2} {4} ", 1,2,3,4,5,6,7,8);

    return 0;
}