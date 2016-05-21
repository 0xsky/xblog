/*
* ----------------------------------------------------------------------------
* Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
* All rights reserved.
* Distributed under GPL license.
* ----------------------------------------------------------------------------
*/


#include "xBlog.h"
#include "xConfig.h"
#include <assert.h>

#include <time.h>

#ifdef WIN32

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <dirent.h>
#endif


xBlog::xBlog()
{
    memset(szDir, 0, sizeof(szDir));
#ifdef WIN32
    WSADATA WSAData;
    WSAStartup(0x101, &WSAData);
#else
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        log_error("void xBlog::xBlog() SIGPIPE error\n");;
    }

#endif
}

xBlog::~xBlog()
{
}

bool xBlog::Init()
{
    StartTime = (uint32_t)time(NULL);

    strncpy(szDir, Config::GetInstance()->strRootFullPath.c_str(),
            Config::GetInstance()->strRootFullPath.length());

    StartMysqlPool();

    return LoadConfig(blogconfig);
}

void xBlog::OnTimer()
{
    //log_debug("void xBlog::OnTimer()\n");
}

int xBlog::base64_encode(const unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen)
{
    static const char *codes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    unsigned long i, len2, leven;
    unsigned char *p;

    assert(in != NULL);
    assert(out != NULL);
    assert(outlen != NULL);

    len2 = 4 * ((inlen + 2) / 3);
    if (*outlen < len2 + 1) {
        *outlen = len2 + 1;
        return -1;
    }
    p = out;
    leven = 3 * (inlen / 3);
    for (i = 0; i < leven; i += 3) {
        *p++ = codes[(in[0] >> 2) & 0x3F];
        *p++ = codes[(((in[0] & 3) << 4) + (in[1] >> 4)) & 0x3F];
        *p++ = codes[(((in[1] & 0xf) << 2) + (in[2] >> 6)) & 0x3F];
        *p++ = codes[in[2] & 0x3F];
        in += 3;
    }

    if (i < inlen) {
        unsigned a = in[0];
        unsigned b = (i + 1 < inlen) ? in[1] : 0;

        *p++ = codes[(a >> 2) & 0x3F];
        *p++ = codes[(((a & 3) << 4) + (b >> 4)) & 0x3F];
        *p++ = (i + 1 < inlen) ? codes[(((b & 0xf) << 2)) & 0x3F] : '=';
        *p++ = '=';
    }

    *p = '\0';

    *outlen = p - out;
    return 0;
}

const char *xBlog::GuessContentType(const char *path)
{
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
        { "swf", "application/x-shockwave-flash" },
        { NULL, NULL },
    };
    const char *last_period, *extension;
    const struct table_entry *ent;
    const char *type = "text/json";
    last_period = strrchr(path, '.');
    if (!last_period || strchr(last_period, '/')) {

    } else {
        extension = last_period + 1;
        for (ent = &content_type_table[0]; ent->extension; ++ent) {
            if (!stricmp(ent->extension, extension)) {
                type = ent->content_type;
            }
        }
    }
    return type;
}

bool xBlog::checkauth(const char *auth)
{
    char *p = (char*)strstr(auth, "Basic ");
    p += strlen("Basic ");

    return (0 == strcmp(p, blogconfig.auth.c_str()));
}

bool xBlog::checksession(struct evhttp_request *req)
{
    struct evkeyvalq* header_req = evhttp_request_get_input_headers(req);
    const char *auth = evhttp_find_header(header_req, "Authorization");
    //log_debug("xBlog::checksession  %s \r\n", auth);
    if ((auth) && (checkauth(auth))) {
        return true;
    }
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "0");
    evhttp_add_header(evhttp_request_get_output_headers(req), "WWW-Authenticate", "Basic realm=\"xblog\"");
    SendHttpResphone(req, 401, "Not authorized");
    return false;
}

void xBlog::HttpDebug(struct evhttp_request *req)
{
#ifdef _WIN32
    if ("DEBUG" == Config::GetInstance()->xBlogAppConfig.LogLevel)
#else
    if (LOG_LEVEL_DEBUG == LOGGER._level)
#endif
    {
        xBlog::DebugHttpGetCommand(req);
        xBlog::DebugHttpHeader(req);
    }
}

void xBlog::DebugHttpHeader(struct evhttp_request *req)
{
    struct evkeyval *header;
    struct evkeyvalq *headers;

    char* peer_addr;
    ev_uint16_t peer_port;
    evhttp_connection_get_peer(evhttp_request_get_connection(req), &peer_addr, &peer_port);
    log_debug("peer %s:%d\n", peer_addr, peer_port);
    headers = evhttp_request_get_input_headers(req);
    for (header = headers->tqh_first; header; header = header->next.tqe_next) {
        log_debug("%s: %s\n", header->key, header->value);
    }
}

const char *xBlog::DebugHttpGetCommand(struct evhttp_request *req)
{
    const char *cmdtype;

    switch (evhttp_request_get_command(req)) {
    case EVHTTP_REQ_GET:     cmdtype = "GET"; break;
    case EVHTTP_REQ_POST:    cmdtype = "POST"; break;
    case EVHTTP_REQ_HEAD:    cmdtype = "HEAD"; break;
    case EVHTTP_REQ_PUT:     cmdtype = "PUT"; break;
    case EVHTTP_REQ_DELETE:  cmdtype = "DELETE"; break;
    case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
    case EVHTTP_REQ_TRACE:   cmdtype = "TRACE"; break;
    case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
    case EVHTTP_REQ_PATCH:   cmdtype = "PATCH"; break;
    default: cmdtype = "unknown"; break;
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
    log_debug("data_len: %d\r\n", data_len);
    evbuffer_copyout(pevBuf, pbuffer, data_len);
    log_debug("%s\r\n", pbuffer);
    evhttp_parse_query_str(pbuffer, evdata);
    free(pbuffer);
}

char* xBlog::GetHttpPostData(struct evhttp_request *req)
{
    evbuffer *input_buf = evhttp_request_get_input_buffer(req);
    int data_len = evbuffer_get_length(input_buf);
    log_debug("GetHttpPostData data_len: %d\r\n", data_len);

    char *data_buffer = (char *)malloc(data_len);
    memset(data_buffer, 0, data_len);
    evbuffer_copyout(input_buf, data_buffer, data_len);
    return data_buffer;
}

const char *xBlog::GetVal(struct evkeyvalq *evdata, const char *key)
{
    const char *pVal = evhttp_find_header(evdata, key);
    if (NULL == pVal) {
        return "";
    }

    return pVal;
}

void xBlog::SendErrorResphone(struct evhttp_request *req, int errcode, const char*fmt, ...)
{
    char szBuf[1024] = { 0 };
    va_list va;
    va_start(va, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, va);
    va_end(va);
    log_debug("%s", szBuf);

    evhttp_send_error(req, errcode, szBuf);
}

void xBlog::SendHttpJsonResphone(struct evhttp_request *req, int code, const string & strData)
{
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/json; charset=utf-8");
    SendHttpResphone(req, code, strData);
}

void xBlog::SendHttpResphone(struct evhttp_request *req, int code, const string & strData)
{
    struct evbuffer *buf = evbuffer_new();

    if (NULL == buf) {
        log_error("failed to create response buffer\r\n");
        return;
    }

    evhttp_add_header(evhttp_request_get_output_headers(req), "Server", SERVER_SIGNATURE);
    int ret = evbuffer_add_printf(buf, "%s", strData.c_str());
    log_debug("evbuffer_add_printf ret= %d length=%d \r\n", ret, strData.length());

    evhttp_send_reply(req, code, "OK", buf);
    evbuffer_free(buf);
}

int xBlog::BindSocket(const char *ip, int port)
{
    int skt;
    skt = socket(AF_INET, SOCK_STREAM, 0);
    if (skt < 0) {
        log_error("socket error, nfd=%d \r\n", skt);
        return -1;
    }

#ifdef _WIN32
    int flag = 1;
    ioctlsocket(skt, FIONBIO, (unsigned long *) &flag);
#else

    /* set socket nonblocking */
    int flags;
    if ((flags = fcntl(skt, F_GETFL, NULL)) < 0 || fcntl(skt, F_SETFL, flags | O_NONBLOCK) == -1) {
        log_error("O_NONBLOCK  error, skt=%d \r\n", skt);
        return -1;
    }

    /* set socket closeonexec */
    if ((flags = fcntl(skt, F_GETFD, NULL)) < 0 || fcntl(skt, F_SETFD, flags | FD_CLOEXEC) == -1) {
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

    if (bind(skt, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        log_error("bind %s %d error\r\n", ip, port);
        return -1;
    }

    if (listen(skt, 128) < 0) {
        log_error("listen %s %d error\r\n", ip, port);
        return -1;
    }

    return skt;
}

bool xBlog::Run(const char *ip, int port, int timeout_secs, int nthreads)
{
    int ret = -1;
    int skt = BindSocket(ip, port);

    if (skt < 0) {
        log_error("BindSocket %s %d error\r\n", ip, port);
        return false;
    }

#ifdef _WIN32

    HANDLE ths[128];

    for (int i = 0; i < nthreads; i++) {
        struct event_base *base = event_base_new();
        if (base == NULL) {
            return false;
        }

        struct evhttp *httpd = evhttp_new(base);
        if (httpd == NULL) {
            return false;
        }

        ret = evhttp_accept_socket(httpd, skt);
        if (ret != 0) {
            return false;
        }

        SetRouteTable(httpd);

        ths[i] = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) xBlog::Dispatch, base, NULL, NULL);
        log_info("%d %d \r\n", i, ths[i]);
        if (ret != 0) {
            return false;
        }
    }

#else

    pthread_t ths[128];
    for (int i = 0; i < nthreads; i++) {
        struct event_base *base = event_base_new();
        if (base == NULL) {
            return false;
        }

        struct evhttp *httpd = evhttp_new(base);
        if (httpd == NULL) {
            return false;
        }

        ret = evhttp_accept_socket(httpd, skt);
        if (ret != 0) {
            return false;
        }

        SetRouteTable(httpd);

        ret = pthread_create(&ths[i], NULL, xBlog::Dispatch, base);
        log_info("%d %d \r\n", i, ths[i]);
        if (ret != 0) {
            return false;
        }
        pthread_detach(ths[i]);
    }

#endif

    return true;
}

void *xBlog::Dispatch(void *arg)
{
    if (NULL == arg) {
        log_error("error arg is null\n");
        return NULL;
    }
    event_base_dispatch((struct event_base *) arg);
    return NULL;
}

void xBlog::StartMysqlPool()
{
    mysqlclient = new MysqlPool;
    mysqlclient->init(Config::GetInstance()->xBlogMysqlcfg.ipaddr.c_str(),
                      Config::GetInstance()->xBlogMysqlcfg.username.c_str(),
                      Config::GetInstance()->xBlogMysqlcfg.passwd.c_str(),
                      Config::GetInstance()->xBlogMysqlcfg.dbname.c_str());
    mysqlclient->create_pool(Config::GetInstance()->xBlogMysqlcfg.poolsize);

}

bool xBlog::LoadConfig(BLOGCONFIG &cfg)
{
    stringstream strSQL;
    strSQL << "SELECT theme, username, password from xb_profile;";
    MYSQL_RES *pRes = mysqlclient->select(strSQL.str().c_str());

    if (NULL == pRes) {
        return false;
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(pRes);
    cfg.theme = row[0];
    cfg.user = row[1];
    cfg.pass = row[2];
    mysqlclient->free_res(pRes);

    log_debug("theme:%s ", cfg.theme.c_str());

    unsigned char user_key[128] = { 0 };
    std::string strKey;
    strKey = cfg.user;
    strKey += ":";
    strKey += cfg.pass;

    unsigned long  in_len = strKey.length();
    unsigned long  out_len = sizeof(user_key);
    base64_encode((unsigned char*)strKey.c_str(), in_len, user_key, &out_len);
    cfg.auth.assign((char*)user_key, out_len);

    return true;
}

void xBlog::SendDocument(struct evhttp_request *req, const char* file)
{
    int    fd = -1;
    const char *decoded_path = NULL;
    char whole_path[1024] = { 0 };
    struct stat st;

    printf("xBlog::SendDocument a GET request for <%s>\n", file);

    decoded_path = file;
    if (decoded_path == NULL) {
        log_error("SendDocument decoded_path %s \r\n", decoded_path);
        SendErrorResphone(req, HTTP_NOTFOUND, " file path is null %s ", whole_path);
        return;
    }

    if (strstr(decoded_path, "..")) {
        return;
    }
    

    evutil_snprintf(whole_path, sizeof(whole_path), "%s%s", szDir, decoded_path);
    if (stat(whole_path, &st) < 0) {
        log_error("stat error(errno:%d) whole_path %s \r\n", errno, whole_path);
        SendErrorResphone(req, HTTP_NOTFOUND, "stat error whole_path %s \r\n", whole_path);
        return;
    }
    log_debug("whole_path %s \r\n", whole_path);

    struct evbuffer *evb = evbuffer_new();
    if (!S_ISDIR(st.st_mode)) {
#ifdef WIN32
        if ((fd = open(whole_path, O_RDONLY | O_BINARY)) < 0)
#else
        if ((fd = open(whole_path, O_RDONLY)) < 0)
#endif
            //if ((fd = open(whole_path, O_RDONLY)) < 0)
        {
            printf("open");
            return;
        }

        if (fstat(fd, &st) < 0) {
            //close(fd);
            printf("fstat");
            return;
        }
        printf("fd: %d len:%zd\r\n", fd, (size_t)st.st_size);

        const char *type = GuessContentType(decoded_path);
        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", type);
        evbuffer_add_file(evb, fd, 0, (size_t)st.st_size);
        evhttp_send_reply(req, 200, "OK", evb);

        if (evb) evbuffer_free(evb);
    }

}
void xBlog::SendDocumentCallback(struct evhttp_request *req, void *arg)
{
    struct evbuffer *evb = NULL;
    xBlog *pHttpd = static_cast<xBlog *>(arg);
    //const char *docroot = (const char *)pHttpd->szDir;
    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded = NULL;
    char *decoded_path = NULL;
    char *whole_path = NULL;
    size_t len = 0;
    int    fd = -1;
    struct stat st;

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        return;
    }

    log_info("xBlog::SendDocumentCallback a GET request for <%s>\n", uri);

    decoded_path = evhttp_uridecode(uri, 0, NULL);

    if (decoded_path == NULL) {
        log_error("SendDocumentCallback decoded_path %s \r\n", decoded_path);
        goto err;
    }

    if (strstr(decoded_path, ".."))
        goto err;

    if (strstr(decoded_path, "/admin/") && !pHttpd->checksession(req)) {
        return;
    }

    len = strlen(decoded_path) + strlen(pHttpd->szDir) + 2;
    if (!(whole_path = (char *)malloc(len))) {
        log_error("malloc");
        goto err;
    }

    evutil_snprintf(whole_path, len, "%s%s",  pHttpd->szDir, decoded_path);
    if (stat(whole_path, &st) < 0) {
        log_error("stat error whole_path %s \r\n", whole_path);
        goto err;
    }
    log_info("whole_path %s \r\n", whole_path);
    log_info("decoded_path: %s \r\n", decoded_path);

    evb = evbuffer_new();
    if (!S_ISDIR(st.st_mode)) {
#ifdef WIN32
        if ((fd = open(whole_path, O_RDONLY | O_BINARY)) < 0)
#else
        if ((fd = open(whole_path, O_RDONLY)) < 0)
#endif
        {
            log_error("open");
            goto err;
        }

        if (fstat(fd, &st) < 0) {
            log_error("fstat");
            goto err;
        }

        const char *type = xBlog::GuessContentType(decoded_path);

        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", type);
        //evb = evbuffer_new ();
        evbuffer_add_file(evb, fd, 0, (size_t)st.st_size);
    } else {
        /* If it's a directory, read the comments and make a little index page */
#ifdef WIN32
        HANDLE d;
        WIN32_FIND_DATAA ent;
        char *pattern;
        size_t dirlen;
#else
        DIR *d;
        struct dirent *ent;
#endif
        const char *trailing_slash = "";

#ifdef WIN32
        dirlen = strlen(whole_path);
        pattern = (char *)malloc(dirlen + 3);
        memcpy(pattern, whole_path, dirlen);
        pattern[dirlen] = '\\';
        pattern[dirlen + 1] = '*';
        pattern[dirlen + 2] = '\0';
        d = FindFirstFileA(pattern, &ent);
        free(pattern);
        if (d == INVALID_HANDLE_VALUE)
            goto err;
#else
        if (!(d = opendir(whole_path)))
            goto err;
#endif

        evbuffer_add_printf(evb, "<html>\n <head>\n" "  <title>%s</title>\n" "  <base href='%s%s'>\n" " </head>\n" " <body>\n" "  <h1>%s</h1><hr>\n" "  <ul>\n", decoded_path,    /* XXX html-escape this. */
                            decoded_path, trailing_slash, decoded_path /* XXX html-escape this */);
#ifdef WIN32
        do {
            const char *name = ent.cFileName;
#else
        while ((ent = readdir(d))) {
            const char *name = ent->d_name;
#endif
            if (0 != stricmp(name, "."))
                evbuffer_add_printf(evb, "    <li><a href=\"%s\">%s</a>\n", name, name);    /* XXX escape this */
#ifdef WIN32
        }
        while (FindNextFileA(d, &ent));
#else
        }
#endif
        evbuffer_add_printf(evb, "</ul></body></html>\n");
#ifdef WIN32
        CloseHandle(d);
#else
        closedir(d);
#endif
        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/html");
    }

    evhttp_send_reply(req, 200, "OK", evb);
    goto done;
err:
    if (fd >= 0)
        close(fd);
    evhttp_send_error(req, 404, "not find!");
done:
    if (decoded)
        evhttp_uri_free(decoded);
    if (decoded_path)
        free(decoded_path);
    if (whole_path)
        free(whole_path);
    if (evb)
        evbuffer_free(evb);
}


