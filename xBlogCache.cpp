/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include <stdlib.h>
#include "xBlogCache.h"
#include "xBlogBase.h"
#include "xConfig.h"

xBlogCache *xBlogCache::GetInstance()
{
    static xBlogCache gData;
    return &gData;
}

string &xBlogCache::GetPage(const uint32 pageid)
{
    XLOCK(mPageCacheLock);
    mPageCacheDataMap[pageid].count++;
    return mPageCacheDataMap[pageid].strData;
}

void xBlogCache::SetPage(const uint32 pageid, const string & data)
{
    XLOCK(mPageCacheLock);
    mPageCacheDataMap[pageid].strData = data;
}

void xBlogCache::delPage(const uint32 pageid)
{
    XLOCK(mPageCacheLock);
    if (mPageCacheDataMap.size()>0)
    {
        log_debug("xBlogCache::delPage cache get =%d \r\n", mPageCacheDataMap.size());
        CACHEDATAMAPITER iter = mPageCacheDataMap.find(pageid);
        if(iter!=mPageCacheDataMap.end())
        {
            mPageCacheDataMap.erase(iter);
        }
    }
}

string &xBlogCache::GetPost(const uint32 postid)
{
    XLOCK(mPostCacheLock);
    return mPostCacheDataMap[postid].strData;
}

void xBlogCache::SetPost(const uint32 postid, const string & data)
{
    XLOCK(mPostCacheLock);
    mPostCacheDataMap[postid].strData = data;
}

void xBlogCache::delPost(const uint32 postid)
{
    XLOCK(mPostCacheLock);
    if (mPostCacheDataMap.size()>0)
    {
        log_debug("xBlogCache::delPage cache get =%d \r\n", mPostCacheDataMap.size());
        CACHEDATAMAPITER iter = mPostCacheDataMap.find(postid);
        if(iter!=mPostCacheDataMap.end())
        {
            mPostCacheDataMap.erase(iter);
        }
    }
}

void xBlogCache::OnTimer()
{
    uint32 now = time(NULL);
    if(now - timestamp<Config::GetInstance()->xBlogAppConfig.CacheTimer)
    {
        return;
    }
    timestamp = now;
    
    {
        XLOCK(mPostCacheLock);
        if (mPostCacheDataMap.size()>0)
        {
            log_info("xBlogCache::OnTimer cache get =%d \r\n", mPostCacheDataMap.size());
            CACHEDATAMAPITER iter = mPostCacheDataMap.begin();
            for(;iter!=mPostCacheDataMap.end();)
            {
                mPostCacheDataMap.erase(iter++);
            }
        }
    }

    {
        XLOCK(mPageCacheLock);
        if (mPageCacheDataMap.size()>0)
        {
            log_info("xBlogCache::OnTimer cache get =%d \r\n", mPageCacheDataMap.size());
            CACHEDATAMAPITER iter = mPageCacheDataMap.begin();
            for (; iter != mPageCacheDataMap.end();)
            {
                    mPageCacheDataMap.erase(iter++);
            }
        }
    }
}
