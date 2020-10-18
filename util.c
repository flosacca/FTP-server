#include <regex.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "util.h"

inline void assert_success(int s) {
    assert(s == 0);
}

inline int re_match(const char* source, const char* pattern, regmatch_t* matches, size_t n, int eflags) {
    regex_t regex;
    regcomp(&regex, pattern, REG_EXTENDED | eflags);
    int s = regexec(&regex, source, n, matches, 0);
    regfree(&regex);
    return s == 0;
}

inline int re_include(const char* source, const char* pattern, int eflags) {
    return re_match(source, pattern, NULL, 0, REG_NOSUB | eflags);
}

inline int write_line(int fd, const char* s) {
    int l = strlen(s);
    char* t = malloc(l + 2);
    memcpy(t, s, l);
    t[l] = '\r';
    t[l + 1] = '\n';
    int n = write(fd, t, l + 2);
    free(t);
    return n;
}
