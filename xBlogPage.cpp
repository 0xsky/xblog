/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 

#include "xBlogPage.h"
#include "xMysqlPool.h"
#include "xBlog.h"
#include "xBlogBase.h"
#include "xConfig.h"
#include "xBlogCache.h"

void xBlogPage::SendFile(struct evhttp_request *req, const char *rootdir, const char *filepath)
{
    if ((NULL==req) || (NULL==rootdir) || (NULL==filepath))
    {
        log_info("xBlogPage::SendFile error \n");
        SendErrorPage(req, NULL, 404);
        return;
    }

    struct evbuffer *evb = NULL;
    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded = NULL;
    const char *path   = NULL;
    char *decoded_path = NULL;

    int fd = -1;
    struct stat st;
    const char *type;
    char szPath[SIZE_1024] = { 0 };

    evutil_snprintf(szPath, sizeof(szPath), "%s%s", rootdir, filepath);

    log_info("xBlogPage::SendFile a GET request for <%s>\n", uri);

    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        log_error("It's not a good URI. Sending BADREQUEST\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        SendErrorPage(req, NULL, 404);
        return;
    }

    path = evhttp_uri_get_path(decoded);
    if (NULL==path)
    {
        path = "/";
    }

    log_info("xBlogPage::SendFile path: %s \r\n", path);
    decoded_path = evhttp_uridecode(path, 0, NULL);
    if (decoded_path == NULL)
    {
            goto err;
    }
        
    if (strstr(decoded_path, ".."))
    {
        goto err;
    }

    if (stat(szPath, &st) < 0)
    {
        log_error("stat : %s \r\n", szPath);
        goto err;
    }

    evb = evbuffer_new();

    type = GuessContentType(szPath);
    log_info("file type: %s %s \r\n", type, filepath);

#ifdef WIN32
    if ((fd = open(szPath, O_RDONLY | O_BINARY)) < 0)
#else
    if ((fd = open(szPath, O_RDONLY | O_BINARY)) < 0)
#endif
    {
        perror("open");
        goto err;
    }

    //log_info(": %s %s \r\n",type, filepath);
    if (fstat(fd, &st) < 0)
    {
        perror("fstat");
        goto err;
    }
    
    evhttp_add_header(evhttp_request_get_output_headers(req),"Content-Type", type);

    log_info(" xBlogPage::SendFile evb:%p fd:%d size:%d \r\n", evb, fd, st.st_size);
    evbuffer_add_file(evb, fd, 0, st.st_size);

    evhttp_send_reply(req, 200, "OK", evb);

    goto done;
  err:
    evhttp_send_error(req, 404, "Document was not found");
    if (fd >= 0)
        close(fd);
    SendErrorPage(req, NULL, 404);
  done:
    if (decoded)
        evhttp_uri_free(decoded);
    if (decoded_path)
        free(decoded_path);
    if (evb)
        evbuffer_free(evb);
}

void xBlogPage::AdminStatusCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pBlog = (xBlog *) arg;

    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    log_info("xBlogPage::dump_status_cb \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_info("xBlogPage::post_manager_request_cb CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    string strJsonData = "jsonpcallback(";

    Json::Value root;
    root["wwwroot"]      = pBlog->szDir;
    root["StartTime"]    = pBlog->StartTime;
    root["ServerIp"]     = Config::GetInstance()->xBlogAppConfig.ServerIp;
    root["port"]         = Config::GetInstance()->xBlogAppConfig.Port;
    root["LogFileName"]  = Config::GetInstance()->xBlogAppConfig.LogFileName;
    root["LogLevel"]     = Config::GetInstance()->xBlogAppConfig.LogLevel;
    root["Httpdthreads"] = Config::GetInstance()->xBlogAppConfig.Httpdthreads;
    root["HttpdTimeOut"] = Config::GetInstance()->xBlogAppConfig.HttpdTimeOut;


    Json::Value jMysqlConfig;
    jMysqlConfig["ipaddr"]   = Config::GetInstance()->xBlogMysqlcfg.ipaddr;
    jMysqlConfig["port"]     = Config::GetInstance()->xBlogMysqlcfg.port;
    jMysqlConfig["dbname"]   = Config::GetInstance()->xBlogMysqlcfg.dbname;
    jMysqlConfig["username"] = Config::GetInstance()->xBlogMysqlcfg.username;
    jMysqlConfig["poolsize"] = Config::GetInstance()->xBlogMysqlcfg.poolsize;

    root["Mysql"] = jMysqlConfig;

    Json::FastWriter writer;
    strJsonData += writer.write(root);
    strJsonData += ")";

    xBlog::SendHttpResphone(req, strJsonData);
}

void xBlogPage::AdminSystemCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pBlog = (xBlog *) arg;

    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    log_info("xBlogPage::AdminSystemCallback \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_info("xBlogPage::AdminSystemCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    string strJsonData = "jsonpcallback(";

    Json::Value root;
    root["wwwroot"] = pBlog->szDir;
    root["StartTime"]    = pBlog->StartTime;
    root["ServerIp"]     = Config::GetInstance()->xBlogAppConfig.ServerIp;
    root["port"]         = Config::GetInstance()->xBlogAppConfig.Port;
    root["LogFileName"]  = Config::GetInstance()->xBlogAppConfig.LogFileName;
    root["LogLevel"]     = Config::GetInstance()->xBlogAppConfig.LogLevel;
    root["Httpdthreads"] = Config::GetInstance()->xBlogAppConfig.Httpdthreads;
    root["HttpdTimeOut"] = Config::GetInstance()->xBlogAppConfig.HttpdTimeOut;

    Json::Value MysqlConfig;
    MysqlConfig["ipaddr"]   = Config::GetInstance()->xBlogMysqlcfg.ipaddr;
    MysqlConfig["port"]     = Config::GetInstance()->xBlogMysqlcfg.port;
    MysqlConfig["dbname"]   = Config::GetInstance()->xBlogMysqlcfg.dbname;
    MysqlConfig["username"] = Config::GetInstance()->xBlogMysqlcfg.username;
    MysqlConfig["poolsize"] = Config::GetInstance()->xBlogMysqlcfg.poolsize;

    root["Mysql"] = MysqlConfig;

    Json::FastWriter writer;
    strJsonData += writer.write(root);
    strJsonData += ")";

    xBlog::SendHttpResphone(req, strJsonData);
}

void xBlogPage::AdminUserCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *) arg;

    log_info("xBlogPage::AdminUserCallback \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_warn("xBlogPage::AdminUserCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    xBlog::HttpParseURL(req, &evURLdata);
    const char *pAction = evhttp_find_header(&evURLdata, "action");    /* 操作类型 */
    if(NULL==pAction)
    {
        return;
    }

    string strHtml;
    bool bRet = false;
    if (0 == stricmp(pAction, "view"))
    {
        xBlogData::GetInstance()->AdminUser_View(strHtml);
        xBlog::SendHttpResphone(req, strHtml);
        return;
    }
    else if (0 == stricmp(pAction, "update"))
    {
        struct evkeyvalq evPostData;
        xBlog::GetHttpPostData(req, &evPostData);

        const char *szId        = evhttp_find_header(&evPostData, "id");
        const char *szUserLogin = evhttp_find_header(&evPostData, "userlogin");
        const char *szNickName  = evhttp_find_header(&evPostData, "nickname");
        const char *szEmail     = evhttp_find_header(&evPostData, "email");

        bRet = xBlogData::GetInstance()->AdminUser_Update(szId, szUserLogin, szNickName, szEmail);
    }
    else
    {
    }

    strHtml += pAction;
    strHtml += " operation ";
    strHtml += bRet?"success":"Failed";
    xBlog::SendHttpResphone(req, strHtml);
}

void xBlogPage::AdminShellCallback(struct evhttp_request *req, void *arg)
{
    struct evkeyvalq evPostData;
    xBlog *pBlog = (xBlog *) arg;

    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    log_info("xBlogPage::AdminShellCallback \r\n");
    if (!CheckSession(req, pBlog))
    {
        log_info("xBlogPage::AdminShellCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    if (0 == stricmp(Config::GetInstance()->xBlogAppConfig.Shell.c_str(), "FALSE"))
    {
        string str = "系统设置为禁止执行命令！";
        xBlog::SendHttpResphone(req, str);
        return;
    }
    else if (0 == stricmp(Config::GetInstance()->xBlogAppConfig.Shell.c_str(), "TRUE"))
    {
#ifdef _WIN32

#else
        string strData;
        char buf[SIZE_1024*8] = { 0 };
        xBlog::GetHttpPostData(req, &evPostData);
        const char *pCmd = evhttp_find_header(&evPostData, "cmd");

        if (NULL == pCmd)
        {
            log_debug("命令为空! \n");
            return;
        }

        log_debug("%s\n", pCmd);

        FILE *fp = popen(pCmd, "r");
        if (fp == NULL)
        {
            log_error("Failed to run command! \n");
            string str = "Failed to run command! \n";
            xBlog::SendHttpResphone(req, str);
            return;
        }

        while (fgets(buf, sizeof(buf) - 1, fp) != NULL)
        {
            log_info("%s", buf);
            strData += buf;
        }

        pclose(fp);

        xBlog::SendHttpResphone(req, strData);
#endif
    }
}

void xBlogPage::IndexRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    const char *PageId = "0";
    RequestPage(req, PageId);
}

void xBlogPage::PageRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq GetData;
    xBlog::HttpParseURL(req, &GetData);
    const char *PageId = evhttp_find_header(&GetData, "page");    /* 分页ID */

    if (NULL==PageId)
    {
        SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }

    RequestPage(req, PageId);
}

void xBlogPage::RequestPage(struct evhttp_request *req, const char *PageId)
{
    XTEMPLATE strHtml;

    strHtml.value = xBlogCache::GetInstance()->GetPage(atoul(PageId));

    if (strHtml.value.length())
    {
        xBlog::SendHttpResphone(req, strHtml.value);
        log_info("xBlogPage::RequestPage cache get =%s \r\n", PageId);
        return;
    }

    xBlogPageTemplate::GetInstance()->BuildHtmlByTemplateKey(K_DEFAULT, strHtml);

    string strTemp = xBlogPageTemplate::GetInstance()->GetActiveTemplate(K_ARTICLE_MULTI);
    string strData = xBlogData::GetInstance()->GetArticleList(strTemp, atoul(PageId));

    StringReplace(strHtml.value, "<#template:article_multi#>", strData);

    char tmp[SIZE_32] = { 0 };
    evutil_snprintf(tmp, sizeof(tmp),"%d", atoul(PageId) >= 0 ? (atoul(PageId) + 1) : 0);
    StringReplace(strHtml.value, "<#template:pagebar_next#>", tmp);
    evutil_snprintf(tmp, sizeof(tmp), "%d", atoul(PageId) > 1 ? (atoul(PageId) - 1) : 0);
    StringReplace(strHtml.value, "<#template:pagebar_previous#>", tmp);

    xBlogCache::GetInstance()->SetPage(atoul(PageId), strHtml.value);

    xBlog::SendHttpResphone(req, strHtml.value);
}

void xBlogPage::CatalogRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq GetData;
    xBlog::HttpParseURL(req, &GetData);

    const char *pPageId = evhttp_find_header(&GetData, "page");
    const char *pCateId = evhttp_find_header(&GetData, "cate");
    uint32 nPageId = atoul(pPageId);
    uint32 nCateId = atoul(pCateId);

    if (NULL==pCateId)
    {
        SendErrorPage(req, (xBlog *)arg, HTTP_NOTFOUND);
        return;
    }
    if (NULL==pPageId)
    {
        pPageId = "0";
    }

    XTEMPLATE strHtml;
    xBlogPageTemplate::GetInstance()->BuildHtmlByTemplateKey(K_CATALOG, strHtml);
    string strTemp = xBlogPageTemplate::GetInstance()->GetActiveTemplate(K_ARTICLE_MULTI);
    string strData = xBlogData::GetInstance()->GetCatalogPage(strTemp, nCateId, nPageId);

    StringReplace(strHtml.value, "<#template:article_multi#>", strData);

    char tmp[SIZE_32] = { 0 };
    evutil_snprintf(tmp, sizeof(tmp), "%d", nPageId >= 0 ? (nPageId + 1) : 0);
    StringReplace(strHtml.value, "<#template:pagebar_next#>", tmp);
    evutil_snprintf(tmp, sizeof(tmp), "%d", nPageId > 1 ? (nPageId - 1) : 0);
    StringReplace(strHtml.value, "<#template:pagebar_previous#>", tmp);
    StringReplace(strHtml.value, "<#article/category/id#>", pCateId);

    xBlog::SendHttpResphone(req, strHtml.value);
}

void xBlogPage::PostRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq GetData;

    xBlog::HttpParseURL(req, &GetData);
    const char *BlogId = evhttp_find_header(&GetData, "id");

    if (NULL==BlogId)
    {
        SendErrorPage(req, (xBlog *)arg, HTTP_NOTFOUND);
        return;
    }

    XTEMPLATE strHtml;
    strHtml.value = xBlogCache::GetInstance()->GetPost(atoul(BlogId));
    if (strHtml.value.length())
    {
        xBlog::SendHttpResphone(req, strHtml.value);
        log_info("xBlogPage::post_request_cb cache get =%s \r\n", BlogId);
        return;
    }

    xBlogPageTemplate::GetInstance()->BuildHtmlByTemplateKey(K_SINGLE, strHtml);
    xBlogData::GetInstance()->GetArticle(strHtml.value, atoul(BlogId));

    // 添加留言列表
    string strTemp = xBlogPageTemplate::GetInstance()->GetActiveTemplate(K_ARTICLE_COMMENT);
    string strData = xBlogData::GetInstance()->GetArticleCommentList(strTemp, atoul(BlogId));

    StringReplace(strHtml.value, "<#template:article_comment#>", strData);
    StringReplace(strHtml.value, "<#template:article_commentpost#>",
                   xBlogPageTemplate::GetInstance()->GetPageTemplate(K_ARTICLE_COMMENTPOST));
    StringReplace(strHtml.value, "<#article/id#>", BlogId);

    xBlogCache::GetInstance()->SetPost(atoul(BlogId), strHtml.value);
    xBlog::SendHttpResphone(req, strHtml.value);
}

void xBlogPage::GuestbookCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq GetData;
    xBlog::HttpParseURL(req, &GetData);

    XTEMPLATE strHtml;
    xBlogPageTemplate::GetInstance()->BuildHtmlByTemplateKey(K_GUESTBOOK, strHtml);

    string strTemp = xBlogPageTemplate::GetInstance()->GetActiveTemplate(K_ARTICLE_COMMENT);
    string strData = xBlogData::GetInstance()->GetArticleCommentList(strTemp, 0);

    StringReplace(strHtml.value, "<#template:article_guestbook#>", strData);
    StringReplace(strHtml.value, "<#article/id#>", "0");
    StringReplace(strHtml.value, "<#template:article_commentpost#>",
                   xBlogPageTemplate::GetInstance()->GetPageTemplate(K_ARTICLE_COMMENTPOST));

    xBlog::SendHttpResphone(req, strHtml.value);
}

void xBlogPage::GuestPostCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;

    xBlog::HttpParseURL(req, &evURLdata);
    const char *pAction = evhttp_find_header(&evURLdata, "action");
    if (NULL==pAction)
    {
        SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }
    
    xBlog::GetHttpPostData(req, &evPostData);
    const char *pIP = evhttp_find_header(evhttp_request_get_input_headers(req), "host");
    if (0 == stricmp(pAction, "comment"))
    {
        const char *pInpAjax = evhttp_find_header(&evPostData, "inpAjax");
        const char *pInpID = evhttp_find_header(&evPostData, "inpID");
        const char *pInpVerify = evhttp_find_header(&evPostData, "inpVerify");
        const char *pInpEmail = evhttp_find_header(&evPostData, "inpEmail");
        const char *pInpName = evhttp_find_header(&evPostData, "inpName");
        const char *pInpArticle = evhttp_find_header(&evPostData, "inpArticle");
        const char *pInpHomePage = evhttp_find_header(&evPostData, "inpHomePage");

        log_info("%s %s %s %s %s %s %s \r\n", pInpAjax, pInpID,
                 pInpVerify, pInpEmail, pInpName, pInpArticle, pInpHomePage);

        xBlogData::GetInstance()->NewComment(pInpID, pInpName,
                                              pInpEmail, pInpHomePage, pIP, pInpArticle);
    }
    string strHtml;

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, strHtml);
}

void xBlogPage::AdminCheckLoginCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq post_data;
    xBlog *pBlog = (xBlog *) arg;

    xBlog::GetHttpPostData(req, &post_data);

    const char *pUserName = evhttp_find_header(&post_data, "UserName");
    const char *pPasswd = evhttp_find_header(&post_data, "password");

    log_info("xBlogPage::AdminCheckLoginCallback username:%s  passwd:%s \r\n", pUserName, pPasswd);

    if ((NULL==pUserName) || (NULL==pPasswd))
    {
        SendLoginPage(req, pBlog);
        return;
    }

    if (0 != strcmp(pPasswd, Config::GetInstance()->xBlogSiteConfigMap["XBLOG_ADMIN_PASS"].c_str()))
    {
        SendLoginPage(req, pBlog);
        return;
    }
    else
    {
        // 认证成功
        pBlog->ssid_token = CreatSession();
        log_debug("xBlogPage::AdminCheckLoginCallback %llu \r\n", pBlog->ssid_token);
        
        char token[SIZE_128]={0};
        evutil_snprintf(token, sizeof(token), "token=%llu;", pBlog->ssid_token);
        evhttp_add_header(evhttp_request_get_output_headers(req), "Set-Cookie", token);
        token[0]='\0';
        evutil_snprintf(token, sizeof(token), "tokentime=%u;", (uint32)time(NULL));
        evhttp_add_header(evhttp_request_get_output_headers(req), "Set-Cookie", token);

        SendAdminPage(req, pBlog);
        return;
    }
}

void xBlogPage::AdminPostManagerCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *) arg;

    log_info("xBlogPage::AdminPostManagerCallback \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_info("xBlogPage::AdminPostManagerCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);
    
    string strHtml;
    const char *pAction = evhttp_find_header(&evURLdata, "action");

    if(NULL==pAction)
    {
        SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }
    
    if (0 == stricmp(pAction, "delete"))
    {
        const char *szPostId = evhttp_find_header(&evPostData, "id");
        strHtml += szPostId;
        xBlogData::GetInstance()->AdminPostManager_UpdatePostStatus(szPostId, "delete");
        xBlogCache::GetInstance()->delPost(atoul(szPostId));
    }
    else if (0 == stricmp(pAction, "revert"))
    {
        const char *szPostId = evhttp_find_header(&evPostData, "id");
        strHtml += szPostId;
        xBlogData::GetInstance()->AdminPostManager_UpdatePostStatus(szPostId, "publish");
        xBlogCache::GetInstance()->delPost(atoul(szPostId));
    }
    else if (0 == stricmp(pAction, "new_post"))
    {
        const char *pTitle = evhttp_find_header(&evPostData, "title");
        const char *pClassify = evhttp_find_header(&evPostData, "classify");
        const char *pContent = evhttp_find_header(&evPostData, "content");
        const char *pBrief = evhttp_find_header(&evPostData, "brief");

        int len_Title = strlen(pTitle);
        int len_Classify = strlen(pClassify);
        int len_Content = strlen(pContent);
        int len_Brief = strlen(pBrief);

        char *strTitle = (char *) malloc(1 + 2 * len_Title);
        char *strClassify = (char *) malloc(1 + 2 * len_Classify);
        char *strContent = (char *) malloc(1 + 2 * len_Content);
        char *strBrief = (char *) malloc(1 + 2 * len_Brief);

        MysqlEscapeString(strTitle, 1 + 2 * len_Title, pTitle, len_Title);
        MysqlEscapeString(strClassify, 1 + 2 * len_Classify, pClassify, len_Classify);
        MysqlEscapeString(strContent, 1 + 2 * len_Content, pContent, len_Content);
        MysqlEscapeString(strBrief, 1 + 2 * len_Brief, pBrief, len_Brief);

        xBlogData::GetInstance()->AdminPostManager_NewPost(strTitle, strClassify, strContent, strBrief);
        free(strTitle);
        free(strClassify);
        free(strContent);
        free(strBrief);
    }
    else if (0 == stricmp(pAction, "update_post"))
    {
        const char *pID = evhttp_find_header(&evPostData, "id");
        const char *pTitle = evhttp_find_header(&evPostData, "title");
        const char *pClassify = evhttp_find_header(&evPostData, "classify");
        const char *pContent = evhttp_find_header(&evPostData, "content");
        const char *pBrief = evhttp_find_header(&evPostData, "brief");

        int len_Title = strlen(pTitle);
        int len_Classify = strlen(pClassify);
        int len_Content = strlen(pContent);
        int len_Brief = strlen(pBrief);
        char *strTitle = (char *) malloc(1 + 2 * len_Title);
        char *strClassify = (char *) malloc(1 + 2 * len_Classify);
        char *strContent = (char *) malloc(1 + 2 * len_Content);
        char *strBrief = (char *) malloc(1 + 2 * len_Brief);

        MysqlEscapeString(strTitle, 1 + 2 * len_Title, pTitle, len_Title);
        MysqlEscapeString(strClassify, 1 + 2 * len_Classify, pClassify, len_Classify);
        MysqlEscapeString(strContent, 1 + 2 * len_Content, pContent, len_Content);
        MysqlEscapeString(strBrief, 1 + 2 * len_Brief, pBrief, len_Brief);

        xBlogData::GetInstance()->AdminPostManager_UpdatePost(pID, strTitle, strClassify, strContent, strBrief);
        free(strTitle);
        free(strClassify);
        free(strContent);
        free(strBrief);
        
        xBlogCache::GetInstance()->delPost(atoul(pID));
    }

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, strHtml);
}

void xBlogPage::AdminSiteConfigCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *) arg;

    log_info("xBlogPage::AdminSiteConfigCallback \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_info("xBlogPage::AdminSiteConfigCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);
    const char *pAction = evhttp_find_header(&evURLdata, "action");
    if(NULL==pAction)
    {
        SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }
    string strHtml;
    if (0 == stricmp(pAction, "mainpage"))
    {
        xBlogData::GetInstance()->AdminGetSiteConfig_View(strHtml);
        xBlog::SendHttpResphone(req, strHtml);
        return;
    }
    else if (0 == stricmp(pAction, "update"))
    {
        const char *szId = evhttp_find_header(&evPostData, "id");
        const char *szValue = evhttp_find_header(&evPostData, "value");

        xBlogData::GetInstance()->AdminGetSiteConfig_Update(szId, szValue);
    }
    else
    {

    }
    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, strHtml);
}

void xBlogPage::AdminLinksCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *) arg;

    log_info("xBlogPage::AdminLinksCallback \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_warn("xBlogPage::AdminLinksCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);
    const char *pAction = evhttp_find_header(&evURLdata, "action");    /* 操作类型 */
    if(NULL==pAction)
    {
        SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }
    
    string strHtml;
    if (0 == stricmp(pAction, "view"))
    {
        xBlogData::GetInstance()->AdminLinks_View(strHtml);
        xBlog::SendHttpResphone(req, strHtml);
        return;
    }
    else if (0 == stricmp(pAction, "update"))
    {
        const char *pId = evhttp_find_header(&evPostData, "id");
        const char *pUrl = evhttp_find_header(&evPostData, "url");
        const char *pName = evhttp_find_header(&evPostData, "name");

        xBlogData::GetInstance()->AdminLinks_Update(pId, pUrl, pName);
    }
    else
    {
    }

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, strHtml);
}

void xBlogPage::AdminCatalogCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *) arg;

    log_info("xBlogPage::AdminCatalogCallback \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_warn("xBlogPage::AdminCatalogCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);
    const char *pAction = evhttp_find_header(&evURLdata, "action");    /* 操作类型 */
    if(NULL==pAction)
    {
        SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }
    
    string strHtml;
    if (0 == stricmp(pAction, "view"))
    {
        xBlogData::GetInstance()->AdminCatalog_View(strHtml);
        xBlog::SendHttpResphone(req, strHtml);
        return;
    }
    if (0 == stricmp(pAction, "viewselect"))
    {
        xBlogData::GetInstance()->AdminCatalog_GetSelect(strHtml);
        xBlog::SendHttpResphone(req, strHtml);
        return;
    }
    else if (0 == stricmp(pAction, "update"))
    {
        const char *szId = evhttp_find_header(&evPostData, "id");
        const char *szClassfyId = evhttp_find_header(&evPostData, "classfyid");
        const char *szName = evhttp_find_header(&evPostData, "classfyname");
        const char *szStatus = evhttp_find_header(&evPostData, "classfystatus");

        xBlogData::GetInstance()->AdminCatalog_Update(szId, szClassfyId, szName, szStatus);
    }
    else if (0 == stricmp(pAction, "add"))
    {
        const char *szClassfyId = evhttp_find_header(&evPostData, "classfyid");
        const char *szName = evhttp_find_header(&evPostData, "classfyname");
        const char *szStatus = evhttp_find_header(&evPostData, "classfystatus");

        xBlogData::GetInstance()->AdminCatalog_Add( szClassfyId, szName, szStatus);
    }
    else
    {
    }

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, strHtml);
}

void xBlogPage::AdminCommentsCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *) arg;

    log_info("xBlogPage::AdminCommentsCallback \r\n");

    if (!CheckSession(req, pBlog))
    {
        log_warn("xBlogPage::AdminCommentsCallback CheckSession error \r\n");
        SendLoginPage(req, pBlog);
        return;
    }

    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);
    const char *pAction = evhttp_find_header(&evURLdata, "action");    /* 操作类型 */
    if(NULL==pAction)
    {
        SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }
    string strHtml;
    if (0 == stricmp(pAction, "view"))
    {
        xBlogData::GetInstance()->AdminComments_View(strHtml);
        xBlog::SendHttpResphone(req, strHtml);
        return;
    }
    else if (0 == stricmp(pAction, "delete"))
    {
        const char *szId = evhttp_find_header(&evPostData, "id");
        xBlogData::GetInstance()->AdminComments_Delete(szId);
    }
    else
    {
    }

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, strHtml);
}

void xBlogPage::AdminCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    struct evkeyvalq GetData;
    xBlog *pBlog = (xBlog *) arg;

    xBlog::HttpParseURL(req, &GetData);

    if (!CheckSession(req, pBlog))
    {
        SendLoginPage(req, pBlog);
        return;
    }

    string strPageData;
    const char *pAction = evhttp_find_header(&GetData, "action");

    if (NULL == pAction)
    {
        SendAdminPage(req, pBlog);
        return;
    }
    else if (0 == stricmp(pAction, "postlist"))
    {
        uint32 Pageid = atoul(evhttp_find_header(&GetData, "page"));
        xBlogData::GetInstance()->Admin_GetPostList(Pageid, strPageData);
        xBlog::SendHttpResphone(req, strPageData);
        return;
    }
    else if (0 == stricmp(pAction, "editpost"))
    {
        const char *pId = evhttp_find_header(&GetData, "id");
        string strHtml = xBlogData::GetInstance()->Admin_GetPostEdit(pId);

        log_info("%s \r\n", strHtml.c_str());
        xBlog::SendHttpResphone(req, strHtml);

        return;
    }
    else if (0==stricmp(pAction, "clearcache"))
    {
        xBlogCache::GetInstance()->OnTimer();
        string str = "clear cache success!";
        xBlog::SendHttpResphone(req, str);
        return;
    }
    else if (0 == stricmp(pAction, "exit"))
    {
        pBlog->ssid_token = 0;
        evhttp_add_header(evhttp_request_get_output_headers(req), "Set-Cookie", "token=");

        SendLoginPage(req, pBlog);
        return;
    }
    else
    {
        SendAdminPage(req, pBlog);
        return;
    }
}

void xBlogPage::SendLoginPage(struct evhttp_request *req, xBlog *pBlog)
{
    SendFile(req, pBlog->szDir, LOGIN_PAGE);
}

void xBlogPage::SendAdminPage(struct evhttp_request *req, xBlog *pBlog)
{
    SendFile(req, pBlog->szDir, ADMIN_PAGE);
}


// /* Response codes */
// #define HTTP_OK            200    /**< request completed ok */
// #define HTTP_NOCONTENT        204    /**< request does not have content */
// #define HTTP_MOVEPERM        301    /**< the uri moved permanently */
// #define HTTP_MOVETEMP        302    /**< the uri moved temporarily */
// #define HTTP_NOTMODIFIED    304    /**< page was not modified from last */
// #define HTTP_BADREQUEST        400    /**< invalid http request was made */
// #define HTTP_NOTFOUND        404    /**< could not find content for uri */
// #define HTTP_BADMETHOD        405     /**< method not allowed for this uri */
// #define HTTP_ENTITYTOOLARGE    413    /**<  */
// #define HTTP_EXPECTATIONFAILED    417    /**< we can't handle this expectation */
// #define HTTP_INTERNAL           500     /**< internal error */
// #define HTTP_NOTIMPLEMENTED     501     /**< not implemented */
// #define HTTP_SERVUNAVAIL    503    /**< the server is not available */


void xBlogPage::SendErrorPage(struct evhttp_request *req, xBlog *pBlog, uint32 errorcode)
{
    switch(errorcode)
    {
    case 404:
        {
            if (NULL==pBlog)
            {
                evhttp_send_error(req, errorcode, "文件找不到!");
            }
            else
            {
                SendFile(req, pBlog->szDir, PAGE_404);
            }
            
            break;
        }
    case 502:
        {
            evhttp_send_error(req, 502, "Bad Gateway.");
            break;
        }
    case 503:
        {
            evhttp_send_error(req, errorcode, "服务器不可用!");
            break;
        }
    default:
        {
            evhttp_send_error(req, errorcode, "未知错误!");
        }
    }
}

bool xBlogPage::CheckSession(struct evhttp_request * req, xBlog * pBlog)
{
    struct evkeyvalq *headers;
    struct evkeyvalq evCookie;

    headers = evhttp_request_get_input_headers(req);
    const char *szCookie = evhttp_find_header(headers, "Cookie");

    if (NULL==szCookie)
    {
        log_info("xBlogPage::CheckSession can not find Cookie \r\n");
        return false;
    }

    evhttp_parse_query_str(szCookie, &evCookie);
    char szToken[SIZE_128] = { 0 };
    GetSubStr((char *) szCookie, "token=", ";", szToken);
    if (strlen(szToken) > 0)
    {
        uint64 nToken = atoull(szToken);
        log_info("%llu %llu-%s \r\n", pBlog->ssid_token, nToken, szToken);

        if (pBlog->ssid_token == nToken)
        {
            return true;
        }
    }
    return false;
}

void xBlogPage::SendDocumentCallback(struct evhttp_request *req, void *arg)
{
    struct evbuffer *evb = NULL;
    xBlog *pBlog = (xBlog *) arg;
    const char *docroot = (const char *) pBlog->szDir;
    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded = NULL;
    char *decoded_path = NULL;
    char *whole_path   = NULL;
    size_t len = 0;
    int    fd  = -1;
    struct stat st;

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET)
    {
        return;
    }

    log_info("xBlogPage::SendDocumentCallback a GET request for <%s>\n", uri);

    /* Decode the URI */
    //decoded = evhttp_uri_parse (uri);
    //if (!decoded)
    //{
    //    log_info ("It's not a good URI. Sending BADREQUEST\n");
    //    evhttp_send_error (req, HTTP_BADREQUEST, 0);
    //    return;
    //}
    //
    //path = evhttp_uri_get_path (decoded);
    //if (!path)
    //{
    //    path = "/";
    //}

    decoded_path = evhttp_uridecode(uri, 0, NULL);
    log_info("SendDocumentCallback decoded_path:<%s>\n", decoded_path);
    if (decoded_path == NULL)
    {
        log_info("SendDocumentCallback decoded_path %s \r\n", decoded_path);
        goto err;
    }

    if (strstr(decoded_path, ".."))
        goto err;

    if (0 == stricmp(decoded_path, "/") || 0 == strnicmp(decoded_path, "/index", strlen("/index")))
    {
        IndexRequestCallback(req, arg);
        return;
    }

    len = strlen(decoded_path) + strlen(docroot) + 2;
    if (!(whole_path = (char *) malloc(len)))
    {
        log_error("malloc");
        goto err;
    }

    evutil_snprintf(whole_path, len, "%s%s", docroot, decoded_path);
    if (stat(whole_path, &st) < 0)
    {
        log_info("stat error whole_path %s \r\n", whole_path);
        goto err;
    }
    log_info("whole_path %s \r\n", whole_path);
    log_info("decoded_path: %s \r\n", decoded_path);

    evb = evbuffer_new();
    if (!S_ISDIR(st.st_mode))
    {
#ifdef WIN32
        if ((fd = open(whole_path, O_RDONLY | O_BINARY)) < 0)
#else
        if ((fd = open(whole_path, O_RDONLY)) < 0)
#endif
        {
            perror("open");
            goto err;
        }

        if (fstat(fd, &st) < 0)
        {
            perror("fstat");
            goto err;
        }

        const char *type = GuessContentType(decoded_path);

        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", type);
        //evb = evbuffer_new ();
        evbuffer_add_file(evb, fd, 0, (size_t) st.st_size);
    }
    else
    {
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
        string strOpenDir = Config::GetInstance()->xBlogSiteConfigMap["XBLOG_DOWNLOAD"];

        if (0 != strncmp(decoded_path, strOpenDir.c_str(), strOpenDir.length()))
        {
            goto err;
        }

        //if (!strlen(path) || path[strlen(path)-1] != '/')
        //    trailing_slash = "/";

#ifdef WIN32
        dirlen = strlen(whole_path);
        pattern = (char *) malloc(dirlen + 3);
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
                            decoded_path, trailing_slash, decoded_path /* XXX html-escape this */ );
#ifdef WIN32
        do
        {
            const char *name = ent.cFileName;
#else
        while ((ent = readdir(d)))
        {
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
    evhttp_send_error(req, 404, "Document was not found");
    if (fd >= 0)
        close(fd);
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

void xBlogPage::DownloadCallback(struct evhttp_request *req, void *arg)
{
    xBlog::DebugHttpGetCommand(req);
    xBlog::DebugHttpHeader(req);

    xBlog *pBlog = (xBlog *) arg;
    const char *docroot = (const char *) pBlog->szDir;
    log_info("xBlogPage::DownloadCallback docroot: %s\n", docroot);
    struct evkeyvalq GetData;

    xBlog::HttpParseURL(req, &GetData);
    const char *pURIPath = evhttp_find_header(&GetData, "path");    /* 取出参数 */

    if (NULL==pURIPath)
    {
        pURIPath = "/";
    }

    struct evbuffer *evb = NULL;

    //const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *pURIPathDecode = NULL;

    //const char *path;
    char *decoded_path;
    char *whole_path = NULL;
    size_t len;
    int fd = -1;
    struct stat st;
    const char *uri_root = "a";

    log_info("Got a GET request for <%s>\n", pURIPath);

    /* Decode the URI *
       pURIPathDecode = evhttp_uri_parse(pURIPath);
       if (!pURIPathDecode) {
       printf("It's not a good URI. Sending BADREQUEST\n");
       evhttp_send_error(req, HTTP_BADREQUEST, 0);
       return;
       } */

    /* Let's see what path the user asked for. */
    //path = evhttp_uri_get_path(pURIPath);
    //if (!path) path = "/";

    /* We need to decode it, to see what path the user really wanted. */
    decoded_path = evhttp_uridecode(pURIPath, 0, NULL);
    log_info("decoded_path:<%s>\n", decoded_path);
    if (decoded_path == NULL)
        goto err;

    if (strstr(decoded_path, ".."))
        goto err;

    len = strlen(decoded_path) + strlen(docroot) + 16 + strlen("download");
    if (!(whole_path = (char *) malloc(len)))
    {
        log_info("malloc");
        goto err;
    }


    if (decoded_path[strlen(decoded_path) - 1] == '/')
    {
        evutil_snprintf(whole_path, len, "%s/download", docroot);
    }
    else
    {
        if (decoded_path[strlen(decoded_path) - 1] == '/')
        {
            decoded_path[strlen(decoded_path) - 1] = '\0';
        }
        evutil_snprintf(whole_path, len, "%s/download/%s", docroot, decoded_path);
    }

    log_info("whole_path %s \r\n", whole_path);
    log_info("decoded_path: %s \r\n", decoded_path);

    if (stat(whole_path, &st) < 0)
    {
        goto err;
    }

    /* This holds the content we're sending. */

    evb = evbuffer_new();
    if (S_ISDIR(st.st_mode))
    {
        /* If it's a directory, read the comments and make a little
         * index page */
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

        if (!strlen(pURIPath) || pURIPath[strlen(pURIPath) - 1] != '/')
            trailing_slash = "/";

#ifdef WIN32
        dirlen = strlen(whole_path);
        pattern = (char *) malloc(dirlen + 3);
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

        evbuffer_add_printf(evb, "<html><meta http-equiv=\"content-Type\" content=\"text/html;charset=gb2312\" />\n <head>\n" "  <title>%s</title>\n" "  <base href='%s%s%s'>\n" " </head>\n" " <body>\n" "  <h1>%s</h1>\n" "  <ul>\n", pURIPath,    /* XXX html-escape this. */
                            uri_root, pURIPath,    /* XXX html-escape this? */
                            trailing_slash, pURIPath /* XXX html-escape this */ );
#ifdef WIN32
        do
        {
            const char *name = ent.cFileName;
#else
        while ((ent = readdir(d)))
        {
            const char *name = ent->d_name;
#endif
            evbuffer_add_printf(evb, "    <li><a href=\"/download?path=%s\">%s</a>\n", name, name);    /* XXX escape this */
#ifdef WIN32
        } while (FindNextFileA(d, &ent));
#else
        }
#endif
        evbuffer_add_printf(evb, "</ul></body></html>\n");
#ifdef WIN32
        //  CloseHandle(d);
#else
        closedir(d);
#endif
        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/html");
    }
    else
    {
        /* Otherwise it's a file; add it to the buffer to get
         * sent via sendfile */

#ifdef WIN32
        if ((fd = open(whole_path, O_RDONLY | O_BINARY)) < 0)
#else
        if ((fd = open(whole_path, O_RDONLY)) < 0)
#endif
        {
            perror("open");
            goto err;
        }

        if (fstat(fd, &st) < 0)
        {
            /* Make sure the length still matches, now that we
             * opened the file :/ */
            log_info("fstat");
            goto err;
        }

        const char *type = GuessContentType(decoded_path);
        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", type);

        log_info("whole_path %s: %ld \r\n", whole_path, st.st_size);

        evbuffer_add_file(evb, fd, 0, st.st_size);
    }

    evhttp_send_reply(req, 200, "OK", evb);
    goto done;
  err:
    evhttp_send_error(req, 404, "Document was not found");
    if (fd >= 0)
        close(fd);
  done:
    if (pURIPathDecode)
        evhttp_uri_free(pURIPathDecode);
    if (decoded_path)
        free(decoded_path);
    if (whole_path)
        free(whole_path);
    if (evb)
        evbuffer_free(evb);
}


