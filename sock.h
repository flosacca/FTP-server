#ifndef SOCK_H
#define SOCK_H

#include "util.h"
#include "def.h"

int ftp_listen(uint16_t port);

int ftp_connect(struct ftp_state* state);
int ftp_accept(struct ftp_state* state);

int ftp_send_list(int data, struct ftp_state* state);
int ftp_send_file(int data, struct ftp_state* state);
int ftp_recv_file(int data, struct ftp_state* state);

#endif
