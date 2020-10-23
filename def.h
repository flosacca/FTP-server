#ifndef DEF_H
#define DEF_H

#include <netinet/in.h>

struct ftp_state {
    int sess;
    char* dir;
    int user;
    struct sockaddr_in* port_addr;
    int pasv_fd;
    const char* msg_ready;
    const char* msg_ok;
    const char* msg_err[2];
    int (*ready)(int, struct ftp_state*);
    void* arg;
    char* old_path;
};

#endif
