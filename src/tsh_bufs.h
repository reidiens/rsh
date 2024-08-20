#ifndef __TSH_BUFS_H__
    #define __TSH_BUFS_H__ 1

#include "shared.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFSIZE   2048
#define OUTPUT_BUFSIZE  4096  

uint8_t init_io_bufs();
void free_io_bufs();
uint8_t flush_io_bufs();

tsh_buf_t* alloc_tsh_buf(size_t bufsize);
void free_tsh_buf(tsh_buf_t *buffer);
void flush_tsh_buf(tsh_buf_t *buffer);

#endif