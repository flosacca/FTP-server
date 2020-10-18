#ifndef FTP_DATA_H
#define FTP_DATA_H

#include <netinet/in.h>

void send_list(struct sockaddr_in* addr, const char* path, int conn);

#endif
