#include "core.h"
#include "CommStruct.h"
#include "praseconfig.h"
#include "tinyxml2.h"
#include "tcpserver.h"
#include "user_vip.h"
#include "user.h"

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

    CUserVip userVip(core);
    core->logger->info("over1");
    if (!userVip.Query(10000, "0"))
    {
        core->logger->error("No Data {},{}", 10000, "0");
        return -1;
    }

    core->logger->info("USER_ID:{},VIP_TYPE:{},VIP_LEVEL:{},START_DATE:{}"
        , userVip.m_nUserId, userVip.m_szVipType, userVip.m_szVipLevel, userVip.m_nStartDate);

    userVip.m_nStartDate = 19999999;

    userVip.Update();
    userVip.Query(10000, "0");

    EventCenter server;
    server.InitNetCenter(config.server.iPort);
    server.InitAmqpCenter();

    core->logger->info("Server started, port:{} waiting for connections...", config.server.iPort);

    server.start();
    core->logger->info("over");

    return 0;
}