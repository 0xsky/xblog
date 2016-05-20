/*
 * ----------------------------------------------------------------------------
 * Copyright (c) 2012-2013, xSky <guozhw at gmail dot com>
 * All rights reserved.
 * Distributed under GPL license.
 * ----------------------------------------------------------------------------
 */

#ifndef _XLOG_H
#define _XLOG_H

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_DEBUG 3

#ifdef WIN32

#define log_error printf
#define log_warn  printf
#define log_info  printf
#define log_debug printf
#else

#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <deque>
#include <string>
#include <pthread.h>
#include <sys/time.h>
using std::deque;
using std::string;
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid) 

#define log_error(...) LOG(ERROR, __VA_ARGS__)
#define log_warn(...)  LOG(WARN, __VA_ARGS__)
#define log_info(...)  LOG(INFO, __VA_ARGS__)
#define log_debug(...) LOG(DEBUG, __VA_ARGS__)


#define LOG_LEVEL(level) LOG_LEVEL_##level, __FILE__, __LINE__, __FUNCTION__
#define LOG_NUM_LEVEL(level) level, __FILE__, __LINE__, __FUNCTION__
#define LOGGER CLogger::_logger
#define PRINT(level, ...) LOGGER.logMessage(LOG_LEVEL(level), __VA_ARGS__)
#define LOG_BASE(level, ...) (LOG_LEVEL_##level>LOGGER._level) ? (void)0 : PRINT(level, __VA_ARGS__) 
#define LOG(level, _fmt_, args...) ((LOG_LEVEL_##level>LOGGER._level) ? (void)0 : LOG_BASE(level, "[%ld] " _fmt_, gettid(), ##args))
#define LOG_US(level, _fmt_, args...) \
  ((LOG_LEVEL_##level>LOGGER._level) ? (void)0 : LOG_BASE(level, "[%ld][%ld][%ld] " _fmt_, \
                                                            pthread_self(), CLogger::get_cur_tv().tv_sec, \
                                                            CLogger::get_cur_tv().tv_usec, ##args))


class CLogger {
public:
    CLogger();
    ~CLogger();

    void rotateLog(const char *filename, const char *fmt = NULL);
    void logMessage(int level, const char *file, int line, const char *function, const char *fmt, ...);
    void setLogLevel(const char *level);
    void setFileName(const char *filename, bool flag = false);
    void checkFile();
    void setCheck(int v) {_check = v;}
    void setMaxFileSize( int64_t maxFileSize=0x40000000);
    void setMaxFileIndex( int maxFileIndex= 0x0F);

    static inline struct timeval get_cur_tv()
    {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      return tv;
    };

private:
    int _fd;
    char *_name;
    int _check;
    size_t _maxFileIndex;
    int64_t _maxFileSize;
    bool _flag;

public:
    static CLogger _logger;
    int _level;

private:
    std::deque<std::string> _fileList;
    static const char *const _errstr[];   
    pthread_mutex_t _fileSizeMutex;
    pthread_mutex_t _fileIndexMutex; 
};


#endif

#endif
