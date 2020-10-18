#ifndef FTP_DATA_H
#define FTP_DATA_H

#include <netinet/in.h>
#include "ftp_def.h"

int ftp_connect(int sess, struct ftp_state* state, int (*ready)(int, void*));

int ftp_send_list(int data, void* arg);

#endif
