#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "ftp_data.h"

#define ASSERT(c) \
    do { if (!(c)) return -1; } while (0)

static int _send_list(struct sockaddr_in* addr, const char* path, int conn) {
    int data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(data != -1);
    ASSERT(0 == connect(data, (struct sockaddr*)addr, sizeof *addr));
    ftp_send(conn, "150 Here comes the directory listing.");
    char buf[1024];
    char* s = str_add(str_new("/bin/ls -l "), path);
    FILE* f = popen(s, "r");
    free(s);
    ASSERT(f != NULL);
    while (fgets(buf, sizeof buf, f)) {
        ftp_write(data, buf, strlen(buf));
    }
    fclose(f);
    close(data);
    ftp_send(conn, "226 Directory send OK.");
    return 0;
}

inline void send_list(struct sockaddr_in* addr, const char* path, int conn) {
    if (0 != _send_list(addr, path, conn)) {
        ftp_send(conn, "425 Failed to establish connection.");
    }
}
