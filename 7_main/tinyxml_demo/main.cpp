#include <iostream>
#include "tinyxml2.h"

int main() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("example.xml"); // 加载 XML 文件

    if (doc.Error() != tinyxml2::XML_SUCCESS) {
        std::cout << "Error loading file: " << doc.ErrorName() << std::endl;
        return 1;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("root"); // 获取根元素
    if (!root) {
        std::cout << "No root element." << std::endl;
        return 1;
    }

    // 遍历子元素并输出内容
    for (tinyxml2::XMLElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
        const char* name = elem->Name();
        const char* text = elem->GetText();
        std::cout << "Element name: " << name << ", Text: " << (text ? text : "(null)") << std::endl;
    }

    return 0;
}
