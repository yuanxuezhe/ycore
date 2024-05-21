#include "msg.h"
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <string.h>

// 回调函数，处理接收到的数据
void echo_read_cb(struct bufferevent *bev, void *ctx) {
    struct evbuffer *input = bufferevent_get_input(bev);
    struct evbuffer *output = bufferevent_get_output(bev);

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
        printf("recv the client msg req: %s\n", std::get<0>(ansmsg));
    }

    evbuffer_drain(input, MSG_HEADER_BYTES + msgLen);

    evbuffer_add(output, msg, MSG_HEADER_BYTES + msgLen);
    delete[] std::get<0>(ansmsg);
    delete[] szMsgLen;
    delete[] msg;

    echo_read_cb(bev, ctx);
}

// 回调函数，处理事件
void event_cb1(struct bufferevent *bev, short events, void *ctx) {
    printf("Client event~.\n");
    if (events & BEV_EVENT_EOF) {
        // 客户端连接断开
        printf("Client disconnected.\n");
        // 连接关闭或发生错误，释放bufferevent
        bufferevent_free(bev);
    } else if (events & BEV_EVENT_ERROR) {
        perror("Error from bufferevent");
        bufferevent_free(bev);
    }
}
void event_cb(struct bufferevent *bev, short events, void *ctx) {
    if (events & BEV_EVENT_CONNECTED) {
        // 连接建立成功
        printf("Client connected.\n");
    } else if (events & BEV_EVENT_ERROR) {
        // 发生错误
        printf("Error on client connection.\n");
        bufferevent_free(bev);
    } else if (events & BEV_EVENT_EOF) {
        // 客户端连接断开
        printf("Client disconnected.\n");
        bufferevent_free(bev);
    } else {
        printf("Client event~.\n");
    }
}

// 回调函数，处理新的连接请求
void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx) {
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    // 设置读写回调函数
    bufferevent_setcb(bev, echo_read_cb, NULL, event_cb, NULL);

    // 启用读写事件
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

int main() {
    struct event_base *base;
    struct evconnlistener *listener;
    struct sockaddr_in sin;

    base = event_base_new();
    if (!base) {
        perror("Error creating event base");
        return -1;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8090);

    listener = evconnlistener_new_bind(base, accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr *)&sin, sizeof(sin));
    if (!listener) {
        perror("Error creating listener");
        return -1;
    }

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);

    return 0;
}
