#ifndef __CORE_H__
#define __CORE_H__

#include "define.h"
#include "CommStruct.h"
#include "tools.h"
#include "ystream.h"
#include <spdlog/spdlog.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

// 基类，所有的开发需要继承此基类
class DLLExport CCore {
public:
    CCore();
    virtual ~CCore() {
        // 关闭 ODBC 连接
        SQLDisconnect(m_hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
        SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
    }

    // 初始化
    int Init(CConfig &config);

    // 运行
    int Run(){}

    // 释放
    int Exit(){}

    // 初始化LOG句柄
    int InitLogger(CLog& log);

    // 初始化ODBC句柄
    int InitOdbc(std::map<std::string, CXa>& mpXa);

    // odbc连接
    SQLHENV m_henv;
    SQLHDBC m_hdbc;
    std::shared_ptr<spdlog::logger> logger;
private:
    bool bInitLogger;
    bool bInitOdbc;
    //std::map<std::string, SQLHDBC> m_mapHdbc;
};

#endif // __CORE_H__
