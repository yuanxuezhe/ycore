#include <iostream>
#include "odbc.h"

int demo() {
    OdbcStmt odbc;

    odbc.Open("SELECT * FROM litemall_ad where LOCATE(id, {@1}) > 0");
    odbc.SetParam<std::string>(1, "12345");
    odbc.Exec();

    char col1[255], col2[255], col3[255], col4[255];
    while (odbc.fetch()) {
        odbc.GetValue("id", col1);
        odbc.GetValue("name", col2);
        odbc.GetValue("link", col3);
        odbc.GetValue("url", col4);
        std::cout << col1 <<", " << col2 <<  "," << col3  <<  "," << col4 << std::endl;
    }

    odbc.close();
 
    return 0;
}
int main(){
    OdbcStmt::InitConnection("smj1");
    demo();
    OdbcStmt::CloseConnection();

    return 0;
}