/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include <stdlib.h>
#include "xBlogData.h"
#include "xUntil.h"
#include "xBlogBase.h"
#include "xBlogPageTemplate.h"
#include <sstream>
using namespace std;

xBlogData *xBlogData::GetInstance()
{
    static xBlogData gData;
	return &gData;
}

bool xBlogData::GetSiteConfig(BLOGSITECONFIG &configmap)
{
    stringstream strSQL;
    strSQL << "SELECT Fid, Fkey, Fvalue FROM  xb_siteconfig LIMIT 100;";
    MYSQL_RES *pRes = pMysqlPool->Select(strSQL.str().c_str());

    log_debug("%s \r\n", strSQL.str().c_str());

    if (NULL!=pRes)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(pRes)))
        {
            configmap[row[1]] = row[2];
            log_debug("%s:%s \r\n", row[1], row[2]);
        }
        pMysqlPool->FreeRecord(pRes);
        return true;
    }
        
    log_error("error \r\n");
    return false;
}

string xBlogData::GetArticleList(string & strHtml, const uint32 page)
{
    uint32 start = 0;
    uint32 num = atoul(Config::GetInstance()->xBlogSiteConfigMap["XBLOG_MAX_VIEW"].c_str());

    if (0 == num)
        num = 1;
    char szSQL[SIZE_1024] = { 0 };

    string strData;
    start = (0==page)?0:(page * num);

    snprintf(szSQL,sizeof(szSQL),
            "SELECT xb_posts.ID,xb_users.user_nicename, xb_posts.post_date, xb_posts.post_content, xb_posts.post_brief,\
                   xb_posts.post_title, IFNULL(xb_classify.classify_name,'未知'), xb_posts.post_modified, xb_posts.post_url, COUNT(xb_comments.comment_post_ID)\
                   FROM `xb_posts` \
                   left join xb_users on xb_users.id = xb_posts.post_author \
                   left join xb_classify on xb_posts.post_classify = xb_classify.classify_id  \
                   LEFT JOIN xb_comments ON xb_comments.comment_post_ID = xb_posts.ID \
                   where post_status=\'publish\' group by ID order by post_date DESC limit %d,%d ",
            start, num);

    MYSQL_RES *pRes = pMysqlPool->Select(szSQL);

    if (NULL!=pRes)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(pRes)))
        {
            string strTemp = strHtml;

            StringReplace(strTemp, "<#article/category/id#>", "3");
            StringReplace(strTemp, "<#article/title#>", row[5]);
            StringReplace(strTemp, "<#article/posttime/longdate#>", row[2]);
            StringReplace(strTemp, "<#article/intro#>", row[4]);
            StringReplace(strTemp, "<#article/url#>", row[0]);
            StringReplace(strTemp, "<#article/author/name#>", row[1]);
            StringReplace(strTemp, "<#article/category/name#>", row[6]);
            StringReplace(strTemp, "<#article/commnums#>", row[9]);

            strData += strTemp;
        }
        pMysqlPool->FreeRecord(pRes);
    }

    strHtml = strData;
    return strData;
}

bool xBlogData::GetCatalogPage(string & strHtml, const uint32 catalog, const uint32 page)
{
    int start = 0;
    char szSQL[SIZE_1024] = { 0 };
    string strTmpl = strHtml;
    strHtml = "";

    int num = atoul(Config::GetInstance()->xBlogSiteConfigMap["XBLOG_MAX_VIEW"].c_str());
    if (0 == num)
    {
        num = 1;
    }
    start = (page==0)?0:(page * num);

    snprintf(szSQL, sizeof(szSQL),
            "SELECT xb_posts.ID,xb_users.user_nicename, xb_posts.post_date, xb_posts.post_content, xb_posts.post_brief,\
                   xb_posts.post_title, IFNULL(xb_classify.classify_name,'未知'), xb_posts.post_modified, xb_posts.post_url, COUNT(xb_comments.comment_post_ID)\
                   FROM `xb_posts` \
                   left join xb_users on xb_users.id = xb_posts.post_author \
                   left join xb_classify on xb_posts.post_classify = xb_classify.classify_id  \
                   LEFT JOIN xb_comments ON xb_comments.comment_post_ID = xb_posts.ID \
                   where post_status=\'publish\' AND xb_classify.classify_id=%d group by ID order by post_date DESC limit %d,%d ",
            catalog, start, num);

    MYSQL_RES *pRes = pMysqlPool->Select(szSQL);

    if (NULL!=pRes)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(pRes)))
        {
            string strTemp = strTmpl;
            char szCataID[SIZE_32] = { 0 };
            snprintf(szCataID, sizeof(szCataID), "%d", catalog);
            StringReplace(strTemp, "<#article/category/id#>", szCataID);
            StringReplace(strTemp, "<#article/title#>", row[5]);
            StringReplace(strTemp, "<#article/posttime/longdate#>", row[2]);
            StringReplace(strTemp, "<#article/intro#>", row[4]);
            StringReplace(strTemp, "<#article/url#>", row[0]);
            StringReplace(strTemp, "<#article/author/name#>", row[1]);
            StringReplace(strTemp, "<#article/category/name#>", row[6]);
            StringReplace(strTemp, "<#article/commnums#>", row[9]);

            strHtml += strTemp;
        }
        pMysqlPool->FreeRecord(pRes);
    }
    else
    {
        return false;
    }
    

    return true;
}

string xBlogData::GetArticleCommentList(string & strHtml, const uint32 postid)
{
    int num = atoul(Config::GetInstance()->xBlogSiteConfigMap["XBLOG_MAX_VIEW"].c_str());
    char szSQL[SIZE_1024] = { 0 };
    string strData;

    snprintf(szSQL, sizeof(szSQL), "SELECT comment_ID, comment_author, comment_author_email, comment_date, comment_content \
            FROM xb_comments WHERE comment_post_ID = %d ORDER BY comment_date DESC LIMIT %d;",
            postid, num);

    MYSQL_RES *pRes = pMysqlPool->Select(szSQL);

    if (NULL!=pRes)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(pRes)))
        {
            string  strTemp = strHtml;
            StringReplace(strTemp, "<#article/comment/id#>", row[0]);
            StringReplace(strTemp, "<#article/comment/name#>", row[1]);
            StringReplace(strTemp, "<#article/comment/emailmd5#>", row[2]);
            StringReplace(strTemp, "<#article/comment/posttime#>", row[3]);
            StringReplace(strTemp, "<#article/comment/content#>", row[4]);

            strData += strTemp;
        }
        pMysqlPool->FreeRecord(pRes);
    }

    strHtml = strData;
    return strData;
}

string xBlogData::GetCacheIncludeCatalog(const string & strHtml)
{
    const char *szSQL =    "select xb_posts.post_classify,IFNULL(xb_classify.classify_name,'未知'), count(xb_posts.post_classify) \
                 from xb_posts \
                 left join xb_classify on xb_classify.classify_id = xb_posts.post_classify \
                 where post_status=\'publish\'\
                 group by  post_classify order by xb_classify.classify_id;";

    string strTemp = strHtml;
    string strData;

    MYSQL_RES *pRes = pMysqlPool->Select(szSQL);

    if (NULL!=pRes)
    {
        MYSQL_ROW    row;
        while ((row = mysql_fetch_row(pRes)))
        {
            strTemp = strHtml;
            StringReplace(strTemp, "<#article/category/id#>", row[0]);
            StringReplace(strTemp, "<#article/category/name#>", row[1]);
            StringReplace(strTemp, "<#article/nums#>", row[2]);
            strData += strTemp;
        }
        pMysqlPool->FreeRecord(pRes);
    }

    return strData;
}

string xBlogData::GetCacheIncludeLink(const string & strHtml)
{
    const char *szSQL = "SELECT link_url, IFNULL(link_name, ' ') FROM xb_links  where link_visible='Y'  ORDER BY link_id ASC ; ";

    string strTemp = strHtml;
    string strData;

    MYSQL_RES *pRes = pMysqlPool->Select(szSQL);
    if (NULL!=pRes)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(pRes)))
        {
            strTemp = strHtml;
            StringReplace(strTemp, "<#article/link/url#>", row[0]);
            StringReplace(strTemp, "<#article/link/name#>", row[1]);
            strData += strTemp;
        }
        pMysqlPool->FreeRecord(pRes);
    }

    return strData;
}

string xBlogData::GetArticle(string & strHtml, const uint32 id)
{
    char szSQL[SIZE_1024] = { 0 };
    snprintf(szSQL, sizeof(szSQL),"SELECT post.post_author, post.post_date, post.post_content, post.post_title, \
                   post.post_classify, post_modified, post_url, IFNULL(classify.classify_name, 'NULL'), USERS.user_nicename, COUNT(comments.comment_post_ID)\
                   FROM `xb_posts`  as post \
                   LEFT JOIN xb_users AS USERS ON post.post_author = USERS.ID \
                   left join xb_classify as classify on post.post_classify = classify.classify_id \
                   LEFT JOIN xb_comments as comments ON post.ID = comments.comment_post_ID \
                   where post_status='publish' and post.id =%d GROUP BY post.ID;", id);

    MYSQL_RES *pRes = pMysqlPool->Select(szSQL);

    if (NULL!=pRes)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(pRes)))
        {
            StringReplace(strHtml, "<#article/category/id#>", row[4]);
            StringReplace(strHtml, "<#article/title#>", row[3]);
            StringReplace(strHtml, "<#article/posttime#>", row[1]);
            StringReplace(strHtml, "<#article/author/name#>", row[8]);
            StringReplace(strHtml, "<#article/commnums#>", row[9]);
            StringReplace(strHtml, "<#article/content#>", row[2]);
            StringReplace(strHtml, "<#article/category/name#>", row[7]);
        }

        pMysqlPool->FreeRecord(pRes);

        char tmp[SIZE_32] = { 0 };
        snprintf(tmp, sizeof(tmp), "%d", id + 1);
        StringReplace(strHtml, "<#template:article_navbar_l#>", tmp);
        snprintf(tmp, sizeof(tmp), "%d", id - 1);
        StringReplace(strHtml, "<#template:article_navbar_r#>", tmp);
    }

    return strHtml;
}

bool xBlogData::Admin_GetPostList(const uint32 pageid, string &strData)
{
    const uint32 MaxView = atoul(Config::GetInstance()->xBlogSiteConfigMap["XBLOG_MAX_VIEW"].c_str());
    uint32 start = pageid * MaxView;

    char szSQL[SIZE_1024] = { 0 };
    snprintf(szSQL, sizeof(szSQL), "select xb_posts.ID, IFNULL(xb_classify.classify_name,'未知'), xb_users.user_nicename, \
                   xb_posts.post_date,  xb_posts.post_title, xb_posts.post_status \
                   from xb_posts \
                   left join xb_classify on xb_classify.classify_id = xb_posts.post_classify \
                   left join xb_users on xb_users.id = xb_posts.post_author \
                   order by post_status DESC, post_date DESC limit %d, %d", start,
                   MaxView);

    return pMysqlPool->SelectJson(szSQL, strData);
}


bool xBlogData::AdminGetSiteConfig_View(string & strData)
{
    return pMysqlPool->SelectJson("SELECT * FROM  xb_siteconfig LIMIT 100;", strData);
}

bool xBlogData::AdminLinks_View(string & strData)
{
    const char *pSQL = "SELECT link_id, link_url, link_name  FROM xb_links where link_visible='Y'  LIMIT 100";
    return pMysqlPool->SelectJson(pSQL, strData);
}

bool xBlogData::AdminUser_View(string & strData)
{
    const char *pSQL = "select ID,user_login, user_pass, user_nicename, user_email from xb_users limit 1;";
    return pMysqlPool->SelectJson(pSQL, strData);
}

bool xBlogData::AdminCatalog_View(string & strData)
{
    const char *pSQL = "SELECT ID, classify_id, classify_name, classify_status FROM xb_classify LIMIT 32";
    return pMysqlPool->SelectJson(pSQL, strData);
}

bool xBlogData::AdminCatalog_GetSelect(string & strData)
{
    const char *pSQL = "SELECT classify_id, classify_name FROM xb_classify where classify_status=0 LIMIT 32";
    return pMysqlPool->SelectJson(pSQL, strData);
}

bool xBlogData::AdminComments_View(string & strData)
{
    const char *pSQL = "SELECT PT.ID,PT.post_title,CM.comment_ID,CM.comment_author,\
        CM.comment_author_email,CM.comment_date,CM.comment_content \
        FROM xb_comments AS CM LEFT JOIN xb_posts as PT \
        ON CM.comment_post_ID=PT.ID ORDER BY CM.comment_date DESC;";
    return pMysqlPool->SelectJson( pSQL, strData);
}

bool xBlogData::AdminPostManager_UpdatePostStatus(const char *szId, const char *szStatus)
{
    stringstream ssSQL;
    ssSQL << "update  xb_posts  set post_status=\'" << szStatus
          << "\' where ID=\'" << szId << "\';";

    return pMysqlPool->Execute(ssSQL.str().c_str());
}

bool xBlogData::AdminGetSiteConfig_Update(const char *szId, const char *szValue)
{
    stringstream ssSQL;
    ssSQL << "update xb_siteconfig set Fvalue=\'"<< szValue
          << "\' where Fid=\'"<< szId << "\';";

    if (pMysqlPool->Execute(ssSQL.str().c_str()))
    {
        Config::GetInstance()->LoadxBlogSiteConfig();
        return true;
    }
    return false;
}

bool xBlogData::AdminUser_Update(const char *szId, const char *szUserLogin, const char *szNickName, const char* szEmail)
{
    char szBuf[SIZE_1024] = {0};

    uint32 nTotal = strlen(szId)+strlen(szNickName)+strlen(szUserLogin)+strlen(szEmail);
    if (nTotal> (sizeof(szBuf)/2))
    {
        return false;
    }

    snprintf(szBuf, sizeof(szBuf), "update xb_users set user_login=\'%s\', user_nicename=\'%s\', user_email=\'%s\' where ID=%s;", 
        szUserLogin, szNickName, szEmail, szId);

    if (pMysqlPool->Execute(szBuf))
    {
        return true;
    }
    return false;
}

bool xBlogData::AdminLinks_Update(const char *szId, const char *szUrl, const char *szName)
{
    stringstream ssSQL;
    ssSQL << "update xb_links set link_url=\'" << szUrl << "\',"
    <<" link_name=\'"<< szName <<"\'"
    <<" where link_id=\'"<< szId << "\';";

    if (pMysqlPool->Execute(ssSQL.str().c_str()))
    {
        return true;
    }
    return false;
}

bool xBlogData::AdminCatalog_Update(const char *szId,const char *szClassifyId, const char *szName, const char *szSatus)
{
    string strSQL = "update xb_classify set ";
    strSQL += " classify_id="; strSQL += szClassifyId;
    strSQL += ", classify_name=\'"; strSQL += szName;
    strSQL += "\', classify_status="; strSQL += szSatus;
    strSQL += " where ID=\'";  strSQL += szId;
    strSQL += "\';";

    if (pMysqlPool->Execute(strSQL.c_str()))
    {
        return true;
    }
    return false;
}

bool xBlogData::AdminCatalog_Add(const char *szClassifyId, const char *szName, const char *szSatus)
{
    string strSQL = "insert into xb_classify set ";
    strSQL += " classify_id="; strSQL += szClassifyId;
    strSQL += ", classify_name=\'"; strSQL += szName;
    strSQL += "\', classify_status="; strSQL += szSatus;
    strSQL += ";";

    if (pMysqlPool->Execute(strSQL.c_str()))
    {
        return true;
    }
    return false;
}

bool xBlogData::AdminComments_Delete(const char *szId)
{
    string strSQL = "DELETE FROM xb_comments ";
    strSQL += " where comment_ID=";
    strSQL += szId;

    if (pMysqlPool->Execute(strSQL.c_str()))
    {
        return true;
    }
    return false;
}

bool xBlogData::AdminPostManager_NewPost(string title, string classify, string content, string brief)
{
    string strSQL = "INSERT INTO xb_posts(post_author, post_date, post_content, post_brief, post_title, post_status, post_classify, comment_status) VALUES (";
    strSQL += "1,now(),'";
    strSQL += content + "','";
    strSQL += brief + "','";
    strSQL += title + "','";
    strSQL += "publish',";
    strSQL += classify + ",";
    strSQL += "'open')";

    return pMysqlPool->Execute(strSQL.c_str());
}

string xBlogData::GetClassify(const char *szId)
{
    string strData = "<select name=\"classify\">";
    const char *pSQL = "SELECT classify_id, classify_name FROM xb_classify WHERE classify_status=0 ORDER BY classify_id ASC;";
    MYSQL_RES *pRes =pMysqlPool->Select(pSQL);
    if (NULL!=pRes)
    {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(pRes)))
        {
            if (0 == stricmp(szId, row[0]))
            {
                strData += "<option value=\"";
                strData += row[0];
                strData += "\" selected=\"selected\">";
                strData += row[1];
                strData += "</option>";
            }
            strData += "<option value=\"";
            strData += row[0];
            strData += "\">";
            strData += row[1];
            strData += "</option>";
        }
        pMysqlPool->FreeRecord(pRes);
    }
    strData += "</select>";
    return strData;
}

string xBlogData::Admin_GetPostEdit(const char *szId)
{
    string strData;
    string strClassify;
    char   szSQL[SIZE_256] = { 0 };
    string strHtml = xBlogPageTemplate::GetInstance()->GetPageTemplate(K_POSTEDIT);

    snprintf(szSQL, sizeof(szSQL), "SELECT post_title, post_content,post_brief, post_classify FROM xb_posts WHERE ID = \'%s\';",szId);

    MYSQL_RES *pRes = pMysqlPool->Select(szSQL);
    if (NULL!=pRes)
    {
        MYSQL_ROW row = mysql_fetch_row(pRes);

        strClassify = GetClassify(row[3]);

        StringReplace(strHtml, "<#POST_CLASSIFY#>", strClassify);
        StringReplace(strHtml, "<#POST_ID#>", szId);
        StringReplace(strHtml, "<#POST_TITLE#>", row[0]);
        StringReplace(strHtml, "<#POST_CONTENT#>", row[1]);
        StringReplace(strHtml, "<#POST_BRIEF#>", row[2]);

        pMysqlPool->FreeRecord(pRes);
    }

    return strHtml;
}

bool xBlogData::AdminPostManager_UpdatePost(const string & postid, const string & title, const string & classify,
                            const string & content, const string & brief)
{
    string strSQL = "update xb_posts set ";

    strSQL += " post_title=\'";    strSQL += title + "\',";
    strSQL += " post_classify=\'"; strSQL += classify + "\',";
    strSQL += " post_content=\'";  strSQL += content + "\',";
    strSQL += " post_brief=\'";    strSQL += brief + "\'";
    strSQL += " where ID=\'";      strSQL += postid + "\';";

    if (pMysqlPool->Execute(strSQL.c_str()))
    {
        return true;
    }
    return false;
}

bool xBlogData::NewComment(const string & postID, const string & author, const string & email,
                            const string & url, const string & IP, const string & content)
{
    string strSQL =    "INSERT INTO xb_comments(comment_post_ID, comment_author, comment_author_email, \
                     comment_author_url, comment_author_IP, comment_date, comment_date_gmt,\
                     comment_content) VALUES ('";
    strSQL += postID + "','";
    strSQL += author + "','";
    strSQL += email + "','";
    strSQL += url + "','";
    strSQL += IP + "',";
    strSQL += "now() ,";
    strSQL += "now() ,'";
    strSQL += content;
    strSQL += "')";

    return pMysqlPool->Execute(strSQL.c_str());
}

