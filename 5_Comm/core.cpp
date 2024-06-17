#include "core.h"


CCore::CCore()
        : bInitLogger(false),
        bInitOdbc(false),
        bInitAmqp(false)
{
    //evbase = event_base_new();
    m_henv = SQL_NULL_HENV;
    m_hdbc = SQL_NULL_HDBC;
}

// 初始化
int CCore::Init(CConfig &config)
{
    // 初始化日志
    InitLogger(config.log);
    // 初始化AMQP连接
    InitAmqp(config);
    // 初始化xa连接
    InitOdbc(config.mpXa);
    return 1;
}

int CCore::InitLogger(CLog& log)
{
    if (bInitLogger)
    {
        return 1;
    }
    
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);

    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log.szFilepath, log.nFilesize * 1048576, log.nMaxfiles);
    rotating_sink->set_level(spdlog::level::info);    

    // 创建多目标 sink
    std::vector<spdlog::sink_ptr> sinks;

    if (log.cModel == '0')
    {
    }
    else if (log.cModel == '1')
    {
        sinks.push_back(console_sink);
    }
    else if (log.cModel == '2')
    {
        sinks.push_back(rotating_sink);
    }
    else if (log.cModel == '3')
    {
        sinks.push_back(console_sink);
        sinks.push_back(rotating_sink);
    }
    else
    {
        sinks.push_back(console_sink);
    }

    logger = std::make_shared<spdlog::logger>("multi_sink", begin(sinks), end(sinks));
    logger->set_pattern(log.szPattern);

    spdlog::register_logger(logger);

    bInitLogger = true;
    return 0;
}

// 初始化AMQP连接
int CCore::InitAmqp(CConfig& config)
{
    if (bInitAmqp)
    {
        return 1;
    }
    // 创建AMQP连接

        // handler for libevent
    //handler = new AMQP::LibEventHandler(evbase);

    // make a connection
    //connection = new AMQP::TcpConnection(handler, AMQP::Address(config.msgqueue.szConnstr));

    // we need a channel too
    //channel = new AMQP::TcpChannel(connection);

    // 创建队列
    //channel->declareQueue("my_queue", AMQP::durable).onSuccess([](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
    //    std::cout << "declared queue " << name << std::endl;
    //});

    //createQueue("my_queue", &channel);

}

// 初始化ODBC句柄
int CCore::InitOdbc(std::map<std::string, CXa>& mpXa)
{
    if (bInitOdbc)
    {
        return 1;
    }
    auto xa = mpXa.begin();
    // 初始化 ODBC   
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
    SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
    SQLDriverConnect(m_hdbc, NULL, (SQLCHAR*)xa->second.szConnstr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    bInitOdbc = true;
    return 0;
}