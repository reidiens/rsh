/*
    -- Include First --
*/

#ifndef __RSH_CORE_H__ 
    #define __RSH_CORE_H__ 1

#define EXIT_SUCCESS        0
#define EXIT_RECOVERABLE    1
#define EXIT_UNRECOVERABLE  2

#define HOSTNAME_MAX_LEN    255
#define USERNAME_MAX_LEN     32
#define HOMEDIR_MAX_LEN     255

#include <stdio.h>
#include <errno.h>

typedef struct Rsh_Buffer {
    char *buf;
    size_t max;
    size_t size;
} rsh_buf_t;

extern FILE *rsh_history_fd;
extern FILE *rsh_rc_fd;
extern rsh_buf_t *rsh_inbuf, *rsh_outbuf, *rsh_errbuf;
extern char *hostname;
extern char *username;
extern char *homedir;
extern char *PATH;

void rsh_err(const char *str);
char* getArgvValue(char *argv[], char *str);

#endif