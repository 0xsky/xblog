
#include "MultipartParase.h"


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

