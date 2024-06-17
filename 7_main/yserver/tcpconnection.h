#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <functional>
#include <memory>
#include <iostream>
#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    TCPConnection(struct event_base* base, evutil_socket_t fd)
        : base(base), _fd(fd) {
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if (!bev) {
            throw std::runtime_error("Could not create bufferevent!");
        }

        bufferevent_setcb(bev, &TCPConnection::readCallback, nullptr, &TCPConnection::eventCallback, this);
        bufferevent_enable(bev, EV_READ | EV_WRITE);
    }

    ~TCPConnection() {
        bufferevent_free(bev);
    }

    void send(const std::string& data) {
        bufferevent_write(bev, data.c_str(), data.length());
    }

    void setReadCallback(std::function<void(const std::string&)> cb) {
        readCallbackFunc = cb;
    }

    void setEventCallback(std::function<void(evutil_socket_t, short)> cb) {
        eventCallbackFunc = cb;
    }

    evutil_socket_t fd() {
        return _fd;
    }

private:
    static void readCallback(struct bufferevent* bev, void* ctx) {
        TCPConnection* conn = static_cast<TCPConnection*>(ctx);
        conn->handleRead();
    }

    static void eventCallback(struct bufferevent* bev, short events, void* ctx) {
        TCPConnection* conn = static_cast<TCPConnection*>(ctx);
        conn->handleEvent(events);
    }

    void handleRead() {
        char buffer[4096];
        int n;
        while ((n = bufferevent_read(bev, buffer, sizeof(buffer))) > 0) {
            if (readCallbackFunc) {
                readCallbackFunc(std::string(buffer, n));
            }
        }
    }

    void handleEvent(short events) {
        if (eventCallbackFunc) {
            eventCallbackFunc(_fd, events);
        }
    }

    struct event_base* base;
    struct bufferevent* bev;
    evutil_socket_t _fd;
    std::function<void(const std::string&)> readCallbackFunc;
    std::function<void(evutil_socket_t, short)> eventCallbackFunc;
};
