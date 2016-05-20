/*
* ----------------------------------------------------------------------------
* Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
* All rights reserved.
* Distributed under GPL license.
* ----------------------------------------------------------------------------
*/

#ifndef _X_UNTIL_H_
#define _X_UNTIL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
    #include <io.h>
    #include <fcntl.h>

    #ifndef S_ISDIR
        #define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
    #endif
    #define SLEEP(ms) Sleep(ms)
    #define snprintf _snprintf_s
    #define stricmp _stricmp
    #define strncpy strncpy_s
    #define vsnprintf vsnprintf_s
#else
    #include <sys/stat.h>
    #include <sys/socket.h>
    #include <sys/file.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <dirent.h>
    
    #define  stricmp strcasecmp
    #define  strnicmp strncasecmp
    #define SLEEP(ms) usleep(ms*1000)

#endif

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


uint64_t CreatSession();
uint32_t atoul(const char *str);
uint64_t atoull(const char *str);
char * strnchr(const char *p, char c, size_t n);
char * strnstr(const char *s, const char *find, size_t slen);
#endif

