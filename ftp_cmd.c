#include <arpa/inet.h>
#include "util.h"
#include "ftp_cmd.h"

int ftp_command_handler(int conn, const char* cmd, struct ftp_state* state) {
    if (re_include(cmd, "^USER\\>", REG_ICASE)) {
        if (re_include(cmd, "^[^ ]+\\> anonymous\r\n$", 0)) {
            write_line(conn, "331 Specifiy an email address as password.");
        } else {
            write_line(conn, "504 User must be anonymous.");
        }
        return FTP_CMD_USER;
    }

    if (re_include(cmd, "^PASS\\>", REG_ICASE)) {
        if (state->loggedin) {
            write_line(conn, "230 Already logged in.");
        } else if (state->last_cmd != FTP_CMD_USER) {
            write_line(conn, "503 Login with USER first.");
        } else {
            state->loggedin = 1;
            write_line(conn, "230 Login successful.");
        }
        return FTP_CMD_PASS;
    }

    if (re_include(cmd, "^QUIT\\>", REG_ICASE)) {
        return FTP_CMD_QUIT;
    }

    if (!state->loggedin) {
        write_line(conn, "503 Login with USER and PASS first.");
        return FTP_CMD_NONE;
    }

    if (re_include(cmd, "^ACCT\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_ACCT;
    }
    if (re_include(cmd, "^CWD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_CWD;
    }
    if (re_include(cmd, "^CDUP\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_CDUP;
    }
    if (re_include(cmd, "^SMNT\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_SMNT;
    }
    if (re_include(cmd, "^REIN\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_REIN;
    }

    if (re_include(cmd, "^PORT\\>", REG_ICASE)) {
        regmatch_t m[7];
        uint8_t a[6] = {0};
        re_match(cmd, "^[^ ]+\\> .*\\<([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+)\\>.*\r\n", 7, m, 0);
        for (int i = 0; i < 6; ++i) {
            for (int j = m[i].rm_so; j < m[i].rm_eo; ++j) {
                a[i] = a[i] * 10 + cmd[j] - '0';
            }
        }
        state->port_addr.sin_family = AF_INET;
        state->port_addr.sin_port = htons(a[4] << 8 | a[5]);
        state->port_addr.sin_addr.s_addr = htonl(a[0] << 24 | a[1] << 16 | a[2] << 8 | a[3]);
        return FTP_CMD_PORT;
    }

    if (re_include(cmd, "^PASV\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_PASV;
    }

    if (re_include(cmd, "^TYPE\\>", REG_ICASE)) {
        if (re_include(cmd, "^TYPE I\r\n", REG_ICASE)) {
            write_line(conn, "200 Type set to I.");
        } else {
            write_line(conn, "504 Type must be set to I.");
        }
        return FTP_CMD_TYPE;
    }

    if (re_include(cmd, "^STRU\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_STRU;
    }
    if (re_include(cmd, "^MODE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_MODE;
    }
    if (re_include(cmd, "^RETR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_RETR;
    }
    if (re_include(cmd, "^STOR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_STOR;
    }
    if (re_include(cmd, "^STOU\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_STOU;
    }
    if (re_include(cmd, "^APPE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_APPE;
    }
    if (re_include(cmd, "^ALLO\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_ALLO;
    }
    if (re_include(cmd, "^REST\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_REST;
    }
    if (re_include(cmd, "^RNFR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_RNFR;
    }
    if (re_include(cmd, "^RNTO\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_RNTO;
    }
    if (re_include(cmd, "^ABOR\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_ABOR;
    }
    if (re_include(cmd, "^DELE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_DELE;
    }
    if (re_include(cmd, "^RMD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_RMD;
    }
    if (re_include(cmd, "^MKD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_MKD;
    }
    if (re_include(cmd, "^PWD\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_PWD;
    }
    if (re_include(cmd, "^LIST\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_LIST;
    }
    if (re_include(cmd, "^NLST\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_NLST;
    }
    if (re_include(cmd, "^SITE\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_SITE;
    }

    if (re_include(cmd, "^SYST\\>", REG_ICASE)) {
        write_line(conn, "215 UNIX Type: L8");
        return FTP_CMD_SYST;
    }

    if (re_include(cmd, "^STAT\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_STAT;
    }
    if (re_include(cmd, "^HELP\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_HELP;
    }
    if (re_include(cmd, "^NOOP\\>", REG_ICASE)) {
        write_line(conn, "502 Command not implemented.");
        return FTP_CMD_NOOP;
    }
}
