
/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _X_UNTIL_H_
#define _X_UNTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <map>

using namespace  std;

#ifdef WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <io.h>
    #include <fcntl.h>
    
    #ifndef S_ISDIR
        #define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
    #endif
    #define SLEEP(ms) Sleep(1000*ms)
#else
    #include <sys/stat.h>
    #include <sys/socket.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <dirent.h>
    
    #define  stricmp strcasecmp
    #define  strnicmp strncasecmp
    #define SLEEP sleep

#endif

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#ifdef _EVENT_HAVE_NETINET_IN_H
    #include <netinet/in.h>
        #ifdef _XOPEN_SOURCE_EXTENDED
            #include <arpa/inet.h>
        #endif
#endif

#ifdef WIN32
    #define stat _stat
    #define fstat _fstat
    #define open _open
    #define close _close
    #define O_RDONLY _O_RDONLY
#endif


#ifdef _WIN32

    typedef signed    __int8         int8;
    typedef unsigned  __int8         uint8;
    typedef signed    __int16        int16;
    typedef unsigned  __int16        uint16;
    typedef signed    __int32        int32;
    typedef unsigned  __int32        uint32;
    typedef signed    __int64        int64;
    typedef unsigned  __int64        uint64;
    typedef unsigned  char           uchar;

#else

    typedef int8_t                   int8;
    typedef u_int8_t                 uint8;
    typedef int16_t                  int16;
    typedef u_int16_t                uint16;
    typedef int32_t                  int32;
    typedef u_int32_t                uint32;
    typedef int64_t                  int64;
    typedef u_int64_t                uint64;
    typedef unsigned char            uchar;
#endif

static const struct table_entry {
    const char *extension;
    const char *content_type;
} content_type_table[] = {
    { "txt", "text/plain" },
    { "c",   "text/plain" },
    { "h",   "text/plain" },
    { "js",  "application/x-javascript" },
    { "html","text/html" },
    { "htm", "text/html" },
    { "css", "text/css" },
    { "gif", "image/gif" },
    { "jpg", "image/jpeg" },
    { "ico", "image/gif" },
    { "jpeg","image/jpeg" },
    { "png", "image/png" },
    { "pdf", "application/pdf" },
    { "ps",  "application/postsript" },
    { NULL, NULL },
};

const char *GuessContentType(const char *path);
void  StringReplace(std::string& strData, const std::string & strsrc, const std::string &strdst);
char * GetSubStr( char *buf, const char *start, const char *end, char * out);
uint64 CreatSession();
size_t MysqlEscapeString(char *to, size_t to_length, const char *from, size_t length);

uint32 atoul(const char *str);
uint64 atoull(const char *str);

#endif

