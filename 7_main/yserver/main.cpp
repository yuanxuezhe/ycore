#include <iostream>
#include "core.h"
#include "CommStruct.h"
#include "praseconfig.h"
#include "ystream.h"
#include "tinyxml2.h"

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


int main() {
    // 配置结构体
    CConfig config;
    PraseConfig::PraseXmlFile(config);

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
    
    CYStream odbc(core);
    odbc.Open("SELECT * FROM USER where ID = {@1}");
    odbc.SetParam<int>(1, 1);
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
    delete core;

    return 0;
}
