/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "xIniFile.h"

typedef struct _MYSQLCONFIG_
{
    string ipaddr;
    int    port;
    string username;
    string passwd;
    string dbname;
    int    poolsize;
} MYSQLCONFIG, *PMYSQLCONFIG;

typedef struct _XBLOG_APP_CONFIG_
{
	string ServerIp;
    int    Port;
    int    Httpdthreads;
	string LogFileName;
	string LogLevel;
    int    HttpdTimeOut;
    int    CacheTimer;
	string RootDir;
	string Shell;
} BLOGAPPCONFIG, *PBLOGAPPCONFIG;

class Config
{
public:
    Config(){}
    virtual ~Config(){}

    static Config *GetInstance();
    static Config *mConfig;

    bool Init(string inifile);
	bool LoadxBlogDBConfig(CIniFile &iniFile);
	bool LoadxBlogAppConfig(CIniFile &iniFile);
	bool LoadxBlogSiteConfig();

public:
    MYSQLCONFIG    xBlogMysqlcfg;
	BLOGAPPCONFIG  xBlogAppConfig;
    string         strRootFullPath;
};


#endif
