
/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */
 
#include "xUntil.h"

const char *GuessContentType(const char *path)
{
    const char *last_period, *extension;
    const struct table_entry *ent;
    last_period = strrchr(path, '.');
    if (!last_period || strchr(last_period, '/'))
        goto not_found; /* no exension */
    extension = last_period + 1;
    for (ent = &content_type_table[0]; ent->extension; ++ent) {
        if (!stricmp(ent->extension, extension))
            return ent->content_type;
    }

not_found:
    return "application/misc";
}

void StringReplace(std::string& strData, const std::string & strsrc, const std::string &strdst)
{
    std::string::size_type pos = 0;
    while( (pos = strData.find(strsrc, pos)) != string::npos)
    {
        strData.replace(pos, strsrc.length(), strdst);
        pos += strdst.length();
    }
}

char * GetSubStr( char *buf, const char *start, const char *end, char * out)
{
    if(!buf||!out)return NULL;

    char *pStart = strstr(buf, start);
    if (!pStart)return NULL;
    
    pStart += strlen(start);
    char *pEnd = strstr(pStart, end);
    if(!pEnd)return NULL;
    
    strncpy(out, pStart, (pEnd - pStart));
    return out;
}

uint32 atoul(const char *str)
{
    if (NULL!=str)
    {
        return atoi(str);
    }
    return 0;
}

uint64 atoull(const char *str)
{
    if (NULL==str)
    {
        return 0;
    }
#ifdef _WIN32
    return _atoi64(str);
#else
    return strtoull(str, (char**)NULL, 0) ;
#endif

}

uint64 CreatSession()
{
    union SESSION
    {
        struct
        {
            uint16 flag;
            uint16 status;
            uint32 time;
        };
        uint64 ssid;
    };

    SESSION sid;
    sid.flag   = 1023;
    sid.status = (uint16)time(NULL);
    sid.status = sid.status<<8;
    sid.time   = time(NULL);

    return sid.ssid;
}

size_t MysqlEscapeString(char *to, size_t to_length, const char *from, size_t length)
{
  const char *to_start= to;
  const char *end, *to_end=to_start + (to_length ? to_length-1 : 2*length);
  bool overflow= false;

  for (end= from + length; from < end; from++)
  {
    char escape= 0;

    switch (*from) {
    case 0:                /* Must be escaped for 'mysql' */
      escape= '0';
      break;
    case '\n':                /* Must be escaped for logs */
      escape= 'n';
      break;
    case '\r':
      escape= 'r';
      break;
    case '\\':
      escape= '\\';
      break;
    case '\'':
      escape= '\'';
      break;
    case '"':                /* Better safe than sorry */
      escape= '"';
      break;
    case '\032':            /* This gives problems on Win32 */
      escape= 'Z';
      break;
    }
    if (escape)
    {
      if (to + 2 > to_end)
      {
        overflow= true;
        break;
      }
      *to++= '\\';
      *to++= escape;
    }
    else
    {
      if (to + 1 > to_end)
      {
        overflow= true;
        break;
      }
      *to++= *from;
    }
  }
  *to= 0;
  return overflow ? (size_t) -1 : (size_t) (to - to_start);
}

