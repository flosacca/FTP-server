#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>
#include <regex.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int re_match(const char* s, const char* pattern, size_t n, regmatch_t* match, int eflags) {
    regex_t regex;
    regcomp(&regex, pattern, REG_EXTENDED | eflags);
    int res = regexec(&regex, s, n, match, 0);
    regfree(&regex);
    return res == 0;
}

static inline int re_include(const char* s, const char* pattern, int eflags) {
    return re_match(s, pattern, 0, NULL, REG_NOSUB | eflags);
}

static inline char* str_chomp(char* s, char c) {
    int n = strlen(s);
    char v = c ? c : '\n';
    if (n > 0 && s[n - 1] == v) {
        s[--n] = 0;
        if (c == 0 && n > 0 && s[n - 1] == '\r') {
            s[--n] = 0;
        }
    }
    return s;
}

static inline int ftp_write(int fd, const char* buf, int size) {
    int i = 0;
    while (i < size) {
        int n = write(fd, buf + i, size - i);
        if (n == -1) {
            return -1;
        }
        i += n;
    }
    return 0;
}

static inline int ftp_send(int fd, const char* s) {
    int n = strlen(s);
    char* buf = malloc(n + 2);
    memcpy(buf, s, n);
    buf[n] = '\r';
    buf[n + 1] = '\n';
    int res = ftp_write(fd, buf, n + 2);
    free(buf);
    return res;
}

#endif
