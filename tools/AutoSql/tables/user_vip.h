//--------------------------------------------------------------------------------------------------
// 版权声明：本程序模块属于YCORE模块功能
//           YUANSHUAI  版权所有
//
// 文件名称：user_vip.h
// 模块名称：user_vip
// 模块描述：
// 开发作者：yuanshuai
// 创建日期：2019-03-25
// 模块版本：001.000.000
//--------------------------------------------------------------------------------------------------
// 修改日期      版本          作者            备注
//--------------------------------------------------------------------------------------------------
// 2019-03-25    001.000.000  yuanshuai       原创
//--------------------------------------------------------------------------------------------------
#ifndef __USER_VIP_H__
#define __USER_VIP_H__

#include "define.h"
#include "core.h"
#include "ystream.h"

class CUserVip
{
public:
    CUserVip(CCore* core) : m_pCore(core)
    {
        Clear();
    }

    void Clear()
    {
        memset(&m_nUserId, 0, ((char*)&m_nCreateDate - (char*)&m_nUserId + sizeof(m_nCreateDate)));
    }

    bool Query(const int   nUserId, const char* szVipType)     
    {
        CYStream strm(m_pCore); 
        strm.Open("select USER_ID, VIP_TYPE, VIP_LEVEL, START_DATE, END_DATE, CREATE_DATE from USER_VIP "
        " where USER_ID = {} and VIP_TYPE = {}", nUserId, P(szVipType));
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
        strm.Open("insert into USER_VIP(USER_ID, VIP_TYPE, VIP_LEVEL, START_DATE, END_DATE, CREATE_DATE) "
        " values ({}, {}, {}, {}, {}, {})", m_nUserId, P(m_szVipType), P(m_szVipLevel), m_nStartDate, m_nEndDate, m_nCreateDate);
        strm.Exec();

        return true;
    }

    inline bool Update()     
    {
        CYStream strm(m_pCore);
        strm.Open("UPDATE  USER_VIP SET VIP_LEVEL = {}, START_DATE = {}, END_DATE = {}, CREATE_DATE = {} "
        " where USER_ID = {} and VIP_TYPE = {}", P(m_szVipLevel), m_nStartDate, m_nEndDate, m_nCreateDate, m_nUserId, P(m_szVipType));
        strm.Exec();

        return true;
    }

    inline bool Delete()     
    {
        CYStream strm(m_pCore);
        strm.Open("DELETE FROM  USER_VIP  "
        " where USER_ID = {} and VIP_TYPE = {}", m_nUserId, P(m_szVipType));
        strm.Exec();

        return true;
    }

    inline void GetQueryValue(CYStream &strm)
    {
        strm.GetValue("USER_ID", m_nUserId);  // 用户ID"
        strm.GetValue("VIP_TYPE", m_szVipType, sizeof(m_szVipType) - 1);  // VIP类型"
        strm.GetValue("VIP_LEVEL", m_szVipLevel, sizeof(m_szVipLevel) - 1);  // VIP等级"
        strm.GetValue("START_DATE", m_nStartDate);  // VIP开始日期"
        strm.GetValue("END_DATE", m_nEndDate);  // VIP结束日期"
        strm.GetValue("CREATE_DATE", m_nCreateDate);  // VIP记录创建日期"
    }

private:
    CCore*  m_pCore;

public:
    int   m_nUserId;      // 用户ID"
    char  m_szVipType[32 + 1];      // VIP类型"
    char  m_szVipLevel[32 + 1];      // VIP等级"
    int   m_nStartDate;      // VIP开始日期"
    int   m_nEndDate;      // VIP结束日期"
    int   m_nCreateDate;      // VIP记录创建日期"
};

#endif // __USER_VIP_H__