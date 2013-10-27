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
    static xBlogCache *gData;

    string &GetPage(const uint32 pageid);
    void SetPage(const uint32 pageid, const string & data);
    string &GetPost(const uint32 postid);
    void SetPost(const uint32 postid, const string & data);

    void OnTimer();

  private:
    CACHEDATAMAP mPageCacheDataMap;
    xLock mPageCacheLock;

    CACHEDATAMAP mPostCacheDataMap;
    xLock mPostCacheLock;
    
    uint32 timestamp;

};

#endif
