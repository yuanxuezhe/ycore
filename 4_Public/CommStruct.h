#ifndef __COMM_STRUCT_H__
#define __COMM_STRUCT_H__

#include <map>
#include <string>

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
    // xa配置
    std::map<std::string, CXa> mpXa;
    // msgqueue配置
    CMsgqueue   msgqueue;
    std::map<std::string, CQueue> mpQueue;
};

#endif // __COMM_STRUCT_H__ 