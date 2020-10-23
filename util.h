#ifndef UTIL_H
#define UTIL_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <regex.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int re_match(const char* s, const char* pattern, size_t n, regmatch_t* match, int cflags);

uint32_t get_sock_addr(int fd);
uint16_t get_sock_port(int fd);

int ftp_write(int fd, const char* buf, int size);
int ftp_read(int fd, char* buf, int size);

static inline int re_include(const char* s, const char* pattern, int cflags) {
    return re_match(s, pattern, 0, NULL, REG_NOSUB | cflags);
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

static inline int ftp_send(int fd, const char* s) {
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

#endif
