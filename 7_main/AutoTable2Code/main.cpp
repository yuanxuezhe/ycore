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
    core->logger->info("");
    core->logger->info("");


    std::vector<TableInfo> vcTables;

    core->logger->info("获取所有表信息");
    // 取odbc所有表信息
    CYStream odbc1(core);
    odbc1.GetAllTables(vcTables);
    odbc1.Close();
    
    core->logger->info("获取所有字段信息");
    // 取odbc所有字段信息
    for (auto& it : vcTables) {
        CYStream odbc2(core);
        odbc2.GetTableAllAttrs(it);
        odbc2.Close();
    }

    core->logger->info("获取主键信息");
    // 取odbc主键索引信息
    for (auto& it : vcTables) {
        CYStream odbc3(core);
        odbc3.GetIndex(it);
        odbc3.Close();
    }
    
    core->logger->info("获取模板test.tmp信息");
    // 取模板信息
    std::string strTemplate = Auto2Code::LoadCodeTemplate("test.tmp");

    // 正则表达式获取参数
    std::vector<std::string> vcRegMatch;
    CTools::RegexMatch(strTemplate, "(%%[^%]+%%)", vcRegMatch);
    core->logger->info("根据表信息生成代码");
    /* 打印vcRegMatch*/
    for (auto& table : vcTables)
    {
        std::string tmp(strTemplate);
        for (auto& regmatch : vcRegMatch)
        {
            std::string res = Auto2Code::RuleTransfor(table, regmatch);
            CTools::ReplaceAll(tmp, regmatch, res);
        }

        std::string file = fmt::format("tables/{}.h",CTools::UpperLowerSwitch(table.szTableName, '0'));
        core->logger->info("生成文件:{}", file);
        Auto2Code::IntoFile(tmp, file);
    }

    core->logger->info("over");

    return 0;
}