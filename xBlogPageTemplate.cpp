/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include "xBlogPageTemplate.h"

xBlogPageTemplate *xBlogPageTemplate::gXBlogPageTemplate = NULL;
xBlogPageTemplate *xBlogPageTemplate::GetInstance()
{
	if (NULL == gXBlogPageTemplate)
	{
		gXBlogPageTemplate = new xBlogPageTemplate;
	}
	return gXBlogPageTemplate;
}

XTEMPLATE & xBlogPageTemplate::GetFile(string filedir, string filename, XTEMPLATE & xtemplate, TEMPLATECB xCall)
{
	char szBuf[MAX_FILE_SIZE] = { 0 };
	string strPath = filedir + filename;

	xtemplate.templateCB = xCall;
	xtemplate.value = "";

	if (!strPath.length())
	{
		log_error("getfile error strPath is null \r\n");
		return xtemplate;
	}

	FILE * fp = fopen(strPath.c_str(), "rb");

	if (NULL==fp)
	{
		log_error("getfile error path=%s \r\n", strPath.c_str());
		return xtemplate;
	}
	int len = fread(szBuf, 1, MAX_FILE_SIZE, fp);

	szBuf[len] = '\0';
	xtemplate.value = szBuf;

	fclose(fp);
	return xtemplate;
}

bool xBlogPageTemplate::BuildHtmlByTemplateKey(const char *pKey, XTEMPLATE & xTemplate)
{
	xTemplate = PageTemplateMap[pKey];
	if (xTemplate.value.length()>0)
	{
		ReplacePageTemplate(xTemplate.value);
		ReplacePageCache(xTemplate.value);
		ReplaceSiteConfig(xTemplate.value);
		return true;
	}
	else
	{
		log_error("Build html error key=%p \n", pKey);
	}

	return false;
}

void xBlogPageTemplate::ReplacePageCache(string & strHtml)
{
	string strKey;

	map < string, XTEMPLATE >::iterator iter;
	for (iter = PageCacheMap.begin(); iter != PageCacheMap.end(); ++iter)
	{
		strKey += "<#";
		strKey += iter->first;
		strKey += "#>";
		StringReplace(strHtml, strKey, iter->second.value);
		strKey = "";
	}
}

void xBlogPageTemplate::ReplacePageTemplate(string & html)
{
	string strKey;

	map < string, XTEMPLATE >::iterator iter;
	for (iter = PageTemplateMap.begin(); iter != PageTemplateMap.end(); ++iter)
	{
		strKey += "<#template:";
		strKey += iter->first;
		strKey += "#>";

		if (iter->second.templateCB)
		{
			string strValue = (iter->second).value;
			if (strValue.length()>0)
			{
				(this->*iter->second.templateCB) (strValue);
			}
			StringReplace(html, strKey, strValue);
		}
		else
		{
			//log_info("%s \r\n", strKey.c_str());
			StringReplace(html, strKey, iter->second.value);
		}

		strKey = "";
	}
}

void xBlogPageTemplate::ReplaceSiteConfig(string & strHtml)
{
	string strKey;

	BLOGSITECONFIG::iterator iter;
	for (iter = Config::GetInstance()->xBlogSiteConfigMap.begin(); 
		iter != Config::GetInstance()->xBlogSiteConfigMap.end(); ++iter)
	{
		strKey += "<#";
		strKey += iter->first;
		strKey += "#>";
		StringReplace(strHtml, strKey, iter->second);
		strKey = "";
	}
}

void xBlogPageTemplate::Init()
{
	LoadTemplatePage();
	LoadCachePage();
	LoadCachePageData();
}

void xBlogPageTemplate::LoadTemplatePage()
{
	XTEMPLATE xTemp;
	string strDir = Config::GetInstance()->strRootFullPath + "/themes/" + Config::GetInstance()->xBlogSiteConfigMap["XBLOG_THEME"] + "/page/";
	string strAdminDir = Config::GetInstance()->strRootFullPath + "/admin/";

	PageTemplateMap[K_DEFAULT]               = GetFile(strDir, V_DEFAULT, xTemp);
	PageTemplateMap[K_SINGLE]                = GetFile(strDir, V_SINGLE, xTemp);
	PageTemplateMap[K_GUESTBOOK]             = GetFile(strDir, V_GUESTBOOK, xTemp);
	PageTemplateMap[K_CATALOG]               = GetFile(strDir, V_CATALOG, xTemp);
	PageTemplateMap[K_ARTICLE_NVABAR]        = GetFile(strDir, V_ARTICLE_NVABAR, xTemp);
	PageTemplateMap[K_ARTICLE_SINGLE]        = GetFile(strDir, V_ARTICLE_SINGLE, xTemp);
	PageTemplateMap[K_GUESTBOOK]             = GetFile(strDir, V_GUESTBOOK, xTemp);
	PageTemplateMap[K_ARTICLE_COMMENTPOST]   = GetFile(strDir, V_ARTICLE_COMMENTPOST, xTemp);
	PageTemplateMap[K_POSTEDIT]              = GetFile(strAdminDir, V_POSTEDIT, xTemp);

	ActiveTemplateMap[K_ARTICLE_MULTI]   = GetFile(strDir, V_ARTICLE_MULTI, xTemp);
	ActiveTemplateMap[K_ARTICLE_COMMENT] = GetFile(strDir, V_ARTICLE_COMMENT, xTemp);
}

void xBlogPageTemplate::LoadCachePage()
{
	XTEMPLATE strTemp;
	string strDir = Config::GetInstance()->strRootFullPath + "/themes/" + Config::GetInstance()->xBlogSiteConfigMap["XBLOG_THEME"] + "/cache/";

	PageCacheMap[K_NAVBAR]        = GetFile(strDir, V_NAVBAR, strTemp);
	PageCacheMap[K_CACHE_CATALOG] = GetFile(strDir, V_CATALOG, strTemp, &xBlogPageTemplate::GetCacheIncludeCatalog);
	PageCacheMap[K_MISC]          = GetFile(strDir, V_MISC, strTemp);
	PageCacheMap[K_ARCHIVES]      = GetFile(strDir, V_ARCHIVES, strTemp);
	PageCacheMap[K_LINKS]         = GetFile(strDir, V_LINKS, strTemp, &xBlogPageTemplate::GetCacheIncludeLink);
}

void xBlogPageTemplate::LoadCachePageData()
{
	map < string, XTEMPLATE >::iterator iter;
	for (iter = PageCacheMap.begin(); iter != PageCacheMap.end(); ++iter)
	{
		if (iter->second.templateCB)
		{
			string strValue = (iter->second).value;
			iter->second.value = (this->*iter->second.templateCB) (strValue);
		}
	}
}

string xBlogPageTemplate::GetCacheIncludeCatalog(string & strHtml)
{
    return xBlogData::GetInstance()->GetCacheIncludeCatalog(strHtml);
}

string xBlogPageTemplate::GetCacheIncludeLink(string & strHtml)
{
    return xBlogData::GetInstance()->GetCacheIncludeLink(strHtml);
}

