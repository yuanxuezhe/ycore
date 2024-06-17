#include "auto2code.h"

std::string Auto2Code::LoadCodeTemplate(const std::string &strPath)
{
    // 创建输入文件流对象
    std::ifstream file(strPath);

    // 检查文件是否成功打开
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << strPath << std::endl;
        return "";
    }

    // 使用 stringstream 将文件内容读取到字符串
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

///////////////////////////////////
void Auto2Code::IntoFile(const std::string &text, const std::string &strPath)
{
    // 打开文件进行写入
    std::ofstream outfile(strPath);
    // 检查文件是否成功打开
    if (!outfile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return 1;
    }

    // 将字符串写入文件
    outfile << text;

    // 关闭文件
    outfile.close();

    return;
}

static std::string Auto2Code::RuleTransfor(const TableInfo &tableInfo, const std::string &strRule)
{
    std::string res("");
    std::string ruleTmp("");
    if (strRule.length() >= 4) {
        // 去掉第一位和最后一位字符
        ruleTmp = strRule.substr(2, strRule.length() - 4);
    }
    // 分割strRule
    std::vector<std::string> vcRule;
    CTools::SplitString(ruleTmp, "|", vcRule);

    // 检查是否有4个元素
    if (vcRule.size() != 5) {
        return "";
    }

    std::vector<Atom> vcAtom;
    vcAtom.clear();
    // 检查第一个元素
    if (vcRule[0] == "TABLE") {
        vcAtom.push_back(Auto2Code::Convert2Atom(tableInfo, vcRule[2]));
    }
    else if (vcRule[0] == "ALL_COLUMN") {
        for (int i = 0; i < tableInfo.mpIndex["ALL_COLUMN"].size(); i++) {
            vcAtom.push_back(Auto2Code::Convert2Atom(tableInfo.mpIndex["ALL_COLUMN"][i], vcRule[1], vcRule[2]));
        }
    }
    else if (vcRule[0] == "PK_COLUMN") {
        for (int i = 0; i < tableInfo.vcColumnInfo.size(); i++) {
            if (!tableInfo.vcColumnInfo[i].bIsPrimaryKey)
            {
                continue;
            }
            vcAtom.push_back(Auto2Code::Convert2Atom(tableInfo.vcColumnInfo[i], vcRule[1], vcRule[2]));
        }
    }
    else if (vcRule[0] == "NOTPK_COLUMN") {
        int pos = 0;
        for (int i = 0; i < tableInfo.vcColumnInfo.size(); i++) {
            if (tableInfo.vcColumnInfo[i].bIsPrimaryKey)
            {
                continue;
            }
            vcAtom.push_back(Auto2Code::Convert2Atom(tableInfo.vcColumnInfo[i], vcRule[1], vcRule[2]));
        }
    }
    else if (strncmp(vcRule[0].c_str(), "COLUMN", 6) == 0 )
    {
        std::string part = vcRule[0].substr(7);
        int pos = atoi(part.c_str());
        if(pos < 0 )
        {
            pos = tableInfo.vcColumnInfo.size() + pos;
        }
        
        if(pos < 0 )
        {
            return "";
        }   

        vcAtom.push_back(Auto2Code::Convert2Atom(tableInfo.vcColumnInfo[pos], vcRule[1], vcRule[2]));
    }

    for (size_t i = 0; i < vcAtom.size(); i++)
    {
        if (i > 0)
        {
            if (vcRule[4] == "NEWLINE")
            {
                res = fmt::format("{}\n", res);
            }
            else
            {
                res += vcRule[4];
            }
        }
        res += Auto2Code::RuleTransfor(vcAtom[i], vcRule);
        /* code */
    }

    return res;
}

std::string Auto2Code::RuleTransfor(const Atom &atom, const std::vector<std::string>& vcRule)
{
    std::string res("");
    res = fmt::format(vcRule[3], atom.atom0, atom.atom1, atom.atom2, atom.atom3, atom.atom4, atom.atom5, atom.atom6);
    return res;
}

Atom Auto2Code::Convert2Atom(const TableInfo &tableInfo, std::string & paramfmt)
{
    Atom atom;
    atom.atom0 = tableInfo.szTableName;
    atom.atom1 = "";
    // 参数2  0|驼峰命名 1|全大写 2|全小写 3|n+驼峰命名 4|m_n+驼峰命名
    if (paramfmt == "0")
    {
        atom.atom2 = CTools::CamelName(tableInfo.szTableName);
    }
    else if (paramfmt == "1")
    {
        atom.atom2 = CTools::UpperLowerSwitch(tableInfo.szTableName, '1');
    }
    else if (paramfmt == "2")
    {
        atom.atom2 = CTools::UpperLowerSwitch(tableInfo.szTableName, '0');
    }
    else if (paramfmt == "3")
    {
        atom.atom2 = CTools::CamelName(tableInfo.szTableName);
    }
    else if (paramfmt == "4")
    {
        atom.atom2 = CTools::CamelName(tableInfo.szTableName);
    }
    
    atom.atom3 = "";
    atom.atom4 = "";
    atom.atom5 = tableInfo.szTableComment;

    return atom;
}

Atom Auto2Code::Convert2Atom(const ColumnInfo &columnInfo, std::string & paramtype, std::string & paramfmt)
{
    Atom atom;
    atom.atom0 = columnInfo.szColumnName;
    atom.atom1 = "";
    atom.atom2 = "";
    atom.atom3 = "";
    atom.atom4 = "";
    // 1  类型type 0 int/ char/double等等    1 int/ char* /double等等
    std::string perfix = "";
    bool bShowLength = (paramtype == "0");
    switch (columnInfo.odbcDataType)
    {
    case 4:
        perfix = "n";
        atom.atom1 = "int  ";
        break;
    case -9:
        perfix = "sz";
        if (bShowLength)
        {
            atom.atom1 = "char ";
            atom.atom3 = fmt::format("[{} + 1]", columnInfo.nColumnSize);
        }
        else
        {
            atom.atom1 = "char*";
        }

        break;
    case -8:
        if (columnInfo.nColumnSize > 1)
        {
            perfix = "sz";
            if (bShowLength)
            {
                atom.atom1 = "char ";
                atom.atom3 = fmt::format("[{} + 1]", columnInfo.nColumnSize);
            }
            else
            {
                atom.atom1 = "char*";
            }
        }
        else if (columnInfo.nColumnSize == 1)
        {
            perfix = "c";
            atom.atom1 = "char ";
        }   
        break;
    }

        // 参数2  0|驼峰命名 1|全大写 2|全小写 3|n+驼峰命名 4|m_n+驼峰命名
    if (paramfmt == "0")
    {
        atom.atom2 = CTools::CamelName(columnInfo.szColumnName);
    }
    else if (paramfmt == "1")
    {
        atom.atom2 = CTools::UpperLowerSwitch(columnInfo.szColumnName, '1');
    }
    else if (paramfmt == "2")
    {
        atom.atom2 = CTools::UpperLowerSwitch(columnInfo.szColumnName, '0');
    }
    else if (paramfmt == "3")
    {
        atom.atom2 = fmt::format("{}{}", perfix,CTools::CamelName(columnInfo.szColumnName));
    }
    else if (paramfmt == "4")
    {
        atom.atom2 = fmt::format("m_{}{}", perfix,CTools::CamelName(columnInfo.szColumnName));
    }

    atom.atom6 = atom.atom2;

    if (perfix == "sz")
    {
        atom.atom4 = fmt::format(", sizeof({}) - 1", atom.atom2);
        atom.atom6 = fmt::format("P({})", atom.atom2);
    }

    atom.atom5 = columnInfo.szColumnComment;

    return atom;
}