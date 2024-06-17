#include <event2/event.h>
#include <event2/listener.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "tcpconnection.h"

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

class TCPConnection; // 前向声明

class EventCenter {
public:
    EventCenter():queueName("queue1")
    {
        base = event_base_new();
        if (!base) {
            throw std::runtime_error("Could not initialize libevent!");
        }
    }

    ~EventCenter() {
        evconnlistener_free(listener);
        event_base_free(base);
    }

    // void onNewConnection(std::shared_ptr<TCPConnection> conn) {
    //     std::cout << "New connection established!" << std::endl;

    //     conn->setReadCallback([](const std::string& data) {
    //         std::cout << "Received data: " << data << std::endl;
    //     });

    //     conn->setEventCallback([](short events) {
    //         if (events & BEV_EVENT_ERROR) {
    //             std::cerr << "Error on connection!" << std::endl;
    //         }
    //         if (events & BEV_EVENT_EOF) {
    //             std::cout << "Connection closed." << std::endl;
    //         }
    //     });
    // }
    void InitNetCenter(int port)
    {
        sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port = htons(port);

        listener = evconnlistener_new_bind(base, &EventCenter::listenerCallback, this,
                                           LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                                           (struct sockaddr*)&sin, sizeof(sin));
        if (!listener) {
            throw std::runtime_error("Could not create listener!");
        }

        onConnection = [&](std::shared_ptr<TCPConnection> conn) {
            connections[conn->fd()] = conn;
            std::cout << "New connection established!  fd:" <<  conn->fd()<< "counts:" << connections.size() << std::endl;
            conn->setReadCallback([&](const std::string& data) {
                this->amqpChannel->publish("", "queue1", data.c_str());

            });

            conn->setEventCallback([&](evutil_socket_t fd, short events) {
                if (events & BEV_EVENT_ERROR) {
                    std::cerr << "Error on connection!" << std::endl;
                }
                if (events & BEV_EVENT_EOF) {
                    std::cout << "Connection closed. fd:" << fd <<  std::endl;
                    connections.erase(fd);
                    std::cout << "Connection closed." << std::endl;
                    std::cout << "erase one connection! fd:" <<  fd << ", last:" << connections.size() << std::endl;
                }
            });
        };
    }

    void InitAmqpCenter()
    {
        amqpHandler = new MyHandler(base);

        amqpConnection = new AMQP::TcpConnection(amqpHandler, AMQP::Address("amqp://admin:admin@74.48.82.137:5672"));

        amqpChannel = new AMQP::TcpChannel(amqpConnection);

        // 检查并创建队列  //durable 永久 exclusive 专属
        // 声明队列并设置回调函数
        amqpChannel->declareQueue(queueName, AMQP::durable)
            .onSuccess([&]() {
                // 消费队列中的消息
                amqpChannel->consume(queueName)
                    .onReceived([&](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
                        // 获取消息内容
                        std::string msgContent(message.body(), message.bodySize());
                        std::cout << "Received message: " << msgContent << std::endl;
                        // 手动确认消息
                        amqpChannel->ack(deliveryTag);
                    })
                    .onSuccess([](const std::string &consumerTag) {
                        std::cout << "Consumer started with tag: " << consumerTag << std::endl;
                    })
                    .onError([](const char *message) {
                        std::cerr << "Consume operation failed: " << message << std::endl;
                    });
            })
            .onError([&](const char *message) {
                // 队列创建失败时调用
                std::cerr << "Failed to declare queue: " << message << std::endl;
            });
    }

    void start() {
        event_base_dispatch(base);
    }

    // 当connection断开时，从connections中移除
    void removeConnection(evutil_socket_t fd) {
        connections.erase(fd);
    }
private:
    static void listenerCallback(struct evconnlistener* listener, evutil_socket_t fd,
                                 struct sockaddr* address, int socklen, void* ctx) {
        EventCenter* server = static_cast<EventCenter*>(ctx);
        server->handleNewConnection(fd);
    }

    void  handleNewConnection(evutil_socket_t fd) {
        auto connection = std::make_shared<TCPConnection>(base, fd);
        onConnection(connection);
    }

    struct event_base* base;
    struct evconnlistener* listener;
    std::function<void(std::shared_ptr<TCPConnection>)> onConnection;
    std::unordered_map<evutil_socket_t, std::shared_ptr<TCPConnection>> connections;

    MyHandler* amqpHandler;
    AMQP::TcpConnection* amqpConnection;
    AMQP::TcpChannel* amqpChannel;
    std::string queueName;
};
