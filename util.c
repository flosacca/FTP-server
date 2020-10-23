#include "util.h"

int re_match(const char* s, const char* pattern, size_t n, regmatch_t* match, int cflags) {
    regex_t regex;
    regcomp(&regex, pattern, REG_EXTENDED | cflags);
    int r = regexec(&regex, s, n, match, 0);
    regfree(&regex);
    return r == 0;
}

uint32_t get_sock_addr(int fd) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof addr;
    getsockname(fd, (struct sockaddr*)&addr, &addrlen);
    return ntohl(addr.sin_addr.s_addr);
}

uint16_t get_sock_port(int fd) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof addr;
    getsockname(fd, (struct sockaddr*)&addr, &addrlen);
    return ntohs(addr.sin_port);
}

int ftp_write(int fd, const char* buf, int size) {
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

int ftp_read(int fd, char* buf, int size) {
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
