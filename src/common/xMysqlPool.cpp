/*
* ----------------------------------------------------------------------------
* Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
* All rights reserved.
* Distributed under GPL license.
* ----------------------------------------------------------------------------
*/
 
#include "xMysqlPool.h"
#include "xLog.h"
#include "json.h"

MysqlPool::MysqlPool()
{

}
 
MysqlPool::~MysqlPool()
{
    XLOCK(mLock);
    MYSQL_CONNECTIONS_LIST_ITER iter = connection_list->begin();

    for (; iter != connection_list->end(); iter++) {
        mysql_close((*iter));
    }
}

void MysqlPool::Keepalive()
{
    XLOCK(mLock);
    MYSQL_CONNECTIONS_LIST_ITER iter = connection_list->begin();
    for (; iter != connection_list->end(); iter++) {
        mysql_ping((*iter));
    }
}

void MysqlPool::init(const char *host, const char *user, const char *password,
                const char *db, unsigned int port /* =3306 */ )
{
    connection_list = new MYSQL_CONNECTIONS_LIST;
    mHost = host;
    mUser = user;
    mPassword = password;
    mDatabase = db;
    mPort = port;
}

bool MysqlPool::create_pool(int poolsize){
    try {
        if (poolsize > MAX_CONNECTION_NUM) {
            poolsize = MAX_CONNECTION_NUM;
        }

        for (int i = 0; i < poolsize; ++i) {
            MYSQL *pMySql = mysql_init((MYSQL *) NULL);

            if (pMySql != NULL) {
                if (!mysql_real_connect(pMySql, mHost, mUser, mPassword, mDatabase, mPort, NULL, CLIENT_MULTI_RESULTS)) {
                    log_error("MysqlPool::ConnectDB  ERROR %s \n", mysql_error(pMySql));
                    return false;
                } else {
                    set_characterset(pMySql, "UTF8");
                    my_bool my_true= true;
                    mysql_options(pMySql, MYSQL_OPT_RECONNECT, &my_true);
                    
                    log_info("%d connect to %s %s %s %s %u success !\n",
                             i, mHost, mUser, mPassword, mDatabase, mPort);
                    connection_list->push_back(pMySql);
                }
            }
        }
    } catch( ...) {
        log_error("connect error  poolsize=%d \n", poolsize);
        return false;
    }
    return true;
}

MYSQL *MysqlPool::get_connection()
{
    MYSQL *mysql = NULL;
    while (1) {
        {
            XLOCK(mLock);
            if (connection_list->size() > 0) {
                mysql = connection_list->front();
                connection_list->pop_front();
                break;
            } else {
                log_warn("MysqlPool::get_connection()  error \n");
            }
        }
        SLEEP(1);
    }

    return mysql;
}

void MysqlPool::free_connection(MYSQL * mysql)
{
    XLOCK(mLock);
    connection_list->push_back(mysql);
}

bool MysqlPool::execute(const char *sql)
{
    log_debug("MysqlPool::Execute %s\n", sql);
    bool bRet = false;
    MYSQL *mysql = get_connection();

    if (mysql == NULL) {
        return false;
    }
    if (!mysql_real_query(mysql, sql, strlen(sql))) {
        bRet = true;
    } else {
        log_error("MysqlPool::Execute mysql_error%s SQL:%s\n", mysql_error(mysql), sql);
    }
    free_connection(mysql);

    return bRet;
}

MYSQL_RES *MysqlPool::select(const char *sql)
{
    log_debug("MysqlPool::Select %s\n", sql);
    MYSQL *mysql = get_connection();
    MYSQL_RES *res = NULL;
    if (NULL == mysql) {
        return NULL;
    }
    if (0!=mysql_real_query(mysql, sql, strlen(sql))) {
        log_error("MysqlPool::SelectRecord mysql_error:%s SQL:%s\n", mysql_error(mysql), sql);
    } else {
        res = mysql_store_result(mysql);
    }
    
    free_connection(mysql);
    return res;
}

my_ulonglong MysqlPool::get_row_num(MYSQL_RES *res)
{
    return mysql_num_rows(res);
}

MYSQL_ROW MysqlPool::get_row(MYSQL_RES *res)
{
    return mysql_fetch_row(res);
}

unsigned int MysqlPool::get_field_num(MYSQL_RES *res)
{
    return mysql_num_fields(res);
}

void MysqlPool::free_res(MYSQL_RES * res)
{
    mysql_free_result(res);
}

bool MysqlPool::set_characterset(MYSQL *mysql, const char *character)
{
    int nRet = mysql_set_character_set(mysql, character);
    if (nRet != 0) {
        log_error("MysqlPool::SetCharacterset mysql_error:%s\n", mysql_error(mysql));
        return false;
    }

    log_info("MysqlPool::SetCharacterset %s\n", character);
    return true;
}

void MysqlPool::escape_string(const string &strin, string &out)
{
    int length = strin.length();
    char *szbuf = (char*)malloc(length*2);
    MYSQL *mysql = get_connection();
    mysql_real_escape_string(mysql, szbuf, strin.c_str(), strin.length());
    free_connection(mysql);
    out = szbuf;
    free(szbuf);
}

bool MysqlPool::select_json(const std::string &strSQL, string & result)
{
    bool bRet = false;
    Json::Value root;

    MYSQL *mysql = get_connection();
    if (mysql == NULL) {
        return false;
    }
    
    log_debug("MysqlPool::select_json SQL:%s \r\n", strSQL.c_str());
    if (mysql_real_query(mysql, strSQL.c_str(), strSQL.length())) {
        log_error("MysqlPool::select_json mysql_error:%s\n", mysql_error(mysql));
        MYSQL_RES *pRes = mysql_store_result(mysql);
        if (pRes) {
            mysql_free_result(pRes);
        }
        goto end;
    } else {
        MYSQL_RES *pRes = mysql_store_result(mysql);
        if (pRes) {
            MYSQL_FIELD *field;
            Json::Value fieldlist;
            //my_ulonglong n = mysql_num_rows(pRes);
            int num_fields = mysql_num_fields(pRes);
            map < int,string > fieldsMap;

            for (int i = 0; i < num_fields; i++) {
                field = mysql_fetch_field_direct(pRes, i);
                fieldsMap[i] = field->name;
                fieldlist.append(field->name);
            }

            MYSQL_ROW row = mysql_fetch_row(pRes);
            while (row) {
                Json::Value Item;
                for (int i = 0; i < num_fields; i++) {
                    if (row[i]) {
                        Item[fieldsMap[i].c_str()] = row[i];
                    } else {
                        Item[fieldsMap[i].c_str()] = "";
                    }
                }
                root.append(Item);
                row = mysql_fetch_row(pRes);
            }

            mysql_free_result(pRes);
            bRet = true;
        } else {
            log_error("MysqlPool::select_json MYSQL_RES %s \r\n", mysql_error(mysql));
        }
    }

  end:

    Json::FastWriter writer;
    result += writer.write(root);
    free_connection(mysql);

    return bRet;
}
