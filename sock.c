#include "sock.h"

#define ASSERT(c) do { if (!(c)) return -1; } while (0)

int ftp_listen(uint16_t port) {
    int serv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(serv != -1);
    struct sockaddr_in addr = {AF_INET, htons(port), htonl(INADDR_ANY)};
    ASSERT(0 == bind(serv, (struct sockaddr*)&addr, sizeof addr));
    ASSERT(0 == listen(serv, 5));
    return serv;
}

#define _FTP_READY \
    do { \
        if (0 == state->ready(data, state)) { \
            ftp_send(state->sess, state->msg_ok); \
        } else { \
            ftp_send(state->sess, state->msg_err[1]); \
        } \
    } while (0)

int ftp_connect(struct ftp_state* state) {
    int data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(data != -1);
    ASSERT(0 == connect(data, (struct sockaddr*)state->port_addr, sizeof(struct sockaddr)));
    _FTP_READY;
    close(data);
    return 0;
}

int ftp_accept(struct ftp_state* state) {
    int data = accept(state->pasv_fd, NULL, NULL);
    ASSERT(data != -1);
    _FTP_READY;
    close(data);
    return 0;
}

#define _FTP_OPEN_FILE \
    do { \
        if (f == NULL) { \
            ftp_send(state->sess, state->msg_err[0]); \
            return -1; \
        } \
        ftp_send(state->sess, state->msg_ready); \
    } while (0)

#define _FTP_SEND_DATA \
    _FTP_OPEN_FILE; \
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

#define _FTP_RECV_DATA \
    _FTP_OPEN_FILE; \
    int n; \
    do { \
        while (n = ftp_read(data, buf, sizeof buf)) { \
            if (n == -1) { \
                break; \
            } \
            fwrite(buf, 1, n, f); \
        } \
    } while (0)

int ftp_send_output(int data, struct ftp_state* state) {
    char buf[PIPE_BUF];
    FILE* f = popen((char*)state->arg, "r");
    _FTP_SEND_DATA;
    pclose(f);
    return r;
}

int ftp_send_file(int data, struct ftp_state* state) {
    char buf[PIPE_BUF];
    FILE* f = fopen((char*)state->arg, "r");
    fseek(f, state->pos, SEEK_SET);
    _FTP_SEND_DATA;
    fclose(f);
    return r;
}

int ftp_recv_file(int data, struct ftp_state* state) {
    char buf[PIPE_BUF];
    FILE* f = fopen((char*)state->arg, state->mode);
    _FTP_RECV_DATA;
    fclose(f);
    return n;
}
