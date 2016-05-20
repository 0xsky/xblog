/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _xBlog_H_
#define _xBlog_H_

#include "xMysqlPool.h"
#include "xLog.h"
#include <sstream>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

template<class T>
string tostring(const T& t) {
    ostringstream oss;
    oss << t;
    return  oss.str();
}

#define SERVER_SIGNATURE "xBlog server v0.0.1"

static const struct table_entry {
    const char *extension;
    const char *content_type;
} content_type_table[] = {
        { "txt", "text/plain" },
        { "c", "text/plain" },
        { "h", "text/plain" },
        { "js", "application/x-javascript" },
        { "html", "text/html" },
        { "htm", "text/html" },
        { "css", "text/css" },
        { "gif", "image/gif" },
        { "jpg", "image/jpeg" },
        { "ico", "image/gif" },
        { "jpeg", "image/jpeg" },
        { "png", "image/png" },
        { "pdf", "application/pdf" },
        { "ps", "application/postsript" },
        { "swf",  "application/x-shockwave-flash" },
        { NULL, NULL },
};

const char *GuessContentType(const char *path);

typedef struct _XBLOGCONFIG_
{
    string theme;
    string user;
    string pass;
} BLOGCONFIG, *PBLOGCONFIG;

class xBlog{
public:
    xBlog();
    ~xBlog();

public:
    bool Init();
    void StartMysqlPool();

    bool checksession(struct evhttp_request *req);
    bool checkauth(const char *auth);
    static int base64_encode(const unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen);
    static void HttpDebug(struct evhttp_request *req);
    static void DebugHttpHeader(struct evhttp_request *req);
    static const char *DebugHttpGetCommand(struct evhttp_request *req);
    static void HttpParseURL(struct evhttp_request *req, struct evkeyvalq *evMyheader);
    static void GetHttpPostData(struct evhttp_request *req, struct evkeyvalq *evdata);
    static char* GetHttpPostData(struct evhttp_request *req);
    static const char *GetVal(struct evkeyvalq *evdata, const char *key);
    static void SendHttpResphone(struct evhttp_request *req, int code, const string & strData);
    static void SendHttpJsonResphone(struct evhttp_request *req, int code, const string & strData);
    static void SendErrorResphone(struct evhttp_request *req, int errcode, const char*fmt, ...);
    static void SendDocumentCallback(struct evhttp_request *req, void *arg);
    void SendDocument(struct evhttp_request *req, const char* file);
    

    bool StartHttpd();
    void SetRouteTable(evhttp * http);
    int BindSocket(const char *ip, int port);
    bool Run(const char *ip, int port, int timeout_secs, int nthreads);
    static void *Dispatch(void *arg);
    static void OnTimer();
    event_base *GetEventBase() {
        return base;
    };

private:
    static void IndexRequestCallback(struct evhttp_request *req, void *arg);
    static void PostlistRequestCallback(struct evhttp_request *req, void *arg);
    static void CatalogRequestCallback(struct evhttp_request *req, void *arg);
    static void PostRequestCallback(struct evhttp_request *req, void *arg);
    static void ClassifyRequestCallback(struct evhttp_request *req, void *arg);
    static void LinksRequestCallback(struct evhttp_request *req, void *arg);
    static void ConfigCallback(struct evhttp_request *req, void *arg);
    static void PostviewCallback(struct evhttp_request *req, void *arg);
    static void PagingCallback(struct evhttp_request *req, void *arg);
    static void UEControllerCallback(struct evhttp_request *req, void *arg);
    static void InfopageRequestCallback(struct evhttp_request *req, void *arg);

private:
    static void AdminCallback(struct evhttp_request *req, void *arg);
    static void AdminCheckLoginCallback(struct evhttp_request *req, void *arg);
    static void AdminPostManager_Callback(struct evhttp_request *req, void *arg);
    static void AdminLinks_Callback(struct evhttp_request *req, void *arg);
    static void AdminSiteConfig_Callback(struct evhttp_request *req, void *arg);
    static void AdminCatalog_Callback(struct evhttp_request *req, void *arg);
private:
    bool LoadConfig(BLOGCONFIG &cfg);
   // bool checksession(struct evhttp_request * req);
    static bool parase_form_data(const char* data, std::string &name, std::string &val)
    {
        int name_len = strlen("name=\"");
        const char* pstart = strstr(data, "name=\"") + name_len;
        const char* pend = strstr(pstart, "\"\r\n");
        name.assign(pstart, pend - pstart);
        pstart = strstr(pstart, "\r\n\r\n");
        val = pstart + 4;
        return true;
    }
   


public:
    uint64_t LoginToken;
    char     szDir[1024];
    uint32_t StartTime;
    char     uri_root[1024];

private:
    struct event_base *base;
    struct evhttp *http;
    MysqlPool *mysqlclient;
    BLOGCONFIG blogconfig;
};

#endif
