#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <functional>
#include <memory>
#include <iostream>
#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include "msg.h"

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
        while(true)
        {
            struct evbuffer *input = bufferevent_get_input(bev);
            if (evbuffer_get_length(input) < MSG_HEADER_BYTES)
            {
                return;
            }

            unsigned char* szMsgLen = new unsigned char[MSG_HEADER_BYTES];
            evbuffer_copyout(input, szMsgLen, MSG_HEADER_BYTES);
        
            // 应答处理，读取两字节的包头，获取消息长度数组，然后通过函数GetMsgLen转为数字int
            int msgLen = Message::GetMsgLen(szMsgLen);

            if (evbuffer_get_length(input) < MSG_HEADER_BYTES + msgLen)
            {
                return;
            }

            char* msg = new char[MSG_HEADER_BYTES + msgLen];
            evbuffer_copyout(input, msg, MSG_HEADER_BYTES + msgLen);

            // 往后读取长度为msgLen的消息内容，校验消息是否以MSG_FOOTER结束，并去掉MSG_FOOTER返回msg指针及长度元组
            auto ansmsg = Message::GetMsg(msg + MSG_HEADER_BYTES, msgLen);
            if (std::get<0>(ansmsg) == nullptr)
            {
                printf("recv the client msg: MSG ERROR\n");
                return;
            }
            else
            {
                //printf("recv the client msg ans: %s\n", std::get<0>(ansmsg));
            }

            evbuffer_drain(input, MSG_HEADER_BYTES + msgLen);

            //bufferevent_write(bev, std::get<0>(ansmsg), std::get<1>(ansmsg) + 1);
            readCallbackFunc(std::string(std::get<0>(ansmsg), std::get<1>(ansmsg) + 1));
            delete[] std::get<0>(ansmsg);
        }
    }
    /// @brief 
    /// @param events 
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
