#ifndef __RSH_LOOP_H__
    #define __RSH_LOOP_H__

#include "rsh_core.h" // always include first
#include "rsh_init.h"
#include "rsh_bufs.h"
#include "rsh_funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void rsh_err(const char *string);
uint8_t rsh_readline(void);
char **parse_input_buf(size_t *argSize);
void rsh_loop(void);

#endif