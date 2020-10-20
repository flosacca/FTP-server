#ifndef FTP_SOCKET_H
#define FTP_SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "def.h"
#include "util.h"

#define ASSERT(c) \
    do { if (!(c)) return -1; } while (0)

static inline uint32_t get_sock_addr(int fd) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof addr;
    getsockname(fd, (struct sockaddr*)&addr, &addrlen);
    return ntohl(addr.sin_addr.s_addr);
}

static inline uint16_t get_sock_port(int fd) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof addr;
    getsockname(fd, (struct sockaddr*)&addr, &addrlen);
    return ntohs(addr.sin_port);
}

static inline int ftp_listen(uint16_t port) {
    int serv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(serv != -1);
    struct sockaddr_in addr = {AF_INET, htons(port), htonl(INADDR_ANY)};
    ASSERT(0 == bind(serv, (struct sockaddr*)&addr, sizeof addr));
    ASSERT(0 == listen(serv, 5));
    return serv;
}

static inline void ftp_response(int sess, int data, struct ftp_state* state, int (*ready)(int, void*)) {
    if (state->msg_ready != NULL) {
        ftp_send(sess, state->msg_ready);
    }
    ready(data, state->arg);
    close(data);
    if (state->msg_ok != NULL) {
        ftp_send(sess, state->msg_ok);
    }
}

static inline int ftp_connect(int sess, struct ftp_state* state, int (*ready)(int, void*)) {
    int data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(data != -1);
    ASSERT(0 == connect(data, (struct sockaddr*)state->port_addr, sizeof(struct sockaddr)));
    ftp_response(sess, data, state, ready);
    return 0;
}

static inline int ftp_accept(int sess, struct ftp_state* state, int (*ready)(int, void*)) {
    int data = accept(state->pasv_fd, NULL, NULL);
    ASSERT(data != -1);
    ftp_response(sess, data, state, ready);
    return 0;
}

static inline int ftp_send_list(int data, void* arg) {
    static char buf[PATH_MAX + 128];
    char* cmd = arg;
    puts(cmd);
    FILE* pipe = popen(cmd, "r");
    ASSERT(pipe != NULL);
    int res = 0;
    while (fgets(buf, sizeof buf, pipe)) {
        res = ftp_write(data, buf, strlen(buf));
        if (res != 0) {
            break;
        }
    }
    pclose(pipe);
    return res;
}

#endif
