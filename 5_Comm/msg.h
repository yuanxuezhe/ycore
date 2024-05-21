#include <cstring>
#include <sstream>
#include <iomanip>
#include "tools.h"
#include <tuple>

#define MSG_HEADER_BYTES 2
#define MSG_FOOTER "\r\n"
#define MSG_MAX_LENTH 10 * 1024

class DLLExport Message {
public:
    // szMsgLen转为数字int
    static int GetMsgLen(const char* szMsgLen);

    // 校验消息是否以MSG_FOOTER结束，并去掉MSG_FOOTER返回msg内容
    static std::tuple<char*, int> GetMsg(const char* message, int msgLen);

    // 格式化字符串,修改入参指针数据，函数返回值输出长度
    static std::tuple<char*, int> FormatMsg(char* message, int len);
};