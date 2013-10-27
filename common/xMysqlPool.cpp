/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include "xMysqlPool.h"
#include "xLog.h"


MysqlPool::MysqlPool()
{

}
 
MysqlPool::~MysqlPool()
{
    XLOCK(mLock);
    ITER_CONNECTION_HANDLE_LIST iter = mMysqConnlList->begin();

    for (; iter != mMysqConnlList->end(); iter++)
    {
        mysql_close((*iter));
    }
}

MysqlPool *MysqlPool::GetInstance()
{
    static MysqlPool gMySQLPool;
    return &gMySQLPool;
}

void MysqlPool::Keepalive()
{
    XLOCK(mLock);
    ITER_CONNECTION_HANDLE_LIST iter = mMysqConnlList->begin();

    for (; iter != mMysqConnlList->end(); iter++)
    {
        mysql_ping((*iter));
    }
}

void MysqlPool::Init(const char *host, const char *user, const char *password,
                const char *db, unsigned int port /* =3306 */ )
{
    mMysqConnlList = new CONNECTION_HANDLE_LIST;
    mHost = host;
    mUser = user;
    mPassword = password;
    mDatabase = db;
    mPort = port;
}

bool MysqlPool::ConnectDB(uint16 poolsize)
{
    try
    {
        if (poolsize > MAX_CONNECTION_NUM)
        {
            poolsize = MAX_CONNECTION_NUM;
        }

        for (int i = 0; i < poolsize; ++i)
        {
            MYSQL *pMySql = mysql_init((MYSQL *) NULL);

            if (pMySql != NULL)
            {
                if (!mysql_real_connect(pMySql, mHost, mUser, mPassword, mDatabase, mPort, NULL, CLIENT_MULTI_RESULTS))
                {
                    log_error("MysqlPool::ConnectDB  ERROR %s \n", mysql_error(pMySql));
                    return false;
                }
                else
                {
                    SetCharacterset(pMySql, "UTF8");
                    my_bool my_true= true;
                    mysql_options(pMySql, MYSQL_OPT_RECONNECT, &my_true);
                    
                    log_info("%d connect to %s %s %s %s %u success !\n",
                             i, mHost, mUser, mPassword, mDatabase, mPort);
                    mMysqConnlList->push_back(pMySql);
                }
            }
        }
    }
    catch( ...)
    {
        log_error("connect error  poolsize=%d \n", poolsize);
        return false;
    }
    return true;
}

MYSQL *MysqlPool::GetConnection()
{
    MYSQL *pMySql = NULL;

    while (1)
    {
        {
            XLOCK(mLock);
            if (mMysqConnlList->size() > 0)
            {
                pMySql = mMysqConnlList->front();
                mMysqConnlList->pop_front();
                break;
            }
            else
            {
                //log_info("MysqlPool::GetIdleMySql()  error id=%d \n", pthread_self());
            }
        }

#ifdef _WIN32
        Sleep(1);
#else
        usleep(10 * 1000);
#endif
    }

    return pMySql;
}

void MysqlPool::FreeConnection(MYSQL * pMySql)
{
    XLOCK(mLock);
    mMysqConnlList->push_back(pMySql);
}

bool MysqlPool::Execute(const char *sqlstring)
{
    //log_debug("MysqlPool::Execute %s\n", szSql);
    bool bRet = false;
    MYSQL *pMySql = GetConnection();

    if (pMySql == NULL)
    {
        return false;
    }
    if (!mysql_real_query(pMySql, sqlstring, strlen(sqlstring)))
    {
        log_debug("MysqlPool::Execute %s\n", sqlstring);
        bRet = true;
    }
    else
    {
        log_debug("MysqlPool::Execute ERROR %s\n", sqlstring);
        log_error("MysqlPool::Execute %s\n", mysql_error(pMySql));
    }
    FreeConnection(pMySql);

    return bRet;
}

MYSQL_RES *MysqlPool::Select(const char *sqlstring)
{
    log_debug("MysqlPool::Select %s\n", sqlstring);
    MYSQL *pMySql = GetConnection();

    if (NULL == pMySql)
    {
        return NULL;
    }
    if (0!=mysql_real_query(pMySql, sqlstring, strlen(sqlstring)))
    {
        log_error("MysqlPool::SelectRecord %s\n", mysql_error(pMySql));
        FreeConnection(pMySql);
        return NULL;
    }

    MYSQL_RES *mysqlRes = NULL;
    mysqlRes = mysql_store_result(pMySql);
    FreeConnection(pMySql);

    return mysqlRes;
}

my_ulonglong MysqlPool::GetRowNum(MYSQL_RES *mysqlRes)
{
    return mysql_num_rows(mysqlRes);
}

MYSQL_ROW MysqlPool::GetRecord(MYSQL_RES *mysqlRes)
{
    return mysql_fetch_row(mysqlRes);
}

unsigned int MysqlPool::GetFieldNum(MYSQL_RES *mysqlRes)
{
    return mysql_num_fields(mysqlRes);
}

void MysqlPool::FreeRecord(MYSQL_RES * mysqlRes)
{
    mysql_free_result(mysqlRes);
}

bool MysqlPool::SetCharacterset(MYSQL *pMySql, const char *character)
{
    int nRet = mysql_set_character_set(pMySql, character);
    if (nRet != 0)
    {
        log_error("%s\n", mysql_error(pMySql));
        return false;
    }

    log_debug("SetCharacterset %s\n", character);
    return true;
}

bool MysqlPool::SelectJson(const char *sqlstring, string & result)
{
    result = "jsonpcallback(";
    bool bRet = false;
    Json::Value root;

    MYSQL *pMySql = GetConnection();
    if (pMySql == NULL)
    {
        root["errno"] = "system error";
        root["error"] = mysql_error(pMySql);
        root["affected"] = mysql_affected_rows(pMySql);
        return false;
    }
    
    log_debug("%s \r\n", sqlstring);
    if (mysql_real_query(pMySql, sqlstring, strlen(sqlstring)))
    {
        log_error("CMySQLPool::Execute %s\n", mysql_error(pMySql));
        goto end;
    }
    else
    {
        MYSQL_RES *pRes = mysql_store_result(pMySql);
        if (pRes)
        {
            MYSQL_FIELD *field;
            Json::Value fieldlist;
            int n = mysql_num_rows(pRes);
            int num_fields = mysql_num_fields(pRes);
            map < int,string > fieldsMap;

            log_debug("fields=%d result: %d \r\n", num_fields, n);
            root["num"] = n;
            for (int i = 0; i < num_fields; i++)
            {
                field = mysql_fetch_field_direct(pRes, i);
                fieldsMap[i] = field->name;
                log_debug("%s\t", field->name);
                fieldlist.append(field->name);
            }
            log_debug("\r\n");

            MYSQL_ROW row = mysql_fetch_row(pRes);

            Json::Value Result;
            Json::Value DataList;

            Result["field"] = fieldlist;

            while (row)
            {
                Json::Value Item;
                for (int i = 0; i < num_fields; i++)
                {
                    //log_debug("%s \t", row[i]);
                    if (row[i])
                    {
                        Item.append(row[i]);
                    }
                    else
                    {
                        Item.append("null");
                    }
                }
                DataList.append(Item);
                row = mysql_fetch_row(pRes);
            }
            Result["xdata"] = DataList;

            mysql_free_result(pRes);
            root["result"] = Result;
            bRet = true;
        }
        else
        {
            log_debug("MYSQL_RES %s \r\n", mysql_error(pMySql));
        }
    }

  end:
    root["errno"] = mysql_errno(pMySql);
    root["error"] = mysql_error(pMySql);
    root["affected"] = mysql_affected_rows(pMySql);
    log_debug("affected=%lld \r\n", mysql_affected_rows(pMySql));
    Json::FastWriter writer;
    result += writer.write(root);
    FreeConnection(pMySql);

    result += ")";
    return bRet;
}

