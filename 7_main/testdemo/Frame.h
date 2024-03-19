#ifndef __FRAME_H__
#define __FRAME_H__

#include "tinyxml2.h"
#include "yscore/Fl_C.h"
#include <map>
#include <vector>

#include <iostream>

// 组件信息结构体
typedef struct _tagComponentInfo {
    void*       pComponent; // 组件指针
    std::string strType; // 组件类型
    std::string strText; // 组件文本
    std::string strCallback; // 回调函数
    int         nId; // 组件ID
    int         nX; // 组件X坐标
    int         nY; // 组件Y坐标
    int         nW; // 组件宽度
    int         nH; // 组件高度


    // std::string strPos; // 组件位置
    // std::string strSize; // 组件大小
    // std::string strText; // 组件文本
    // std::string strFont; // 组件字体
    // std::string strFontSize; // 组件字体大小
    // std::string strFontColor; // 组件字体颜色
    // std::string strBackColor; // 组件背景颜色
    // std::string strBorderColor; // 组件边框颜色
    // std::string strBorderWidth; // 组件边框宽度
    // std::string strBorderRadius; // 组件边框圆角
    // std::string strImage; // 组件图片
    // std::string strImagePos; // 组件图片位置
    // std::string strImageSize; // 组件图片大小
    // std::string strImageColor; // 组件图片颜色
    // std::string strImageBorderColor; // 组件图片边框颜色
    // std::string strImageBorderWidth; // 组件图片边框宽度
    // std::string strImageBorderRadius; // 组件图片边框圆角
    // std::string strImageShadowColor; // 组件图片阴影颜色
    // std::string strImageShadowWidth; // 组件图片阴影宽度
    // std::string strImageShadowOffsetX; // 组件图片阴影X偏移
    // std::string strImageShadowOffsetY; // 组件图片阴影Y偏移
    // std::string strImageShadowRadius; // 组件图片阴影圆角
    // std::string strImageShadowInner; // 组件图片阴影内阴
    // std::string strImageShadowSpread; // 组件图片阴影扩展
    // std::string strImageShadowClip; // 组件图片阴影裁剪
    // std::string strImageShadowInset; // 组件图片阴影插入
    // std::string strImageShadowDirection; // 组件图片阴影方向
    // std::string strImageShadowDistance; // 组件图片阴影距离
    // std::string strImageShadowSize; // 组件图片阴影大小
    // std::string strImageShadowOpacity; // 组件图片阴影透明度
    // std::string strImageShadowBlur; // 组件图片阴影模糊
    // std::string strImageShadowSpread; // 组件图片阴影扩展
    // std::string strImageShadowColor; // 组件图片阴影颜色
    // std::string strImageShadowInset; // 组件图片阴影插入
};

class Frame {
public:
    Frame();
    Frame(const char* pos);
    ~Frame();

    static void CallBack(fl_Widget b, void* data);

    void GetPos(const char* pos, int& x, int& y, int& w, int& h);

    bool LoadXml(const char* xmlPath);

    void* LoadDllAndReturnFunction(const char* str, const char* split);

    bool LoadElementComponents(tinyxml2::XMLElement* element);

    bool LoadElementLayout(tinyxml2::XMLElement* pElement);

    bool GetComponentInfoById(int nId, _tagComponentInfo* &componentInfo);

    void InitComponentByType(_tagComponentInfo& componentInfo);

    bool SetCallbackByComponentInfo(_tagComponentInfo& componentInfo);


public:
    std::map<std::string, HMODULE>      m_mapDlls;
    std::map<int, _tagComponentInfo>    m_mapComponents;
    int m_nFrameX;
    int m_nFrameY;
    int m_nFrameW;
    int m_nFrameH;
 };
 
#endif // __FRAME_H__
