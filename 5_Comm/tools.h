#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <string>
#include <sstream>
//#include <cctype>
#include "define.h"
#include <regex>

class DLLExport CTools
{
public:
    template <typename T, typename... Args>
    static std::string Concatenate(T t, Args... args);

    // 长度拼接
    static std::string GetVirtualStkAcct(std::string strCuacctCode, std::string strBoard, int len = 0);

    // 在字符串中替换所有的子串
    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

    // 正则表达式匹配
    static void RegexMatch(const std::string& str, const std::string& pattern, std::vector<std::string>& result);
    
    // 字符串分割
    static void SplitString(const std::string& str, const std::string& pattern, std::vector<std::string>& result);

    static bool CheckAndSetEmpty(char*& szSrc);
    // 驼峰命名
    static std::string CamelName(std::string name);

    // 全部大写或者全部小写 cFlag 1 | 全部大写  2 | 全部小写
    static std::string UpperLowerSwitch(std::string name, char cFlag);
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