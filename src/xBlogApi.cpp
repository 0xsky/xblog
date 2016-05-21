/*
* ----------------------------------------------------------------------------
* Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
* All rights reserved.
* Distributed under GPL license.
* ----------------------------------------------------------------------------
*/

#include "xBlog.h"
#include "xMysqlPool.h"
#include "MultipartParase.h"

void xBlog::SetRouteTable(evhttp * http)
{
    evhttp_set_cb(http, "/", xBlog::IndexRequestCallback, this);                //
    evhttp_set_cb(http, "/index.htm", xBlog::IndexRequestCallback, this);       //
    evhttp_set_cb(http, "/index.html", xBlog::IndexRequestCallback, this);      //
    evhttp_set_cb(http, "/postlist", xBlog::PostlistRequestCallback, this);     //
    evhttp_set_cb(http, "/post", xBlog::PostRequestCallback, this);             //
    evhttp_set_cb(http, "/classify", xBlog::ClassifyRequestCallback, this);     //
    evhttp_set_cb(http, "/links", xBlog::LinksRequestCallback, this);           //
    evhttp_set_cb(http, "/config", xBlog::ConfigCallback, this);                //
    evhttp_set_cb(http, "/postview", xBlog::PostviewCallback, this);            //
    evhttp_set_cb(http, "/paging",  xBlog::PagingCallback, this);               //
    evhttp_set_cb(http, "/catalog",      xBlog::CatalogRequestCallback, this);  //
    evhttp_set_cb(http, "/infogape", xBlog::InfopageRequestCallback, this);     //

    // 管理后台功能
    evhttp_set_cb(http, "/admin", xBlog::AdminCallback, this);                     //
    //evhttp_set_cb(http, "/checklogin", xBlog::AdminCheckLoginCallback, this);      //
    evhttp_set_cb(http, "/admin_post", xBlog::AdminPostManager_Callback, this);    //
    evhttp_set_cb(http, "/admin_links", xBlog::AdminLinks_Callback, this);         //
    evhttp_set_cb(http, "/admin_config", xBlog::AdminSiteConfig_Callback, this);   //
    evhttp_set_cb(http, "/admin_catalog", xBlog::AdminCatalog_Callback, this);     //

    evhttp_set_cb(http, "/ueditor1_4_3_2-src/uecontroller", xBlog::UEControllerCallback, this);

    evhttp_set_timeout(http, 5);
    evhttp_set_gencb(http, xBlog::SendDocumentCallback, this);
}

void xBlog::IndexRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);

    log_debug("theme %s", pthis->blogconfig.theme.c_str());
    string strpath = "/themes/";
    strpath += pthis->blogconfig.theme + "/index.htm";
    log_debug("%s", strpath.c_str());
    pthis->SendDocument(req, strpath.c_str());
}

void xBlog::PostlistRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);
    const char *arg_page = xBlog::GetVal(&uri_arg, "page");

    int pageid = 0;
    if (NULL != arg_page) {
        pageid = atoi(arg_page);
        pageid = (pageid <= 0) ? 1 : pageid;
    }

    int count = 10;
    int index = (pageid - 1) * count;
    index = (index > 0) ? index : 0;

    string strData;
    std::string strSQL = "SELECT ID, post_author, post_date, post_classify, post_brief, post_title FROM xb_posts ";
    strSQL += " WHERE post_status = 'publish' order by post_date DESC ";
    strSQL += " limit " + tostring(index) + ", 10 ";
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::CatalogRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);
    const char *arg_cate = xBlog::GetVal(&uri_arg, "id");

    int cateid = 0;
    if (NULL != arg_cate) {
        cateid = atoi(arg_cate);
    } else {
        cateid = 0;
    }

    std::string strData;
    std::string strSQL = "SELECT * FROM xb_posts WHERE post_status = 'publish' AND post_classify = ";
    strSQL += tostring(cateid);
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::InfopageRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);
    const char *arg_cate = xBlog::GetVal(&uri_arg, "id");

    int cateid = 0;
    if (NULL != arg_cate) {
        cateid = atoi(arg_cate);
    } else {
        cateid = 0;
    }

    std::string strData;
    std::string strSQL = "SELECT page_id, page_title, post_id FROM xb_page";
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::PostRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);
    const char *arg_id = xBlog::GetVal(&uri_arg, "id");
    const char *arg_pos = xBlog::GetVal(&uri_arg, "pos");

    bool last = false;
    bool next = false;
    int postid = 0;
    if (NULL != arg_id) {
        postid = atoi(arg_id);
        postid = (postid <= 0) ? 1 : postid;
    }
    if ((NULL != arg_pos) && (0 == strcmp(arg_pos, "last"))) {
        last = true;
    }
    if ((NULL != arg_pos) && (0 == strcmp(arg_pos, "next"))) {
        next = true;
    }

    std::string strData;
    std::string strSQL = "SELECT PT.ID, PT.post_author, PT.post_date, CY.classify_name, PT.post_content, PT.post_title ";
    strSQL += " FROM xb_posts AS PT  LEFT JOIN xb_classify AS CY ON PT.post_classify = CY.ID ";
    if (!last && !next) {
        strSQL += " WHERE PT.post_status = 'publish'  AND PT.ID = " + tostring(postid);
    } else if (last) {
        strSQL += " WHERE PT.post_status = 'publish'  AND PT.ID < " + tostring(postid) + " order by PT.ID DESC limit 1";
    } else if (next) {
        strSQL += " WHERE PT.post_status = 'publish'  AND PT.ID > " + tostring(postid) + " order by PT.ID ASC limit 1";
    }

    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::ClassifyRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);

    std::string strData;
    std::string strSQL = "SELECT cs.classify_id AS id, cs.classify_name,  COUNT(1) as cnt  FROM xb_posts as ps \
    LEFT JOIN xb_classify AS cs ON ps.post_classify = cs.classify_id WHERE ps.post_status = 'publish' \
    GROUP BY ps.post_classify; ";
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::LinksRequestCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);

    std::string strData;
    std::string strSQL = "SELECT link_id, link_url, link_name, link_description FROM xb_links;";
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::ConfigCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);

    std::string strData;
    std::string strSQL = "SELECT * FROM xb_profile;";
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::PostviewCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);
    struct evkeyvalq uri_arg;
    xBlog::HttpParseURL(req, &uri_arg);

    std::string strData;
    std::string strSQL = "SELECT  ID, post_title FROM xb_posts WHERE post_status='publish' ORDER BY post_date DESC limit 10;";
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::PagingCallback(struct evhttp_request *req, void *arg)
{
    xBlog *pthis = (xBlog*)arg;
    xBlog::HttpDebug(req);

    std::string strData;
    std::string strSQL = "SELECT COUNT(1) AS total,  10 as page FROM xb_posts WHERE post_status = 'publish';";
    bool bRet = pthis->mysqlclient->select_json(strSQL.c_str(), strData);
    if (bRet) {
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strData);
    }
}

void xBlog::AdminCallback(struct evhttp_request *req, void *arg)
{
    xBlog::HttpDebug(req);
    struct evkeyvalq GetData;
    xBlog *pBlog = (xBlog *)arg;
    xBlog::HttpParseURL(req, &GetData);

    if (!pBlog->checksession(req)) {
        return;
    } else {
        pBlog->SendDocument(req, "/admin/admin.htm");
    }
}

void xBlog::AdminPostManager_Callback(struct evhttp_request *req, void *arg)
{
    xBlog::HttpDebug(req);
    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *)arg;
    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);

    if (!pBlog->checksession(req)) {
        log_info("CheckSession error \r\n");
        SendErrorResphone(req, HTTP_OK, "CheckSession error");
        return;
    }

    std::string strHtml;
    std::string msg;
    bool bRet = false;
    const char *pAction = xBlog::GetVal(&evURLdata, "action");
    log_info("pAction %s \r\n", pAction);
    if (NULL == pAction) {
        SendErrorResphone(req, HTTP_OK, "pAction error");
        return;
    }

    if (0 == stricmp(pAction, "delete")) {
        const char *szPostId = xBlog::GetVal(&evPostData, "id");
        log_info("szPostId %s \r\n", szPostId);
        if (NULL == szPostId) {
            SendErrorResphone(req, HTTP_OK, "id error");
            return;
        }
        stringstream ssSQL;
        ssSQL << "update xb_posts set post_status='delete' where ID= " << szPostId;
        bRet  = pBlog->mysqlclient->execute(ssSQL.str().c_str());
        msg = bRet ? ("success") : ("error");
    } else if (0 == stricmp(pAction, "revert")) {
        const char *szPostId = xBlog::GetVal(&evPostData, "id");
        if (NULL == szPostId) {
            SendErrorResphone(req, HTTP_OK, "id error");
            return;
        }
        stringstream ssSQL;
        ssSQL << "update xb_posts set post_status='publish' where ID= " << szPostId;
        bRet = pBlog->mysqlclient->execute(ssSQL.str().c_str());
        msg = bRet ? ("success") : ("error");
    } else if (0 == stricmp(pAction, "view")) {
        // 查看文章列表
        pBlog->mysqlclient->select_json("SELECT ID, post_author, post_date, post_title, post_status, post_classify FROM xb_posts order by post_date DESC;", strHtml);
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strHtml);
        return;
    } else if (0 == stricmp(pAction, "edit")) {
        // 编辑文章
        const char *szPostId = xBlog::GetVal(&evURLdata, "id");
        if (NULL == szPostId) {
            SendErrorResphone(req, HTTP_OK, "id error");
            return;
        }
        stringstream ssSQL;
        ssSQL << "SELECT ID, post_author, post_date, post_title, post_status, post_classify, post_content, post_brief FROM xb_posts where ID=" << atoi(szPostId);
        pBlog->mysqlclient->select_json(ssSQL.str().c_str(), strHtml);
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strHtml);
        return;
    } else if (0 == stricmp(pAction, "new_post")) {
        // 建立新文章
        std::string title = xBlog::GetVal(&evPostData, "title");
        std::string classify = xBlog::GetVal(&evPostData, "classify");
        std::string content = xBlog::GetVal(&evPostData, "content");
        std::string brief = xBlog::GetVal(&evPostData, "brief");

        pBlog->mysqlclient->escape_string(title, title);
        pBlog->mysqlclient->escape_string(classify, classify);
        pBlog->mysqlclient->escape_string(content, content);
        pBlog->mysqlclient->escape_string(brief, brief);
        stringstream ssSQL;
        ssSQL << "insert into xb_posts set post_date=now(), post_classify=" << classify
              << ", post_title = '" << title << "'"
              << ", post_content = '" << content << "'"
              << ", post_brief = '" << brief << "'";
        bRet = pBlog->mysqlclient->execute(ssSQL.str().c_str());
        msg = bRet ? ("success") : ("error");
    } else if (0 == stricmp(pAction, "update_post")) {
        // 更新文章内容
        std::string id = xBlog::GetVal(&evPostData, "id");
        std::string title = xBlog::GetVal(&evPostData, "title");
        std::string classify = xBlog::GetVal(&evPostData, "classify");
        std::string content = xBlog::GetVal(&evPostData, "content");
        std::string brief = xBlog::GetVal(&evPostData, "brief");

        pBlog->mysqlclient->escape_string(id, id);
        pBlog->mysqlclient->escape_string(title, title);
        pBlog->mysqlclient->escape_string(classify, classify);
        pBlog->mysqlclient->escape_string(content, content);
        pBlog->mysqlclient->escape_string(brief, brief);

        // ID, post_author, post_date, post_title, post_status, post_classify, post_content
        stringstream ssSQL;
        ssSQL << "update xb_posts set post_classify=" << classify
              << ", post_title = '" << title << "'"
              << ", post_content = '" << content << "'"
              << ", post_brief = '" << brief << "'"
              << " where ID=" << id;
        bRet = pBlog->mysqlclient->execute(ssSQL.str().c_str());
        msg = bRet ? ("success") : ("error");
    } else {
        SendErrorResphone(req, HTTP_OK, "arg error");
        return;
    }

    strHtml += pAction;
    strHtml += " ";
    strHtml += msg;
    xBlog::SendHttpResphone(req, HTTP_OK, strHtml);
}

void xBlog::AdminLinks_Callback(struct evhttp_request *req, void *arg)
{
    xBlog::HttpDebug(req);
    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *)arg;
    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);

    if (!pBlog->checksession(req)) {
        log_info("CheckSession error \r\n");
        SendErrorResphone(req, HTTP_OK, "CheckSession error");
        return;
    }

    std::string strHtml;
    const char *pAction = xBlog::GetVal(&evURLdata, "action");
    if (NULL == pAction) {
        SendErrorResphone(req, HTTP_OK, "action error");
        return;
    }

    if (0 == stricmp(pAction, "del")) {
        const char *szPostId = xBlog::GetVal(&evPostData, "id");
        if (NULL == szPostId) {
            SendErrorResphone(req, HTTP_OK, "error id");
            return;
        }
        std::string strSQL = "delete from xb_links where link_id=";
        strSQL += szPostId;
        pBlog->mysqlclient->execute(strSQL.c_str());
    } else if (0 == stricmp(pAction, "view")) {
        pBlog->mysqlclient->select_json("SELECT link_id, link_url, link_name, link_description, link_visible FROM xb_links;", strHtml);
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strHtml);
        return;
    } else if (0 == stricmp(pAction, "revert")) {
        const char *szPostId = xBlog::GetVal(&evPostData, "id");
        strHtml += szPostId;

    } else if (0 == stricmp(pAction, "add")) {
        std::string link_url = xBlog::GetVal(&evPostData, "link_url");
        std::string link_name = xBlog::GetVal(&evPostData, "link_name");
        std::string link_description = xBlog::GetVal(&evPostData, "link_description");
        std::string link_visible = xBlog::GetVal(&evPostData, "link_visible");

        pBlog->mysqlclient->escape_string(link_url, link_url);
        pBlog->mysqlclient->escape_string(link_name, link_name);
        pBlog->mysqlclient->escape_string(link_description, link_description);
        pBlog->mysqlclient->escape_string(link_visible, link_visible);
        stringstream ssSQL;
        ssSQL << "insert into xb_links set link_url='" << link_url << "'"
              << ", link_name = '" << link_name << "'"
              << ", link_description = '" << link_description << "'"
              << ", link_visible = '" << link_visible << "'";
        pBlog->mysqlclient->execute(ssSQL.str().c_str());

    } else if (0 == stricmp(pAction, "update")) {
        std::string link_id = xBlog::GetVal(&evPostData, "link_id");
        std::string link_url = xBlog::GetVal(&evPostData, "link_url");
        std::string link_name = xBlog::GetVal(&evPostData, "link_name");
        std::string link_description = xBlog::GetVal(&evPostData, "link_description");
        std::string link_visible = xBlog::GetVal(&evPostData, "link_visible");

        pBlog->mysqlclient->escape_string(link_url, link_url);
        pBlog->mysqlclient->escape_string(link_name, link_name);
        pBlog->mysqlclient->escape_string(link_description, link_description);
        pBlog->mysqlclient->escape_string(link_visible, link_visible);
        stringstream ssSQL;
        ssSQL << "update xb_links set link_url='" << link_url << "'"
              << " link_name = '" << link_name << "'"
              << " link_description = '" << link_description << "'"
              << " link_visible = '" << link_visible << "'"
              << " where  link_id=" << link_id;
        pBlog->mysqlclient->execute(ssSQL.str().c_str());
    }

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, HTTP_OK, strHtml);
}

void xBlog::AdminSiteConfig_Callback(struct evhttp_request *req, void *arg)
{
    xBlog::HttpDebug(req);
    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *)arg;
    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);

    if (!pBlog->checksession(req)) {
        log_info("CheckSession error \r\n");
        SendErrorResphone(req, HTTP_OK, "CheckSession error");
        return;
    }

    std::string strHtml;
    const char *pAction = xBlog::GetVal(&evURLdata, "action");
    if (NULL == pAction) {
        SendErrorResphone(req, HTTP_OK, "action error");
        return;
    }

    if (0 == stricmp(pAction, "view")) {
        pBlog->mysqlclient->select_json("SELECT * FROM xb_profile;", strHtml);
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strHtml);
        return;
    } else if (0 == stricmp(pAction, "update")) {
        //const char *pID = xBlog::GetVal(&evPostData, "id");
        //const char *pTitle = xBlog::GetVal(&evPostData, "title");
        //const char *pClassify = xBlog::GetVal(&evPostData, "classify");
        //const char *pContent = xBlog::GetVal(&evPostData, "content");
        //const char *pBrief = xBlog::GetVal(&evPostData, "brief");

    }

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, HTTP_OK, strHtml);
}

void xBlog::AdminCatalog_Callback(struct evhttp_request *req, void *arg)
{
    xBlog::HttpDebug(req);
    struct evkeyvalq evPostData;
    struct evkeyvalq evURLdata;
    xBlog *pBlog = (xBlog *)arg;
    xBlog::HttpParseURL(req, &evURLdata);
    xBlog::GetHttpPostData(req, &evPostData);

    if (!pBlog->checksession(req)) {
        log_info("CheckSession error \r\n");
        SendErrorResphone(req, HTTP_OK, "CheckSession error");
        return;
    }

    std::string strHtml;
    const char *pAction = xBlog::GetVal(&evURLdata, "action");
    if (NULL == pAction) {
        //SendErrorPage(req, (xBlog *)arg, 404);
        return;
    }

    if (0 == stricmp(pAction, "del")) {
        const char *szPostId = xBlog::GetVal(&evPostData, "id");
        if (NULL == szPostId) {
            SendErrorResphone(req, HTTP_OK, "error id");
            return;
        }
        std::string strSQL = "delete from xb_links where link_id=";
        strSQL += szPostId;
        pBlog->mysqlclient->execute(strSQL.c_str());
    } else if (0 == stricmp(pAction, "view")) {
        pBlog->mysqlclient->select_json("SELECT * FROM xb_classify;", strHtml);
        xBlog::SendHttpJsonResphone(req, HTTP_OK, strHtml);
        return;
    } else if (0 == stricmp(pAction, "revert")) {
        const char *szPostId = xBlog::GetVal(&evPostData, "id");
        strHtml += szPostId;

    } else if (0 == stricmp(pAction, "add")) {
        // const char *pTitle = xBlog::GetVal(&evPostData, "title");

    } else if (0 == stricmp(pAction, "update")) {
        //const char *pID = xBlog::GetVal(&evPostData, "id");

    }

    strHtml += pAction;
    strHtml += " operation success!";
    xBlog::SendHttpResphone(req, HTTP_OK, strHtml);
}

// https://www.google.com/?gws_rd=ssl#q=libevent+evhttp+upload
void xBlog::UEControllerCallback(struct evhttp_request *req, void *arg)
{
    xBlog::HttpDebug(req);
    xBlog *pBlog = (xBlog *)arg;
    struct evkeyvalq GetData;
    xBlog::HttpParseURL(req, &GetData);
    struct evkeyvalq evURLdata;
    //struct evkeyvalq evPostData;
    //xBlog::GetHttpPostData(req, &evPostData);
    //evbuffer *pevBuf = evhttp_request_get_input_buffer(req);
    //int data_len = evbuffer_get_length(pevBuf);

    log_debug("UEControllerCallback ");
    //log_debug("UEControllerCallback %d", data_len);


    evhttp_cmd_type cmd_type = evhttp_request_get_command(req);
    if (EVHTTP_REQ_GET == cmd_type) {
        pBlog->SendDocument(req, "/config.json");
        return;
    }

    if (EVHTTP_REQ_POST != cmd_type) {
        SendErrorResphone(req, HTTP_OK, "error cmd");
        return;
    }

    xBlog::HttpParseURL(req, &evURLdata);
    struct evkeyvalq *headers = evhttp_request_get_input_headers(req);
    const char *content_type = xBlog::GetVal(headers, "Content-Type");
    const char *content_length = xBlog::GetVal(headers, "Content-Length");

    //const char *pAction = xBlog::GetVal(&evURLdata, "action");

    if (NULL == strstr(content_type, "boundary")) {
        log_info("boundary error \r\n");
        return;
    }

    const char *boundary = NULL;
    const char *boundary_end = NULL;
    char boundaryPattern[64] = {0};
    int boundary_len = 0;

    boundary = strchr(content_type, '=');
    boundary++;
    boundary_len = strlen(boundary);

    if (boundary[0] == '"') {
        boundary++;
        boundary_end = strchr(boundary, '"');
        if (!boundary_end) {
            log_error("Invalid boundary in multipart/form-data POST data");
            return;
        }
    } else {
        boundary_end = strpbrk(boundary, ",;");
    } if (boundary_end) {
        //boundary_end[0] = '\0';
        boundary_len = boundary_end - boundary;
    }

    log_debug("boundary Find. boundary = %s Content_Length:%s ", boundary, content_length);
    snprintf(boundaryPattern, boundary_len + 3, "--%s", boundary);
    log_debug("boundaryPattern = %s  strlen = %d", boundaryPattern, (int)strlen(boundaryPattern));

    evbuffer *pevBuf = evhttp_request_get_input_buffer(req);
    int data_len = evbuffer_get_length(pevBuf);
    char *buf = xBlog::GetHttpPostData(req);

    MultipartConsumer   Multipart(boundaryPattern);
    string strBody;
    strBody.assign(buf, data_len);
    Multipart.CountHeaders(strBody);

    //if (0 == stricmp(pAction, "uploadimage")) {
    //    log_debug("uploadimage ");
    //    char *pParase = buf;
    //    while ((pParase = strstr(pParase, boundaryPattern)) && pParase) {
    //        char *pEnd = strstr(pParase, "\r\n\r\n");
    //        pEnd = strstr(pEnd + strlen("\r\n\r\n"), "\r\n");
    //        pEnd[0] = '\0';
    //        log_info("w: %s ", pParase);
    //
    //        pParase = pEnd + 2;
    //    }
    //
    //} else if (0 == stricmp(pAction, "uploadfile")) {
    //    log_debug("uploadfile ");
    //    char *pParase = buf;
    //    while ((pParase = strstr(pParase, boundaryPattern)) && pParase) {
    //        char *pEnd = strstr(pParase, "\r\n\r\n");
    //        pEnd = strstr(pEnd+strlen("\r\n\r\n"), "\r\n");
    //        pEnd[0] = '\0';
    //        if (pParase) {
    //            log_info("w: %s \r\n", pParase);
    //        }
    //
    //        std::string name;
    //        std::string val;
    //        xBlog::parase_form_data(pParase, name, val);
    //        log_info("%s: %s \r\n", name.c_str(), val.c_str());
    //        //if (name=="upfile"){
    //        //    std::string strData;
    //        //    strData.assign(pEnd + 2, atoi(val.c_str()));
    //        //    log_info("%s\r\n", strData.c_str());
    //        //    break;
    //        //}
    //        pParase = pEnd + 2;
    //    }
    //} else {
    //
    //}
    free(buf);
    string str = "{\"ret\":true,\"info\":{\"size\":\"1024\", \"md5\":\"edac35fd4b0059d3218f0630bc56a6f4\"}}";
    xBlog::SendHttpJsonResphone(req, HTTP_OK, str);
}

