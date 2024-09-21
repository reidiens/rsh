#include "rsh_core.h"

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

FILE *rsh_history_fd;
FILE *rsh_rc_fd;
rsh_buf_t *rsh_inbuf = {}, *rsh_outbuf = {}, *rsh_errbuf = {};
char *hostname, *username, *homedir, *PATH;

void rsh_err(const char *string) {
    if (string)
        fprintf(stderr, "\033[31mrsh: \033[0m%s\n", string);
    if (errno != 0)
        perror("");
}

char* getArgvValue(char *argv[], char *str) {
    char *string = str;
    uint8_t found = 0;

    for (char **a_p = argv + 2; *a_p; a_p++) {
        for (char *p = *a_p, *string = str; *p && *p == *string; p++, string++) {
            if (*(p + 1) == '=') return p + 2;               
        }
    }
    return NULL;
}