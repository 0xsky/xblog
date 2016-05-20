/*
* ----------------------------------------------------------------------------
* Copyright (c) 2012-2016, xSky <guozhw at gmail dot com>
* All rights reserved.
* Distributed under GPL license.
* ----------------------------------------------------------------------------
*/
 
#include "xUntil.h"


uint32_t atoul(const char *str)
{
    if (NULL!=str){
        return atoi(str);
    }
    return 0;
}

uint64_t atoull(const char *str)
{
    if (NULL==str){
        return 0;
    }
#ifdef _WIN32
    return _atoi64(str);
#else
    return strtoull(str, (char**)NULL, 0) ;
#endif

}

uint64_t CreatSession()
{
    union SESSION {
        struct {
            uint16_t flag;
            uint16_t status;
            uint32_t time;
        };
        uint64_t ssid;
    };

    SESSION sid;
    sid.flag   = 1023;
    sid.status = (uint16_t)time(NULL);
    sid.status = sid.status<<8;
    sid.time   = (uint32_t)time(NULL);

    return sid.ssid;
}

/**
* @brief strnchr find the pointer of a char in a string
*
* @param p the string
* @param c the char
* @param n find length
*
* @return the char position or 0
*/
char * strnchr(const char *p, char c, size_t n)
{
    if (!p)
        return 0;

    while (n-- > 0) {
        if (*p == c)
            return ((char *)p);
        p++;
    }
    return 0;
}

/**
* @brief strnstr find the sub string in a string
*
* @param s the string
* @param find the sub string
* @param slen find length
*
* @return the position of sub string or NULL
*/
char * strnstr(const char *s, const char *find, size_t slen)
{
    char c, sc;
    size_t len;

    if ((c = *find++) != '\0') {
        len = strlen(find);
        do {
            do {
                if ((sc = *s++) == '\0' || slen-- < 1)
                    return (NULL);
            } while (sc != c);
            if (len > slen)
                return (NULL);
        } while (strncmp(s, find, len) != 0);
        s--;
    }
    return ((char *)s);
}



