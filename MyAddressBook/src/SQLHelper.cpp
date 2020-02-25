#include "SQLHelper.h"
#include<iostream>
#include<windows.h>
#include<mysql.h>
#include<string>

using namespace std;
SQLHelper::SQLHelper()
{
conn=mysql_init(0);
conn=mysql_real_connect(conn,"192.168.0.109","admin","admin","MyAddressBook",0,NULL,0);

if(!conn)
    cout<<"Cannot Connect to Database!"<<endl;
}

SQLHelper::~SQLHelper()
{
    mysql_close(conn);
}

void SQLHelper::ExecuteQuery(string query)
 {
     if(conn){
        int qstate=0;
        qstate=mysql_query(conn,query.c_str());
        if(qstate!=0){
            cout<<"Database Update has Failed! Please check!"<<endl;
        }
    }
 }

 MYSQL_RES* SQLHelper::ReadResultFromQuery(string query)
 {
      MYSQL_RES* res=NULL;
     if(conn)
     {
        int qstate=0;
        qstate=mysql_query(conn,query.c_str());
        if(qstate!=0){
            cout<<"Database Update has Failed! Please check! Query "<<endl;
        }
        else{
            res = mysql_store_result(conn);
        }
     }
    return res;
 }
