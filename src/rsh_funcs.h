#ifndef __RSH_FUNCS_H__
    #define __RSH_FUNCS_H__ 1

#include "rsh_core.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/wait.h>

uint8_t rsh_cd(char **args);
uint8_t rsh_exec_command(char **args);

#endif