#include "tools.h"

// 通过市场、资金账号获取虚拟股东代码
std::string CTools::GetVirtualStkAcct(std::string strCuacctCode, std::string strBoard, int len)
{
  if (len == 0)
  {
    return strCuacctCode + strBoard;
  }

  int nLenFirst = 0;
  int nLenLast = 0;
  nLenFirst = (len - strBoard.length()) / 2;
  nLenLast = len - strBoard.length() - nLenFirst;

  std::string firstStr;
  std::string lastStr;

  if (strCuacctCode.length() < nLenFirst)
  {
    firstStr = strCuacctCode + std::string(nLenFirst - strCuacctCode.length() , '0');
  }
  else
  {
    firstStr = strCuacctCode.substr(0, nLenFirst);
  }

  if (strCuacctCode.length() < nLenLast)
  {
    lastStr = std::string(nLenLast - strCuacctCode.length() , '0') + strCuacctCode;
  }
  else
  {
    lastStr = strCuacctCode.substr(strCuacctCode.length() - nLenLast, nLenLast);
  }

  // 拼接前四位和后四位
  return firstStr + lastStr + strBoard;
}