#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "msg.h"

//auto logger = std::make_shared<spdlog::logger>("LoggerTest", std::make_shared<spdlog::sinks::basic_file_sink_mt>("testLog.csv"));
// 创建一个具有颜色支持的控制台日志器
auto logger = spdlog::stdout_color_mt("LoggerTest");

int main() {
    // 原始消息
    char* originalMessage = "Hello, erwsrefsfeworld!";
    // 原始请求发送消息串，通过FormatMsg函数添加包头和包尾，返回值为添加包头和包尾后的消息指针及长度元组
    auto msg = Message::FormatMsg(originalMessage, strlen(originalMessage));
    // 应答处理，读取两字节的包头，获取消息长度数组，然后通过函数GetMsgLen转为数字int
    int msgLen = Message::GetMsgLen(std::get<0>(msg));
    // 往后读取长度为msgLen的消息内容，校验消息是否以MSG_FOOTER结束，并去掉MSG_FOOTER返回msg指针及长度元组
    auto ansmsg = Message::GetMsg(std::get<0>(msg) + 2, msgLen);

    // 打印消息内容
    logger->info("Logger Info: {0} Extracted content: <{1}>", __LINE__, std::get<0>(ansmsg));
    
    return 0;
}
// int main() {
//     // 创建事件处理器
//     MyHandler handler;

//     // 创建 AMQP 连接，并设置事件处理器
//     AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://localhost:5672/"));

//     // 运行事件循环，等待事件到达
//     AMQP::EventLoop::run();

//     return 0;
// }


// class MyHandler : public AMQP::LibEventHandler {
// public:
//     MyHandler() {}

//     // 处理连接成功事件
//     void onConnected(AMQP::TcpConnection *connection) override {
//         std::cout << "Connected to RabbitMQ server" << std::endl;

//         // 创建通道
//         m_channel = new AMQP::TcpChannel(connection);

//         // 声明一个名为 test 的队列
//         m_channel->declareQueue("test");

//         // 订阅消费队列
//         m_channel->consume("test")
//             .onReceived([](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
//                 std::cout << "Received message: " << message.message() << std::endl;
//             })
//             .onSuccess([]() {
//                 std::cout << "Consume operation successful" << std::endl;
//             })
//             .onError([](const char *message) {
//                 std::cerr << "Error: " << message << std::endl;
//             });
//     }

//     // 处理连接关闭事件
//     void onClosed(AMQP::TcpConnection *connection) override {
//         std::cout << "Connection closed" << std::endl;
//     }

//     // 处理通道关闭事件
//     void onChannelClosed(AMQP::TcpChannel *channel) override {
//         std::cout << "Channel closed" << std::endl;
//     }

//     // 处理错误事件
//     void onError(AMQP::TcpConnection *connection, const char *message) override {
//         std::cerr << "Error: " << message << std::endl;
//     }

// private:
//     AMQP::TcpChannel *m_channel;
// };

// /**
//  *  Main program
//  *  @return int
//  */
// int main()
// {
//     //auto logger = std::make_shared<spdlog::logger>("LoggerTest", std::make_shared<spdlog::sinks::basic_file_sink_mt>("testLog.csv"));
//     // 创建一个具有颜色支持的控制台日志器
//     auto logger = spdlog::stdout_color_mt("LoggerTest");
//     logger->info("Logger Info: {0}", __LINE__);
//     // 创建 libevent 事件循环
//     struct event_base *base = event_base_new();
//     logger->info("Logger Info: {0}", __LINE__);
//     // handler for libevent
//     AMQP::LibEventHandler handler(base);
//     logger->info("Logger Info: {0}", __LINE__);
//     // 创建 AMQP 连接
//     AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://localhost:5672/"));
//     logger->info("Logger Info: {0}", __LINE__);
//     // 创建 AMQP 通道
//     AMQP::TcpChannel channel(&connection);
//     logger->info("Logger Info: {0}", __LINE__);
//     // 声明一个名为 test 的队列
//     channel.declareQueue("test");
//     logger->info("Logger Info: {0}", __LINE__);
//     // 发送消息到队列
//     channel.publish("", "test", "Hello, RabbitMQ!");

//     logger->info("Logger Info: {0}", __LINE__);
//     // 运行 libevent 事件循环
//     event_base_dispatch(base);
//     logger->info("Logger Info: {0}", __LINE__);
//     // 释放 libevent 资源
//     event_base_free(base);
//     logger->info("Logger Info: {0}", __LINE__);
//     return 0;
// }
