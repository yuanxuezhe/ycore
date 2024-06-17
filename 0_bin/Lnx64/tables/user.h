//--------------------------------------------------------------------------------------------------
// 版权声明：本程序模块属于YCORE模块功能
//           YUANSHUAI  版权所有
//
// 文件名称：user.h
// 模块名称：user
// 模块描述：
// 开发作者：yuanshuai
// 创建日期：2019-03-25
// 模块版本：001.000.000
//--------------------------------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//--------------------------------------------------------------------------------------------------
// 2019-03-25    001.000.000  yuanshuai       原创
//--------------------------------------------------------------------------------------------------
#ifndef __USER_H__
#define __USER_H__

#include "define.h"
#include "core.h"
#include "ystream.h"

class CUser
{
public:
    CUser(CCore* core) : m_pCore(core)
    {
        Clear();
    }

    void Clear()
    {
        memset(&m_nUserId, 0, ((char*)&m_nCreateData - (char*)&m_nUserId + sizeof(m_nCreateData)));
    }

    bool Query(const int   nUserId)     
    {
        CYStream strm(m_pCore); 
        strm.Open("select USER_ID, USERNAME, PHONE, ID_TYPE, ID_NUMBER, ADDRESS, BIRTHDAY, SEX, CREATE_DATA from USER "
        " where USER_ID = {}", nUserId);
        strm.Exec();

        if (strm.Fetch())
        {
            GetQueryValue(strm);
            return true;
        }

        return false;
    }

    inline bool Insert()     
    {
        CYStream strm(m_pCore);
        strm.Open("insert into USER(USER_ID, USERNAME, PHONE, ID_TYPE, ID_NUMBER, ADDRESS, BIRTHDAY, SEX, CREATE_DATA) "
        " values ({}, {}, {}, {}, {}, {}, {}, {}, {})", m_nUserId, P(m_szUsername), P(m_szPhone), m_cIdType, P(m_szIdNumber), P(m_szAddress), m_nBirthday, m_cSex, m_nCreateData);
        strm.Exec();

        return true;
    }

    inline bool Update()     
    {
        CYStream strm(m_pCore);
        strm.Open("UPDATE  USER SET USERNAME = {}, PHONE = {}, ID_TYPE = {}, ID_NUMBER = {}, ADDRESS = {}, BIRTHDAY = {}, SEX = {}, CREATE_DATA = {} "
        " where USER_ID = {}", P(m_szUsername), P(m_szPhone), m_cIdType, P(m_szIdNumber), P(m_szAddress), m_nBirthday, m_cSex, m_nCreateData, m_nUserId);
        strm.Exec();

        return true;
    }

    inline bool Delete()     
    {
        CYStream strm(m_pCore);
        strm.Open("DELETE FROM  USER  "
        " where USER_ID = {}", m_nUserId);
        strm.Exec();

        return true;
    }

    inline void GetQueryValue(CYStream &strm)
    {
        strm.GetValue("USER_ID", m_nUserId);  // 用户ID"
        strm.GetValue("USERNAME", m_szUsername, sizeof(m_szUsername) - 1);  // 用户名"
        strm.GetValue("PHONE", m_szPhone, sizeof(m_szPhone) - 1);  // 电话号码"
        strm.GetValue("ID_TYPE", m_cIdType);  // 证件类型"
        strm.GetValue("ID_NUMBER", m_szIdNumber, sizeof(m_szIdNumber) - 1);  // 证件编号"
        strm.GetValue("ADDRESS", m_szAddress, sizeof(m_szAddress) - 1);  // 地址"
        strm.GetValue("BIRTHDAY", m_nBirthday);  // 生日"
        strm.GetValue("SEX", m_cSex);  // 性别"
        strm.GetValue("CREATE_DATA", m_nCreateData);  // 开户日期"
    }

private:
    CCore*  m_pCore;

public:
    int   m_nUserId;      // 用户ID"
    char  m_szUsername[32 + 1];      // 用户名"
    char  m_szPhone[16 + 1];      // 电话号码"
    char  m_cIdType;      // 证件类型"
    char  m_szIdNumber[32 + 1];      // 证件编号"
    char  m_szAddress[256 + 1];      // 地址"
    int   m_nBirthday;      // 生日"
    char  m_cSex;      // 性别"
    int   m_nCreateData;      // 开户日期"
};

#endif // __USER_H__