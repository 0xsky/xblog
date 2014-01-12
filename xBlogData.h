/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _BLOG_DATA_H_
#define _BLOG_DATA_H_

#include <string>
#include <map>
#include "xMysqlPool.h"
#include "xLog.h"
#include "xConfig.h"
using namespace std;

class xBlogData
{
  public:
    xBlogData()
    {
    };

    ~xBlogData()
    {
    };

    static xBlogData *GetInstance();

    bool GetSiteConfig(BLOGSITECONFIG &configmap);

    bool NewComment(const string & postID, const string & author, const string & email,
                     const string & url, const string & IP, const string & content);

    string GetClassify(const char *szId);

    string GetArticleList(string & strHtml, const uint32 page);
    string GetArticle(string & strHtml, const uint32 id);
    string GetArticleCommentList(string & strHtml, const uint32 postid);

    string GetCacheIncludeCatalog(const string & strHtml);
    string GetCacheIncludeLink(const string & strHtml);

    bool GetCatalogPage(string & strHtml, const uint32 catalog, const uint32 page);

    bool AdminGetSiteConfig_View(string & strData);
    bool AdminGetSiteConfig_Update(const char *szId, const char *szValue);

    bool   Admin_GetPostList(const uint32 id, string &strData);
    string Admin_GetPostEdit(const char *szId);

    bool AdminPostManager_NewPost(string title, string classify, string content, string brief);
    bool AdminPostManager_UpdatePostStatus(const char *szId, const char *szStatus);
    bool AdminPostManager_UpdatePost(const string & postid, const string & title, const string & classify, 
        const string & content, const string & brief);
    
    bool AdminLinks_View(string & strData);
    bool AdminLinks_Update(const char *szId, const char *szUrl, const char *Name);

    bool AdminUser_View(string & strData);
    bool AdminUser_Update(const char *szId, const char *szUserLogin, const char *szNickName, const char* szEmail);

    bool AdminCatalog_View(string & strData);
    bool AdminCatalog_GetSelect(string & strData);
    bool AdminCatalog_Update(const char *szId,const char *szClassifyId, const char *szName, const char *szSatus);

    bool AdminCatalog_Add(const char *szClassifyId, const char *szName, const char *szSatus);
    
    bool AdminComments_View(string & strData);
    bool AdminComments_Delete(const char *szId);

    MysqlPool *pMysqlPool;
};




#endif
