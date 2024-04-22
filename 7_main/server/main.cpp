#include <iostream>
#include "odbc.h"

struct LiteMallAd
{
    int  id;
    char name[64 + 1];
    char link[256 + 1];
    char url[256 + 1];
};


int demo() {
    OdbcStmt odbc;
odbc.getLastError();
    odbc.Open("SELECT * FROM litemall_ad where LOCATE(id, {@1}) > 0");
    odbc.SetParam<std::string>(1, "12345");
    odbc.Exec();

    char col1[255], col2[255], col3[255], col4[255];
    while (odbc.fetch()) {
        LiteMallAd ad;
        odbc.GetValue("id", col1);
        odbc.GetValue("name", col2);
        odbc.GetValue("link", col3);
        odbc.GetValue("url", col4);
        std::cout << col1 <<", " << col2 <<  "," << col3  <<  "," << col4 << std::endl;
    }

    odbc.close();
 
    return 0;
}

int demo1() {
    OdbcStmt odbc;
    odbc.getLastError();
    odbc.Open("SELECT * FROM litemall_ad");
    odbc.Exec();
    char col1[255], col2[255], col3[255], col4[255];
    while (odbc.fetch()) {
        odbc.GetValue("id", col1);
        odbc.GetValue("name", col2);
        odbc.GetValue("url", col3);
        odbc.GetValue("add_time", col4);
        std::cout << col1 <<", " << col2 <<  "," << col3  <<  "," << col4 << std::endl;
    }

    odbc.close();
 
    return 0;
}

int demo2() {
    OdbcStmt odbc;
    odbc.getLastError();
    odbc.Open("SELECT top {@1} * FROM COS_ORDER where CUACCT_CODE = {@2}");
    odbc.SetParam<int>(1, 5);
    odbc.SetParam<std::string>(2, "110002398187");
    odbc.Exec();
    char col1[255], col2[255], col3[255], col4[255];
    while (odbc.fetch()) {
        odbc.GetValue("ORDER_NO", col1);
        odbc.GetValue("CUACCT_CODE", col2);
        odbc.GetValue("TRD_CODE", col3);
        odbc.GetValue("ATTR_CODE", col4);
        std::cout << col1 <<", " << col2 <<  "," << col3  <<  "," << col4 << std::endl;
    }

    odbc.close();
 
    return 0;
}

// sqlserver  "DRIVER={SQL Server};SERVER=10.118.9.223,1433;DATABASE=kbss_cos;UID=kbssuser;PWD=ts5585354!;"
int main(){
    OdbcStmt::InitConnection("DRIVER={MySQL ODBC 8.3 ANSI Driver};SERVER=123.207.218.225:53306;DATABASE=smj;USER=root;PASSWORD=123456;");
    demo1();
    OdbcStmt::CloseConnection();
    return 0;
}