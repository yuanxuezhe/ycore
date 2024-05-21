#include <iostream>
#include "odbc.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

struct LiteMall {
    int nId;
    char szName[63 + 1];
    char szUrl[255 + 1];
};
int main() {
    //auto logger = std::make_shared<spdlog::logger>("LoggerTest", std::make_shared<spdlog::sinks::basic_file_sink_mt>("testLog.csv"));
    // 创建一个具有颜色支持的控制台日志器
    auto logger = spdlog::stdout_color_mt("console");

    OdbcStmt::InitConnection("DRIVER={MySQL ODBC 8.4 Unicode Driver};SERVER=123.207.218.225:53306;DATABASE=smj;USER=root;PASSWORD=123456;CHARSET=UTF8;");

    OdbcStmt odbc;
    odbc.Open("SELECT * FROM litemall_ad where ID > {@1}");
    odbc.SetParam<int>(1, 0);
    logger->info("Exec Sql: 【{}】", odbc.GetExecSql()); // ok

    odbc.Exec();

    LiteMall liteMall;
    while (odbc.Fetch()) {
        odbc.GetValue("id", liteMall.nId);
        odbc.GetValue("name", liteMall.szName, sizeof(liteMall.szName));
        odbc.GetValue("url", liteMall.szUrl, sizeof(liteMall.szUrl));
        logger->info("Qry Result: id:{0}, name:{1:>20}, url:{2}", liteMall.nId, liteMall.szName, liteMall.szUrl);
    }
    odbc.Close();

    OdbcStmt::CloseConnection();
    return 0;
}
