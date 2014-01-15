/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#define XBLOG_VERSION "0.1"
 
#include "xBlog.h"

void version()
{
    log_info("xBlog version=%s evHttpd=%s:%d bits=%d\n",
        XBLOG_VERSION,
        event_get_version(),
        (int)event_get_version_number(),
        sizeof(long) == 4 ? 32 : 64);
}

int main(int argc, char **argv)
{
    Config::GetInstance()->Init("xblog.ini");

#ifdef _WIN32

#else
    LOGGER.setFileName(Config::GetInstance()->xBlogAppConfig.LogFileName.c_str());
    LOGGER.setLogLevel(Config::GetInstance()->xBlogAppConfig.LogLevel.c_str());
    LOGGER.setMaxFileSize(1 << 23);
#endif
    version();
    log_info("Listen on: %s %u\n", Config::GetInstance()->xBlogAppConfig.ServerIp.c_str(), 
                                   Config::GetInstance()->xBlogAppConfig.Port);
                                   
    log_info("MYSQL: %s:%u %s/%s dbname:%s poolsize:%d \n", 
                Config::GetInstance()->xBlogMysqlcfg.ipaddr.c_str(),
                Config::GetInstance()->xBlogMysqlcfg.port,
                Config::GetInstance()->xBlogMysqlcfg.username.c_str(),
                Config::GetInstance()->xBlogMysqlcfg.passwd.c_str(),
                Config::GetInstance()->xBlogMysqlcfg.dbname.c_str(),
                Config::GetInstance()->xBlogMysqlcfg.poolsize );
                
    log_info("Shell:%s\n",         Config::GetInstance()->xBlogAppConfig.Shell.c_str());
    log_info("RootDir:%s\n",       Config::GetInstance()->xBlogAppConfig.RootDir.c_str());
    log_info("LogFileName:%s\n",   Config::GetInstance()->xBlogAppConfig.LogFileName.c_str());
    log_info("LogLevel:%s\n",      Config::GetInstance()->xBlogAppConfig.LogLevel.c_str());
    log_info("Httpdthreads:%d \n", Config::GetInstance()->xBlogAppConfig.Httpdthreads);
    log_info("HttpdTimeOut:%d \n", Config::GetInstance()->xBlogAppConfig.HttpdTimeOut);
    log_info("CacheTimer:%d \n",   Config::GetInstance()->xBlogAppConfig.CacheTimer);

    xBlog xblog;

    xblog.StartMysqlPool();
    xblog.Init();

    bool bRet = xblog.Run(Config::GetInstance()->xBlogAppConfig.ServerIp.c_str(),
              Config::GetInstance()->xBlogAppConfig.Port,
              Config::GetInstance()->xBlogAppConfig.HttpdTimeOut, 
              Config::GetInstance()->xBlogAppConfig.Httpdthreads);
    if(!bRet)
    {
        log_info("xblog.Run error \n");
        exit(1);
    }
    
    while (true)
    {
        xblog.OnTimer();
        SLEEP(1);
        xBlogData::GetInstance()->pMysqlPool->Keepalive();
    }

    return 0;
}
