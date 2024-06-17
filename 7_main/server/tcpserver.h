#include <event2/event.h>
#include <event2/listener.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "tcpconnection.h"

class TCPConnection; // 前向声明

class EventCenter {
public:
    EventCenter(int port, std::function<void(std::shared_ptr<TCPConnection>)> onConnection)
        : onConnection(onConnection) {
        base = event_base_new();
        if (!base) {
            throw std::runtime_error("Could not initialize libevent!");
        }

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
    }

    ~EventCenter() {
        evconnlistener_free(listener);
        event_base_free(base);
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
        connections[fd] = connection;
        onConnection(connection);
    }

    struct event_base* base;
    struct evconnlistener* listener;
    std::function<void(std::shared_ptr<TCPConnection>)> onConnection;
    std::unordered_map<evutil_socket_t, std::shared_ptr<TCPConnection>> connections;
};
