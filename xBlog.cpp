/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include <event.h>
#include "xBlog.h"
#include "xMysqlPool.h"
#include "xConfig.h"
#include "xBlogCache.h"

xBlog::xBlog()
{
    memset(szDir, 0, sizeof(szDir));
#ifdef WIN32
    WSADATA WSAData;
    WSAStartup(0x101, &WSAData);
#else
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        log_error("void xBlog::xBlog() SIGPIPE error\n");;
    }

#endif
}

xBlog::~xBlog()
{
}

bool xBlog::Init()
{
    StartTime = time(NULL);
    xBlogPageTemplate::GetInstance()->Init();

    strncpy(szDir, Config::GetInstance()->strRootFullPath.c_str(), 
                Config::GetInstance()->strRootFullPath.length());
    return true;
}

void xBlog::OnTimer()
{
    //log_debug("void xBlog::OnTimer()\n");
    xBlogCache::GetInstance()->OnTimer();
}

void xBlog::DebugHttpHeader(struct evhttp_request *req)
{
    struct evkeyval *header;
    struct evkeyvalq *headers;

    headers = evhttp_request_get_input_headers(req);
    for (header = headers->tqh_first; header; header = header->next.tqe_next)
    {
        log_debug("%s: %s\n", header->key, header->value);
    }
}

const char *xBlog::DebugHttpGetCommand(struct evhttp_request *req)
{
    const char *cmdtype;

    switch (evhttp_request_get_command(req))
    {
    case EVHTTP_REQ_GET:
        cmdtype = "GET";
        break;
    case EVHTTP_REQ_POST:
        cmdtype = "POST";
        break;
    case EVHTTP_REQ_HEAD:
        cmdtype = "HEAD";
        break;
    case EVHTTP_REQ_PUT:
        cmdtype = "PUT";
        break;
    case EVHTTP_REQ_DELETE:
        cmdtype = "DELETE";
        break;
    case EVHTTP_REQ_OPTIONS:
        cmdtype = "OPTIONS";
        break;
    case EVHTTP_REQ_TRACE:
        cmdtype = "TRACE";
        break;
    case EVHTTP_REQ_CONNECT:
        cmdtype = "CONNECT";
        break;
    case EVHTTP_REQ_PATCH:
        cmdtype = "PATCH";
        break;
    default:
        cmdtype = "unknown";
        break;
    }

    log_debug("Received a %s request for %s \n", cmdtype, evhttp_request_get_uri(req));
    return cmdtype;
}

void xBlog::HttpParseURL(struct evhttp_request *req, struct evkeyvalq *evMyheader)
{
    const char *xBlog_query;

    xBlog_query = evhttp_uri_get_query(evhttp_request_get_evhttp_uri(req));
    evhttp_parse_query_str(xBlog_query, evMyheader);
}

void xBlog::GetHttpPostData(struct evhttp_request *req, struct evkeyvalq *evdata)
{
    evbuffer *pevBuf = evhttp_request_get_input_buffer(req);
    int data_len = evbuffer_get_length(pevBuf);

    char *pbuffer = (char *) malloc(data_len + 1);
    memset(pbuffer, 0, data_len + 1);

    evbuffer_copyout(pevBuf, pbuffer, data_len);
    log_debug("%s\r\n", pbuffer);
    evhttp_parse_query_str(pbuffer, evdata);
    free(pbuffer);
}

void xBlog::SendHttpResphone(struct evhttp_request *req, const string & strData)
{
    struct evbuffer *buf = evbuffer_new();

    if (NULL==buf)
    {
        log_error("failed to create response buffer\r\n");
        return;
    }

    evbuffer_add_printf(buf, "%s", strData.c_str());

    evhttp_add_header(evhttp_request_get_output_headers(req),
                      "Content-Type", "text/html; charset=utf-8");
    evhttp_send_reply(req, 200, "OK", buf);
    evbuffer_free(buf);
}

int xBlog::BindSocket(const char *ip, uint16 port)
{
    int skt;
    skt = socket(AF_INET, SOCK_STREAM, 0);
    if (skt < 0)
    {
        log_error("socket error, nfd=%d \r\n", skt);
        return -1;
    }

#ifdef _WIN32
    int flag = 1;
    ioctlsocket(skt, FIONBIO, (unsigned long *) &flag);
#else

    /* set socket nonblocking */
    int flags;
    if ((flags = fcntl(skt, F_GETFL, NULL)) < 0 || fcntl(skt, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        log_error("O_NONBLOCK  error, skt=%d \r\n", skt);
        return -1;
    }

    /* set socket closeonexec */
    if ((flags = fcntl(skt, F_GETFD, NULL)) < 0 || fcntl(skt, F_SETFD, flags | FD_CLOEXEC) == -1)
    {
        log_error("FD_CLOEXEC  error, skt=%d \r\n", skt);
        return -1;
    }
#endif

    int on = 1;
    setsockopt(skt, SOL_SOCKET, SO_KEEPALIVE, (const char *) &on, sizeof(on));
    setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port        = htons(port);

    if (bind(skt, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        log_error("bind %s %d error\r\n", ip, port);
        return -1;
    }

    if (listen(skt, 128) < 0)
    {
        log_error("listen %s %d error\r\n", ip, port);
        return -1;
    }

    return skt;
}

bool xBlog::Run(const char *ip, uint16 port, uint32 timeout_secs, uint32 nthreads)
{
    int ret = -1;
    int skt = BindSocket(ip, port);

    if (skt < 0)
    {
        log_error("BindSocket %s %d error\r\n", ip, port);
        return false;
    }

#ifdef _WIN32

    HANDLE ths[128];

    for (uint32 i = 0; i < nthreads; i++)
    {
        struct event_base *base = event_base_new();
        if (base == NULL)
        {
            return false;
        }

        struct evhttp *httpd = evhttp_new(base);
        if (httpd == NULL)
        {
            return false;
        }

        ret = evhttp_accept_socket(httpd, skt);
        if (r != 0)
        {
            return false;
        }

        SetRouteTable(httpd);

        ths[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) xBlog::Dispatch, base, NULL, NULL);
        log_info("%d %d \r\n", i, ths[i]);
        if (ret != 0)
        {
            return false;
        }
    }

#else

    pthread_t ths[128];
    for (uint32 i = 0; i < nthreads; i++)
    {
        struct event_base *base = event_base_new();
        if (base == NULL)
        {
            return false;
        }

        struct evhttp *httpd = evhttp_new(base);
        if (httpd == NULL)
        {
            return false;
        }

        ret = evhttp_accept_socket(httpd, skt);
        if (ret != 0)
        {
            return false;
        }

        SetRouteTable(httpd);

        ret = pthread_create(&ths[i], NULL, xBlog::Dispatch, base);
        log_info("%d %d \r\n", i, ths[i]);
        if (ret != 0)
        {
            return false;
        }
            
    }

    /*
       for (int i = 0; i < nthreads; i++)
       { 
       pthread_join(ths[i], NULL); 
       }
     */

#endif

    return true;
}

void *xBlog::Dispatch(void *arg)
{
    if (NULL==arg)
    {
        log_error("error arg is null\n");
        return NULL;
    }
    event_base_dispatch((struct event_base *) arg);
    return NULL;
}

void xBlog::SetRouteTable(evhttp * http)
{
    evhttp_set_cb(http, "/download",     xBlogPage::DownloadCallback, this);        // 测试
    evhttp_set_cb(http, "/post",         xBlogPage::PostRequestCallback, this);     // 请求文章
    evhttp_set_cb(http, "/guestbook",    xBlogPage::GuestbookCallback, this);       // 查看-留言
    evhttp_set_cb(http, "/postmessage",  xBlogPage::GuestPostCallback, this);       // 添加-评论留言
    evhttp_set_cb(http, "/catalog",      xBlogPage::CatalogRequestCallback, this);  // 分类
    evhttp_set_cb(http, "/",             xBlogPage::IndexRequestCallback, this);    // 首页
    evhttp_set_cb(http, "/page",         xBlogPage::PageRequestCallback, this);     // 页面

    // 管理后台功能
    evhttp_set_cb(http, "/admin",        xBlogPage::AdminCallback, this);            // 管理后台
    evhttp_set_cb(http, "/checklogin",   xBlogPage::AdminCheckLoginCallback, this);  // 权限验证
    evhttp_set_cb(http, "/shell",        xBlogPage::AdminShellCallback, this);       // 测试
    evhttp_set_cb(http, "/status",       xBlogPage::AdminStatusCallback, this);      // 测试
    evhttp_set_cb(http, "/postmanager",  xBlogPage::AdminPostManagerCallback, this); // 文章管理
    evhttp_set_cb(http, "/siteconfig",   xBlogPage::AdminSiteConfigCallback, this);  // 网站配置参数管理
    evhttp_set_cb(http, "/links",        xBlogPage::AdminLinksCallback, this);       // 链接管理
    evhttp_set_cb(http, "/catalogset",   xBlogPage::AdminCatalogCallback, this);     // 链接管理
    evhttp_set_cb(http, "/comments",     xBlogPage::AdminCommentsCallback, this);    // 留言评论管理
    evhttp_set_cb(http, "/system",       xBlogPage::AdminSystemCallback, this);      // 系统配置
    evhttp_set_cb(http, "/user",         xBlogPage::AdminUserCallback, this);        // 系统配置

    evhttp_set_timeout(http, Config::GetInstance()->xBlogAppConfig.HttpdTimeOut);

    evhttp_set_gencb(http, xBlogPage::SendDocumentCallback, this);
}

void xBlog::StartMysqlPool()
{
    xBlogData::GetInstance()->pMysqlPool = MysqlPool::GetInstance();
    xBlogData::GetInstance()->pMysqlPool->Init(Config::GetInstance()->xBlogMysqlcfg.ipaddr.c_str(),
                                               Config::GetInstance()->xBlogMysqlcfg.username.c_str(),
                                               Config::GetInstance()->xBlogMysqlcfg.passwd.c_str(),
                                               Config::GetInstance()->xBlogMysqlcfg.dbname.c_str());
    xBlogData::GetInstance()->pMysqlPool->ConnectDB(Config::GetInstance()->xBlogMysqlcfg.poolsize);
    
    bool bRet = Config::GetInstance()->LoadxBlogSiteConfig();
    if (!bRet)
    {
        log_error("xBlog::StartMysqlPool() site config error, exit\r\n");
        exit(0);
    }
}


