/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _BLOG_CACHE_H_
#define _BLOG_CACHE_H_

#include "xMysqlPool.h"
#include "xLog.h"
#include "xUntil.h"
#include <string>
#include <map>
using namespace std;

struct xCacheData
{
    xCacheData()
    {
        count = 0;
    }
    string strData;
    uint32 count;
};

typedef map < uint32, xCacheData > CACHEDATAMAP;
typedef map < uint32, xCacheData >::iterator CACHEDATAMAPITER;

class xBlogCache
{
  public:
    xBlogCache()
    {
        timestamp = time(NULL);
    };

    ~xBlogCache()
    {
    };

    static xBlogCache *GetInstance();

    string &GetPage(const uint32 pageid);
    void SetPage(const uint32 pageid, const string & data);
    void delPage(const uint32 pageid);
    
    string &GetPost(const uint32 postid);
    void SetPost(const uint32 postid, const string & data);
    void delPost(const uint32 pageid);
    
    void OnTimer();
    void CacheClear();

  private:
    CACHEDATAMAP mPageCacheDataMap;
    xLock mPageCacheLock;

    CACHEDATAMAP mPostCacheDataMap;
    xLock mPostCacheLock;
    
    uint32 timestamp;

};

#endif
