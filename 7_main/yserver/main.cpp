#include <iostream>
#include "core.h"
#include "CommStruct.h"
#include "praseconfig.h"
#include "ystream.h"
#include "tinyxml2.h"
#include "tcpserver.h"

struct LiteMall {
    int nId;
    char cId;
    char szName[63 + 1];
    char szUrl[255 + 1];
};

struct CUser {
    int  iId;
    char szUsername[32 + 1];
    char szPhone[16 + 1];    
    char cIdType;
    char szIdNumber[32 + 1];
    char szAddress[256 + 1];
    int  iBirthday;
    char cSex;
};

void Call(CCore* core, std::string msg) {
    core->logger->info("Call:{}", msg);
}

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
    core->logger->info("*                         yserver                        *");
    core->logger->info("*                        v:1.0.0.1                       *");
    core->logger->info("**********************************************************");
    core->logger->info("system start successfully");
    core->logger->info("");
    core->logger->info("");

    core->logger->info("msgqueue:{}, connstr:{}", config.msgqueue.szName, config.msgqueue.szConnstr);
    // 打印配置里的队列信息map信息 config.mpQueue
    for (auto& it : config.mpQueue) {
        core->logger->info("queue:{} maxdepth:{}", it.first, it.second.nMaxDepth);
    }

    CYStream odbc(core);
    odbc.Open("SELECT * FROM USER where ID in ({})", P(10000));
    odbc.Exec();

    CUser user;
    while (odbc.Fetch()) {
        odbc.GetValue("ID", user.iId);
        odbc.GetValue("USERNAME", user.szUsername, sizeof(user.szUsername));
        odbc.GetValue("PHONE", user.szPhone, sizeof(user.szPhone));
        odbc.GetValue("ID_TYPE", user.cIdType);
        odbc.GetValue("ID_NUMBER", user.szIdNumber, sizeof(user.szIdNumber));
        odbc.GetValue("ADDRESS", user.szAddress, sizeof(user.szAddress));
        odbc.GetValue("BIRTHDAY", user.iBirthday);
        odbc.GetValue("SEX", user.cSex);
    }
   
    odbc.Close();

    EventCenter server;
    server.InitNetCenter(config.server.iPort);
    server.InitAmqpCenter();

    core->logger->info("Server started, port:{} waiting for connections...", config.server.iPort);

    server.start();
    core->logger->info("over");

    return 0;
}