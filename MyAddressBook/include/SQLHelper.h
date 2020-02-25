#ifndef SQLHELPER_H
#define SQLHELPER_H
#include "SQLHelper.h"
#include<string>
#include<windows.h>
#include<mysql.h>
#include<iostream>
using namespace std;
class SQLHelper
{
    public:
        SQLHelper();
        virtual ~SQLHelper();

        void ExecuteQuery(string query);
        MYSQL_RES* ReadResultFromQuery(string query);


    private:
        MYSQL *conn;
};

#endif // SQLHELPER_H
