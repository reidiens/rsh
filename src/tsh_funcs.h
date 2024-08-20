#ifndef __TSH_FUNCS_H__
    #define __TSH_FUNCS_H__ 1

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <sys/wait.h>

uint8_t change_dir(char **args);
uint8_t run(char **args);

#endif