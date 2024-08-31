#include "rsh_core.h"

#include <stdio.h>
#include <errno.h>

FILE *rsh_history_fd;
FILE *rsh_rc_fd;
rsh_buf_t *rsh_inbuf = {};
rsh_buf_t *rsh_outbuf = {};
char *hostname, *username, *homedir;

void rsh_err(const char *string) {
    if (string)
        fprintf(stderr, "\033[31mrsh: \033[0m%s\n", string);
    if (errno != 0)
        perror("");
}