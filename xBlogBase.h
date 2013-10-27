/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#ifndef _BLOG_BASE_H_
#define _BLOG_BASE_H_

#ifndef O_BINARY
#define O_BINARY 0
#endif

const int MAX_FILE_SIZE = 1024 * 128;

const int SIZE_32     = 32;
const int SIZE_128    = 128;
const int SIZE_256    = 256;
const int SIZE_512    = 512;
const int SIZE_1024   = 1024;

#define K_DEFAULT              "default"
#define V_DEFAULT              "default.htm"

#define K_SINGLE               "single"
#define V_SINGLE               "single.htm"

#define K_GUESTBOOK            "guestbook"
#define V_GUESTBOOK            "guestbook.htm"

#define K_CATALOG              "catalog"
#define V_CATALOG              "catalog.htm"

#define K_ARTICLE_NVABAR       "article_nvabar"
#define V_ARTICLE_NVABAR       "article_nvabar.htm"

#define K_ARTICLE_SINGLE       "article_single"
#define V_ARTICLE_SINGLE       "article_single.htm"

#define K_ARTICLE_GUESTBOOK    "article_guestbook"
#define V_ARTICLE_GUESTBOOK    "article_guestbook.htm"

#define K_ARTICLE_COMMENTPOST  "article_commentpost"
#define V_ARTICLE_COMMENTPOST  "article_commentpost.htm"

#define K_POSTEDIT             "postedit"
#define V_POSTEDIT             "postedit.htm"

#define K_ARTICLE_MULTI        "article_multi"
#define V_ARTICLE_MULTI        "article_multi.htm"

#define K_ARTICLE_COMMENT      "article_comment"
#define V_ARTICLE_COMMENT      "article_comment.htm"

#define K_NAVBAR               "CACHE_INCLUDE_NAVBAR"
#define V_NAVBAR               "navbar.htm"
#define K_CACHE_CATALOG        "CACHE_INCLUDE_CATALOG"
#define V_CACHE_CATALOG        "catalog.htm"
#define K_MISC                 "CACHE_INCLUDE_MISC"
#define V_MISC                 "misc.htm"
#define K_ARCHIVES             "CACHE_INCLUDE_ARCHIVES"
#define V_ARCHIVES             "archives.htm"
#define K_LINKS                "CACHE_INCLUDE_LINKS"
#define V_LINKS                "link.htm"


#define LOGIN_PAGE             "/admin/login.htm"
#define ADMIN_PAGE             "/admin/admin.htm"
#define PAGE_404               "/admin/404.htm"




#endif
