#include <sys/types.h>
#include <sys/socket.h>
#include "util.h"
#include "ftp_data.h"

#define ASSERT(c) \
    do { if (!(c)) return -1; } while (0)

int ftp_connect(int sess, struct ftp_state* state, int (*ready)(int, void*)) {
    int data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ASSERT(data != -1);
    ASSERT(0 == connect(data, (struct sockaddr*)&state->port_addr, sizeof(struct sockaddr)));
    if (state->msg_ready != NULL) {
        ftp_send(sess, state->msg_ready);
    }
    ready(data, state->arg);
    close(data);
    if (state->msg_ok != NULL) {
        ftp_send(sess, state->msg_ok);
    }
    return 0;
}

int ftp_send_list(int data, void* arg) {
    char buf[1024];
    char* shell_cmd = str_add(str_new("/bin/ls "), (char*)arg);
    puts(shell_cmd);
    FILE* pipe = popen(shell_cmd, "r");
    free(shell_cmd);
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
