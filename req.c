#include <arpa/inet.h>
#include <assert.h>
#include "util.h"
#include "sock.h"
#include "req.h"

int ftp_request_handler(int sess, const char* req, struct ftp_state* state) {
    char msg[PATH_MAX + 128];
    char cmd[PATH_MAX + 128];
    char arg[128];

    if (re_include(req, "^USER\\>", REG_ICASE)) {
        if (re_include(req, "^[^ ]+\\>( anonymous\\>|.*)?", 0)) {
            ftp_send(sess, "331 Specifiy an email address as password.");
            state->user = 0;
        } else {
            ftp_send(sess, "504 User must be anonymous.");
        }
        return FTP_CMD_USER;
    }

    if (re_include(req, "^PASS\\>", REG_ICASE)) {
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

    if (re_include(req, "^QUIT\\>", REG_ICASE)) {
        return FTP_CMD_QUIT;
    }

    if (state->user <= 0) {
        ftp_send(sess, "503 Login with USER and PASS first.");
        return FTP_CMD_NONE;
    }

    if (re_include(req, "^ACCT\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_ACCT;
    }
    if (re_include(req, "^CWD\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_CWD;
    }
    if (re_include(req, "^CDUP\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_CDUP;
    }
    if (re_include(req, "^SMNT\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_SMNT;
    }
    if (re_include(req, "^REIN\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_REIN;
    }

    if (re_include(req, "^PORT\\>", REG_ICASE)) {
        regmatch_t m[7];
        uint8_t a[6] = {0};
        re_match(req, "^[^ ]+\\> .*\\<([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+),([0-9]+)\\>", 7, m, 0);
        for (int i = 0; i < 6; ++i) {
            for (int j = m[i + 1].rm_so; j < m[i + 1].rm_eo; ++j) {
                a[i] = a[i] * 10 + req[j] - '0';
            }
        }
        static struct sockaddr_in addr = {AF_INET};
        addr.sin_port = htons(a[4] << 8 | a[5]);
        addr.sin_addr.s_addr = htonl(a[0] << 24 | a[1] << 16 | a[2] << 8 | a[3]);
        state->port_addr = &addr;
        ftp_send(sess, "200 PORT command successful.");
        return FTP_CMD_PORT;
    }

    if (re_include(req, "^PASV\\>", REG_ICASE)) {
        state->pasv_fd = ftp_listen(0);
        assert(state->pasv_fd != -1);
        uint32_t a = get_sock_addr(sess);
        uint16_t p = get_sock_port(state->pasv_fd);
        sprintf(msg, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).", a >> 24, a >> 16 & 255, a >> 8 & 255, a & 255, p >> 8, p & 255);
        ftp_send(sess, msg);
        return FTP_CMD_PASV;
    }

    if (re_include(req, "^TYPE\\>", REG_ICASE)) {
        if (re_include(req, "^TYPE I\\>", REG_ICASE)) {
            ftp_send(sess, "200 Type set to I.");
        } else {
            ftp_send(sess, "504 Type must be set to I.");
        }
        return FTP_CMD_TYPE;
    }

    if (re_include(req, "^STRU\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STRU;
    }
    if (re_include(req, "^MODE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_MODE;
    }
    if (re_include(req, "^RETR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RETR;
    }
    if (re_include(req, "^STOR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STOR;
    }
    if (re_include(req, "^STOU\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STOU;
    }
    if (re_include(req, "^APPE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_APPE;
    }
    if (re_include(req, "^ALLO\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_ALLO;
    }
    if (re_include(req, "^REST\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_REST;
    }
    if (re_include(req, "^RNFR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RNFR;
    }
    if (re_include(req, "^RNTO\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RNTO;
    }
    if (re_include(req, "^ABOR\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_ABOR;
    }
    if (re_include(req, "^DELE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_DELE;
    }
    if (re_include(req, "^RMD\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_RMD;
    }
    if (re_include(req, "^MKD\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_MKD;
    }

    if (re_include(req, "^PWD\\>", REG_ICASE)) {
        sprintf(msg, "257 \"%s\" is the current directory.", state->dir);
        ftp_send(sess, msg);
        return FTP_CMD_PWD;
    }

    if (re_include(req, "^(LIST|NLST)\\>", REG_ICASE)) {
        state->msg_ready = "150 Here comes the directory listing.";
        state->msg_ok = "226 Directory send OK.";
        sprintf(arg, "-l %s", req + 4);
        if (toupper(req[0]) == 'N') {
            arg[1] = '1';
        }
        state->arg = arg;
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
        return FTP_CMD_LIST;
    }

    if (re_include(req, "^NLST\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_NLST;
    }
    if (re_include(req, "^SITE\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_SITE;
    }

    if (re_include(req, "^SYST\\>", REG_ICASE)) {
        ftp_send(sess, "215 UNIX Type: L8");
        return FTP_CMD_SYST;
    }

    if (re_include(req, "^STAT\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_STAT;
    }
    if (re_include(req, "^HELP\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_HELP;
    }
    if (re_include(req, "^NOOP\\>", REG_ICASE)) {
        ftp_send(sess, "502 Command not implemented.");
        return FTP_CMD_NOOP;
    }
}
