#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <string>
#include <sstream>
#include "define.h"

class DLLExport CTools
{
public:
    template <typename T, typename... Args>
    static std::string Concatenate(T t, Args... args);

    // 长度拼接
    static std::string GetVirtualStkAcct(std::string strCuacctCode, std::string strBoard, int len = 0);

    // 在字符串中替换所有的子串
    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

    static bool CheckAndSetEmpty(char*& szSrc);
};

// 可变参数模板，处理多个参数
template<typename T, typename... Args>
std::string CTools::Concatenate(T t, Args... args) {
    std::ostringstream oss;
    oss << t;
    if constexpr (sizeof...(args) > 0) {
        oss << Concatenate(args...);
    }
    return oss.str();
}

#endif // __TOOLS_H__