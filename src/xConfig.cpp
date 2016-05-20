/*
* ----------------------------------------------------------------------------
* Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
* All rights reserved.
* Distributed under GPL license.
* ----------------------------------------------------------------------------
*/


#include "xConfig.h"
#include "xLog.h"
#include "xUntil.h"
#include "MultipartParase.h"

#ifdef WIN32
#include <windows.h>
#endif


Config *Config::mConfig = NULL;
Config *Config::GetInstance()
{
    if (NULL == mConfig) {
        mConfig = new Config;
    }
    return mConfig;
}

bool Config::Init(string IniFile)
{
    CIniFile iniFile(IniFile);
	if (iniFile.ReadFile())
	{
		LoadxBlogDBConfig(iniFile);
		LoadxBlogAppConfig(iniFile);
        
    char szTemp[1024] = { 0 };
    char szDir[1024]  = { 0 };
    
#ifdef WIN32
    GetCurrentDirectory(sizeof(szTemp), szTemp);
#else
    if (NULL == getcwd(szTemp, sizeof(szTemp)))
    {
        log_error("get dir error \r\n");
        return false;
    }
    //log_info("CurrentDirectory: %s \r\n", szTemp);
#endif
        
    snprintf(szDir, sizeof(szDir), "%s/%s", szTemp, Config::GetInstance()->xBlogAppConfig.RootDir.c_str());
    //log_info("xblog root dir: %s \r\n", szDir);
    strRootFullPath = szDir;
    
		return true;
	}

	return false;
}

bool Config::LoadxBlogDBConfig(CIniFile &iniFile)
{
	xBlogMysqlcfg.ipaddr   = iniFile.GetValue("MYSQLPOOL", "IPADDR");
	xBlogMysqlcfg.port     = iniFile.GetValueI("MYSQLPOOL", "PORT");
	xBlogMysqlcfg.username = iniFile.GetValue("MYSQLPOOL", "USER");
	xBlogMysqlcfg.passwd   = iniFile.GetValue("MYSQLPOOL", "PASSWD");
	xBlogMysqlcfg.dbname   = iniFile.GetValue("MYSQLPOOL", "DBNAME");
	xBlogMysqlcfg.poolsize = iniFile.GetValueI("MYSQLPOOL", "POOLSIZE");

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
	return true;
}
