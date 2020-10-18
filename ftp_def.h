#ifndef FTP_DEF_H
#define FTP_DEF_H

#include <stdint.h>

struct ftp_state {
    int last_cmd;
    int loggedin;
    uint32_t port_addr;
};

#endif
