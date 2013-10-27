/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 

#ifndef _xBlogPage_H_
#define _xBlogPage_H_

#include "xUntil.h"
#include "xBlogPage.h"
#include "xLog.h"
#include "xBlogPageTemplate.h"

class xBlog;
class xBlogPage
{
  public:
    xBlogPage()
    {
    };

    ~xBlogPage()
    {
    };

  public:
    static void AdminCallback(struct evhttp_request *req, void *arg);
    static void AdminShellCallback(struct evhttp_request *req, void *arg);
    static void AdminStatusCallback(struct evhttp_request *req, void *arg);
    static void AdminCheckLoginCallback(struct evhttp_request *req, void *arg);
    static void AdminSiteConfigCallback(struct evhttp_request *req, void *arg);
    static void AdminLinksCallback(struct evhttp_request *req, void *arg);
    static void AdminCatalogCallback(struct evhttp_request *req, void *arg);
    static void AdminSystemCallback(struct evhttp_request *req, void *arg);
    static void AdminUserCallback(struct evhttp_request *req, void *arg);
    static void AdminCommentsCallback(struct evhttp_request *req, void *arg);
    static void AdminPostManagerCallback(struct evhttp_request *req, void *arg);
    
    static void IndexRequestCallback(struct evhttp_request *req, void *arg);
    static void PageRequestCallback(struct evhttp_request *req, void *arg);
    static void CatalogRequestCallback(struct evhttp_request *req, void *arg);
    static void PostRequestCallback(struct evhttp_request *req, void *arg);
    static void RequestPage(struct evhttp_request *req, const char *PageId);
    
    static void SendDocumentCallback(struct evhttp_request *req, void *arg);
    static void GuestPostCallback(struct evhttp_request *req, void *arg);
    static void GuestbookCallback(struct evhttp_request *req, void *arg);
    static void DownloadCallback(struct evhttp_request *req, void *arg);
    static bool CheckSession(struct evhttp_request *req, xBlog * pBlog);
    
    static void SendFile(struct evhttp_request *req, const char *rootdir, const char *filepath);
    static void SendLoginPage(struct evhttp_request *req, xBlog *pBlog);
    static void SendAdminPage(struct evhttp_request *req, xBlog *pBlog);
    static void SendErrorPage(struct evhttp_request *req, xBlog *pBlog, uint32 errorcode);

};

#endif
