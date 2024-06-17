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

// 在字符串中替换所有的子串
void CTools::ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void CTools::SplitString(const std::string& str, const std::string& pattern, std::vector<std::string>& result)
{
    std::string::size_type pos;
    std::string::size_type start = 0;
    result.clear();
    pos = str.find(pattern);
    while (pos != std::string::npos)
    {
        result.push_back(str.substr(start, pos - start));
        start = pos + pattern.size();
        pos = str.find(pattern, start);
    }
    result.push_back(str.substr(start));
}

bool CTools::CheckAndSetEmpty(char*& szSrc) {
    if (szSrc == nullptr) {
        szSrc = new char[1]; // 分配一个字符的空间
        szSrc[0] = '\0';     // 设置为空字符
        return false;
    }
    return true;
}

// 正则表达式匹配
void CTools::RegexMatch(const std::string& str, const std::string& pattern, std::vector<std::string>& result)
{
  result.clear();
  std::regex regex(pattern);
  // 进行匹配并输出所有匹配的子串
  auto words_begin = std::sregex_iterator(str.begin(), str.end(), regex);
  auto words_end = std::sregex_iterator();

  std::map<std::string, std::string> mpReplace;

  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
      std::smatch match = *i;
      std::string match_str = match.str();
      //std::cout << "Found match: " << match_str << std::endl;
      result.push_back(match_str);
  }

  return;
}

// 驼峰命名
std::string CTools::CamelName(std::string name)
{
    std::istringstream iss(name);
    std::string word;
    std::string camelCaseName;
    bool firstWord = true;
    while (std::getline(iss, word, '_')) {
        camelCaseName += [](std::string& tmp) -> std::string {
            if (tmp.empty()) return tmp;
            std::string result = tmp;
            // 单词全部小写
            for (char &c : result) {
                c = std::tolower(c);
            }
            // 第一个字母大写
            result[0] = std::toupper(result[0]);
            return result;
        }(word);
    }

    return camelCaseName;
}

std::string CTools::UpperLowerSwitch(std::string src, char cFlag)
{
    std::transform(src.begin(), src.end(), src.begin(),
        [cFlag](unsigned char c){ return cFlag == '1' ? std::toupper(c) : std::tolower(c); }
    );
    return src;
}