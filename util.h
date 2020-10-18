#ifndef UTIL_H
#define UTIL_H

#include <regex.h>

int re_match(const char* s, const char* pattern, size_t n, regmatch_t* match, int eflags);
int re_include(const char* s, const char* pattern, int eflags);

int ftp_write(int fd, const char* buf, int size);
int ftp_send(int fd, const char* s);

char* str_new(const char* s);
char* str_add(char* t, const char* s);

#endif
