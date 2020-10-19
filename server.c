#include <sys/socket.h>
#include <pthread.h>
#include <assert.h>
#include "util.h"
#include "ftp_socket.h"
#include "ftp_cmd.h"

#define BUFFER_SIZE 4096

void connection_handler(int conn) {
    char buf[BUFFER_SIZE + 1];
    struct ftp_state state = {0};
    state.pasv_fd = -1;
    ftp_send(conn, "200 FTP server is ready.");
    while (1) {
        int pos = 0;
        while (pos < BUFFER_SIZE) {
            int n = read(conn, buf + pos, BUFFER_SIZE - pos);
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
        int cmd = ftp_command_handler(conn, buf, &state);
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
    int serv = ftp_listen(21);
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
