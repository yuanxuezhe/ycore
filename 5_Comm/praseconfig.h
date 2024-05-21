#ifndef __PRASECONFIG_H__
#define __PRASECONFIG_H__

#include "CommStruct.h"
#include "define.h"
#include <string.h>

// 参考CommStruct.h头文件中定义的结构体，和example.xml文件中的定义，写一个解析类函数，将example.xml文件中的内容解析到结构体中
class DLLExport PraseConfig
{
public:
    //PraseConfig();
    //~PraseConfig();

    // 解析xml文件
    static bool PraseXmlFile(CConfig &config);

//private:
//    static char m_szFilePath[64];
};

#endif // __PRASECONFIG_H__