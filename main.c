#include <sys/socket.h>
#include <pthread.h>
#include <getopt.h>
#include <assert.h>
#include "util.h"
#include "sock.h"
#include "req.h"

char root_dir[PATH_MAX];

void connection_handler(int conn) {
    char buf[PIPE_BUF];
    char dir[PATH_MAX];
    strcpy(dir, "");
    struct ftp_state state = { conn, dir, -1 };
    state.pasv_fd = -1;
    ftp_send(conn, "200 FTP server is ready.");
    while (1) {
        int pos = 0;
        while (pos < sizeof buf - 1) {
            int n = read(conn, buf + pos, sizeof buf - 1 - pos);
            if (n == -1) {
                puts("Failed to read connection.");
                close(conn);
                return;
            }
            if (n > 0) {
                pos += n;
                if (buf[pos - 1] == '\n') {
                    break;
                }
            }
        }
        buf[pos] = 0;
        if (!re_include(buf, "^[A-Za-z]+[^\r\n]*\r\n$", 0)) {
            ftp_send(conn, "500 Unknown command.");
            continue;
        }
        fwrite(buf, 1, pos, stdout);
        buf[pos - 2] = 0;
        int cmd = ftp_request_handler(buf, &state);
        if (cmd == FTP_CMD_QUIT || cmd == FTP_CMD_ABOR) {
            break;
        }
    }
    close(conn);
    return;
}

void* connection_thread(void* arg) {
    connection_handler(*(int*)arg);
    return NULL;
}

int main(int argc, char** argv) {
    uint16_t port = 21;
    struct option opts[3] = {
        { "root", 1, NULL, 'r' },
        { "port", 1, NULL, 'p' },
    };
    strcpy(root_dir, "/tmp");
    while (1) {
        int opt = getopt_long(argc, argv, "", opts, NULL);
        if (opt == -1) {
            break;
        }
        switch (opt) {
        case 'r':
            str_chomp(strncpy(root_dir, optarg, sizeof root_dir - 1), '/');
            break;
        case 'p':
            port = atoi(optarg);
            break;
        }
    }

    int serv = ftp_listen(port);
    assert(serv != -1);
    pthread_attr_t attr;
    assert(0 == pthread_attr_init(&attr));
    while (1) {
        int conn = accept(serv, NULL, NULL);
        if (conn == -1) {
            continue;
        }
        pthread_t* thread = malloc(sizeof(pthread_t));
        assert(0 == pthread_create(thread, &attr, connection_thread, &conn));
    }
    return 0;
}
