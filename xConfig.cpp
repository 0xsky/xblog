/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include <string.h>
#include <stdio.h>
#include "xConfig.h"
#include "xLog.h"
#include "xBlogData.h"

Config *Config::mConfig = NULL;
Config *Config::GetInstance()
{
    if (NULL == mConfig)
    {
        mConfig = new Config;
    }
    return mConfig;
}

Config::Config()
{
    pMysqlcfg = new MYSQLCONFIG;
}

Config::~Config()
{
    delete pMysqlcfg;
}

bool Config::Init(string IniFile)
{
    
    CIniFile iniFile(IniFile);
	if (iniFile.ReadFile())
	{
		LoadxBlogDBConfig(iniFile);
		LoadxBlogAppConfig(iniFile);
        
    char szTemp[SIZE_1024] = { 0 };
    char szDir[SIZE_1024];
    
#ifdef WIN32
    GetCurrentDirectory(sizeof(szTemp), szTemp);
#else
    if (NULL == getcwd(szTemp, SIZE_1024))
    {
        log_error("get dir error \r\n");
        return false;
    }
    log_info("CurrentDirectory: %s \r\n", szTemp);
#endif
        
    snprintf(szDir, sizeof(szDir), "%s/%s", szTemp, Config::GetInstance()->xBlogAppConfig.RootDir.c_str());
    log_info("xblog root dir: %s \r\n", szDir);
    strRootFullPath = szDir;
    
		return true;
	}

	return false;
}

bool Config::LoadxBlogDBConfig(CIniFile &iniFile)
{
	pMysqlcfg->ipaddr   = iniFile.GetValue("MYSQLPOOL", "IPADDR");
	pMysqlcfg->port     = iniFile.GetValueI("MYSQLPOOL", "PORT");
	pMysqlcfg->username = iniFile.GetValue("MYSQLPOOL", "USER");
	pMysqlcfg->passwd   = iniFile.GetValue("MYSQLPOOL", "PASSWD");
	pMysqlcfg->dbname   = iniFile.GetValue("MYSQLPOOL", "DBNAME");
	pMysqlcfg->poolsize = iniFile.GetValueI("MYSQLPOOL", "POOLSIZE");

	return true;
}

bool Config::LoadxBlogAppConfig(CIniFile &iniFile)
{
	xBlogAppConfig.RootDir      = iniFile.GetValue("XBLOG", "ROOTDIR");
	xBlogAppConfig.LogFileName  = iniFile.GetValue("XBLOG", "LOGFILENAME");
	xBlogAppConfig.LogLevel     = iniFile.GetValue("XBLOG", "LOGLEVEL");
	xBlogAppConfig.Shell        = iniFile.GetValue("XBLOG", "SHELL");
    xBlogAppConfig.CacheTimer   = iniFile.GetValueI("XBLOG", "CACHETIMER");
    
	xBlogAppConfig.ServerIp     = iniFile.GetValue("HTTPD", "IP");
	xBlogAppConfig.Port         = iniFile.GetValueI("HTTPD", "PORT");
	xBlogAppConfig.Httpdthreads = iniFile.GetValueI("HTTPD", "HTTPDTHREADS");
	xBlogAppConfig.HttpdTimeOut = iniFile.GetValueI("HTTPD", "HTTPDTIMEOUT");
    
	return true;
}

bool Config::LoadxBlogSiteConfig()
{
	xBlogData::GetInstance()->GetSiteConfig(xBlogSiteConfigMap);
	return true;
}
