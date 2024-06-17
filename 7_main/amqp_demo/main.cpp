/**
 *  Libevent.cpp
 *
 *  Test program to check AMQP functionality based on Libevent
 *
 *  @author Brent Dimmig <brentdimmig@gmail.com>
 */

/**
 *  Dependencies
 */
#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>


// 定义处理器类
class MyHandler : public AMQP::LibEventHandler {
public:
    MyHandler(struct event_base *evbase) : AMQP::LibEventHandler(evbase) {}

    void onError(AMQP::TcpConnection *connection, const char *message) override {
        std::cerr << "AMQP connection error: " << message << std::endl;
    }

    void onConnected(AMQP::TcpConnection *connection) override {
        std::cout << "AMQP connected" << std::endl;
    }

    // 处理心跳发送事件
    void onHeartbeat(AMQP::TcpConnection *connection) override {
        std::cout << "Sending heartbeat" << std::endl;
    }

    void onReady(AMQP::TcpConnection *connection) override {
        std::cout << "AMQP connection ready" << std::endl;
    }

    void onClosed(AMQP::TcpConnection *connection) override {
        std::cout << "AMQP connection closed" << std::endl;
    }
};

/**
 *  Main program
 *  @return int
 */
int main()
{
    // access to the event loop
    auto evbase = event_base_new();

    // handler for libevent
    MyHandler handler(evbase);
    //AMQP::LibEventHandler handler(evbase);

    // make a connection
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://admin:admin@74.48.82.137:5672"));

    // 设置心跳间隔
    //AMQP::Connection amqpConnection(&connection, AMQP::Login("admin", "admin"), "/", 5); // 心跳间隔为60秒

    // we need a channel too
    AMQP::TcpChannel channel(&connection);

    std::string inQueueName("Queue_Req");
    std::string outQueueName("Queue_Ans");
    
    // 检查并创建队列  //durable 永久 exclusive 专属
    channel.declareQueue(inQueueName, AMQP::durable)
        .onSuccess([&]() {
            std::cout << "Queue " << inQueueName << " exists or created successfully." << std::endl;
            // 消费队列中的消息
            channel.consume(inQueueName)
                .onReceived([&](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
                    //std::cout << "Received message: " << message.body() << std::endl;
                    std::string msgContent(message.body(), message.bodySize());
                    std::cout << "Received message from queue [" << inQueueName << "]: " << msgContent << std::endl;
                    // 手动确认消息
                    channel.ack(deliveryTag);

                    channel.publish("", outQueueName, msgContent.c_str());
                })
                .onSuccess([](const std::string &consumerTag) {
                    std::cout << "Consumer started with tag: " << consumerTag << std::endl;
                })
                .onError([](const char *message) {
                    std::cerr << "Consume operation failed: " << message << std::endl;
                });
        })
        .onError([&](const char *message) {
            if (strstr(message, "PRECONDITION_FAILED")) {
                std::cout << "Queue " << inQueueName << " already exists." << std::endl;
            } else {
                std::cerr << "Failed to declare queue: " << message << std::endl;
            }
        });

        channel.declareQueue(outQueueName, AMQP::durable)
            .onSuccess([&]() {
                std::cout << "created queue ok : " << outQueueName << std::endl;
            })
            .onError([&](const char *message) {
                // 队列创建失败时调用
                std::cerr << "Failed to declare queue: " << message << std::endl;
            });
    // 断开连接
    //connection.close();

    // run the loop
    event_base_dispatch(evbase);
  
    event_base_free(evbase);

    // done
    return 0;
}
