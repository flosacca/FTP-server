#ifndef UTIL_H
#define UTIL_H

#include <regex.h>

void assert_success(int s);
int re_match(const char* source, const char* pattern, regmatch_t* matches, size_t n, int eflags);
int re_include(const char* source, const char* pattern, int eflags);
int write_line(int fd, const char* s);

#endif
