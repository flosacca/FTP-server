#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define BUF_SIZE 4096

#define ASSERT_SUCCESS(n) assert((n) == 0)

void command_handler(int conn, const char* cmd) {
}

void connection_handler(int conn) {
    char buf[BUF_SIZE + 1];
    write(conn, "220\r\n", 5);
    while (1) {
        int pos = 0;
        while (pos < BUF_SIZE) {
            int n = read(conn, buf + pos, BUF_SIZE - pos);
            if (n == -1) {
                puts("read error");
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
        command_hanlder(conn, buf);
    }
    close(conn);
    return;
}

void* connection_thread(void* arg) {
    connection_handler(*(int*)arg);
    return NULL;
}

int main(int argc, char** argv) {
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(server != -1);
    struct sockaddr_in addr = {AF_INET, htons(21), htonl(INADDR_ANY)};
    ASSERT_SUCCESS(bind(server, (struct sockaddr*)&addr, sizeof addr));
    ASSERT_SUCCESS(listen(server, 5));
    pthread_attr_t attr;
    ASSERT_SUCCESS(pthread_attr_init(&attr));
    while (1) {
        int conn = accept(server, NULL, NULL);
        if (conn == -1) {
            continue;
        }
        pthread_t* thread = malloc(sizeof(pthread_t));
        ASSERT_SUCCESS(pthread_create(thread, &attr, connection_handler, &conn));
    }
    return 0;
}
