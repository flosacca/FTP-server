#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "util.h"

#define CMD_NONE 0
#define CMD_USER 1
#define CMD_PASS 2
#define CMD_ACCT 3
#define CMD_CWD  4
#define CMD_CDUP 5
#define CMD_SMNT 6
#define CMD_REIN 7
#define CMD_QUIT 8
#define CMD_PORT 9
#define CMD_PASV 10
#define CMD_TYPE 11
#define CMD_STRU 12
#define CMD_MODE 13
#define CMD_RETR 14
#define CMD_STOR 15
#define CMD_STOU 16
#define CMD_APPE 17
#define CMD_ALLO 18
#define CMD_REST 19
#define CMD_RNFR 20
#define CMD_RNTO 21
#define CMD_ABOR 22
#define CMD_DELE 23
#define CMD_RMD  24
#define CMD_MKD  25
#define CMD_PWD  26
#define CMD_LIST 27
#define CMD_NLST 28
#define CMD_SITE 29
#define CMD_SYST 30
#define CMD_STAT 31
#define CMD_HELP 32
#define CMD_NOOP 33

#define BUFFER_SIZE 4096

int command_handler(int conn, const char* cmd, int last_cmd, int* loggedin) {
    if (re_include(cmd, "^USER\\>", REG_ICASE)) {
        if (re_include(cmd, "^[^ ]+\\> anonymous\r\n$", 0)) {
            write_line(conn, "331 Specifiy an email address as password.");
        } else {
            write_line(conn, "504 User other than anonymous is not supported.");
        }
        return CMD_USER;
    }

    if (re_include(cmd, "^PASS\\>", REG_ICASE)) {
        if (*loggedin) {
            write_line(conn, "230 Already logged in.");
        } else if (last_cmd != CMD_USER) {
            write_line(conn, "503 Login with USER first.");
        } else {
            *loggedin = 1;
            write_line(conn, "230 Login successfully.");
        }
        return CMD_PASS;
    }

    if (re_include(cmd, "^QUIT\\>", REG_ICASE)) {
        return CMD_QUIT;
    }

    if (!*loggedin) {
        write_line(conn, "503 Login with USER and PASS first.");
        return CMD_NONE;
    }

    if (re_include(cmd, "^ACCT\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_ACCT;
    }
    if (re_include(cmd, "^CWD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_CWD;
    }
    if (re_include(cmd, "^CDUP\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_CDUP;
    }
    if (re_include(cmd, "^SMNT\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_SMNT;
    }
    if (re_include(cmd, "^REIN\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_REIN;
    }
    if (re_include(cmd, "^PORT\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_PORT;
    }
    if (re_include(cmd, "^PASV\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_PASV;
    }
    if (re_include(cmd, "^TYPE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_TYPE;
    }
    if (re_include(cmd, "^STRU\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_STRU;
    }
    if (re_include(cmd, "^MODE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_MODE;
    }
    if (re_include(cmd, "^RETR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_RETR;
    }
    if (re_include(cmd, "^STOR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_STOR;
    }
    if (re_include(cmd, "^STOU\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_STOU;
    }
    if (re_include(cmd, "^APPE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_APPE;
    }
    if (re_include(cmd, "^ALLO\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_ALLO;
    }
    if (re_include(cmd, "^REST\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_REST;
    }
    if (re_include(cmd, "^RNFR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_RNFR;
    }
    if (re_include(cmd, "^RNTO\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_RNTO;
    }
    if (re_include(cmd, "^ABOR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_ABOR;
    }
    if (re_include(cmd, "^DELE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_DELE;
    }
    if (re_include(cmd, "^RMD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_RMD;
    }
    if (re_include(cmd, "^MKD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_MKD;
    }
    if (re_include(cmd, "^PWD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_PWD;
    }
    if (re_include(cmd, "^LIST\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_LIST;
    }
    if (re_include(cmd, "^NLST\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_NLST;
    }
    if (re_include(cmd, "^SITE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_SITE;
    }
    if (re_include(cmd, "^SYST\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_SYST;
    }
    if (re_include(cmd, "^STAT\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_STAT;
    }
    if (re_include(cmd, "^HELP\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_HELP;
    }
    if (re_include(cmd, "^NOOP\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return CMD_NOOP;
    }
}

void connection_handler(int conn) {
    char buf[BUFFER_SIZE + 1];
    int last_cmd = CMD_NONE;
    int loggedin = 0;
    write_line(conn, "200 FTP server is ready.");
    while (1) {
        int pos = 0;
        while (pos < BUFFER_SIZE) {
            int n = read(conn, buf + pos, BUFFER_SIZE - pos);
            if (n == -1) {
                puts("Failed to read connection.");
                close(conn);
                return;
            }
            pos += n;
            if (buf[pos - 1] == '\n') {
                break;
            }
        }
        buf[pos] = 0;
        fwrite(buf, 1, pos, stdout);
        last_cmd = command_handler(conn, buf, last_cmd, &loggedin);
        if (last_cmd == CMD_QUIT || last_cmd == CMD_ABOR) {
            break;
        }
    }
    close(conn);
    return;
}

void* connection_thread(void* arg) {
    connection_handler(*(int*)arg);
    return NULL;
}

int main(int argc, char** argv) {
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(server != -1);
    struct sockaddr_in addr = {AF_INET, htons(21), htonl(INADDR_ANY)};
    assert_success(bind(server, (struct sockaddr*)&addr, sizeof addr));
    assert_success(listen(server, 5));
    pthread_attr_t attr;
    assert_success(pthread_attr_init(&attr));
    while (1) {
        int conn = accept(server, NULL, NULL);
        if (conn == -1) {
            continue;
        }
        pthread_t* thread = malloc(sizeof(pthread_t));
        assert_success(pthread_create(thread, &attr, connection_thread, &conn));
    }
    return 0;
}
