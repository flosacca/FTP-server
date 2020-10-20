#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "util.h"
#include "ftp_cmd.h"

#define BUFFER_SIZE 4096

void connection_handler(int conn) {
    char buf[BUFFER_SIZE + 1];
    struct ftp_state state = {0};
    ftp_send(conn, "200 FTP serv is ready.");
    while (1) {
        int pos = 0;
        while (pos < BUFFER_SIZE) {
            int n = read(conn, buf + pos, BUFFER_SIZE - pos);
            if (n == -1) {
                puts("Failed to read connection.");
                close(conn);
                return;
            }
            pos += n;
            if (buf[pos - 1] == '\n') {
                break;
            }
        }
        buf[pos] = 0;
        fwrite(buf, 1, pos, stdout);
        state.last_cmd = ftp_command_handler(conn, buf, &state);
        if (state.last_cmd == FTP_CMD_QUIT || state.last_cmd == FTP_CMD_ABOR) {
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
    int serv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(serv != -1);
    struct sockaddr_in addr = {AF_INET, htons(21), htonl(INADDR_ANY)};
    assert(0 == bind(serv, (struct sockaddr*)&addr, sizeof addr));
    assert(0 == listen(serv, 5));
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
