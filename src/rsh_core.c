#include "rsh_core.h"

#include <stdio.h>

FILE *rsh_history_fd;
rsh_buf_t *rsh_inbuf;
rsh_buf_t *rsh_outbuf;
char *hostname, *username, *homedir;

void rsh_err(const char *string) {
    fprintf(stderr, "\033[31mrsh: \033[0m%s\n", string);
}