/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _xBlog_H_
#define _xBlog_H_

#include "xBlogBase.h"
#include "xUntil.h"
#include "xBlogPage.h"
#include "xLog.h"
#include "xBlogData.h"

class xBlog
{
public:
    xBlog();
    ~xBlog();

public:
    bool Init();
    void StartMysqlPool();

    static void DebugHttpHeader(struct evhttp_request *req);
    static const char *DebugHttpGetCommand(struct evhttp_request *req);
    static void HttpParseURL(struct evhttp_request *req, struct evkeyvalq *evMyheader);
    static void GetHttpPostData(struct evhttp_request *req, struct evkeyvalq *evdata);
    static void SendHttpResphone(struct evhttp_request *req, string & strData);

    bool StartHttpd();
    void SetRouteTable(evhttp * http);
    int BindSocket(const char *ip, uint16 port);
    bool Run(const char *ip, uint16 port, uint32 timeout_secs, uint32 nthreads);
    static void *Dispatch(void *arg);
    static void OnTimer();
    event_base *GetEventBase()
    {
        return base;
    };

public:
    uint64 ssid_token;
    char   szDir[SIZE_1024];
    uint32 StartTime;
    char   uri_root[SIZE_1024];

private:
    struct event_base *base;
    struct evhttp *http;
};

#endif
