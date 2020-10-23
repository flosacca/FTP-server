#ifndef FTP_SOCKET_H
#define FTP_SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include "def.h"
#include "util.h"

#define ASSERT(c) \
    do { if (!(c)) return -1; } while (0)

static uint32_t get_sock_addr(int fd) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof addr;
    getsockname(fd, (struct sockaddr*)&addr, &addrlen);
    return ntohl(addr.sin_addr.s_addr);
}

static uint16_t get_sock_port(int fd) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof addr;
    getsockname(fd, (struct sockaddr*)&addr, &addrlen);
    return ntohs(addr.sin_port);
}

static int ftp_listen(uint16_t port) {
    int serv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(serv != -1);
    struct sockaddr_in addr = {AF_INET, htons(port), htonl(INADDR_ANY)};
    ASSERT(0 == bind(serv, (struct sockaddr*)&addr, sizeof addr));
    ASSERT(0 == listen(serv, 5));
    return serv;
}

#define _FTP_SEND_DATA \
    if (0 == state->ready(data, state)) { \
        ftp_send(state->sess, state->msg_ok); \
    } \
    close(data)

static inline int ftp_connect(struct ftp_state* state) {
    int data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(data != -1);
    ASSERT(0 == connect(data, (struct sockaddr*)state->port_addr, sizeof(struct sockaddr)));
    _FTP_SEND_DATA;
    return 0;
}

static inline int ftp_accept(struct ftp_state* state) {
    int data = accept(state->pasv_fd, NULL, NULL);
    ASSERT(data != -1);
    _FTP_SEND_DATA;
    return 0;
}

#define _FTP_SEND_INPUT \
    if (f == NULL) { \
        ftp_send(state->sess, state->msg_error); \
        return -1; \
    } \
    ftp_send(state->sess, state->msg_ready); \
    int r = 0; \
    do { \
        int n; \
        while (n = fread(buf, 1, sizeof buf, f)) { \
            r = ftp_write(data, buf, n); \
            if (r == -1) { \
                break; \
            } \
        } \
    } while (0)

static int ftp_send_list(int data, struct ftp_state* state) {
    char buf[PIPE_BUF];
    FILE* f = popen((char*)state->arg, "r");
    _FTP_SEND_INPUT;
    pclose(f);
    return r;
}

static int ftp_send_file(int data, struct ftp_state* state) {
    char buf[PIPE_BUF];
    FILE* f = fopen((char*)state->arg, "r");
    _FTP_SEND_INPUT;
    fclose(f);
    return r;
}

static int ftp_recv_file(int data, struct ftp_state* state) {
    char buf[PIPE_BUF];
    FILE* f = fopen((char*)state->arg, "w");
    if (f == NULL) {
        ftp_send(state->sess, state->msg_error);
        return -1;
    }
    ftp_send(state->sess, state->msg_ready);
    int n;
    while (n = ftp_read(data, buf, sizeof buf)) {
        if (n == -1) {
            break;
        }
        fwrite(buf, 1, n, f);
    }
    fclose(f);
    return n;
}

#endif
