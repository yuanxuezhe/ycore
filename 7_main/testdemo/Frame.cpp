#include "Frame.h"

Frame::Frame()
    : m_nFrameX(0)
    , m_nFrameY(0)
    , m_nFrameW(0)
    , m_nFrameH(0)
{
}

Frame::Frame(const char* pos){
    GetPos(pos, m_nFrameX, m_nFrameY, m_nFrameW, m_nFrameH);
}

Frame::~Frame(){
}


void Frame::CallBack(fl_Widget b, void* data){
    _tagComponentInfo* pComponentInfo = (_tagComponentInfo*)data;
    if (!pComponentInfo) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return;
    }
    Fl_File_Input_set_value((fl_File_Input)pComponentInfo->pComponent, "zxzxzzxxz");
    //(Fl_Button*)b->label(pComponentInfo->strText.c_str());
}

void Frame::GetPos(const char* pos, int& x, int& y, int& w, int& h){
    if (pos) {
        std::string posStr(pos);
        std::string::size_type pos = posStr.find(',');
        if (pos != std::string::npos) {
            x = atoi(posStr.substr(0, pos).c_str());
            posStr = posStr.substr(pos + 1);
            pos = posStr.find(',');
            if (pos != std::string::npos) {
                y = atoi(posStr.substr(0, pos).c_str());
                posStr = posStr.substr(pos + 1);
                pos = posStr.find(',');
                if (pos != std::string::npos) {
                    w = atoi(posStr.substr(0, pos).c_str());
                    h = atoi(posStr.substr(pos + 1).c_str());
                }
            }
        }
    }
}

void* Frame::LoadDllAndReturnFunction(const char* str, const char* split){
    if (!str || !split) {
        return NULL;
    }

    std::vector<std::string> vecStr;
    std::string strStr(str);
    std::string strSplit(split);
    std::string::size_type pos = strStr.find(strSplit);
    while (pos != std::string::npos) {
        vecStr.push_back(strStr.substr(0, pos));
        strStr = strStr.substr(pos + 1);
        pos = strStr.find(strSplit);
    }
    vecStr.push_back(strStr);

    std::string dllNameStr(vecStr[0]);
    std::string functionNameStr(vecStr[1]);
    pos = dllNameStr.find('.');
    if (pos != std::string::npos) {
        dllNameStr = dllNameStr.substr(0, pos);
    }

    // 如果已经加载过，直接返回
    if (m_mapDlls.find(dllNameStr) != m_mapDlls.end()) {
        return GetProcAddress(m_mapDlls[dllNameStr], functionNameStr.c_str());
    }

    // 加载dll
    HMODULE hModule = LoadLibraryA(dllNameStr.c_str());
    if (!hModule) {
        return NULL;
    }

    // 加载函数
    void* pFunction = GetProcAddress(hModule, functionNameStr.c_str());
    if (!pFunction) {
        return NULL;
    }

    // 保存dll
    m_mapDlls[dllNameStr] = hModule;

    return pFunction;
}

// 将获取的Components写入m_mapComponents
bool Frame::LoadXml(const char* xmlPath){
    if (!xmlPath) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    // 加载xml
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xmlPath) != tinyxml2::XML_SUCCESS) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    // 获取根节点
    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("Root");
    if (!rootElement) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    // 获取组件元素
    tinyxml2::XMLElement* elementComponents = rootElement->FirstChildElement("Components");

    // 遍历子元素
    if (elementComponents) {
        if (!LoadElementComponents(elementComponents))
        {
            std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
            return false;
        }
    }

    // 获取组件元素
    tinyxml2::XMLElement* elementLayout = rootElement->FirstChildElement("Layout");
    // 遍历子元素
    if (elementLayout) {
        if (!LoadElementLayout(elementLayout))
        {
            std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
            return false;
        }
    }

    return true;
}
bool Frame::LoadElementComponents(tinyxml2::XMLElement* element) {
    if (!element) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    _tagComponentInfo componentInfo;

    // 遍历子元素
    for (tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement()) {
        // 初始化组件信息
        componentInfo.pComponent = NULL;
        // 获取名称
        componentInfo.strText = childElement->Attribute("title");
        // 获取组件类型
        componentInfo.strType = childElement->Attribute("type");
        // 获取组件ID
        const char* id = childElement->Attribute("id");
        if (!id) {
            std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
            return false;
        }
        componentInfo.nId = atoi(id);
        // 如果m_mapComponents存在则报错，不存在则插入
        if (m_mapComponents.find(componentInfo.nId) != m_mapComponents.end()) {
            std::cout << "Error: " << "组件[" << componentInfo.nId << "]冲突" << std::endl;
            return false;
        }
        m_mapComponents[componentInfo.nId] = componentInfo;
    }

    return true;

    // // 获取组件位置
    // const char* pos = element->Attribute("pos");
    // if (pos) {
    //     GetPos(pos, nFrameX, nFrameY, nFrameW, nFrameH);
    // }

    // // 获取组件文本
    // const char* text = element->Attribute("text");
    // if (text) {
    //     strFrameText = text;
    // }

    // // 获取组件字体
    // const char* font = element->Attribute("font");
    // if (font) {
    //     strFrameFont = font;
    // }

    // // 获取组件字体大小
    // const char* fontSize = element->Attribute("fontSize");
    // if (fontSize) {
    //     nFrameFontSize = atoi(fontSize);
    // }

    // // 获取组件字体颜色
    // const char* fontColor = element->Attribute("fontColor");
    // if (fontColor) {
    //     strFrameFontColor = fontColor;
    // }

    // // 获取组件背景颜色
    // const char* backColor = element->Attribute("backColor");
    // if (backColor) {
    //     strFrameBackColor = backColor;
    // }

    // // 获取组件边框颜色
    // const char* borderColor = element->Attribute("borderColor");
    // if (borderColor) {
    //     strFrameBorderColor = borderColor;
    // }

    // // 获取组件边框宽度
    // const char* borderWidth = element->Attribute("borderWidth");
    // if (borderWidth) {
    //     nFrameBorderWidth = atoi(borderWidth);
    // }

    // // 获取组件边框圆角
    // const char* borderRadius = element->Attribute("borderRadius");
    // if (borderRadius) {
    //     nFrameBorderRadius = atoi(borderRadius);
    // }

    // // 获取组件图片
    // const char* image = element->Attribute("image");
    // if (image) {
    //     strFrameImage = image;
    // }

    // // 获取组件图片位置
    // const char* imagePos = element->Attribute("imagePos");
    // if (imagePos) {
    //     strFrameImagePos = imagePos;
    // }

    // // 获取组件图片大小
    // const char* imageSize = element->Attribute("imageSize");
    // if (imageSize) {
    //     strFrameImageSize = imageSize;
    // }

    //
}

// 根据ID查找并返回组件信息，找不到则返回false
bool Frame::GetComponentInfoById(int nId, _tagComponentInfo* &componentInfo) {
    std::map<int, _tagComponentInfo>::iterator iter = m_mapComponents.find(nId);
    if (iter == m_mapComponents.end()) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }
    componentInfo = &iter->second;

    return true;
}

// 按钮回调函数，将数据设置到组件中
void ButtonCallback(Fl_Widget* widget, void* data) {
    if (!widget || !data) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return;
    }

    _tagComponentInfo* pComponentInfo = (_tagComponentInfo*)data;
    if (!pComponentInfo->pComponent) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return;
    }
}

// 根据组件类型初始化组件指针
void Frame::InitComponentByType(_tagComponentInfo& componentInfo) {
    if (componentInfo.strType == "MainFrame") {
        componentInfo.pComponent = Fl_Double_Window_New_WithLabel(m_nFrameW, m_nFrameH, componentInfo.strText.c_str());;
    }
    else if (componentInfo.strType == "Button") {
        componentInfo.pComponent = Fl_Button_New_WithLabel(componentInfo.nX, componentInfo.nY, componentInfo.nW, componentInfo.nH, componentInfo.strText.c_str());;
    }
    // else if (componentInfo.strType == "Label") {
    //     componentInfo.pComponent = new Label();
    // }
    // else if (componentInfo.strType == "Image") {
    //     componentInfo.pComponent = new Image();
    // }
    else if (componentInfo.strType == "Edit") {
        componentInfo.pComponent = Fl_File_Input_New_WithLabel(componentInfo.nX, componentInfo.nY, componentInfo.nW, componentInfo.nH, componentInfo.strText.c_str());
        //(*Fl_DerivedFile_Input)componentInfo.pComponent->value("XXXXXX");
        Fl_File_Input_set_value((fl_File_Input)componentInfo.pComponent, "XXXXXX");
    }
    // else if (componentInfo.strType == "Progress") {
    //     componentInfo.pComponent = new Progress();
    // }
    // else if (componentInfo.strType == "Slider") {
    //     componentInfo.pComponent = new Slider();
    // }
    // else if (componentInfo.strType == "CheckBox") {
    //     componentInfo.pComponent = new CheckBox();
    // }
    // else if (componentInfo.strType == "RadioBox") {
    //     componentInfo.pComponent = new RadioBox();
    // }
    // else if (componentInfo.strType == "ComboBox") {
    //     componentInfo.pComponent = new ComboBox();
    // }
    // else if (componentInfo.strType == "ListView") {
    //     componentInfo.pComponent = new ListView();
    // }
    // else if (componentInfo.strType == "TreeView") {
    //     componentInfo.pComponent = new TreeView();
    // }
    // else if (componentInfo.strType == "TabView") {
    //     componentInfo.pComponent = new TabView();
    // }
    // else if (componentInfo.strType == "WebView") {
    //     componentInfo.pComponent = new WebView();
    // }
    // else if (componentInfo.strType == "VideoView") {
    //     componentInfo.pComponent = new VideoView();
    // }
    // else if (componentInfo.strType == "Canvas") {
    //     componentInfo.pComponent = new Canvas();
    // }
    // else if (componentInfo.strType == "Scroll") {
    //     componentInfo.pComponent = new Scroll();
    // }
    // else if (componentInfo.strType == "ScrollBar") {
    //     componentInfo.pComponent = new ScrollBar();
    // }
    // else if (componentInfo.strType == "ScrollBarH") {
    //     componentInfo.pComponent = new ScrollBarH();
    // }
    // else if (componentInfo.strType == "ScrollBarV") {
    //     componentInfo.pComponent = new ScrollBarV();
    // }
}

// 根据callback字段设置组件的回调函数
bool Frame::SetCallbackByComponentInfo(_tagComponentInfo& componentInfo) {
    if (!componentInfo.pComponent) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    // 如果callback字段长度为0，则不设置回调函数
    if (componentInfo.strCallback.length() == 0) {
        return true;
    }
    // 获取回调函数
    //const char* callback = componentInfo.strCallback.c_str();


    // 获取回调函数指针
    void* pFunction = LoadDllAndReturnFunction(componentInfo.strCallback.c_str(), ":");
    if (!pFunction) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    // 设置回调函数
    if (componentInfo.strType == "MainFrame") {
        Fl_Double_Window_set_callback(componentInfo.pComponent, (fl_Callback*)pFunction);
    }
    else if (componentInfo.strType == "Button") {
        Fl_Button_set_callback_with_user_data(componentInfo.pComponent, CallBack, (void*)&m_mapComponents[6]);
    }
    // else if (componentInfo.strType == "Label") {
    //     componentInfo.pComponent = new Label();
    // }
    // else if (componentInfo.strType == "Image") {
    //     componentInfo.pComponent = new Image();
    // }
    else if (componentInfo.strType == "Edit") {
        Fl_File_Input_set_callback(componentInfo.pComponent, (fl_Callback*)pFunction);
    }
    // else if (componentInfo.strType == "Progress") {
    //     componentInfo.pComponent = new Progress();
    // }
    // else if (componentInfo.strType == "Slider") {
    //     componentInfo.pComponent = new Slider();
    // }
    // else if (componentInfo.strType == "CheckBox") {
    //     componentInfo.pComponent = new CheckBox();
    // }
    // else if (componentInfo.strType == "RadioBox") {
    //     componentInfo.pComponent = new RadioBox();
    // }
    // else if (componentInfo.strType == "ComboBox") {
    //     componentInfo.pComponent = new ComboBox();
    // }
    // else if (componentInfo.strType == "ListView") {
    //     componentInfo.pComponent = new ListView();
    // }
    // else if (componentInfo.strType == "TreeView") {
    //     componentInfo.p

    return true;
}

bool Frame::LoadElementLayout(tinyxml2::XMLElement* pElement) {
    if (!pElement) {
        std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    // 遍历子元素
    for (tinyxml2::XMLElement* childElement = pElement->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement()) {
        // 获取组件ID
        const char* id = childElement->Attribute("id");
        if (!id) {
            std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
            return false;
        }
        int nId = atoi(id);

        _tagComponentInfo* pComponentInfo;
        if (!GetComponentInfoById(nId, pComponentInfo)) {
            std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
            return false;
        }
 
        // 获取位置
        const char* pos = childElement->Attribute("pos");

        if (pComponentInfo->strType == "MainFrame")
        {
            GetPos(pos, m_nFrameX, m_nFrameY, m_nFrameW, m_nFrameH);
            //v = Fl_Double_Window_New_WithLabel(nFrameW, nFrameH, title);
        }
        else
        {
            GetPos(pos, pComponentInfo->nX, pComponentInfo->nY, pComponentInfo->nW, pComponentInfo->nH);
        }

        InitComponentByType(*pComponentInfo);
        
        // 获取回调函数
        const char* callback = childElement->Attribute("callback");
        if (callback) {
            pComponentInfo->strCallback = callback;
            SetCallbackByComponentInfo(*pComponentInfo);
        }

        // 如果有嵌套元素，继续遍历
        if (!LoadElementLayout(childElement))
        {
            std::cout << "Error: " << __FILE__ << ":" << __LINE__ << std::endl;
            return false;
        }

        if (pComponentInfo->strType == "MainFrame")
        {
            Fl_Double_Window_end(pComponentInfo->pComponent);
            //Fl_Double_Window_set_resizable(win,table);
            Fl_Double_Window_show(pComponentInfo->pComponent);

            Fl_run();
        }
    }

    return true;
    } 