
#ifndef _xMULTIPART_PARSER_H_
#define _xMULTIPART_PARSER_H_


#include "multipart_parser.h"
#include "xLog.h"
#include "xUntil.h"

#include <string>
#include <iostream>

using namespace std;

char * strnchr(const char *p, char c, size_t n);
char * strnstr(const char *s, const char *find, size_t slen);

class MultipartConsumer
{
public:
    MultipartConsumer()
    {
        m_parser = NULL;
        filename_done = 0;
    }
    MultipartConsumer(const std::string& boundary)
    {
        memset(&m_callbacks, 0, sizeof(multipart_parser_settings));
        m_callbacks.on_header_field = ReadHeaderName;
        m_callbacks.on_header_value = ReadHeaderValue;
        m_callbacks.on_part_data = on_part_data;
        
        m_callbacks.on_headers_complete = on_headers_complete;
        m_callbacks.on_part_data_begin = on_part_data_begin;
        m_callbacks.on_part_data_end = on_part_data_end;

        m_parser = multipart_parser_init(boundary.c_str(), &m_callbacks);
        multipart_parser_set_data(m_parser, this);
    }

    ~MultipartConsumer()
    {
        multipart_parser_free(m_parser);
    }

    int CountHeaders(const std::string& body)
    {
        multipart_parser_execute(m_parser, body.c_str(), body.size());
        return m_headers;
    }

private:

    static int on_part_data(multipart_parser *p, const char *at, size_t length)
    {
        MultipartConsumer* me = (MultipartConsumer*)multipart_parser_get_data(p);
        if (0==me->filename_done) {
            log_info("on_part_data %.*s\n", length, at);
            return 0;
        }
        log_info("on_part_data: len:%d", length);
        //std::cout << "Data: " <<  at;
        //std::cout << "Data" << "\n";
        //write_data(fr, at, length);
        savefile(at, length, me->upload_filename.c_str());
        return 0;
    }
    static int on_part_data_begin(multipart_parser *p)
    {
        log_info("on_part_data_begin:");
        return 0;
    }

    static int on_part_data_end(multipart_parser *p)
    {
        log_info("on_part_data_end:");
        //fclose(fr);
        return 0;
    }

    static int on_headers_complete(multipart_parser *p)
    {
        log_info("on_headers_complete:");

        return 0;
    }

    static int ReadHeaderName(multipart_parser* p, const char *at, size_t length)
    {
        MultipartConsumer* me = (MultipartConsumer*)multipart_parser_get_data(p);

        //log_info("ReadHeaderName: %s \r\n", at);
        log_info("%.*s\n", length, at);

        me->m_headers++;
        return 0;
    }

    static int ReadHeaderValue(multipart_parser *p, const char *buf, size_t len) {
        MultipartConsumer* me = (MultipartConsumer*)multipart_parser_get_data(p);
        log_info("ReadHeaderValue %.*s\n", len, buf);
        char *filename = strnstr(buf, "filename=", len);
        char *nameend = NULL;
        if (filename) {
            filename += 9;
            if (filename[0] == '\"') {
                filename++;
                nameend = strnchr(filename, '\"', len - (filename - buf));
                nameend[0] = '\0';
                log_info("ReadHeaderValue: %s\n", filename);
                me->upload_filename = filename;
                me->filename_done = 1;
            }
        }
        return 0;
    }

    static int savefile(const char *buff, const size_t len, const char *save_name)
    {
        if (0==strlen(save_name)){
            return 0;
        }
        int result = -1;
        log_debug("Start to Storage the New Image %s...", save_name);
        int fd = -1;
        int wlen = 0;

        if ((fd = open(save_name, O_WRONLY | O_TRUNC | O_CREAT, 00644)) < 0)
        {
            log_debug("fd(%s) open failed!", save_name);
            goto done;
        }

//        if (flock(fd, LOCK_EX | LOCK_NB) == -1)
//        {
//            log_debug("This fd is Locked by Other thread.");
//            goto done;
//        }

        wlen = write(fd, buff, len);
        if (wlen == -1){
            log_debug("write(%s) failed!", save_name);
            goto done;
        } else if (wlen < (int)len){
            log_debug("Only part of [%s] is been writed.", save_name);
            goto done;
        }
//        flock(fd, LOCK_UN | LOCK_NB);
        log_debug("Image [%s] Write Successfully!", save_name);
        result = 1;

    done:
        if (fd != -1)
            close(fd);
        return result;
    }


    multipart_parser* m_parser;
    multipart_parser_settings m_callbacks;
    int m_headers;
    string upload_filename;
    int filename_done;
};

#endif



