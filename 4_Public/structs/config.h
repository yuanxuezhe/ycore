#ifndef __STRUCT_CONFIG_H__
#define __STRUCT_CONFIG_H__

#include "define.h"

// 配置信息结构体
struct CLog
{
    char cModel;
    char szConsolelvl[8 + 1];
    char szLoglvl[8 + 1];
    char szPattern[128 + 1];
    char szFilepath[128 + 1];
    int nFilesize;
    int nMaxfiles;
};

struct CXa
{  
    char szName[32 + 1];
    char szConnstr[128 + 1];
};

struct CServer
{  
    int iPort;
};

struct CMsgqueue
{  
    char szName[32 + 1];
    char szType[8 + 1];
    char szConnstr[128 + 1];
};

struct CQueue
{  
    char szName[32 + 1];
    int nMaxDepth;
};

struct CConfig
{
    // 日志配置
    CLog log;
    // 服务配置
    CServer server;
    // xa配置
    std::map<std::string, CXa> mpXa;
    // msgqueue配置
    CMsgqueue   msgqueue;
    std::map<std::string, CQueue> mpQueue;
};


#endif // __STRUCT_CONFIG_H__ 