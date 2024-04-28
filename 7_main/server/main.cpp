#include <iostream>
#include "odbc.h"

#include <sql.h>
#include <sqlext.h>

int main() {
    OdbcStmt::InitConnection("DRIVER={MySQL ODBC 8.3 ANSI Driver};SERVER=123.207.218.225:53306;DATABASE=smj;USER=root;PASSWORD=123456;");
    
    OdbcStmt odbc;
    odbc.Open("SELECT * FROM litemall_ad where ID = {@1}");
    odbc.SetParam<int>(1, 1);
    odbc.Exec();

    int nId;
    char szName[256 + 1] = {0};
    char szUrl[8 + 1] = {0};
    while (odbc.Fetch()) {
        odbc.GetValue("id", nId);
        odbc.GetValue("name", szName, sizeof(szName));
        odbc.GetValue("123", szUrl, sizeof(szUrl));
        std::cout << "id: " << nId << ", name: " << szName << ", url: " << szUrl << std::endl;
    }
    odbc.Close();

    odbc.Open("SELECT * FROM litemall_ad where ID = {@1}");
    odbc.SetParam<int>(1, 3);
    odbc.Exec();

    while (odbc.Fetch()) {
        odbc.GetValue("id", nId);
        odbc.GetValue("name", szName, sizeof(szName));
        odbc.GetValue("123", szUrl, sizeof(szUrl));
        std::cout << "id: " << nId << ", name: " << szName << ", url: " << szUrl << std::endl;
    }
    odbc.Close();
    OdbcStmt::CloseConnection();
    return 0;
}
