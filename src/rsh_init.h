#ifndef __RSH_INIT_H__
    #define __RSH_INIT_H__ 1

#include "rsh_core.c"
#include "rsh_bufs.h"

#include <errno.h>
#include <unistd.h>
#include <stdint.h>

uint8_t rsh_init(int argc, char *argv[]);
uint8_t rsh_parse_init_args(int argc, char *argv[]);
void rsh_printhelp(void);
void rsh_cleanup(void);

#endif