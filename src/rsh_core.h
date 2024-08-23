#ifndef __RSH_CORE__ 
    #define __RSH_CORE__ 1

#include "shared.h"
#include "rsh_bufs.h"
#include "rsh_funcs.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>

#define ARG_BUFSIZE         32

void rsh_err(const char *str);
uint8_t rsh_init(int argc, char *argv[]);
void rsh_cleanup();
void rsh_loop(void);

#endif