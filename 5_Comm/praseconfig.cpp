#include "praseconfig.h"
#include "tinyxml2.h"

// 参考CommStruct.h头文件中定义的结构体，和example.xml文件中的定义，写一个解析类函数，将example.xml文件中的内容解析到结构体中，实现class PraseConfig的方法
// PraseConfig::PraseConfig()
// {
//     //strncpy(m_szFilePath, "example.xml", sizeof(m_szFilePath));
// }

// PraseConfig::~PraseConfig()
// {
// }

// 解析xml文件
bool PraseConfig::PraseXmlFile(CConfig &config)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile("example.xml");
    if (doc.Error() != tinyxml2::XML_SUCCESS) {
        printf("Error loading file: %s\n", doc.ErrorName());
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("root"); // 获取根元素
    if (!root) {
        printf("No root element.\n");
        return false;
    }

    // 遍历子元素查找log标签
    tinyxml2::XMLElement* log = root->FirstChildElement("log");
    config.log.cModel = log->Attribute("model")[0];
    strncpy(config.log.szConsolelvl, log->Attribute("consolelvl"), sizeof(config.log.szConsolelvl) - 1) ;
    strncpy(config.log.szLoglvl, log->Attribute("loglvl"), sizeof(config.log.szLoglvl) - 1) ;
    strncpy(config.log.szPattern, log->Attribute("pattern"), sizeof(config.log.szPattern) - 1) ;
    strncpy(config.log.szFilepath, log->Attribute("filepath"), sizeof(config.log.szFilepath) - 1) ;
    config.log.nFilesize = atoi(log->Attribute("filesize"));
    config.log.nMaxfiles = atoi(log->Attribute("maxfiles"));

    // 按名称查找msgqueues标签
    tinyxml2::XMLElement* msgqueues = root->FirstChildElement("msgqueues");
    if (!msgqueues) {
        printf("No msgqueues element.\n");
        return false;
    }

    strncpy(config.msgqueue.szName, msgqueues->Attribute("name"), sizeof(config.msgqueue.szName) - 1) ;
    strncpy(config.msgqueue.szType, msgqueues->Attribute("type"), sizeof(config.msgqueue.szType) - 1) ;
    strncpy(config.msgqueue.szConnstr, msgqueues->Attribute("connstr"), sizeof(config.msgqueue.szConnstr) - 1) ;

    // 遍历子元素并输出内容
    for (tinyxml2::XMLElement* elem = msgqueues->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        CQueue cq;
        strncpy(cq.szName, elem->Attribute("name"), sizeof(cq.szName) - 1) ;
        cq.nMaxDepth = atoi(elem->Attribute("maxdepth"));
        config.mpQueue.insert(std::make_pair(cq.szName, cq));
    }

    // 按名称查找xas标签
    tinyxml2::XMLElement* xas = root->FirstChildElement("xas");
    if (!xas) {
        printf("No xas element.\n");
        return false;
    }

    // 遍历子元素并输出内容
    for (tinyxml2::XMLElement* elem = xas->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        CXa cx;
        strncpy(cx.szName, elem->Attribute("name"), sizeof(cx.szName) - 1) ;
        strncpy(cx.szConnstr, elem->Attribute("connstr"), sizeof(cx.szConnstr) - 1) ;
        config.mpXa.insert(std::make_pair(cx.szName, cx));
    }

    return true;
}
