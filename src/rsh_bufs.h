#ifndef __RSH_BUFS_H__
    #define __RSH_BUFS_H__ 1

#include "rsh_core.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFSIZE   2048 
#define OUTPUT_BUFSIZE  4096  

uint8_t init_io_bufs();
void free_io_bufs();
void flush_io_bufs();
void reset_io_bufs();

rsh_buf_t* alloc_rsh_buf(size_t bufsize);
void free_rsh_buf(rsh_buf_t *buffer);
void flush_rsh_buf(rsh_buf_t *buffer);

#endif