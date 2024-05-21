#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <arpa/inet.h>
#include "msg.h"
#define MESSAGE_COUNT 10

// 回调函数，当读取到数据时被调用
void read_cb(struct bufferevent *bev, void *ctx) {
    struct evbuffer *input = bufferevent_get_input(bev);
    // char *msg;
    // size_t len;
    // // 读取数据
    // while ((msg = evbuffer_readln(input, &len, EVBUFFER_EOL_CRLF))) {
    //     printf("Received message: %s\n", msg);
    //     free(msg);
    // }
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
        printf("recv the client msg ans: %s\n", std::get<0>(ansmsg));
    }

    evbuffer_drain(input, MSG_HEADER_BYTES + msgLen);

    bufferevent_write(bev, std::get<0>(ansmsg), std::get<1>(ansmsg) + 1);
    delete[] std::get<0>(ansmsg);
    delete[] szMsgLen;
    delete[] msg;

    read_cb(bev, ctx);
}

// 回调函数，当连接建立成功时被调用
void event_cb(struct bufferevent *bev, short events, void *ctx) {
    if (events & BEV_EVENT_CONNECTED) {
        // 启用读写事件
        bufferevent_enable(bev, EV_READ | EV_WRITE);
        printf("Connection ok.\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("Connection error.\n");
        bufferevent_free(bev);
    }
}

void stdin_cb(evutil_socket_t fd, short events, void *arg) {
    struct bufferevent *bev = (struct bufferevent *)arg;
    char msg[1024];
    if (fgets(msg, sizeof(msg), stdin)) {
        // 移除末尾的换行符
       //msg[strcspn(msg, "\n")] = '\0';

        
        for (int i = 0; i < MESSAGE_COUNT; ++i) {
            // 发送消息到服务端
            char message[256] = {0};
            snprintf(message, sizeof(message), "Message %d", i+1);
            auto formatmsg = Message::FormatMsg(message, strlen(message));
            
            bufferevent_write(bev, std::get<0>(formatmsg), std::get<1>(formatmsg));
        }
    }
}

int main() {
    struct event_base *base = event_base_new();
    struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

    // 设置服务器地址和端口
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(8090);

    // 连接服务器
    bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin));

    // 设置回调函数
    bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);
    
    // 创建事件监听标准输入的事件
    struct event *ev_stdin = event_new(base, fileno(stdin), EV_READ | EV_PERSIST, stdin_cb, bev);
    event_add(ev_stdin, NULL);

    // 开始事件循环
    event_base_dispatch(base);

    // 释放资源
    bufferevent_free(bev);
    event_base_free(base);

    return 0;

}
