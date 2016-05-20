/*
* ----------------------------------------------------------------------------
* Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
* All rights reserved.
* Distributed under GPL license.
* ----------------------------------------------------------------------------
*/
 
#ifndef _MYSQL_POOL_H_
#define _MYSQL_POOL_H_

const int MAX_CONNECTION_NUM = 128;

#ifdef	WIN32
typedef	unsigned int SOCKET;
typedef	unsigned int ulong;
#define	my_socket SOCKET
#endif

#include <mysql.h>
#include <string>
#include <list>
#include "xLock.h"
#include "xUntil.h"

using namespace std;

typedef std::list <MYSQL *> MYSQL_CONNECTIONS_LIST;
typedef std::list <MYSQL *>::iterator MYSQL_CONNECTIONS_LIST_ITER;

class MysqlPool
{
public:
    MysqlPool();
    ~MysqlPool();

    void init(const char *host, const char *user, const char *password,
                      const char *db, unsigned int port = 3306);
    bool create_pool(int poolsize);

public:    
    MYSQL_RES *select(const char *sql);
    bool execute(const char *sql);
    bool select_json(const char *sql, string &result);

    void free_res(MYSQL_RES *res);
    unsigned int get_field_num(MYSQL_RES *res);
    MYSQL_ROW get_row(MYSQL_RES *res);
    my_ulonglong get_row_num(MYSQL_RES *res);
    
    char *error_string(MYSQL *mysql);
    bool set_characterset(MYSQL *mysql, const char *character);
    void Keepalive();
    void escape_string(const string &strin, string &out);


private:
    MYSQL *get_connection();
    void free_connection(MYSQL *mysql);
    
private:
    MYSQL_CONNECTIONS_LIST *connection_list;
    xLock mLock;

private:
    const char *mHost;
    const char *mUser;
    const char *mPassword;
    const char *mDatabase;
    int         mPort;
}; 


#endif
