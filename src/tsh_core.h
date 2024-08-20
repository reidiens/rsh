#ifndef __TSH_CORE__ 
    #define __TSH_CORE__ 1

#include "shared.h"
#include "tsh_bufs.h"
#include "tsh_funcs.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>

#define ARG_BUFSIZE         32

extern FILE *historyfd;
extern char *homedir;

void tsh_err(const char *str);
uint8_t tsh_init(int argc, char *argv[]);
void tsh_cleanup();
void tsh_loop(void);

#endif