
/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _MYSQL_POOL_H_
#define _MYSQL_POOL_H_

const int MAX_CONNECTION_NUM = 128;

//////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib,"libmySQL.lib")
#else
typedef int BOOL;
#endif

#include <string>
#include <mysql.h>
#include <list>
#include "xLock.h"
#include "json/json.h"
#include "xUntil.h"
using namespace std;

typedef std::list <MYSQL *> CONNECTION_HANDLE_LIST;
typedef std::list <MYSQL *>::iterator ITER_CONNECTION_HANDLE_LIST;

#ifdef MYDLL_EXPORTS
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

#ifdef WIN32
class
__declspec(dllexport)
    MysqlPool
#else
class MysqlPool
#endif
{
public:
    MysqlPool();
    ~MysqlPool();

    static MysqlPool *GetInstance();

    void Init(const char *host, const char *user, const char *password,
                      const char *db, unsigned int port = 3306);
public:
    bool ConnectDB(uint16 poolsize);
    MYSQL_RES *Select(const char *sqlstring);
    bool Execute(const char *sqlstring);
    bool SelectJson(const char *sqlstring, string & result);

    void FreeRecord(MYSQL_RES * mysqlRes);
    unsigned int GetFieldNum(MYSQL_RES * mysqlRes);
    MYSQL_ROW GetRecord(MYSQL_RES * mysqlRes);
    my_ulonglong GetRowNum(MYSQL_RES * mysqlRes);
    
    char *GetErrorString(MYSQL * pMySql);
    bool SetCharacterset(MYSQL * pMySql, const char *character);
    void Keepalive();

    MYSQL *GetConnection();
    void FreeConnection(MYSQL * pMySql);

private:

    CONNECTION_HANDLE_LIST * mMysqConnlList;
    xLock mLock;

private:
    const char *mHost;
    const char *mUser;
    const char *mPassword;
    const char *mDatabase;
    uint16      mPort;
}; 


#endif
