/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "xBlogBase.h"
#include "xUntil.h"
#include "xIniFile.h"
#include <string>
using namespace std;

typedef struct _MYSQLCONFIG_
{
    string ipaddr;
    uint16 port;
    string username;
    string passwd;
    string dbname;
    uint16 poolsize;
} MYSQLCONFIG, *PMYSQLCONFIG;

typedef struct _XBLOG_APP_CONFIG_
{
	string ServerIp;
	uint16 Port;
	uint16 Httpdthreads;
	string LogFileName;
	string LogLevel;
	uint32 HttpdTimeOut;
    uint32 CacheTimer;
	string RootDir;
	string Shell;
} BLOGAPPCONFIG, *PBLOGAPPCONFIG;

typedef map < string, string >    BLOGSITECONFIG;

class Config
{
public:
    Config();
    virtual ~Config();

    static Config *GetInstance();
    static Config *mConfig;

    bool Init(string inifile);
	bool LoadxBlogDBConfig(CIniFile &iniFile);
	bool LoadxBlogAppConfig(CIniFile &iniFile);
	bool LoadxBlogSiteConfig();

public:
    MYSQLCONFIG *  pMysqlcfg;
	BLOGAPPCONFIG  xBlogAppConfig;
	BLOGSITECONFIG xBlogSiteConfigMap;
    string strRootFullPath;
};


#endif
