#ifndef UTIL_H
#define UTIL_H

#include <regex.h>

void assert_success(int s);

int re_match(const char* source, const char* pattern, size_t n, regmatch_t* matches, int eflags);
int re_include(const char* source, const char* pattern, int eflags);

int write_line(int fd, const char* s);

#endif
