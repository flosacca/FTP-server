#include <arpa/inet.h>
#include <assert.h>
#include "util.h"
#include "ftp_socket.h"
#include "ftp_cmd.h"

int ftp_command_handler(int sess, const char* cmd, struct ftp_state* state) {
    if (re_include(cmd, "^USER\\>", REG_ICASE)) {
        if (re_include(cmd, "^[^ ]+\\>( anonymous\\>|.*)?", 0)) {
            ftp_send(sess, "331 Specifiy an email address as password.");
            state->user = 0;
        } else {
            ftp_send(sess, "504 User must be anonymous.");
        }
        return FTP_CMD_USER;
    }

    if (re_include(cmd, "^PASS\\>", REG_ICASE)) {
        if (state->user > 0) {
            ftp_send(sess, "230 Already logged in.");
        } else if (state->user < 0) {
            ftp_send(sess, "503 Login with USER first.");
        } else {
            state->user = 1;
            ftp_send(sess, "230 Login successful.");
        }
        return FTP_CMD_PASS;
    }

    if (re_include(cmd, "^QUIT\\>", REG_ICASE)) {
        return FTP_CMD_QUIT;
    }

    if (state->user <= 0) {
        ftp_send(sess, "503 Login with USER and PASS first.");
        return FTP_CMD_NONE;
    }

    if (re_include(cmd, "^ACCT\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_ACCT;
    }
    if (re_include(cmd, "^CWD\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_CWD;
    }
    if (re_include(cmd, "^CDUP\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_CDUP;
    }
    if (re_include(cmd, "^SMNT\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_SMNT;
    }
    if (re_include(cmd, "^REIN\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_REIN;
    }

    if (re_include(cmd, "^PORT\\>", REG_ICASE)) {
        regmatch_t m[7];
        uint8_t a[6] = {0};
        re_match(cmd, "^[^ ]+\\> .*\\<([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+)\\>", 7, m, 0);
        for (int i = 0; i < 6; ++i) {
            for (int j = m[i + 1].rm_so; j < m[i + 1].rm_eo; ++j) {
                a[i] = a[i] * 10 + cmd[j] - '0';
            }
        }
        static struct sockaddr_in addr = {AF_INET};
        addr.sin_port = htons(a[4] << 8 | a[5]);
        addr.sin_addr.s_addr = htonl(a[0] << 24 | a[1] << 16 | a[2] << 8 | a[3]);
        state->port_addr = &addr;
        ftp_send(sess, "200 PORT command successful.");
        return FTP_CMD_PORT;
    }

    if (re_include(cmd, "^PASV\\>", REG_ICASE)) {
        state->pasv_fd = ftp_listen(0);
        assert(state->pasv_fd != -1);
        uint32_t a = get_sock_addr(sess);
        uint16_t p = get_sock_port(state->pasv_fd);
        char msg[64];
        sprintf(msg, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).", a >> 24, a >> 16 & 255, a >> 8 & 255, a & 255, p >> 8, p & 255);
        ftp_send(sess, msg);
        return FTP_CMD_PASV;
    }

    if (re_include(cmd, "^TYPE\\>", REG_ICASE)) {
        if (re_include(cmd, "^TYPE I\\>", REG_ICASE)) {
            ftp_send(sess, "200 Type set to I.");
        } else {
            ftp_send(sess, "504 Type must be set to I.");
        }
        return FTP_CMD_TYPE;
    }

    if (re_include(cmd, "^STRU\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STRU;
    }
    if (re_include(cmd, "^MODE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_MODE;
    }
    if (re_include(cmd, "^RETR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RETR;
    }
    if (re_include(cmd, "^STOR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STOR;
    }
    if (re_include(cmd, "^STOU\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STOU;
    }
    if (re_include(cmd, "^APPE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_APPE;
    }
    if (re_include(cmd, "^ALLO\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_ALLO;
    }
    if (re_include(cmd, "^REST\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_REST;
    }
    if (re_include(cmd, "^RNFR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RNFR;
    }
    if (re_include(cmd, "^RNTO\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RNTO;
    }
    if (re_include(cmd, "^ABOR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_ABOR;
    }
    if (re_include(cmd, "^DELE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_DELE;
    }
    if (re_include(cmd, "^RMD\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RMD;
    }
    if (re_include(cmd, "^MKD\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_MKD;
    }
    if (re_include(cmd, "^PWD\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_PWD;
    }

    if (re_include(cmd, "^(LIST|NLST)\\>", REG_ICASE)) {
        state->msg_ready = "150 Here comes the directory listing.";
        state->msg_ok = "226 Directory send OK.";
        state->arg = str_add(str_new(cmd[0] == 'L' ? "-l" : "-1"), cmd + 4);
        if (state->port_addr != NULL) {
            if (0 != ftp_connect(sess, state, ftp_send_list)) {
                ftp_send(sess, "425 Failed to establish connection.");
            }
            state->port_addr = NULL;
        } else if (state->pasv_fd != -1) {
            if (0 != ftp_accept(sess, state, ftp_send_list)) {
                ftp_send(sess, "425 Failed to establish connection.");
            }
            state->pasv_fd = -1;
        } else {
            ftp_send(sess, "425 Use PORT or PASV first.");
        }
        free(state->arg);
        return FTP_CMD_LIST;
    }

    if (re_include(cmd, "^NLST\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_NLST;
    }
    if (re_include(cmd, "^SITE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_SITE;
    }

    if (re_include(cmd, "^SYST\\>", REG_ICASE)) {
        ftp_send(sess, "215 UNIX Type: L8");
        return FTP_CMD_SYST;
    }

    if (re_include(cmd, "^STAT\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STAT;
    }
    if (re_include(cmd, "^HELP\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_HELP;
    }
    if (re_include(cmd, "^NOOP\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_NOOP;
    }
}
