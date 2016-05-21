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

    #define  stricmp strcasecmp
    #define  strnicmp strncasecmp
    #define  SLEEP(ms) usleep(ms*1000)
//
#endif

#endif

