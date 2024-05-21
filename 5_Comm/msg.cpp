#include "msg.h"

// szMsgLen转为数字int
int Message::GetMsgLen(const char* szMsgLen) {
    int nMsgLen = 0;
    for (size_t i = 0; i < MSG_HEADER_BYTES; i++)
    {
        nMsgLen += (szMsgLen[i] << (MSG_HEADER_BYTES - 1 - i) * 8);
    }
    return nMsgLen;
}

// 校验消息是否以MSG_FOOTER结束，并去掉MSG_FOOTER返回msg内容
std::tuple<char*, int> Message::GetMsg(const char* message, int msgLen) {
    if (msgLen < strlen(MSG_FOOTER)) {
        return std::make_tuple(nullptr, 0);
    }

    // message中是否包含MSG_FOOTER
    if (std::strncmp(message + msgLen - strlen(MSG_FOOTER), MSG_FOOTER, strlen(MSG_FOOTER)) != 0) {
        return std::make_tuple(nullptr, 0);
    }

    // 提取消息内容
    int extractedContentLen = msgLen - strlen(MSG_FOOTER) + 1;
    char* extractedContent = new char[extractedContentLen];
    std::strncpy(extractedContent, message, msgLen - 2);
    extractedContent[extractedContentLen - 1] = '\0';

    return std::make_tuple(extractedContent, extractedContentLen);
}

// 格式化字符串,修改入参指针数据，函数返回值输出长度
std::tuple<char*, int> Message::FormatMsg(char* message, int msgLen)
{
    if (!CTools::CheckAndSetEmpty(message)) {
        msgLen = 0;
    }

    int len = msgLen + strlen(MSG_FOOTER);
    // 构造两字节的整数
    unsigned char szMsgLen[MSG_HEADER_BYTES];
    for (size_t i = 0; i < MSG_HEADER_BYTES; i++)
    {
        szMsgLen[i] = (len >> (MSG_HEADER_BYTES - 1 - i) * 8) & 0xFF;
    }

    // 创建带有包头和包尾的新消息
    int newMessageLen = MSG_HEADER_BYTES + len;
    char* newMessage = new char[newMessageLen];

    memcpy(newMessage, szMsgLen, MSG_HEADER_BYTES);
    memcpy(newMessage + MSG_HEADER_BYTES, message, msgLen);
    memcpy(newMessage + MSG_HEADER_BYTES + msgLen, MSG_FOOTER, strlen(MSG_FOOTER));

    return std::make_tuple(newMessage, newMessageLen);
}

