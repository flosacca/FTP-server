#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>
#include <regex.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int re_match(const char* s, const char* pattern, size_t n, regmatch_t* match, int eflags) {
    regex_t regex;
    regcomp(&regex, pattern, REG_EXTENDED | eflags);
    int r = regexec(&regex, s, n, match, 0);
    regfree(&regex);
    return r == 0;
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

static int ftp_write(int fd, const char* buf, int size) {
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

static int ftp_send(int fd, const char* s) {
    if (s == NULL) {
        return -1;
    }
    int n = strlen(s);
    char* buf = malloc(n + 2);
    memcpy(buf, s, n);
    buf[n] = '\r';
    buf[n + 1] = '\n';
    int r = ftp_write(fd, buf, n + 2);
    free(buf);
    return r;
}

static int ftp_read(int fd, char* buf, int size) {
    int i = 0;
    while (i < size) {
        int n = read(fd, buf + i, size - i);
        if (n == -1) {
            return -1;
        }
        if (n == 0) {
            break;
        }
        i += n;
    }
    return i;
}

#endif
